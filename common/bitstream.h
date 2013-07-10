#ifndef _BISTREAM_H_
#define _BISTREAM_H_

#include <cassert>
#include <vector>

namespace HEVC
{

/// InputBitstream class for extracting bits from unsigned char array.
class InputBitstream
{
public:
  InputBitstream(const unsigned char* buf, long long len);
  virtual ~InputBitstream() {}

  unsigned int read(int num_bits);
  unsigned int try_read(int num_bits) const;
  unsigned int read_byte();

  long long get_num_read_bits() const { return _num_read_bits; }

  unsigned char get_held_bits() const { return _held_bits; }
  unsigned char get_num_held_bits() const { return _num_held_bits; }

  const unsigned char* get_byte_stream() const { return _stream; }
  long long get_byte_stream_len() const { return _stream_len; }

private:
  InputBitstream() {}
  InputBitstream(const InputBitstream&);
  void operator=(const InputBitstream&);

  const unsigned char* _stream;
  unsigned char _held_bits;
  int _num_held_bits;

  long long _stream_len;
  long long _stream_idx;
  long long _num_read_bits;
};

/// OutputBitstream class for inserting bits data to a vector container
class OutputBitstream
{
public:
  OutputBitstream();
  virtual ~OutputBitstream();

  void write(unsigned int bits, int num_bits);
  void write_align_zero();
  void write_align_one();

  long long get_num_written_bits() const { return _num_written_bits; }

  const std::vector<unsigned char>& get_stream() const { return _stream; }
  unsigned char get_held_bits() const { return _held_bits; }
  unsigned char get_num_held_bits() const { return _num_held_bits; }

  const unsigned char* get_byte_stream() const { return _stream.data(); }
  long long get_byte_stream_len() const { return _stream.size(); }

private:
  OutputBitstream(const OutputBitstream&);
  void operator=(const OutputBitstream&);

  std::vector<unsigned char> _stream;
  unsigned char _held_bits;
  int _num_held_bits;

  long long _num_written_bits;
};

} // _BITSTREAM_H_

#endif