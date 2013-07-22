#ifndef _SPS_H_
#define _SPS_H_

namespace HEVC
{

/// JCTVC-L1003 rev. 34.
/// 7.3.2.2 Sequence parameter set RBSP syntax.
class SPS
{
public:
  SPS() {}
  virtual ~SPS() {}

  int vps_id; /// u(4)
  int max_sub_layers; /// u(3) + 1
  bool temporal_id_nesting_flag; /// u(1)
  // profile_tier_level( max_sub_layers_minus1 )
  int sps_id; /// ue(v)
  int chroma_format_idc; /// ue(v)
  bool separate_colour_plane_flag; /// u(1)
  int pic_width_in_luma_samples; /// ue(v)
  int pic_height_in_luma_samples; ///  ue(v)
  bool conformance_window_flag; /// u(1)
  int conf_win_left_offset; /// ue(v)
  int conf_win_right_offset; /// ue(v)
  int conf_win_top_offset; /// ue(v)
  int conf_win_bottom_offset; /// ue(v)
  int bit_depth_luma; /// ue(v) + 8
  int bit_depth_chroma; /// ue(v) + 8
  int log2_max_pic_order_cnt_lsb; /// ue(v) + 4
  bool sub_layer_ordering_info_present; /// u(1)
  int* max_dec_pic_buffering; /// ue(v) + 1
  int* max_num_reorder_pics; /// ue(v)
  int* max_latency_increase; /// ue(v) - 1
  int log2_min_luma_coding_block_size; /// ue(v) + 3
  int log2_diff_max_min_luma_coding_block_size; /// ue(v)
  int log2_min_transform_block_size; /// ue(v) + 2
  int log2_diff_max_min_transform_block_size; /// ue(v)
  int max_transform_hierarchy_depth_inter; /// ue(v)
  int max_transform_hierarchy_depth_intra; /// ue(v)
  bool scaling_list_enabled; /// u(1)
  bool scaling_list_data_present; /// u(1)
  // scaling_list_data( )
  bool amp_enabled; /// u(1)
  bool sao_enabled; /// u(1)
  bool pcm_enabled; /// u(1)
  int pcm_sample_bit_depth_luma; /// u(4) + 1
  int pcm_sample_bit_depth_chroma; /// u(4) + 1
  int log2_min_pcm_luma_coding_block_size; /// ue(v) + 3
  int log2_diff_max_min_pcm_luma_coding_block_size; /// ue(v)
  int pcm_loop_filter_disabled; /// u(1)
  int num_short_term_ref_pic_sets; /// ue(v)
  // short_term_ref_pic_set( i )
  bool long_term_ref_pics_present; /// u(1)
  int num_long_term_ref_pics_sps; /// ue(v)
  int* long_term_ref_pic_poc_lsb_sps; /// u(v)
  int* used_by_curr_pic_long_term_flag; /// u(1)
  bool temporal_mvp_enabled; /// u(1)
  bool strong_intra_smoothing_enabled; /// u(1)
  bool vui_parameters_present; /// u(1)
  // vui_parameters( )
  bool extension_flag; /// u(1)
  bool extension_data_flag; /// u(1)

private:
  SPS(const SPS&);
  void operator=(const SPS&);
};

} // namespace HEVC

#endif // _SPS_H_