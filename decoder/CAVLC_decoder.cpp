#include "CAVLC_decoder.h"
#include "common/bitstream.h"
#include "common/HRD.h"
#include "common/PPS.h"
#include "common/PTL.h"
#include "common/scaling_list.h"
#include "common/SPS.h"
#include "common/syntax_element.h"
#include "common/VPS.h"
#include "common/VUI.h"

#include <cassert>
#include <algorithm>

namespace HEVC
{

using namespace SyntaxElement;

void CAVLCDecoder::decode_vps(InputBitstream* bs, VPS* vps) const
{
  assert(bs!=NULL);
  assert(vps!=NULL);

  unsigned int code;
  bool flag;
  vps->vps_id = read_code(bs, 4);
  code = read_code(bs, 2); assert(code==3u);
  vps->max_layers = read_code(bs, 6) + 1;
  vps->max_sub_layers = read_code(bs, 3) + 1;
  vps->temporal_id_nesting_flag = read_code(bs, 1);
  code = read_code(bs, 16); assert(code==0x0FF);
  decode_ptl(bs, &vps->profile_tier_level, vps->max_sub_layers);
  flag = read_flag(bs);
  vps->sub_layer_ordering_info_present = flag;
  int size = (flag) ? vps->max_sub_layers : 1;
  vps->max_dec_pic_buffering = new int[size];
  vps->max_num_reorder_pics = new int[size];
  vps->max_latency_increase = new int[size];
  for(int i=0; i<size; i++)
  {
    vps->max_dec_pic_buffering[i] = read_uvlc(bs) + 1 ;
    vps->max_num_reorder_pics[i] = read_uvlc(bs);
    vps->max_latency_increase[i] = read_uvlc(bs) - 1;
  }
  unsigned int max_layer_id = read_code(bs, 6);
  unsigned int num_layer_sets = read_uvlc(bs) + 1;
  vps->max_layer_id = max_layer_id;
  vps->num_layer_sets = num_layer_sets;
  if(max_layer_id*(num_layer_sets+1)>0)
    vps->layer_id_included_flag = new bool[max_layer_id*(num_layer_sets+1)];
  for(unsigned int i=1; i<num_layer_sets; i++ )
    for(unsigned int j=0u; j<=max_layer_id; j++ )
      vps->layer_id_included_flag[i*(max_layer_id+1)+j] = read_flag(bs);
  flag = read_flag(bs);
  vps->timing_info_present = flag;
  if(flag) {
    vps->num_units_in_tick = read_code(bs, 32);
    vps->time_scale = read_code(bs, 32);
    flag = read_flag(bs);
    vps->poc_proportional_to_timing_flag = flag;
    if(flag)
      vps->num_ticks_poc_diff_one = read_uvlc(bs) + 1;
    unsigned int num_hrd_parameters = read_uvlc(bs);
    vps->num_hrd_parameters = num_hrd_parameters;
    if(num_hrd_parameters>0u)
    {
      vps->hrd_layer_set_idx = new int[num_hrd_parameters];
      vps->cprms_present = new bool[num_hrd_parameters];
    }
    for(unsigned int i=0; i<num_hrd_parameters; i++) {
      vps->hrd_layer_set_idx[i] = read_uvlc(bs);
      if(i>0)
        vps->cprms_present[i] = read_flag(bs);
      //hrd_parameters( cprms_present[i], max_sub_layers_minus1 )
    }
  }
  vps->extension_flag = read_flag(bs);
  // if( extension_flag )
  //   while( more_rbsp_data( ) )
  //     extension_data_flag = read_flag(bs);
  // rbsp_trailing_bits( )
}

void CAVLCDecoder::decode_sps(InputBitstream* bs, SPS* sps) const
{
  assert(bs!=NULL);
  assert(sps!=NULL);

  unsigned int code;
  bool flag;
  sps->vps_id = read_code(bs, 4);
  sps->max_sub_layers = read_code(bs, 3) + 1;
  sps->temporal_id_nesting_flag = read_flag(bs);
  decode_ptl(bs, &sps->profile_tier_level, sps->max_sub_layers);
  sps->sps_id = read_uvlc(bs);
  code = read_uvlc(bs);
  sps->chroma_format_idc = code;
  if(code == 3u)
    sps->separate_colour_plane_flag  = read_flag(bs);
  sps->pic_width_in_luma = read_uvlc(bs);
  sps->pic_height_in_luma = read_uvlc(bs);
  flag = read_flag(bs);
  sps->conformance_window_flag = read_flag(bs);
  if(flag)
  {
    sps->conf_win_left_offset  = read_uvlc(bs);
    sps->conf_win_right_offset = read_uvlc(bs);
    sps->conf_win_top_offset = read_uvlc(bs);
    sps->conf_win_bottom_offset  = read_uvlc(bs);
  }
  sps->bit_depth_luma = read_uvlc(bs) + 8;
  sps->bit_depth_chroma = read_uvlc(bs) + 8;
  sps->log2_max_pic_order_cnt_lsb = read_uvlc(bs) + 4;
  flag = read_flag(bs);
  sps->sub_layer_ordering_info_present = flag;
  int size = (flag) ? sps->max_sub_layers : 1;
  sps->max_dec_pic_buffering = new int[size];
  sps->max_num_reorder_pics = new int[size];
  sps->max_latency_increase = new int[size];
  for(int i =0; i< size; i++)
  {
    sps->max_dec_pic_buffering[i] = read_uvlc(bs) + 1;
    sps->max_num_reorder_pics[i] = read_uvlc(bs);
    sps->max_latency_increase[i] = read_uvlc(bs) - 1;
  }
  sps->log2_min_luma_coding_block_size = read_uvlc(bs) + 3;
  sps->log2_diff_max_min_luma_coding_block_size = read_uvlc(bs);
  sps->log2_min_transform_block_size = read_uvlc(bs) + 2;
  sps->log2_diff_max_min_transform_block_size = read_uvlc(bs);
  sps->max_transform_hierarchy_depth_inter = read_uvlc(bs);
  sps->max_transform_hierarchy_depth_intra = read_uvlc(bs);
  flag = read_flag(bs);
  sps->scaling_list_enabled = flag;
  if(flag)
  {
    flag = read_flag(bs);
    sps->scaling_list_data_present = flag;
    if(flag)
    {
      sps->scaling_list = new ScalingList;
      decode_scaling_list(bs, sps->scaling_list);
    }
  }
  sps->amp_enabled = read_flag(bs);
  sps->sao_enabled = read_flag(bs);
  flag = read_flag(bs);
  sps->pcm_enabled = flag;
  if(flag)
  {
    sps->pcm_sample_bit_depth_luma = read_code(bs, 4) + 1;
    sps->pcm_sample_bit_depth_chroma = read_code(bs, 4) + 1;
    sps->log2_min_pcm_luma_coding_block_size = read_uvlc(bs) + 3;
    sps->log2_diff_max_min_pcm_luma_coding_block_size = read_uvlc(bs);
    sps->pcm_loop_filter_disabled = read_flag(bs);
  }
  unsigned int num_short_term_ref_pic_sets = read_uvlc(bs);
  sps->num_short_term_ref_pic_sets = num_short_term_ref_pic_sets;
  // for(int i=0; i<num_short_term_ref_pic_sets; i++)
  //   short_term_ref_pic_set( i )
  flag = read_flag(bs);
  sps->long_term_ref_pics_present = flag;
  if(flag)
  {
    unsigned int num_long_term_ref_pics = read_uvlc(bs);
    sps->num_long_term_ref_pics = num_long_term_ref_pics;
    sps->used_by_curr_pic_lt_flag = new bool[num_long_term_ref_pics];
    for(unsigned int i=0; i<num_long_term_ref_pics; i++ ) {
      // sps->lt_ref_pic_poc_lsb[i] = read_code(bs, v)
      sps->used_by_curr_pic_lt_flag[i] = read_flag(bs);
    }
  }
  sps->temporal_mvp_enabled = read_flag(bs);
  sps->strong_intra_smoothing_enabled = read_flag(bs);
  flag = read_flag(bs);
  sps->vui_parameters_present = flag;
  if(flag) decode_vui(bs, &sps->vui);

  sps->extension_flag = read_flag(bs);
  // if( extension_flag )
  //   while( more_rbsp_data( ) )
  //     extension_data_flag = read_flag(bs);
  // rbsp_trailing_bits( )
}

void CAVLCDecoder::decode_pps(InputBitstream* bs, PPS* pps) const
{
  assert(bs != NULL);
  assert(pps != NULL);

  bool flag;

  pps->pps_id = read_uvlc(bs);
  pps->sps_id = read_uvlc(bs);
  pps->dependent_slice_segments_enabled = read_flag(bs);
  pps->output_flag_present = read_flag(bs);
  pps->num_extra_slice_header_bits = read_code(bs, 3);
  pps->sign_data_hiding_enabled = read_flag(bs);
  pps->cabac_init_present = read_flag(bs);
  pps->num_ref_idx_l0_default_active = read_uvlc(bs) + 1;
  pps->num_ref_idx_l1_default_active = read_uvlc(bs) + 1;
  pps->init_qp = read_svlc(bs) + 26;
  pps->constrained_intra_pred_flag = read_flag(bs);
  pps->transform_skip_enabled = read_flag(bs);
  flag = read_flag(bs);
  pps->cu_qp_delta_enabled = flag;
  if(flag)
    pps->diff_cu_qp_delta_depth = read_uvlc(bs);
  pps->cb_qp_offset = read_svlc(bs);
  pps->cr_qp_offset = read_svlc(bs);
  pps->slice_chroma_qp_offsets_present = read_flag(bs);
  pps->weighted_pred_flag = read_flag(bs);
  pps->weighted_bipred_flag = read_flag(bs);
  pps->transquant_bypass_enabled = read_flag(bs);
  flag = read_flag(bs);
  pps->tiles_enabled  = flag;
  pps->entropy_coding_sync_enabled = read_flag(bs);
  if(flag)
  {
    unsigned int num_tile_columns = read_uvlc(bs) + 1;
    unsigned int num_tile_rows = read_uvlc(bs) + 1;
    pps->num_tile_columns = num_tile_columns;
    pps->num_tile_rows = num_tile_rows;
    flag = read_flag(bs);
    pps->uniform_spacing_flag  = flag;
    if(!flag)
    {
      pps->column_width = new int[num_tile_columns];
      pps->row_height = new int[num_tile_rows];
      for(unsigned int i = 0; i < num_tile_columns; i++ )
        pps->column_width[i] = read_uvlc(bs) + 1;
      for(unsigned int i = 0; i < num_tile_rows; i++ )
        pps->row_height[i] = read_uvlc(bs) + 1;
    }
    pps->loop_filter_across_tiles_enabled = read_flag(bs);
  }

  pps->loop_filter_across_slices_enabled = read_flag(bs);
  flag = read_flag(bs);
  pps->deblocking_filter_control_present = flag;
  if(flag) {
    pps->deblocking_filter_override_enabled = read_flag(bs);
    flag = read_flag(bs);
    pps->deblocking_filter_disabled = flag;
    if(!flag) {
      pps->beta_offset = read_svlc(bs) * 2;
      pps->tc_offset = read_svlc(bs) * 2;
    }
  }
  flag = read_flag(bs);
  pps->scaling_list_data_present = flag;
  if(flag)
  {
    pps->scaling_list = new ScalingList;
    decode_scaling_list(bs, pps->scaling_list);
  }
  pps->lists_modification_present = read_flag(bs);
  pps->log2_parallel_merge_level = read_uvlc(bs) + 2;
  pps->slice_segment_header_extension_present = read_flag(bs);
  pps->extension_flag = read_flag(bs);
  // if( pps_extension_flag )
    // while( more_rbsp_data( ) )
      // pps_extension_data_flag = read_flag(bs);
  // rbsp_trailing_bits( )
}

void CAVLCDecoder::decode_ptl(InputBitstream* bs, PTL* ptl,
                              int max_sub_layers) const
{
  assert(bs!=NULL);
  assert(ptl!=NULL);

  unsigned int code;
  ptl->general_profile_space = read_code(bs, 2);
  ptl->general_tier_flag = read_flag(bs);
  ptl->general_profile_idc = read_code(bs, 5);
  for(int i=0; i<32; i++)
    ptl->general_profile_compatibility_flag[i] = read_flag(bs);
  ptl->general_progressive_source_flag = read_flag(bs);
  ptl->general_interlaced_source_flag = read_flag(bs);
  ptl->general_non_packed_constraint_flag = read_flag(bs);
  ptl->general_frame_only_constraint_flag = read_flag(bs);
  code = read_code(bs, 32);
  code = read_code(bs, 12);
  ptl->general_level_idc = read_code(bs, 8);
  if(max_sub_layers > 2)
  {
    ptl->sub_layer_profile_present = new bool[max_sub_layers - 1];
    ptl->sub_layer_level_present = new bool[max_sub_layers - 1];

    for(int i=0; i<max_sub_layers -1; i++)
    {
      ptl->sub_layer_profile_present[i] = read_flag(bs);
      ptl->sub_layer_level_present[i] = read_flag(bs);
    }
  }
  if(max_sub_layers > 1)
  {
    for(int i = max_sub_layers - 1; i<8; i++ )
      code = read_code(bs, 2);
  }
  if(max_sub_layers > 2)
  {
    ptl->sub_layer_profile_space = new int[max_sub_layers-1];
    ptl->sub_layer_tier_flag = new bool[max_sub_layers-1];
    ptl->sub_layer_profile_idc = new int[max_sub_layers-1];
    ptl->sub_layer_profile_compatibility_flag =
      new bool[(max_sub_layers-1)*32];
    ptl->sub_layer_progressive_source_flag = new bool[max_sub_layers-1];
    ptl->sub_layer_interlaced_source_flag = new bool[max_sub_layers-1];
    ptl->sub_layer_non_packed_constraint_flag = new bool[max_sub_layers-1];
    ptl->sub_layer_frame_only_constraint_flag = new bool[max_sub_layers-1];
    ptl->sub_layer_level_idc = new int[max_sub_layers-1];
  }
  for(int i=0; i<max_sub_layers - 1; i++ )
  {
    if(ptl->sub_layer_profile_present[i])
    {
      ptl->sub_layer_profile_space[i] = read_code(bs, 2);
      ptl->sub_layer_tier_flag[i] = read_flag(bs);
      ptl->sub_layer_profile_idc[i] = read_code(bs, 5);
      for(int j=0; j<32; j++)
        ptl->sub_layer_profile_compatibility_flag[i*32 + j] = read_flag(bs);
      ptl->sub_layer_progressive_source_flag[i] = read_flag(bs);
      ptl->sub_layer_interlaced_source_flag[i] = read_flag(bs);
      ptl->sub_layer_non_packed_constraint_flag[i] = read_flag(bs);
      ptl->sub_layer_frame_only_constraint_flag[i] = read_flag(bs);
      code = read_code(bs, 32);
      code = read_code(bs, 12);
    }
    if(ptl->sub_layer_level_present[i])
      ptl->sub_layer_level_idc[i] = read_code(bs, 8);
  }
}

void CAVLCDecoder::decode_scaling_list(InputBitstream* bs,
                                       ScalingList* sl) const
{
  assert(bs!=NULL);
  assert(sl!=NULL);

  for(int size_id=0; size_id<4; size_id++)
  {
    static const int matrix_id_max[] = {6, 6, 6, 2};
    for(int matrix_id=0; matrix_id<matrix_id_max[size_id]; matrix_id++)
    {
      bool flag = read_flag(bs);
      sl->pred_mode_flag[size_id][matrix_id] = flag;
      if(!flag)
        sl->pred_matrix_id_delta[size_id][matrix_id] = read_uvlc(bs);
      else
      {
        int next_coef = 8;
        int coef_num = std::min(64, 1<<(4+(size_id<<1)));
        if(size_id>1)
        {
          next_coef = read_svlc(bs) + 8;
          sl->dc_coef[size_id-2][matrix_id] = next_coef;
        }
        for(int i=0; i<coef_num; i++)
        {
          int delta_coeff = read_svlc(bs);
          next_coef = (next_coef+delta_coeff+256)%256;
          sl->scaling_list[size_id][matrix_id][i] = next_coef;
        }
      }
    }
  }
}

void CAVLCDecoder::decode_vui(InputBitstream* bs, VUI* vui) const
{
  assert(bs!=NULL);
  assert(vui!=NULL);

  unsigned int code;
  bool flag;
  flag = read_flag(bs);
  vui->aspect_ratio_info_present = flag;
  if(flag)
  {
    code = read_code(bs, 8);
    vui->aspect_ratio_idc = code;
    if(code==ASPECT_EXTENDED_SAR )
    {
      vui->sar_width = read_code(bs, 16);
      vui->sar_height = read_code(bs, 16);
    }
  }
  flag = read_flag(bs);
  vui->overscan_info_present = flag;
  if(flag)
    vui->overscan_appropriate_flag = read_flag(bs);
  flag = read_flag(bs);
  vui->video_signal_type_present = flag;
  if(flag)
  {
    vui->video_format = read_code(bs, 3);
    vui->video_full_range_flag = read_flag(bs);
    flag = read_flag(bs);
    vui->colour_description_present = flag;
    if(flag)
    {
      vui->colour_primaries = read_code(bs, 8);
      vui->transfer_characteristics = read_code(bs, 8);
      vui->matrix_coeffs = read_code(bs, 8);
    }
  }
  flag = read_flag(bs);
  vui->chroma_loc_info_present = flag;
  if(flag)
  {
    vui->chroma_sample_loc_type_top_field = read_uvlc(bs);
    vui->chroma_sample_loc_type_bottom_field = read_uvlc(bs);
  }
  vui->neutral_chroma_indication_flag = read_flag(bs);
  vui->field_seq_flag  = read_flag(bs);
  vui->frame_field_info_present = read_flag(bs);
  flag = read_flag(bs);
  vui->default_display_window_flag = flag;
  if(flag)
  {
    vui->def_disp_win_left_offset = read_uvlc(bs);
    vui->def_disp_win_right_offset = read_uvlc(bs);
    vui->def_disp_win_top_offset = read_uvlc(bs);
    vui->def_disp_win_bottom_offset = read_uvlc(bs);
  }
  flag = read_flag(bs);
  vui->timing_info_present = flag;
  if(flag)
  {
    vui->num_units_in_tick = read_code(bs, 32);
    vui->time_scale  = read_code(bs, 32);
    flag = read_flag(bs);
    vui->poc_proportional_to_timing_flag = flag;
    if(flag)
      vui->num_ticks_poc_diff_one_minus1 = read_uvlc(bs);
    flag = read_flag(bs);
    vui->hrd_parameters_present = flag;
    // if(flag)
    //   hrd_parameters( 1, sps_max_sub_layers_minus1 )
  }
  flag = read_flag(bs);
  vui->bitstream_restriction_flag = flag;
  if(flag)
  {
    vui->tiles_fixed_structure_flag  = read_flag(bs);
    vui->motion_vectors_over_pic_boundaries_flag = read_flag(bs);
    vui->restricted_ref_pic_lists_flag = read_flag(bs);
    vui->min_spatial_segmentation_idc = read_uvlc(bs);
    vui->max_bytes_per_pic_denom = read_uvlc(bs);
    vui->max_bits_per_min_cu_denom = read_uvlc(bs);
    vui->log2_max_mv_length_horizontal = read_uvlc(bs);
    vui->log2_max_mv_length_vertical = read_uvlc(bs);
  }
}

void CAVLCDecoder::decode_hrd(InputBitstream* bs, HRD* hrd,
                              bool common_info_present,
                              int max_sub_layers) const
{
  assert(bs!=NULL);
  assert(hrd!=NULL);

  bool flag;
  if(common_info_present)
  {
    hrd->nal_hrd_parameters_present = read_flag(bs);
    hrd->vcl_hrd_parameters_present = read_flag(bs);
    if(hrd->nal_hrd_parameters_present || hrd->vcl_hrd_parameters_present)
    {
      flag = read_flag(bs);
      hrd->sub_pic_hrd_params_present = flag;
      if(flag) {
        hrd->tick_divisor = read_code(bs, 8) + 2;
        hrd->du_cpb_removal_delay_increment_length = read_code(bs, 5) + 1;
        hrd->sub_pic_cpb_params_in_pic_timing_sei_flag = read_flag(bs);
        hrd->dpb_output_delay_du_length = read_code(bs, 5) + 1;
      }
      hrd->bit_rate_scale = read_code(bs, 4);
      hrd->cpb_size_scale = read_code(bs, 4);
      if(flag)
        hrd->cpb_size_du_scale = read_code(bs, 4);
      hrd->initial_cpb_removal_delay_length = read_code(bs, 5) + 1;
      hrd->au_cpb_removal_delay_length = read_code(bs, 5) + 1;
      hrd->dpb_output_delay_length = read_code(bs, 5) + 1;
    }
  }

  for(int i=0; i<max_sub_layers; i++)
  {
    flag = read_flag(bs);
    hrd->fixed_pic_rate_general_flag[i] = flag;
    if(!flag)
      hrd->fixed_pic_rate_within_cvs_flag[i] = read_flag(bs);
    if(hrd->fixed_pic_rate_within_cvs_flag[i])
      hrd->elemental_duration_in_tc[i] = read_uvlc(bs);
    else
      hrd->low_delay_hrd_flag[i] = read_flag(bs);
    if(!hrd->low_delay_hrd_flag[i])
      hrd->cpb_cnt[i] = read_uvlc(bs) + 1;
    if(hrd->nal_hrd_parameters_present)
      decode_sub_layer_hrd(bs, hrd, hrd->sub_layer_hrd_parameters, i);
    if(hrd->vcl_hrd_parameters_present)
      decode_sub_layer_hrd(bs, hrd, hrd->sub_layer_hrd_parameters, i);
  }
}

void CAVLCDecoder::decode_sub_layer_hrd(InputBitstream* bs, HRD* hrd,
                                        SubLayerHRD* sub_hrd, int ix) const
{

}


} // namespace HEVC