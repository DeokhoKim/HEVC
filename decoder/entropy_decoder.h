#ifndef _ENTROPY_DECODER_H_
#define _ENTROPY_DECODER_H_

#include <cassert>

namespace HEVC
{

class HRD;
class InputBitstream;
class PPS;
class PTL;
class ScalingList;
class SPS;
class SubLayerHRD;
class VPS;
class VUI;

class EntropyDecIf
{
public:
  EntropyDecIf() {}
  virtual ~EntropyDecIf() {}

  virtual void decode_vps(InputBitstream* bs, VPS* vps) const = 0;
  virtual void decode_sps(InputBitstream* bs, SPS* sps) const = 0;
  virtual void decode_pps(InputBitstream* bs, PPS* sps) const = 0;
private:
  virtual void decode_ptl(InputBitstream* bs,
                          PTL* ptl, int max_sub_layers) const = 0;
  virtual void decode_scaling_list(InputBitstream* bs,
                                   ScalingList* sl) const = 0;
  virtual void decode_vui(InputBitstream* bs, VUI* vui) const = 0;
  virtual void decode_hrd(InputBitstream* bs, HRD* hrd,
                          bool common_info_present,
                          int max_sub_layers) const = 0;
  virtual void decode_sub_layer_hrd(InputBitstream* bs, HRD* hrd,
                                    SubLayerHRD* sub_hrd, int ix) const = 0;
};

}
#endif