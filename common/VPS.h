#ifndef _VPS_H_
#define _VPS_H_

#include "PTL.h"

#include <vector>

namespace HEVC
{

/// Video Parameter Set(VPS) data based on JCTVC-L1003 rev. 34 VPS RBSP syntax
class VPS
{
public:
  VPS()
  {
    max_dec_pic_buffering = NULL;
    max_num_reorder_pics = NULL;
    max_latency_increase = NULL;
    layer_id_included_flag = NULL;
    hrd_layer_set_idx = NULL;
    cprms_present_flag = NULL;
  }

  virtual ~VPS()
  {
    if(max_dec_pic_buffering != NULL) delete [] max_dec_pic_buffering;
    if(max_num_reorder_pics != NULL) delete [] max_num_reorder_pics;
    if(max_latency_increase != NULL) delete [] max_latency_increase;
    if(layer_id_included_flag != NULL) delete [] layer_id_included_flag;
    if(hrd_layer_set_idx != NULL) delete [] hrd_layer_set_idx;
    if(cprms_present_flag != NULL) delete [] cprms_present_flag;
  }

  int vps_id; ///< vips_video_parameter_set_id
  int max_layers; ///< vps_max_layers_minus1 + 1
  int max_sub_layers; ///< vps_max_sub_layers_minus1 + 1
  bool temporal_id_nesting_flag; ///< vps_temporal_id_nesting_flag

  PTL profile_tier_level;

  bool sub_layer_ordering_info_present_flag;
  ///< vps_sub_layers_ordering_info_present_flag
  // for(i=vps_sub_layer_ordering_info_present_flag ? 0 :
  //       vps_max_sub_layers_minus1;
  //     i<=vps_max_sub_layers_minus1; i++)
  // {
  int* max_dec_pic_buffering; ///< vps_max_dec_pic_buffering_minus1[i] + 1
  int* max_num_reorder_pics; ///< vps_max_num_reorder_pics[i];
  int* max_latency_increase; ///< vps_max_latency_increase_plus1[i] + 1
  // }
  int max_layer_id; ///< vps_max_layer_id
  int num_layer_sets; ///< vps_num_sets_minus1 + 1
  // for(i=1; i<=vps_num_layer_sets_minus1; i++)
  //   for(j=0; j<=vps_max_layer_id;j++)
  //     layer_id_included_flag[i][j];
  int* layer_id_included_flag;

  bool timing_info_present_flag; ///< vps_timing_info_present_flag
  // if(vps_timing_info_present_flag)
  // {
  unsigned int num_units_in_tick; ///< vps_num_units_in_tick
  unsigned int time_scale; ///< vps_time_scale
  bool poc_proportional_to_timing_flag; ///< vps_poc_proportional_to_timing_flag
  // if(vps_poc_proportional_to_timing_flag)
  int num_ticks_poc_diff_one; ///< vps_num_ticks_poc_diff_one_minus1 + 1
  // }
  int num_hrd_parameters; ///< vps_num_hrd_parameters
  // for(i=0; i<vps_num_hrd_parameters; i++)
  // {
  int* hrd_layer_set_idx; ///< hrd_layer_set_idx[i]
  // if(i>0)
  bool* cprms_present_flag; ///< cprms_present_flag[i]
  // hrd_parameters(cprms_present_flag[i], vps_max_sub_layers_minus1)
  // }
  // }

  bool extension_flag; ///< vps_extension_flag
  // if(vps_extension_flag)
  //   while(more_rbsp_data())
  bool extenson_data_flag; ///< vps_extension_data_flag
  // rbsp_trailing_bits()

private:
  VPS(const VPS&);
  void operator=(const VPS&);
};

} // namespace HEVC

#endif // _VPS_H_