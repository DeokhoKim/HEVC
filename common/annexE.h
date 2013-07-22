#ifndef _ANNEX_E_H_
#define _ANNEX_E_H_

#include <vector>

namespace HEVC
{

class InputBitstream;
class OutputBitstream;
class SubLayerHRD;

/// JCTVC-L1003 rev. 34.
/// Annex E.1.1 Video Usability Information (VUI) parameters syntax.
class VUI
{

};


/// JCTVC-L1003 rev. 34.
/// Annex E.1.2 Hypothetical Reference Decoder HRD parameters syntax.
/// A hypothetical decoder model that specifies constraints on the variability
/// of conforming NAL unit streams or conforming byte streams that an encoding
/// process may produce.
class HRD
{
public:
  HRD() {}
  virtual ~HRD() {}

  // common for all sub-layers
  class CPRMS
  {
  public:
    CPRMS() {}
    ~CPRMS() {}

    void read(InputBitstream* bs);
    void write(OutputBitstream* bs);

    bool nal_hrd_parameters_present; /// u(1)
    bool vcl_hrd_parameters_present; /// u(1)
    bool sub_pic_hrd_params_present; /// u(1)
    int tick_divisor; /// u(8) + 2
    int du_cpb_removal_delay_increment_length; /// u(5) + 1
    bool sub_pic_cpb_params_in_pic_timing_sei_flag; /// u(1)
    int dpb_output_delay_du_length; /// u(5) + 1
    int bit_rate_scale; /// u(4)
    int cpb_size_scale; /// u(4)
    int cpb_size_du_scale; /// u(4)
    int initial_cpb_removal_delay_length; /// u(5) + 1
    int au_cpb_removal_delay_length; /// u(5) + 1
    int dpb_output_delay_length; /// u(5) + 1
  };

  void read(InputBitstream* bs, bool cprms_present, int max_sub_layers);
  void write(OutputBitstream* bs, bool cprms_present);

private:
  CPRMS _cprms;
  std::vector<bool> _fixed_pic_rate_general_flag; /// u(1)
  std::vector<bool> _fixed_pic_rate_within_cvs_flag; /// u(1)
  std::vector<int> _elemental_duration_in_tc; /// ue(v) + 1
  std::vector<bool> _low_delay_hrd_flag; /// u(1)
  std::vector<int> _cpb_cnt; /// ue(v) + 1
  // sub_layer_hrd_parameters( i )
  // sub_layer_hrd_parameters( i )
};

class SubLayerHRD
{
public:
  SubLayerHRD() {}
  virtual ~SubLayerHRD() {}

private:
  SubLayerHRD(const SubLayerHRD&);
  void operator=(const SubLayerHRD&);

  std::vector<int> _bit_rate_value; /// ue(v) + 1
  std::vector<int> _cpb_size_vlaue; /// ue(v) + 1
  std::vector<int> _cpb_size_du_value; /// ue(v) + 1;
  std::vector<int> _bit_rate_du_value; /// ue(v) + 1;
  std::vector<bool> _cbr_flag; /// u(1)
};


} // namespace HEVC

#endif // _ANNEX_E_H_