#include "NAL.h"

#include <cassert>

namespace HEVC
{

NALUnitHeader::NALUnitHeader(NALUnitHeader type,
                             int temporal_id, int reserved_zero_6bits)
{
  _nal_unit_type = type;
  _temporal_id = temporal_id;
  _reserved_zero_6bits = reserved_zero_6bits;
}

bool NALUnitHeader::is_slice() const
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

bool NALUnitHeader::is_SEI() const
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

bool NALUnitHeader::is_VCL() const
{
  assert(L0045_NON_NESTED_SEI_RESTRICTIONS);
  return _nal_unit_type < 32u;
}

} // namespace HEVC