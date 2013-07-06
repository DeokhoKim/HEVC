#ifndef _BISTREAM_H_
#define _BISTREAM_H_

#include <cassert>
#include <deque>

namespace HEVC
{

class InputBitstream
{
public:
  InputBitstream(const std::deque<unsigned char>* buf);
  virtual ~InputBitstream();

  unsigned int read(int num_bits);
  unsigned int try_read(int num_bits) const;
  unsigned int read_byte();

  long long get_num_read_bits() const { return _num_read_bits; }

  const std::deque<unsigned char>& get_stream() const { return *_stream; }
  unsigned char get_held_bits() const { return _held_bits; }
  unsigned char get_num_held_bits() const { return _num_held_bits; }

  const unsigned char* get_byte_stream() const;
  long long get_byte_stream_len() const;

private:
  InputBitstream() {}
  InputBitstream(const InputBitstream&);
  void operator=(const InputBitstream&);

  const std::deque<unsigned char>* _stream;
  unsigned char _held_bits;
  int _num_held_bits;

  long long _stream_idx;
  long long _num_read_bits;
};

class OutputBitstream
{
public:
  OutputBitstream();
  virtual ~OutputBitstream();

  void write(unsigned int bits, int num_bits);
  void write_align_zero();
  void write_align_one();

  long long get_num_written_bits() const { return _num_written_bits; }

  std::deque<unsigned char>& get_stream() const { return *_stream; }
  unsigned char get_held_bits() const { return _held_bits; }
  unsigned char get_num_held_bits() const { return _num_held_bits; }

  unsigned char* get_byte_stream() const;
  long long get_byte_stream_len() const;

private:
  OutputBitstream(const OutputBitstream&);
  void operator=(const OutputBitstream&);

  std::deque<unsigned char>* _stream;
  unsigned char _held_bits;
  int _num_held_bits;

  long long _num_written_bits;
};

} // _BITSTREAM_H_

#endif