#ifndef _ENTROPY_DECODER_H_
#define _ENTROPY_DECODER_H_

#include <cassert>

namespace HEVC
{

class VPS;

class EntropyDecIf
{
public:
  EntropyDecIf() { assert(0); }
  virtual ~EntropyDecIf() { assert(0); }

  virtual void parse_vps(VPS* vps) = 0;
  virtual void parse_sps(SPS* sps) = 0;
  virtual void parse_pps(PPS* sps) = 0;

};

}
#endif