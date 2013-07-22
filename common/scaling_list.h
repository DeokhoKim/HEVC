#ifndef _SCALING_LIST_H_
#define _SCALING_LIST_H_

namespace HEVC
{

/// JCTVC-L1003 rev. 34.
/// 7.3.4 Scaling list data syntax.
class ScalingList
{
public:
  ScalingList() {}
  virtual ~ScalingList() {}

  bool scaling_list_pred_mode_flag[4][6]; /// u(1)
  int scaling_list_pred_matrix_id_delta[4][6]; /// ue(v)
  int scaling_list_dc_coef[2][6]; /// se(v) + 8
  int scaling_list_delta_coef; /// se(v)

private:
  ScalingList(const ScalingList&);
  void operator=(const ScalingList&);
};

} // namespace HEVC

#endif // _SCALING_LIST_H_