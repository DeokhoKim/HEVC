#include "common/bytestream.h"
#include "common/access_unit.h"
#include "common/NAL.h"

#include <cassert>
#include <cstdlib>
#include <vector>

namespace HEVC
{

namespace Bytestream
{

static bool eof_before_n_bytes(std::istream* in, int n)
{
  assert(in!=NULL);
  assert(n>0 && n<5);
  if(in->eof() | in->fail() | in->bad()) return true;

  bool ret = false;
  const std::streampos cur = in->tellg();
  in->seekg(n, in->cur);

  ret |= in->eof() | in->fail() | in->bad();
  if(ret) in->clear();
  in->seekg(cur);

  return ret;
}

static unsigned int peek_bytes(std::istream* in, int n)
{
  assert(in!=NULL);
  assert(n>0 && n<5);
  if(in->eof() | in->fail() | in->bad()) return 0;

  const std::streampos cur = in->tellg();
  char buf[4] = {0};
  in->read(buf, n);

  if(in->eof() | in->fail() | in->bad()) in->clear();
  in->seekg(cur);

  unsigned int ret = 0;
  for(int i=0; i<n; i++)
  {
    ret = (ret << 8) | static_cast<unsigned char>(buf[i]);
  }
  return ret;
}

void bytestream_to_NAL_unit(std::istream* in, NALUnit* nalu)
{
  assert(in!=NULL);
  assert(nalu!=NULL);

  while(true)
  {
    char byte = in->get();
    if(in->eof()) return;
    if(byte==0x01) break;
    assert(byte==0x00);
  }

  std::vector<unsigned char>& data = nalu->get_rbsp();
  data.clear();

  while(!eof_before_n_bytes(in, 3))
  {
    unsigned int bytes = peek_bytes(in, 3);

    switch(bytes & 0x0FF)
    {
      case 0x00:
        if(eof_before_n_bytes(in, 4) || peek_bytes(in, 4) == 0x00000001) break;
        data.push_back(in->get());
        continue;
      case 0x01:
        if(bytes == 0x000001) break;
        data.push_back(in->get());
        continue;
      default:
        data.push_back(in->get());
        data.push_back(in->get());
        data.push_back(in->get());
        continue;
    }
    break;
  }

  while(eof_before_n_bytes(in, 3) ||
        (eof_before_n_bytes(in, 4) && peek_bytes(in, 3)!=0x000001))
  {
    char byte = in->get();
    if(in->eof()) break;
    data.push_back(byte);
  }

  while(data.back()==0x00) data.pop_back();
}

void NAL_units_to_bytestream(std::ostream* out, NALUnit* nalu, bool zero_byte)
{
    assert(out!=NULL);
    assert(nalu!=NULL);

    static const char start_prefix_code[] = {0x00, 0x00, 0x01};
    if(zero_byte) out->put(0x00);
    out->write(start_prefix_code, 3);
    out->write(reinterpret_cast<const char*>(nalu->get_rbsp().data()),
               nalu->get_rbsp().size());
}

} // namespace Bytestream

} // namespace HEVC