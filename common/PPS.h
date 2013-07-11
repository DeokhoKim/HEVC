#ifndef _PPS_H_
#define _PPS_H_

#include <vector>

namespace HEVC
{

/// Data object: Picture Parameter Set (PPS) syntax based on JCTVC-L1003
/// rev. 34.
class PPS
{
public:
  PPS()
  {
    column_width = NULL;
    row_height = NULL;
  }

  virtual ~PPS()
  {
    if(column_width != NULL) delete column_width;
    if(row_height != NULL) delete row_height;
  }

  int pps_id; ///< pps_pic_parameter_set_id
  int sps_id; ///< pps_seq_parameter_set_id
  bool dependent_slice_segment_enabled_flag;
  bool output_flag_present_flag;
  int num_extra_slice_header_bits;
  bool sign_data_hiding_enabled_flag;
  bool cabac_init_present_flag;
  int num_ref_idx_l0_default_active; ///< num_ref_idx_l0_default_active_minus1+1
  int num_ref_idx_l1_default_active; ///< num_ref_idx_l1_default_active_minus1+1
  int init_qp; ///< init_qp_minus26 + 26;
  bool constrained_intra_pred_flag;
  bool transform_skip_enabled_flag;
  bool cu_qp_delta_enabled_flag;
  // if(cu_qp_delta_enabled_flag)
  int diff_cu_qp_delta_depth;
  int cb_qp_offset; ///< pps_cb_qp_offset
  int cr_qp_offset; ///< pps_cr_qp_offset
  bool slice_chroma_qp_offsets_present_flag;
  ///< pps_slice_chroma_qp_offsets_present_flag
  bool weighted_pred_flag;
  bool weighted_bipred_flag;
  bool transquant_bypass_enabled_flag;
  bool tiles_enabled_flag;
  bool entropy_codign_sync_enabled_flag;
  // if(tiles_enabled_flag)
  // {
  int num_tile_columns; ///< num_tile_columns_minus1 + 1
  int num_tile_rows; ///< num_tile_rows_minus1 + 1
  bool uniform_spacing_flag;
  // if(!uniform_spacing_flag)
  // {
  // for(i=0; i<num_tile_columns_minus1; i++)
  int* column_width; ///< column_width_minus1[i] + 1
  // for(i=0; i<num_tile_rows_minus1; i++)
  int* row_height; ///< row_height_minus1[i] + 1
  // }
  bool loop_filter_accross_tiles_enabled_flag;
  // }
  bool loop_filter_accross_slices_enabled_flag;
  ///< pps_loop_filter_accross_slices_enabled_flag
  bool deblocking_filter_control_present_flag;
  // if(deblocking_filter_control_present_flag)
  // {
  bool deblocking_filter_overrid_enabled_flag;
  bool deblocking_filter_disabled_flag; ///< pps_deblocking_filter_disabled_flag
  // if(!pps_deblocking_filter_disabled_flag)
  // {
  int beta_offset; ///< pps_beta_offset_div2 * 2
  int tc_offset; ///< pps_tc_offset_div2 * 2
  // }
  // }
  bool scaling_list_data_present_flag; ///< pps_scaling_list_data_present_flag
  // if(pps_scaling_list_data_present_flag)
  // scaling_list_data()
  bool lists_modification_present_flag;
  int log2_parallel_merge_level; ///< log2_parallel_merge_level_minus2 + 2
  bool slice_segment_header_extension_present_flag;
  bool extension_flag; ///< pps_extension_flag
  // if(pps_extension_flag)
  //  while(more_rbsp_data())
  bool extension_data_flag; ///< pps_extension_data_flag;
  // rbsp_trailing_bits()

private:
  PPS(const PPS&);
  void operator=(const PPS&);
};

} // namespace HEVC

#endif // _PPS_H_