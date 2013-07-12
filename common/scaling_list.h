#ifndef _SCALING_LIST_H_
#define _SCALING_LIST_H_

#include <cstdlib>
#include <cstring>

namespace HEVC
{
class ScalingList
{
public:
  ScalingList()
  {
    memset(pred_mode_flag, 0, 24*sizeof(bool));
    memset(pred_matrix_id_delta, 0, 24*sizeof(int));
    memset(dc_coef, 0, 12*sizeof(int));
    memset(scaling_list, 0, 1536*sizeof(int));
  }
  virtual ~ScalingList() {}

  bool pred_mode_flag[4][6];
  int pred_matrix_id_delta[4][6];
  int dc_coef[2][6];
  int scaling_list[4][6][64];

private:
  ScalingList(const ScalingList&);
  void operator=(const ScalingList&);
};

} // namespace HEVC

#endif // _SCALING_LIST_H_