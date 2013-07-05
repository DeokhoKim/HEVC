#ifndef _BISTREAM_H_
#define _BISTREAM_H_

#include <cassert>
#include <deque>

namespace HEVC
{

class Bitstream
{
public:
  Bitstream();
  virtual ~Bitstream();

std::deque<unsigned char>* get_stream() const { return _stream; }
unsigned char get_held_bits() const { return _held_bits; }
unsigned char get_num_held_bits() const { return _num_held_bits; }

unsigned char* get_byte_stream() const;
long long get_byte_stream_len() const;

protected:
  std::deque<unsigned char>* _stream;
  unsigned char _held_bits;
  int _num_held_bits;

private:
  Bitstream(const Bitstream&);
  void operator=(const Bitstream&);
};

class InputBitstream: public Bitstream
{
public:
  InputBitstream();
  virtual ~InputBitstream();

private:
  InputBitstream(const InputBitstream&);
  void operator=(const InputBitstream&);
};

class OutputBitstream: public Bitstream
{
public:
  OutputBitstream();
  virtual ~OutputBitstream();

private:
  OutputBitstream(const OutputBitstream&);
  void operator=(const OutputBitstream&);
};

} // _BITSTREAM_H_

#endif