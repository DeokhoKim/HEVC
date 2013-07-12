#ifndef _VUI_H_
#define _VUI_H_

namespace HEVC
{

enum ASPECT_RATIO_IDC
{
  ASPECT_UNSPECIFIED = 0,
  ASPECT_1_TO_1 = 1,
  ASPECT_12_TO_11 = 2,
  ASPECT_10_TO_11 = 3,
  ASPECT_16_TO_11 = 4,
  ASPECT_40_TO_33 = 5,
  ASPECT_24_TO_11 = 6,
  ASPECT_20_TO_11 = 7,
  ASPECT_32_TO_11 = 8,
  ASPECT_80_TO_33 = 9,
  ASPECT_18_TO_11 = 10,
  ASPECT_15_TO_11 = 11,
  ASPECT_64_TO_33 = 12,
  ASPECT_160_TO_99 = 13,
  ASPECT_4_TO_3 = 14,
  ASPECT_3_TO_2 = 15,
  ASPECT_2_TO_1 = 16,
  ASPECT_RESERVED = 17, // 17...254
  ASPECT_EXTENDED_SAR = 255
};

/// Data object: Visual Usuability Information (VUI) syntax based on JCTVC-L1003
/// rev. 34.
class VUI
{
public:
  VUI() {}
  virtual ~VUI() {}

  bool aspect_ratio_info_present;
  int aspect_ratio_idc;
  int sar_width;
  int sar_height;
  bool overscan_info_present;
  bool overscan_appropriate_flag;
  bool video_signal_type_present;
  int video_format;
  bool video_full_range_flag;
  bool colour_description_present;
  int colour_primaries;
  int transfer_characteristics;
  int matrix_coeffs;
  bool chroma_loc_info_present;
  int chroma_sample_loc_type_top_field;
  int chroma_sample_loc_type_bottom_field;
  bool neutral_chroma_indication_flag;
  bool field_seq_flag;
  bool frame_field_info_present;
  bool default_display_window_flag;
  int def_disp_win_left_offset;
  int def_disp_win_right_offset;
  int def_disp_win_top_offset;
  int def_disp_win_bottom_offset;
  bool timing_info_present;
  unsigned int num_units_in_tick;
  unsigned int time_scale;
  bool poc_proportional_to_timing_flag;
  int num_ticks_poc_diff_one_minus1;
  bool hrd_parameters_present;
  // bool  hrd_parameters( 1, sps_max_sub_layers_minus1 )
  bool bitstream_restriction_flag;
  bool tiles_fixed_structure_flag;
  bool motion_vectors_over_pic_boundaries_flag;
  bool restricted_ref_pic_lists_flag;
  int min_spatial_segmentation_idc;
  int max_bytes_per_pic_denom;
  int max_bits_per_min_cu_denom;
  int log2_max_mv_length_horizontal;
  int log2_max_mv_length_vertical;

private:
  VUI(const VUI&);
  void operator=(const VUI&);
};

} // namespace HEVC
#endif // _VUI_H_