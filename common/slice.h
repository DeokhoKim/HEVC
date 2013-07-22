#ifndef _SLICE_H_
#define _SLICE_H_

namespace HEVC
{

/// JCTVC-L1003 rev. 34.
/// 7.3.6.1 Slice segment header syntax.
class SliceSegmentHeader
{
public:
  SliceSegmentHeader() {}
  virtual ~SliceSegmentHeader() {}

  bool first_slice_segment_in_pic_flag; /// u(1)
  bool no_output_of_prior_pics_flag; /// u(1)
  int slice_pps_id; /// ue(v)
  bool dependent_slice_segment_flag; /// u(1)
  int slice_segment_address; /// u(v)
  bool *slice_reserved_flag; /// u(1)
  int slice_type; /// ue(v)
  bool pic_output_flag; /// u(1)
  int colour_plane_id; /// u(2)
  bool slice_pic_order_cnt_lsb; /// u(v)
  bool short_term_ref_pic_set_sps_flag; /// u(1)
  // short_term_ref_pic_set( num_short_term_ref_pic_sets )
  int short_term_ref_pic_set_idx; /// u(v)
  int num_long_term_sps; /// ue(v)
  int num_long_term_pics; /// ue(v)
  int* long_term_idx_sps; /// u(v)
  int* poc_lsb_long_term; /// u(v)
  bool* used_by_curr_pic_long_term_flag; /// u(1)
  bool* delta_poc_msb_present_flag; /// u(1)
  int* delta_poc_msb_cycle_long_term; /// ue(v)
  bool slice_temporal_mvp_enabled; /// u(1)
  bool slice_sao_luma_flag; /// u(1)
  bool num_ref_idx_active_override; /// u(1)
  int num_ref_idx_list_active[2]; /// ue(v) + 1, l0 and l1
  // ref_pic_lists_modification( )
  bool mvd_l1_zero_flag; /// u(1)
  bool cabac_init_flag; /// u(1)
  bool collocated_from_l0_flag; /// u(1)
  int collocated_ref_idx; /// ue(v)
  // pred_weight_table( )
  int five_minus_max_num_merge_cand; /// ue(v)
  int slice_qp_delta; /// se(v)
  int slice_cb_qp_offset; /// se(v)
  bool deblocking_filter_override; /// u(1)
  bool slice_deblocking_filter_disabled; /// u(1)
  int slice_beta_offset; /// se(v) * 2
  int slice_tc_offset; /// se(v) * 2
  bool slice_loop_filter_across_slices_enabled; /// u(1)
  int num_entry_point_offsets; /// ue(v)
  int offset_len; /// ue(v) + 1
  int* entry_point_offset; /// u(v) + 1
  int slice_segment_header_extension_length; /// ue(v)
  int* slice_segment_header_extension_data_byte; /// u(8)

private:
  SliceSegmentHeader(const SliceSegmentHeader&);
  void operator=(const SliceSegmentHeader&);
};

/// JCTVC-L1003 rev. 34.
/// 7.3.6.2 Reference picture list modification syntax.
class RPLModification
{
public:
  RPLModification() {}
  virtual ~RPLModification() {}

  bool ref_pic_list_modification_flag[2]; /// u(1), l0 and l1
  int* list_entry[2]; /// u(v), l0 and l1

private:
  RPLModification(const RPLModification&);
  void operator=(const RPLModification&);
};

/// JCTVC-L1003 rev. 34.
/// 7.3.6.3 Weighted prediction parameters syntax.
class PredWeightTable
{
public:
  PredWeightTable() {}
  virtual ~PredWeightTable() {}

  int luma_log2_weight_denom; /// ue(v)
  int delta_chroma_log2_weight_denom; /// se(v)
  bool* luma_weight_flag[2]; /// u(1), l0 and l1
  bool* chroma_weight_flag[2]; /// u(1), l0 and l1
  int* delta_luma_weight[2]; /// se(v), l0 and l1
  int* luma_offset[2]; /// se(v), l0 and l1

  int (*delta_chroma_weight[2])[2]; /// se(v), l0 and l1
  int (*delta_chroma_offset[2])[2]; /// se(v), l0 and l1

private:
  PredWeightTable(const PredWeightTable&);
  void operator=(const PredWeightTable&);
};

/// JCTVC-L1003 rev. 34.
/// 7.3.7 Short-term reference picture set syntax.
class SRPS
{
public:
  SRPS() {}
  virtual ~SRPS() {}

  bool inter_ref_pic_set_prediction_flag; /// u(1)
  int delta_idx; /// ue(v) + 1
  bool delta_rps_sign; /// u(1)
  int abs_delta_rps; /// ue(v) + 1
  bool* used_by_curr_pic_flag; /// u(1)
  bool* use_delta_flag; /// u(1)
  int num_negative_pics; /// ue(v)
  int num_positive_pics; /// ue(v)
  int* delta_poc[2]; /// ue(v) + 1, s0 and s1
  bool* used_by_curr_pic_flag[2]; /// u(1) s0 and s1

private:
  SRPS(const SRPS&);
  void operator=(const SRPS&);
};


} // namespace HEVC

#endif // _SLICE_H_