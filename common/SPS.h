#ifndef _SPS_H_
#define _SPS_H_

#include "PTL.h"

#include <vector>

namespace HEVC
{

/// Data object: Sequence Parameter Set (SPS) syntax based on JCTVC-L1003
/// rev. 34.
class SPS
{
public:
  SPS()
  {
    profile_tier_level = NULL;
    max_dec_pic_buffering = NULL;
    max_num_reorder_pics = NULL;
    max_latency_increase = NULL;
    lt_ref_pic_poc_lsb_sps = NULL;
    used_by_curr_pic_it_sps_flag = NULL;
  }

  virtual ~SPS()
  {
    if(profile_tier_level != NULL)
      delete profile_tier_level;
    if(max_dec_pic_buffering != NULL)
      delete[] max_dec_pic_buffering;
    if(max_num_reorder_pics != NULL)
      delete[] max_num_reorder_pics;
    if(max_latency_increase != NULL)
      delete[] max_latency_increase;
    if(lt_ref_pic_poc_lsb_sps != NULL)
      delete[] lt_ref_pic_poc_lsb_sps;
    if(used_by_curr_pic_it_sps_flag != NULL)
      delete[] used_by_curr_pic_it_sps_flag;
  }

  int vps_id; ///< sps_video_parameter_set_id
  int max_sub_layers; ///< sps_max_sub_layers_minus1 + 1
  bool temporal_id_nesting_flag;
  PTL* profile_tier_level;
  // profile_tier_level(max_sub_layers_minus1)

  int sps_id; ///< sps_seq_parameter_set_id
  int chorma_format_idc;
  // if(chroma_format_idc == 3)
  bool separate_color_plane_flag; ///< separate_colour_plane_flag
  int pic_width_in_luma; ///< pic_width_in_luma_samples
  int pic_height_in_luma; ///< pic_height_in_luma_samples

  bool conformance_widow_flag;
  // if(conformance_window_flag)
  int conf_win_left_offset;
  int conf_win_right_offset;
  int conf_win_top_offset;
  int conf_win_bottom_offset;

  int bit_depth_luma; ///< bit_depth_luma_minus8 + 8
  int bit_depth_chroma; ///< bit_depth_chroma_minus8 + 8
  int log2_max_pic_order_cnt_lsb; ///< log2_max_pic_order_cnt_lsb_minus4 + 4

  bool sps_sublayer_ordering_info_present_flag;

  // for(i=sps_sub_layer_ordering_info_present_flag? 0:
  //       sps_max_sub_layers_minus1;
  //     i<=sps_max_sub_layers_minus1; i++)1
  // {
  int* max_dec_pic_buffering; ///< sps_max_dec_pic_buffering_minus+1
  int* max_num_reorder_pics;
  int* max_latency_increase; ///< sps_max_latency_increase_plus1-1
  // }

  int log2_min_luma_coding_block_size;
  ///< log2_min_luma_coding_block_size_minus3+3
  int log2_diff_max_min_luma_coding_block_size;
  int log2_min_transform_block_size; ///< log2_min_transform_block_size_minus2+2
  int log2_diff_max_min_transform_block_size;

  int max_transform_hierarchy_dpeth_inter;
  int max_transform_hierarchy_depth_intra;

  bool scaling_list_enabled_flag;
  //if(scaling_list_enabled_flag)
  bool sps_scaling_list_data_present_flag;
  //  if(sps_scaling_list_data_present_flag)
  //    scaling_list_data()

  bool amp_enabled_flag;
  bool sao_enabled_flag; ///< sample adaptive offset_enabled_flag
  bool pcm_enabled_flag;

  // if(PCM_enabled_flag)
  // {
  int pcm_sample_bit_depth_luma; ///< pcm_sample_bit_depth_luma_minus1 + 1
  int pcm_sample_bit_depth_chroma; ///< pcm_sample_bit_depth_chroma_minus1 + 1
  int log2_min_PCM_luma_coding_block_size;
  ///< log2_min_pcm_luma_coding_block_size_minus3 + 3
  int log2_diff_max_min_pcm_luma_coding_block_size;
  bool pcm_loop_filter_disabled_flag;
  // }

  int num_short_term_ref_pic_sets;
  // for(i=0; i<num_shrot_term_ref_pic_sets; i++)
  //  short_term_ref_pic_set(i);

  bool long_term_ref_pic_present_flag;
  // if(long_term_ref_pic_present_flag)
  // {
  int num_long_term_ref_pics_sps;
  // for(i=0; i<num_long_term_ref_pics_sps; i++)
  // {
  int* lt_ref_pic_poc_lsb_sps;
  bool* used_by_curr_pic_it_sps_flag;
  // }
  // }
  bool sps_temporal_mvp_enabled_flag;
  bool stroing_intra_smoothing_enabled_flag;
  bool vui_parameters_present_flag;
  // if(vui_parameter_present_flag)
  //  vui_parameters()
  bool sps_extension_flag;
  // if(sps_extension_flag)
  //   while(more_rbsp_data())
  //    sps_extension_data_flag;
  // rbsp_trailing_bits()
private:
  SPS(const SPS&);
  void operator=(const SPS&);
};

} // namespace HEVC

#endif // _SPS_H_