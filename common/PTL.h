#ifndef _PROFILE_TIER_LEVEL_H_
#define _PROFILE_TIER_LEVEL_H_

#include <cassert>
#include <vector>

namespace HEVC
{

class InputBitstream;
class OutputBitstream;

/// JCTVC-L1003 rev. 34.
/// 7.3.3 Profile, tier and level syntax.
class PTL
{
public:
  PTL(int max_sub_layers)
  {
    assert(max_sub_layers>=1);
    _sub_layer_profile_present.resize(max_sub_layers-1);
    _sub_layer_level_present.resize(max_sub_layers-1);
    _sub_layer_profile.assign(max_sub_layers-1, NULL);
    _sub_layer_level_idc.resize(max_sub_layers-1);
  }
  virtual ~PTL()
  {
    for(auto it=_sub_layer_profile.begin(); it!=_sub_layer_profile.end(); it++)
    {
      if(*it!=NULL) delete *it;
    }
  }

  void read(InputBitstream* bs);
  void write(OutputBitstream* bs);

  class SubLayerProfile
  {
  public:
    SubLayerProfile() {}
    ~SubLayerProfile() {}

    void read(InputBitstream* bs);
    void write(OutputBitstream* bs);

    int profile_space; /// u(2)
    bool tier_flag; /// u(1)
    int profile_idc; /// u(5)
    bool profile_compatibility_flag[32]; /// u(1)
    bool progressive_source_flag; /// u(1)
    bool interlaced_source_flag; /// u(1)
    bool non_packed_constraint_flag; /// u(1)
    bool frame_only_constraint_flag; /// u(1)
  private:
    SubLayerProfile(const SubLayerProfile&);
    void operator=(const SubLayerProfile&);
  };

private:
  PTL(const PTL&);
  void operator=(const PTL&);

  int _general_profile_space; /// u(2)
  bool _general_tier_flag; /// u(1)
  int _general_profile_idc; /// u(5)
  bool _general_profile_compatibility_flag[32]; /// u(1)
  bool _general_progressive_source_flag; /// u(1)
  bool _general_interlaced_source_flag; /// u(1)
  bool _general_non_packed_constraint_flag; /// u(1)
  bool _general_frame_only_constraint_flag; /// u(1)
  int _general_level_idc; /// u(8)

  std::vector<bool> _sub_layer_profile_present; /// u(1)
  std::vector<bool> _sub_layer_level_present; /// u(1)
  std::vector<SubLayerProfile*> _sub_layer_profile;
  std::vector<int> _sub_layer_level_idc; /// u(8)
};

} // namespace HEVC

#endif // _PROFILE_TIER_LEVEL_H_