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
  if(_stream != NULL) return _stream.data();
  return NULL;
}

long long get_bytet_stream_len() const
{
  if(_stream != NULL) return _stream.size();
  return 0;
}

InputBitstream::InputBitstream(std::deque<unsigned char>* buf)
{
  _stream = buf;
  _held_bits = 0;
  _num_held_bits = 0;

  _stream_idx = 0;
  _num_read_bits = 0;
}

InputBitstream::~InputBitstream()
{
  _stream = NULL;
}

OutputBitstream::OutputBitstream()
{

}

OutputBitstream::~OutputBitstream()
{

}

} // namespace HEVC