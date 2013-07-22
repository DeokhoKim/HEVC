#ifndef _VPS_H_
#define _VPS_H_

#include <cstdlib>
#include <vector>

#define MAX_VPS_NUH_RESERVED_ZERO_LAYER_ID_PLUS1 1

namespace HEVC
{

class InputBitstream;
class OutputBitstream;
class PTL;
class HRD;

/// JCTVC-L1003 rev. 34.
/// 7.3.2.1 Video parameter set RBSP syntax.
class VPS
{
public:
  VPS();
  virtual ~VPS();

  void read(InputBitstream* bs);
  void write(OutputBitstream* bs);

  class SubLayerOrderingInfo
  {
  public:
    SubLayerOrderingInfo() {}
    ~SubLayerOrderingInfo() {}

    void read(InputBitstream* bs);
    void write(OutputBitstream* bs);

    int max_dec_pic_buffering; /// ue(v) + 1
    int max_num_reorder_pics; /// ue(v)
    int max_latency_increase; /// ue(v) - 1
  private:
    SubLayerOrderingInfo(const SubLayerOrderingInfo&);
    void operator=(const SubLayerOrderingInfo&);
  };

  class TimingInfo
  {
  public:
    TimingInfo() {};
    ~TimingInfo() {};

    void read(InputBitstream* bs, int max_sub_layers);
    void write(OutputBitstream* bs);

    unsigned int num_units_in_tick; /// u(32)
    unsigned int time_scale; /// u(32)
    bool poc_proportional_to_timing_flag; // u(1)
    int num_ticks_poc_diff_one; /// ue(v) + 1
    int num_hrd_parameters; /// ue(v)
    std::vector<int> hrd_layer_set_idx; /// ue(v)
    std::vector<bool> cprms_present; // u(1)
    std::vector<HRD*> hrd_parameters;
  private:
    TimingInfo(const TimingInfo&);
    void operator=(const TimingInfo&);
  };

private:
  VPS(const VPS&);
  void operator=(const VPS&);

  int _vps_id; /// u(4)
  int _max_layers; /// u(6) + 1
  int _max_sub_layers; /// u(3) + 1
  bool _temporal_id_nesting_flag; /// u(1)
  PTL* _profile_tier_level;
  bool _sub_layer_ordering_info_present; /// u(1)
  std::vector<SubLayerOrderingInfo*> _sub_layer_ordering_info;

  int _max_layer_id; /// u(6)
  int _num_layer_sets; /// ue(v) + 1
  bool (*_layer_id_included_flag)[MAX_VPS_NUH_RESERVED_ZERO_LAYER_ID_PLUS1];
  bool _timing_info_present; /// u(1)
  TimingInfo* _timing_info;

  bool _extension_flag; /// u(1)
  bool _extension_data_flag; /// u(1)
};

} // namespace HEVC

#endif // _VPS_H_