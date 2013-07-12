#ifndef _SPS_H_
#define _SPS_H_

#include "PTL.h"
#include "scaling_list.h"
#include "VUI.h"

#include <cstdlib>

namespace HEVC
{

/// Data object: Sequence Parameter Set (SPS) syntax based on JCTVC-L1003
/// rev. 34.
class SPS
{
public:
  SPS()
  {
    max_dec_pic_buffering = NULL;
    max_num_reorder_pics = NULL;
    max_latency_increase = NULL;
    scaling_list = NULL;
    lt_ref_pic_poc_lsb = NULL;
    used_by_curr_pic_lt_flag = NULL;
  }

  virtual ~SPS()
  {
    if(max_dec_pic_buffering!=NULL) delete[] max_dec_pic_buffering;
    if(max_num_reorder_pics!=NULL) delete[] max_num_reorder_pics;
    if(max_latency_increase!=NULL) delete[] max_latency_increase;
    if(scaling_list!=NULL) delete scaling_list;
    if(lt_ref_pic_poc_lsb!=NULL) delete[] lt_ref_pic_poc_lsb;
    if(used_by_curr_pic_lt_flag!=NULL) delete[] used_by_curr_pic_lt_flag;
  }

  int vps_id;
  int max_sub_layers;
  bool temporal_id_nesting_flag;
  PTL profile_tier_level;
  int sps_id;
  int chroma_format_idc;
  bool separate_colour_plane_flag;
  int pic_width_in_luma;
  int pic_height_in_luma;
  bool conformance_window_flag;
  int conf_win_left_offset;
  int conf_win_right_offset;
  int conf_win_top_offset;
  int conf_win_bottom_offset;
  int bit_depth_luma;
  int bit_depth_chroma;
  int log2_max_pic_order_cnt_lsb;
  bool sub_layer_ordering_info_present;
  int* max_dec_pic_buffering;
  int* max_num_reorder_pics;
  int* max_latency_increase;
  int log2_min_luma_coding_block_size;
  int log2_diff_max_min_luma_coding_block_size;
  int log2_min_transform_block_size;
  int log2_diff_max_min_transform_block_size;
  int max_transform_hierarchy_depth_inter;
  int max_transform_hierarchy_depth_intra;
  bool scaling_list_enabled;
  bool scaling_list_data_present;
  ScalingList* scaling_list;
  bool amp_enabled;
  bool sao_enabled;
  bool pcm_enabled;
  int pcm_sample_bit_depth_luma;
  int pcm_sample_bit_depth_chroma;
  int log2_min_pcm_luma_coding_block_size;
  int log2_diff_max_min_pcm_luma_coding_block_size;
  bool pcm_loop_filter_disabled;
  int num_short_term_ref_pic_sets;
  // bool   short_term_ref_pic_set( i )
  bool long_term_ref_pics_present;
  int num_long_term_ref_pics;
  int* lt_ref_pic_poc_lsb;
  bool* used_by_curr_pic_lt_flag;
  bool temporal_mvp_enabled;
  bool strong_intra_smoothing_enabled;
  bool vui_parameters_present;
  VUI vui;
  bool extension_flag;
  bool extension_data_flag;

private:
  SPS(const SPS&);
  void operator=(const SPS&);
};

} // namespace HEVC

#endif // _SPS_H_