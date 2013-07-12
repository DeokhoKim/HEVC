#ifndef _VPS_H_
#define _VPS_H_

#include "PTL.h"

#include <cstdlib>

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
    cprms_present = NULL;
  }

  virtual ~VPS()
  {
    if(max_dec_pic_buffering != NULL) delete[] max_dec_pic_buffering;
    if(max_num_reorder_pics != NULL) delete[] max_num_reorder_pics;
    if(max_latency_increase != NULL) delete[] max_latency_increase;
    if(layer_id_included_flag != NULL) delete[] layer_id_included_flag;
    if(hrd_layer_set_idx != NULL) delete[] hrd_layer_set_idx;
    if(cprms_present != NULL) delete[] cprms_present;
  }

  int vps_id;
  int max_layers;
  int max_sub_layers;
  bool temporal_id_nesting_flag;
  PTL profile_tier_level;
  bool sub_layer_ordering_info_present;
  int* max_dec_pic_buffering;
  int* max_num_reorder_pics;
  int* max_latency_increase;
  int max_layer_id;
  int num_layer_sets;
  bool* layer_id_included_flag;
  bool timing_info_present;
  unsigned int num_units_in_tick;
  unsigned int time_scale;
  bool poc_proportional_to_timing_flag;
  int num_ticks_poc_diff_one;
  int num_hrd_parameters;
  int* hrd_layer_set_idx;
  bool* cprms_present;
  // int     hrd_parameters( cprms_present_flag[ i ], max_sub_layers; )
  bool extension_flag;
  bool extension_data_flag;
private:
  VPS(const VPS&);
  void operator=(const VPS&);
};

} // namespace HEVC

#endif // _VPS_H_