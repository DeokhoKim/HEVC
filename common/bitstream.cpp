#include "bitstream.h"

#include <cassert>
#include <cstdlib>
#include <deque>

namespace HEVC
{

Bitstream::Bitstream()
{
  assert(0);
}

Bitstream::~Bitstream()
{
  assert(_stream==NULL);
}

unsigned char* get_byte_stream() const
{
  if(_stream != NULL)
  {
    return _stream.data();
  }
}

long long get_bytet_stream_len() const
{
  if(_stream == NULL) return _stream.size();
}

InputBitstream::InputBitstream()
{

}

InputBitstream::~InputBitstream()
{

}

OutputBitstream::OutputBitstream()
{

}

OutputBitstream::~OutputBitstream()
{

}

} // namespace HEVC