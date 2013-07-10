#include "NAL.h"
#include "bitstream.h"
#include "macro.h"

#include <cassert>
#include <cstdlib>
#include <iterator>
#include <list>

namespace HEVC
{

NALUnit::NALUnit()
{
  _nal_unit_type = NAL_UNIT_INVALID;
  _temporal_id = 0;
  _reserved_zero_6bits = 0;
}

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

void NALUnit::read(NALUnit* nalu, const std::vector<unsigned char>* data)
{
  assert(nalu!=NULL);

  if(data!=NULL)
  {
    // copy data to _nal_unit_data
    nalu->_nal_unit_data.assign(data->begin(), data->end());
  }
  assert(nalu->_nal_unit_data.size()>2);

  bool is_VCL_NAL_UNIT = nalu->_nal_unit_data.front() & 64;
  read_NAL_unit_header(nalu);
  convert_payload_to_RBSP(&nalu->_nal_unit_data, is_VCL_NAL_UNIT);
}

void NALUnit::read_NAL_unit_header(NALUnit* nalu)
{
  assert(nalu != NULL);
  assert(!nalu->_nal_unit_data.empty());

  InputBitstream bitstream(nalu->_nal_unit_data.data(),
                           nalu->_nal_unit_data.size());

  bool forbidden_zero_bit = bitstream.read(1);
  assert(forbidden_zero_bit == 0);

  nalu->_nal_unit_type = static_cast<NAL_UNIT_TYPE>(bitstream.read(6));
  nalu->_reserved_zero_6bits = bitstream.read(6);
  nalu->_temporal_id = bitstream.read(3) - 1;
  assert(nalu->_reserved_zero_6bits == 0);

  nalu->_nal_unit_data.erase(nalu->_nal_unit_data.begin(),
                             nalu->_nal_unit_data.begin()+2);

  if(nalu->_temporal_id > 0)
  {
    switch(nalu->_nal_unit_type)
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
    switch(nalu->_nal_unit_type)
    {
      case NAL_UNIT_CODED_SLICE_TLA_R:
      case NAL_UNIT_CODED_SLICE_TSA_N:
      case NAL_UNIT_CODED_SLICE_STSA_R:
      case NAL_UNIT_CODED_SLICE_STSA_N: assert(0);
      default: break;
    }
  }
}

void NALUnit::convert_payload_to_RBSP(std::vector<unsigned char>* payload,
                                      bool is_VCL_NAL_UNIT)
{
  assert(payload!=NULL);

  // no emulation prevention three byte exists.
  if (payload->size() < 4) return;

  // find all locations of emulation prevention three byte with Wu-Manber
  std::list<std::vector<unsigned char>::iterator> three_bytes;
  for(auto it=payload->begin()+4; it!=payload->end();)
  {
    unsigned char rb = *it;
    unsigned char prefix;
    unsigned int buf;

    switch(rb)
    {
      case 0x03:
      case 0x02:
      case 0x01:
      case 0x00:
        prefix = *(it-3);
        if(prefix!=0x00) break;
        buf = *(it-2) << 8 | *(it-1);
        if(buf==0x0003) three_bytes.push_back(it-1);
        break;
      default:
        if(std::distance(it, payload->end()) < 4) break;
        std::advance(it, 4); continue;
    }

    it++;
  }

  for(auto it=three_bytes.rbegin(); it!=three_bytes.rend(); it++)
  {
    payload->erase(*it);
  }

  if(is_VCL_NAL_UNIT)
  {
    // Remove cabac_zero_word from payload if present
    int num_cabac_zero_word = 0;
    for(auto it=payload->rbegin(); it!=payload->rend(); it++)
    {
      if(*it!=0x00) break;
      num_cabac_zero_word++;
      payload->pop_back();
    }
  }
}

void NALUnit::write_NAL_unit_header(const NALUnit& nalu,
                                    std::vector<unsigned char>* out)
{
  assert(out!=NULL);
  OutputBitstream stream;

  stream.write(0,1); // forbidden zero bits
  stream.write(nalu._nal_unit_type, 6);
  stream.write(nalu._reserved_zero_6bits, 6);
  stream.write(nalu._temporal_id+1, 3);

  out->insert(out->end(), stream.get_stream().begin(),
                          stream.get_stream().end());
}

void NALUnit::write_NAL_unit_header(const NALUnit& nalu, std::ostream* out)
{
  assert(out!=NULL);
  OutputBitstream stream;

  stream.write(0,1); // forbidden zero bits
  stream.write(nalu._nal_unit_type, 6);
  stream.write(nalu._reserved_zero_6bits, 6);
  stream.write(nalu._temporal_id+1, 3);

  out->write(reinterpret_cast<const char*>(stream.get_stream().data()),
             stream.get_stream().size());

}

void NALUnit::convert_RBSP_to_payload(std::vector<unsigned char>* rbsp)
{
  assert(rbsp!=NULL);

  // Not enough size to have emulation three bytes
  if(rbsp->size() < 3) return;

  std::list<std::vector<unsigned char>::iterator> three_bytes;
  for(auto it=rbsp->begin()+2; it!=rbsp->end();)
  {
    unsigned char rbsp_data = *it;
    unsigned char prefix;
    unsigned int buf;
    switch(rbsp_data)
    {
      case 0x03:
      case 0x02:
      case 0x01:
      case 0x00:
          prefix = *(it-2);
          if(prefix!=0x00) break;
          buf = *(it-1) << 8 | rbsp_data;
          if(buf==0x0000)
          {
            three_bytes.push_back(it);
            if(std::distance(it, rbsp->end()) < 2) break;
            std::advance(it, 2); continue;
          }
        break;
      default:
        if(std::distance(it, rbsp->end()) < 3) break;
        std::advance(it, 3); continue;
    }

    it++;
  }

  for(auto it=three_bytes.rbegin(); it!=three_bytes.rend(); it++)
  {
    rbsp->insert(*it, 0x03);
  }

  if(rbsp->back() == 0x00)
    rbsp->push_back(0x03);
}

void NALUnit::write(NALUnit* nalu, std::vector<unsigned char>* out)
{
  assert(nalu!=NULL);
  assert(out!=NULL);
  write_NAL_unit_header(*nalu, out);
  convert_RBSP_to_payload(&nalu->_nal_unit_data);
  out->insert(out->end(), nalu->_nal_unit_data.begin(),
                          nalu->_nal_unit_data.end());
}

void NALUnit::write(NALUnit* nalu, std::ostream* out)
{
  assert(nalu!=NULL);
  assert(out!=NULL);
  write_NAL_unit_header(*nalu, out);
  convert_RBSP_to_payload(&nalu->_nal_unit_data);
  out->write(reinterpret_cast<char*>(nalu->_nal_unit_data.data()),
             nalu->_nal_unit_data.size());
}

} // namespace HEVC