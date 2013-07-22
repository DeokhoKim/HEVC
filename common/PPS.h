#ifndef _PPS_H_
#define _PPS_H_

namespace HEVC
{

/// JCTVC-L1003 rev. 34.
/// 7.3.2.3 Picture parameter set RBSP syntax.
class PPS
{
public:
  PPS() {}
  virtual ~PPS() {}

  int pps_id; /// ue(v)
  int sps_id; /// ue(v)
  bool dependent_slice_segments_enabled; /// u(1)
  bool output_flag_present; /// u(1)
  int num_extra_slice_header_bits; /// u(3)
  bool sign_data_hiding_enabled; /// u(1)
  bool cabac_init_present; /// u(1)
  int num_ref_idx_default_active[2]; /// ue(v) + 1, l0 and l1
  int init_qp; /// se(v) + 26
  bool constrained_intra_pred_flag; /// u(1)
  bool transform_skip_enabled; /// u(1)
  bool cu_qp_delta_enabled; /// u(1)
  int diff_cu_qp_delta_depth; /// ue(v)
  int cb_qp_offset; /// se(v)
  int cr_qp_offset; /// se(v)
  bool slice_chroma_qp_offsets_present; /// u(1)
  bool weighted_pred_flag; /// u(1)
  bool weighted_bipred_flag; /// u(1)
  bool transquant_bypass_enabled; /// u(1)
  bool tiles_enabled; /// u(1)
  bool entropy_coding_sync_enabled; /// u(1)
  int num_tile_columns; /// ue(v) + 1
  int num_tile_rows; /// ue(v) + 1
  bool uniform_spacing_flag; /// u(1)
  int* column_width; /// ue(v) + 1
  int* row_height; /// ue(v) + 1
  bool loop_filter_across_tiles_enabled; /// u(1)
  bool loop_filter_across_slices_enabled; /// u(1)
  bool deblocking_filter_control_present; /// u(1)
  bool deblocking_filter_override_enabled /// u(1)
  bool deblocking_filter_disabled_flag; /// u(1)
  int beta_offset; /// se(v) * 2
  int tc_offset; /// se(v) * 2
  bool scaling_list_data_present; /// u(1)
  // scaling_list_data( )
  bool lists_modification_present; /// u(1)
  int log2_parallel_merge_level; /// ue(v) + 2
  bool slice_segment_header_extension_present; /// u(1)
  bool extension_flag; /// u(1)
  bool extension_data_flag; /// u(1)

private:
  PPS(const PPS&);
  void operator=(const PPS&);
};

} // namespace HEVC

#endif // _PPS_H_