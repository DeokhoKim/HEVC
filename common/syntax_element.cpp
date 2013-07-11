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

unsigned int read_code(InputBitstream* bs, int len)
{
  assert(bs!=NULL);
  return bs->read(len);
}

unsigned int read_uvlc(InputBitstream* bs)
{
  assert(bs!=NULL);
  unsigned int code = bs->read(1);

  if(code!=0) return 0;

  int len = 0;
  unsigned int ret = 0;
  while((code & 0x01) == 0)
  {
    code = bs->read(1);
    len++;
  }

  ret = bs->read(len);
  ret += (1u << len) - 1;
  return ret;
}

int read_svlc(InputBitstream* bs)
{
  assert(bs!=NULL);
  unsigned int bits = bs->read(1);

  if(bits!=0) return 0;

  int len = 0;
  unsigned int ret = 0;

  while((bits & 0x01) == 0)
  {
    bits = bs->read(1);
    len++;
  }

  ret = bs->read(len);
  ret = convert_to_int(ret);
  return ret;
}

unsigned int read_flag(InputBitstream* bs)
{
  assert(bs!=NULL);
  return bs->read(1);
}

void write_code(OutputBitstream* bs, unsigned int code, int len)
{
  assert(bs!=NULL);
  assert(len > 0);
  bs->write(code, len);
}

void write_uvlc(OutputBitstream* bs, unsigned int code)
{
  assert(bs!=NULL);
  int len = 1;
  unsigned int tmp_code = ++code;
  assert(tmp_code!=0);

  while(tmp_code != 1)
  {
    tmp_code >>= 1;
    len += 2;
  }

  bs->write(0, len>>1);
  bs->write(code, (len+1)>>1);
}

void write_flag(OutputBitstream* bs, unsigned int code)
{
  assert(bs!=NULL);
  bs->write(code, 1);
}

void write_svlc(OutputBitstream* bs, int code)
{
  assert(bs!=NULL);
  write_uvlc(bs, convert_to_uint(code));
}

} // namespace SyntaxElement

} // namespace HEVC