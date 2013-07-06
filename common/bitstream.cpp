#include "bitstream.h"
#include "bits.h"

#include <cassert>
#include <cstdlib>
#include <deque>

namespace HEVC
{

InputBitstream::InputBitstream(const std::deque<unsigned char>* buf)
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

unsigned int InputBitstream::read(int num_bits)
{
  assert(num_bits <= 32 && num_bits > 0);
  assert((_held_bits >> _num_held_bits) == 0);
  assert(_stream != NULL);

  long long len = _stream->size();
  int buffered_len = _num_held_bits;
  unsigned long long ret = _held_bits;

  for(int i=num_bits - _num_held_bits; i>0; i-=8)
  {
    assert(_stream_idx < len);
    ret = (ret << 8) + _stream->at(_stream_idx++);
    buffered_len += 8;
  }

  int next_held_bits = buffered_len - num_bits;

  _held_bits = static_cast<unsigned char>(P2PHASE(ret, 1u << next_held_bits));
  _num_held_bits = next_held_bits;
  _num_read_bits += num_bits;

  ret >>= next_held_bits;
  return static_cast<unsigned int>(ret);
}

unsigned int InputBitstream::try_read(int num_bits) const
{
  assert(num_bits <= 32 && num_bits > 0);
  assert(_stream != NULL);

  long long len = _stream->size();
  long long stream_ix = _stream_idx;
  int buffered_len = _num_held_bits;
  unsigned long long ret = _held_bits;

  for(int i=num_bits - _num_held_bits; i>0 && stream_ix < len; i-=8)
  {
    ret = (ret << 8) + _stream->at(stream_ix++);
    buffered_len += 8;
  }

  int next_held_bits = buffered_len - num_bits;

  ret >>= next_held_bits;
  return static_cast<unsigned int>(ret);
}

unsigned int InputBitstream::read_byte()
{
  return read(8);
}

const unsigned char* InputBitstream::get_byte_stream() const
{
  if(_stream==NULL) return NULL;
  return &_stream->front();
}

long long InputBitstream::get_byte_stream_len() const
{
  if(_stream==NULL) return 0;
  return _stream->size();
}

OutputBitstream::OutputBitstream()
{
  _stream = new std::deque<unsigned char>;
  _held_bits = 0;
  _num_held_bits = 0;

  _num_written_bits = 0;
}

OutputBitstream::~OutputBitstream()
{
  delete _stream;
}

void OutputBitstream::write(unsigned int bits, int num_bits)
{
  if(num_bits==0) return;

  assert(num_bits <= 32 && num_bits > 0);
  assert((bits >> num_bits) == 0);
  assert((_held_bits >> _num_held_bits) == 0);

  unsigned long long buf = _held_bits;
  buf <<= num_bits;
  buf += bits;

  int num_total_bits = num_bits + _num_held_bits;
  int num_next_held_bits = num_total_bits & 0x07;

  _held_bits = P2PHASE(buf, 1u << num_next_held_bits);
  _num_held_bits = num_next_held_bits;

  buf >>= num_next_held_bits;

  switch(num_total_bits >> 3)
  {
    case 4: _stream->push_back((buf >> 24) & 0x0FF);
    case 3: _stream->push_back((buf >> 16) & 0x0FF);
    case 2: _stream->push_back((buf >>  8) & 0x0FF);
    case 1: _stream->push_back((buf      ) & 0x0FF);
    case 0: break;
    default: assert(0);
  }

  _num_written_bits += num_bits;
}

void OutputBitstream::write_align_zero()
{
  if (_held_bits == 0) return;
  int num_bits = 8u - _held_bits;
  write(0, num_bits);

  assert(_held_bits == 0);
  assert(_num_held_bits == 0);
}

void OutputBitstream::write_align_one()
{
  int num_bits = 8u - _held_bits;
  write(ALIGN2MASK(num_bits), num_bits);
}

unsigned char* OutputBitstream::get_byte_stream() const
{
  if(_stream==NULL) return NULL;
  return &_stream->front();
}

long long OutputBitstream::get_byte_stream_len() const
{
  if(_stream==NULL) return 0;
  return _stream->size();
}

} // namespace HEVC