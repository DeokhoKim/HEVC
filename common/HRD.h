#ifndef _HRD_H_
#define _HRD_H_

#include <cstdlib>

namespace HEVC
{

class SubLayerHRD
{
public:
  SubLayerHRD()
  {
    bit_rate_value = NULL;
    cpb_size_value = NULL;
    cpb_size_du_value = NULL;
    bit_rate_du_value = NULL;
    cbr_flag = NULL;
  }
  virtual ~SubLayerHRD()
  {
    if(bit_rate_value!=NULL) delete[] bit_rate_value;
    if(cpb_size_value!=NULL) delete[] cpb_size_value;
    if(cpb_size_du_value!=NULL) delete[] cpb_size_du_value;
    if(bit_rate_du_value!=NULL) delete[] bit_rate_du_value;
    if(cbr_flag!=NULL) delete[] cbr_flag;
  }
  int* bit_rate_value;
  int* cpb_size_value;
  int* cpb_size_du_value;
  int* bit_rate_du_value;
  bool* cbr_flag;
private:
  SubLayerHRD(const SubLayerHRD&);
  void operator=(const SubLayerHRD&);
};


class HRD
{
public:
  HRD()
  {
    fixed_pic_rate_general_flag = NULL;
    fixed_pic_rate_within_cvs_flag = NULL;
    elemental_duration_in_tc = NULL;
    low_delay_hrd_flag = NULL;
    cpb_cnt = NULL;
    sub_layer_hrd_parameters = NULL;
  }
  virtual ~HRD()
  {
    if(fixed_pic_rate_general_flag != NULL)
      delete[] fixed_pic_rate_general_flag;
    if(fixed_pic_rate_within_cvs_flag != NULL)
      delete[] fixed_pic_rate_within_cvs_flag;
    if(elemental_duration_in_tc != NULL)
      delete[] elemental_duration_in_tc;
    if(low_delay_hrd_flag != NULL)
      delete[] low_delay_hrd_flag;
    if(cpb_cnt != NULL)
      delete[] cpb_cnt;
    if(sub_layer_hrd_parameters != NULL)
      delete[] sub_layer_hrd_parameters;
  }

  bool nal_hrd_parameters_present;
  bool vcl_hrd_parameters_present;
  bool sub_pic_hrd_params_present;
  int tick_divisor;
  int du_cpb_removal_delay_increment_length;
  bool sub_pic_cpb_params_in_pic_timing_sei_flag;
  int dpb_output_delay_du_length;
  int bit_rate_scale;
  int cpb_size_scale;
  int cpb_size_du_scale;
  int initial_cpb_removal_delay_length;
  int au_cpb_removal_delay_length;
  int dpb_output_delay_length;
  bool* fixed_pic_rate_general_flag;
  bool* fixed_pic_rate_within_cvs_flag;
  int* elemental_duration_in_tc;
  bool* low_delay_hrd_flag;
  int* cpb_cnt;

  SubLayerHRD* sub_layer_hrd_parameters;

private:
  HRD(const HRD&);
  void operator=(const HRD&);
};

} // namespace HRD

#endif // _HRD_H_