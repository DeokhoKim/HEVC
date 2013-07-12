#ifndef _DEC_CAVLC_H_
#define _DEC_CAVLC_H_

#include "entropy_decoder.h"

namespace HEVC
{

class CAVLCDecoder: public EntropyDecIf
{
public:
  CAVLCDecoder() {}
  virtual ~CAVLCDecoder() {}

  void decode_vps(InputBitstream* bs, VPS* vps) const;
  void decode_sps(InputBitstream* bs, SPS* sps) const;
  void decode_pps(InputBitstream* bs, PPS* pps) const;

private:
  CAVLCDecoder(const CAVLCDecoder&);
  void operator=(const CAVLCDecoder&);

  void decode_ptl(InputBitstream* bs, PTL* ptl, int max_sub_layers) const;
  void decode_scaling_list(InputBitstream* bs, ScalingList* sl) const;
  void decode_vui(InputBitstream* bs, VUI* vui) const;
  void decode_hrd(InputBitstream* bs, HRD* hrd,
                  bool common_info_present, int max_sub_layers) const;
  void decode_sub_layer_hrd(InputBitstream* bs, HRD* hrd,
                            SubLayerHRD* sub_hrd, int ix) const;
};

} // namespaces HEVC

#endif // _DEC_CAVLC_H_