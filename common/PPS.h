#ifndef _PPS_H_
#define _PPS_H_

#include "scaling_list.h"

#include <cstdlib>

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
    scaling_list = NULL;
  }

  virtual ~PPS()
  {
    if(column_width != NULL) delete[] column_width;
    if(row_height != NULL) delete[] row_height;
    if(scaling_list!=NULL) delete scaling_list;
  }

  int pps_id;
  int sps_id;
  bool dependent_slice_segments_enabled;
  bool output_flag_present;
  int num_extra_slice_header_bits;
  bool sign_data_hiding_enabled;
  bool cabac_init_present;
  int num_ref_idx_l0_default_active;
  int num_ref_idx_l1_default_active;
  int init_qp;
  bool constrained_intra_pred_flag;
  bool transform_skip_enabled;
  bool cu_qp_delta_enabled;
  int diff_cu_qp_delta_depth;
  int cb_qp_offset;
  int cr_qp_offset;
  bool slice_chroma_qp_offsets_present;
  bool weighted_pred_flag;
  bool weighted_bipred_flag;
  bool transquant_bypass_enabled;
  bool tiles_enabled;
  bool entropy_coding_sync_enabled;
  int num_tile_columns;
  int num_tile_rows;
  bool uniform_spacing_flag;
  int* column_width;
  int* row_height;
  bool loop_filter_across_tiles_enabled;
  bool loop_filter_across_slices_enabled;
  bool deblocking_filter_control_present;
  bool deblocking_filter_override_enabled;
  bool deblocking_filter_disabled;
  int beta_offset;
  int tc_offset;
  bool scaling_list_data_present;
  ScalingList* scaling_list;
  bool lists_modification_present;
  int log2_parallel_merge_level;
  bool slice_segment_header_extension_present;
  bool extension_flag;
  bool extension_data_flag;

private:
  PPS(const PPS&);
  void operator=(const PPS&);
};

} // namespace HEVC

#endif // _PPS_H_