#ifndef _DEC_CAVLC_H_
#define _DEC_CAVLC_H_

#include "entropy_decoder.h"

namespace HEVC
{

class PPS;
class PTL;
class SPS;
class VPS;

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
};

} // namespaces HEVC

#endif // _DEC_CAVLC_H_