#ifndef _ENTROPY_DECODER_H_
#define _ENTROPY_DECODER_H_

#include <cassert>

namespace HEVC
{

class InputBitstream;
class PPS;
class PTL;
class SPS;
class VPS;

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
};

}
#endif