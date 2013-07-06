#include "NAL.h"
#include "bitstream.h"
#include "macro.h"

#include <cassert>
#include <cstdlib>

namespace HEVC
{

NALUnit::NALUnit(NAL_UNIT_TYPE type, int temporal_id, int reserved_zero_6bits)
{
  _nal_unit_type = type;
  _temporal_id = temporal_id;
  _reserved_zero_6bits = reserved_zero_6bits;
}

bool NALUnit::is_slice() const
{
  switch(_nal_unit_type)
  {
    case NAL_UNIT_CODED_SLICE_TRAIL_R:
    case NAL_UNIT_CODED_SLICE_TRAIL_N:
    case NAL_UNIT_CODED_SLICE_TLA_R:
    case NAL_UNIT_CODED_SLICE_TSA_N:
    case NAL_UNIT_CODED_SLICE_STSA_R:
    case NAL_UNIT_CODED_SLICE_STSA_N:
    case NAL_UNIT_CODED_SLICE_BLA_W_LP:
    case NAL_UNIT_CODED_SLICE_BLA_W_RADL:
    case NAL_UNIT_CODED_SLICE_BLA_N_LP:
    case NAL_UNIT_CODED_SLICE_IDR_W_RADL:
    case NAL_UNIT_CODED_SLICE_IDR_N_LP:
    case NAL_UNIT_CODED_SLICE_CRA:
    case NAL_UNIT_CODED_SLICE_RADL_N:
    case NAL_UNIT_CODED_SLICE_RADL_R:
    case NAL_UNIT_CODED_SLICE_RASL_N:
    case NAL_UNIT_CODED_SLICE_RASL_R:
      return true;
    default:
      return false;
  }
}

bool NALUnit::is_SEI() const
{
  assert(L0045_NON_NESTED_SEI_RESTRICTIONS);
  switch(_nal_unit_type)
  {
    case NAL_UNIT_PREFIX_SEI:
    case NAL_UNIT_SUFFIX_SEI:
      return true;
    default:
      return false;
  }
}

bool NALUnit::is_VCL() const
{
  assert(L0045_NON_NESTED_SEI_RESTRICTIONS);
  return _nal_unit_type < 32;
}

InputNALUnit::InputNALUnit()
{
  _bitstream = NULL;
}

InputNALUnit::~InputNALUnit()
{
  if(_bitstream!=NULL) delete _bitstream;
}

void InputNALUnit::read(const std::deque<unsigned char>* buf)
{
  _bitstream = new InputBitstream(buf);
  read_NAL_unit_header();
  convert_payload_to_RBSP();
}

void InputNALUnit::read_NAL_unit_header()
{
  assert(_bitstream!=NULL);

  bool forbidden_zero_bit = _bitstream->read(1);
  assert(!forbidden_zero_bit);

  _nal_unit_type = static_cast<NAL_UNIT_TYPE>(_bitstream->read(6));
  _reserved_zero_6bits = _bitstream->read(6);
  _temporal_id = _bitstream->read(3) - 1;
  assert(_reserved_zero_6bits == 0);

  _RBSP_locations.push_back(&_bitstream->get_stream().at(3));

  if(_temporal_id > 0)
  {
    switch(_nal_unit_type)
    {
      case NAL_UNIT_CODED_SLICE_BLA_W_LP:
      case NAL_UNIT_CODED_SLICE_BLA_W_RADL:
      case NAL_UNIT_CODED_SLICE_BLA_N_LP:
      case NAL_UNIT_CODED_SLICE_IDR_W_RADL:
      case NAL_UNIT_CODED_SLICE_IDR_N_LP:
      case NAL_UNIT_CODED_SLICE_CRA:
      case NAL_UNIT_VPS:
      case NAL_UNIT_SPS:
      case NAL_UNIT_EOS:
      case NAL_UNIT_EOB: assert(0);
      default: break;
    }
  }
  else
  {
    switch(_nal_unit_type)
    {
      case NAL_UNIT_CODED_SLICE_TLA_R:
      case NAL_UNIT_CODED_SLICE_TSA_N:
      case NAL_UNIT_CODED_SLICE_STSA_R:
      case NAL_UNIT_CODED_SLICE_STSA_N: assert(0);
      default: break;
    }
  }
}

void InputNALUnit::convert_payload_to_RBSP()
{
  const std::deque<unsigned char>& stream = _bitstream->get_stream();
  assert(stream.empty()==false);
  _RBSP_locations.clear();

  int zero_count = 0;
  for(auto it=stream.cbegin(); it!=stream.cend(); it++)
  {
    assert(zero_count < 2 || *it > 0x02);

    if(zero_count == 2 && *it == 0x03)
    {
      // find emulation prevention three byte.
      it++;
      zero_count = 0;

      if(it == stream.cend()) break;
      assert(*it<0x04);

      _RBSP_locations.push_back(&(*it));
    }

    zero_count = (*it == 0x00) ? zero_count + 1 : 0;
  }

  bool is_VCL_NAL_UNIT = stream[0] & 64u;
  if(is_VCL_NAL_UNIT)
  {
    // remove cabac_zero_word from payload if present
    assert(0);
  }
}

OutputNALUnit::OutputNALUnit(NAL_UNIT_TYPE type,
                             int temporal_id, int reserved_zero_6bits)
: NALUnit(type, temporal_id, reserved_zero_6bits)
{
  _bitstream = new OutputBitstream;
}

OutputNALUnit::~OutputNALUnit()
{
  delete _bitstream;
}

} // namespace HEVC