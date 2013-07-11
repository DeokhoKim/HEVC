#ifndef _PROFILE_AND_TIER_LEVEL_H_
#define _PROFILE_AND_TIER_LEVEL_H_

#include <cstdlib>

namespace HEVC
{

/// Data object: Profile, tier and level syntax based on JCTVC-L1003 rev. 34.
class PTL
{
public:
  PTL()
  {
    sub_layer_profile_present_flag = NULL;
    sub_layer_level_present_flag = NULL;
    sub_layer_profile_space = NULL;
    sub_layer_tier_flag = NULL;
    sub_layer_profile_idc = NULL;
    sub_layer_profile_compatibility_flag = NULL;
    sub_layer_progressive_source_flag = NULL;
    sub_layer_interlaced_source_flag = NULL;
    sub_layer_non_packed_constraint_flag = NULL;
    sub_layer_frame_only_constraint_flag = NULL;
    sub_layer_level_idc = NULL;
  }

  virtual ~PTL()
  {
    if(sub_layer_profile_present_flag != NULL)
      delete[] sub_layer_profile_present_flag;
    if(sub_layer_level_present_flag != NULL)
      delete[] sub_layer_level_present_flag;
    if(sub_layer_profile_space != NULL)
      delete[] sub_layer_profile_space;
    if(sub_layer_tier_flag != NULL)
      delete[] sub_layer_tier_flag;
    if(sub_layer_profile_idc != NULL)
      delete[] sub_layer_profile_idc;
    if(sub_layer_profile_compatibility_flag != NULL)
      delete[] sub_layer_profile_compatibility_flag;
    if(sub_layer_progressive_source_flag != NULL)
      delete[] sub_layer_progressive_source_flag;
    if(sub_layer_interlaced_source_flag != NULL)
      delete[] sub_layer_interlaced_source_flag;
    if(sub_layer_non_packed_constraint_flag != NULL)
      delete[] sub_layer_non_packed_constraint_flag;
    if(sub_layer_frame_only_constraint_flag != NULL)
      delete[] sub_layer_frame_only_constraint_flag;
    if(sub_layer_level_idc != NULL)
      delete[] sub_layer_level_idc;
  }

  int general_profile_space;
  bool general_tier_flag;
  int general_profile_idc;
  bool general_profile_compatibility_flag[32];
  bool general_progressive_source_flag;
  bool general_interlaced_source_flag;
  bool general_non_packed_constraint_flag;
  bool general_frame_only_constraint_flag;
  // int general_reserved_zero_44bits;
  int general_level_idc;
  bool* sub_layer_profile_present_flag;
  bool* sub_layer_level_present_flag;
  // int* reserved_zero_bits;
  int* sub_layer_profile_space;
  bool* sub_layer_tier_flag;
  int* sub_layer_profile_idc;
  bool* sub_layer_profile_compatibility_flag;
  bool* sub_layer_progressive_source_flag;
  bool* sub_layer_interlaced_source_flag;
  bool* sub_layer_non_packed_constraint_flag;
  bool* sub_layer_frame_only_constraint_flag;
  // int* sub_layer_reserved_zero_44bits;
  int* sub_layer_level_idc;
private:
  PTL(const PTL&);
  void operator=(const PTL&);
};

} // namespace HEVC

#endif // _PROFILE_AND_TIER_LEVEL_H_