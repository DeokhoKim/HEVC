#include "NAL.h"
#include "common/bitstream.h"
#include "common/syntax_element.h"

#include <cassert>
#include <iterator>
#include <list>

namespace HEVC
{

using namespace SyntaxElement;

void NALUnitHeader::read_header(InputBitstream* bs)
{
  assert(bs!=NULL);
  unsigned int code;
  code = read_flag(bs);
  assert(code==0); // forbidden zero bit
  _nal_unit_type = static_cast<NAL_UNIT_TYPE>(read_code(bs, 6));
  _nuh_layer_id = read_code(bs, 6);
  _nuh_temporal_id = read_code(bs, 3) - 1;
}

void NALUnitHeader::write_header(OutputBitstream* bs)
{
  assert(bs!=NULL);
  write_flag(bs, 0); // forbidden zero bit
  write_code(bs, static_cast<unsigned int>(_nal_unit_type), 6);
  write_code(bs, _nuh_layer_id, 6);
  write_code(bs, _nuh_temporal_id + 1, 3);
}

void NALUnit::read_rbsp()
{
  unsigned char* ix_beg = _rbsp_bytes.data();
  unsigned char* ix_end = ix_beg + _rbsp_bytes.size();

  std::list<std::vector<unsigned char>::iterator> positions;
  for(unsigned char* ix = ix_beg+3; ix<ix_end;)
  {
    static const int shift[4] = {2, 4, 4, 1};
    switch(*ix)
    {
      case 0x00:
      case 0x01:
      case 0x02:
      case 0x03:
        if(*(ix-3)==0x00 && *(ix-2)==0x00 && *(ix-1)==0x03)
        {
          std::vector<unsigned char>::iterator it = _rbsp_bytes.begin();
          std::advance(it, ix-ix_beg-1);
          positions.push_back(it);
          ix++;
          break;
        }
        ix+=shift[*ix];
        break;

      default:
        ix+=4;
        break;
    }
  }

  for(auto it=positions.rbegin(); it!=positions.rend(); it++)
  {
    _rbsp_bytes.erase(*it);
  }
}

void NALUnit::write_rbsp()
{
  if(_rbsp_bytes.size() < 3) return;

  for(auto it=_rbsp_bytes.begin()+2; it!=_rbsp_bytes.end();)
  {
    switch(*it)
    {
      case 0x00:
      case 0x01:
      case 0x02:
      case 0x03:
        if(*(it-1)==0x00 && *(it-2)==0x00)
        {
          _rbsp_bytes.insert(it, 0x03);
        }
        it++;
        break;

      default:
        if(std::distance(it, _rbsp_bytes.end())>=3)
        {
          std::advance(it, 3);
          break;
        }
        return;
    }
  }
}

void NALUnit::read()
{
  {
    InputBitstream bs(_rbsp_bytes.data(), _rbsp_bytes.size());
    read_header(&bs);
    _rbsp_bytes.erase(_rbsp_bytes.begin(), _rbsp_bytes.begin()+2);
  }
  read_rbsp();
}

void NALUnit::write()
{
  write_rbsp();
  {
    OutputBitstream bs;
    write_header(&bs);
    _rbsp_bytes.insert(_rbsp_bytes.begin(),
                       bs.get_stream().cbegin(), bs.get_stream().cend());
  }
}

} // namespace HEVC