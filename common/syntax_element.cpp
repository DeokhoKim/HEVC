#include "syntax_element.h"
#include "bitstream.h"

namespace HEVC
{

namespace SyntaxElement
{

static inline unsigned int convert_to_uint(int val)
{
  return (val<=0) ? -val<<1 : (val<<1) - 1;
}

static inline int convert_to_int(unsigned int val)
{
  return (val & 0x01) ? -int(val>>1) : int(val>>1);
}

unsigned int read_code(InputBitstream* bitstream, int len)
{
  assert(bitstream!=NULL);
  return bitstream->read(len);
}

unsigned int read_uvlc(InputBitstream* bitstream)
{
  assert(bitstream!=NULL);
  unsigned int code = bitstream->read(1);

  if(code!=0) return 0;

  int len = 0;
  unsigned int ret = 0;
  while((code & 0x01) == 0)
  {
    code = bitstream->read(1);
    len++;
  }

  ret = bitstream->read(len);
  ret += (1u << len) - 1;
  return ret;
}

int read_svlc(InputBitstream* bitstream)
{
  assert(bitstream!=NULL);
  unsigned int bits = bitstream->read(1);

  if(bits!=0) return 0;

  int len = 0;
  unsigned int ret = 0;

  while((bits & 0x01) == 0)
  {
    bits = bitstream->read(1);
    len++;
  }

  ret = bitstream->read(len);
  ret = convert_to_int(ret);
  return ret;
}

unsigned int read_flag(InputBitstream* bitstream)
{
  assert(bitstream!=NULL);
  return bitstream->read(1);
}

void write_code(OutputBitstream* bitstream, unsigned int code, int len)
{
  assert(bitstream!=NULL);
  assert(len > 0);
  bitstream->write(code, len);
}

void write_uvlc(OutputBitstream* bitstream, unsigned int code)
{
  assert(bitstream!=NULL);
  int len = 1;
  unsigned int tmp_code = ++code;
  assert(tmp_code!=0);

  while(tmp_code != 1)
  {
    tmp_code >>= 1;
    len += 2;
  }

  bitstream->write(0, len>>1);
  bitstream->write(code, (len+1)>>1);
}

void write_flag(OutputBitstream* bitstream, unsigned int code)
{
  assert(bitstream!=NULL);
  bitstream->write(code, 1);
}

void write_svlc(OutputBitstream* bitstream, int code)
{
  assert(bitstream!=NULL);
  write_uvlc(bitstream, convert_to_uint(code));
}

} // namespace SyntaxElement

} // namespace HEVC