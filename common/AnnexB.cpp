#include "AnnexB.h"
#include "NAL.h"

#include <cassert>

namespace HEVC
{

static bool eof_before_n_bytes(std::istream* bs, int n)
{
  assert(n>0 && n<5);
  if(bs->eof() | bs->fail() | bs->bad()) return true;

  bool ret = false;
  const std::streampos cur = bs->tellg();
  bs->seekg(n, bs->cur);

  ret |= bs->eof() | bs->fail() | bs->bad();

  if (ret) bs->clear();
  bs->seekg(cur);

  return ret;
}

static unsigned int peek_bytes(std::istream* bs, int n)
{
  assert(n>0 && n<5);
  if(bs->eof() | bs->fail() | bs->bad()) return 0;
  std::streampos cur = bs->tellg();

  char buf[4] = {0};
  bs->read(buf, n);

  if(bs->eof() | bs->fail() | bs->bad()) bs->clear();
  bs->seekg(cur);

  unsigned int ret = 0;

  for(int i=0; i<n; i++)
  {
    ret |= (ret << 8) | static_cast<unsigned char>(buf[i]);
  }

  return ret;
}

static unsigned char read_byte(std::istream* bs)
{
  unsigned char ret = bs->get();
  return ret;
}

AnnexBStats::AnnexBStats()
{
  num_leading_zero_bytes = 0;
  num_zero_byte_bytes = 0;
  num_startcode_prefix_bytes = 0;
  num_bytes_in_nal_unit = 0;
  num_trailling_zero_bytes = 0;
}

namespace AnnexB
{

/// Parse an AVC AnnexB bytestream, in to extract a single nal unit while
/// accumulating bytestream statistics into stats.
///
/// Returns false if EOF is reached otherwise true.
bool bytestream_to_NAL_unit(std::istream* bs,
                            AnnexBStats* stats,
                            std::vector<unsigned char>* nal_data)
{
  assert(bs!=NULL && stats != NULL && nal_data != NULL);
  bool ret = false;

  int num_read_bytes = 0;

  // Remove all zero byte until 0x01 appears.
  while(true)
  {
    unsigned char byte = read_byte(bs);
    if(bs->eof()) return true;
    num_read_bytes++;
    if(byte==0x01) break;
    assert(byte==0);
  }

  // At least, three zero byte should be read because start code prefix three
  // byte is 0x000001.
  assert(num_read_bytes>=3);

  // The last three byte belongs to start code prefix bytes and a byte
  // precedes to the start code prefix bytes is zero byte of 0x00000001.
  // Otherwise, the zero bytes are leading zero 8bits.
  num_read_bytes -= 3;
  stats->num_startcode_prefix_bytes += 3;
  if(num_read_bytes > 0)
  {
    stats->num_zero_byte_bytes++;
    stats->num_leading_zero_bytes += --num_read_bytes;
  }

  while(eof_before_n_bytes(bs, 24/8) || peek_bytes(bs, 24/8) > 2)
  {
    unsigned char byte = read_byte(bs);
    if(bs->eof()) break;
    nal_data->push_back(byte);
  }

  while(!bs->eof() &&
        (eof_before_n_bytes(bs, 24/8) || peek_bytes(bs, 24/8)!=0x000001) &&
        (eof_before_n_bytes(bs, 32/8) || peek_bytes(bs, 32/8)!=0x00000001))
  {
    unsigned int trailing_zero_8bits = read_byte(bs);
    if(bs->eof()) break;
    assert(trailing_zero_8bits == 0);
    stats->num_trailling_zero_bytes++;
  }

  stats->num_bytes_in_nal_unit = nal_data->size();
  ret |= eof_before_n_bytes(bs, 32/8) | eof_before_n_bytes(bs, 24/8);
  return ret;
}

/// Write all NAL units in an access unit @au to bytestream @out in a manner
/// satisfying AnnexB of AVC. NAL units are written in the order they are found
/// in @au. The zero byte word is appended to:
/// - the initial startcode in the access unit
/// - any SPS / PPS NAL units
void NAL_units_to_bytestream(std::ostream* out, const AccessUnit& au)
{
  for(auto it=au.cbegin(); it!=au.cend(); it++)
  {
    NALUnit& nalu = **it;

    static const char start_code_prefix[] = {0x00, 0x00, 0x00, 0x01};

    if(it == au.begin() ||
       nalu.get_type() == NAL_UNIT_SPS || nalu.get_type() == NAL_UNIT_PPS)
    {
      out->write(start_code_prefix, 4);
    }
    else
    {
      out->write(start_code_prefix+1, 3);
    }
    NALUnit::write(&nalu, out);
  }
}

} // namespace AnnexB

} // namespace HEVC