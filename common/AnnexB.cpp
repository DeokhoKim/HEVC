#include <istream>

namespace HEVC
{

static bool eof_bofore_n_bytes(std::istream* in, int n)
{
  assert(n>0 && n<5);
  assert(!in->eof() && !in->fail() && !in->bad());

  bool ret = false;
  std::streampos cur = in->tellg();
  in->seekg(n, in.cur);

  ret |= in->eof() | in->fail() | in->bad();

  if (ret) in->clear();
  in->seekg(0, cur);

  return ret;
}

static unsigned int peek_bytes(std::istream* in, int n)
{
  assert(n>0 && n<5);
  assert(!in->eof() && !in->fail() && !in->bad());
  std::streampo cur = in->tellg();

  unsigned char ret[4] = {0};
  in->read(ret, n);

  if(in->eof() | in->fail() | in->bad()) in->clear();
  in->seekg(0, cur);

  return *reinterpret_cast<unsigned int*>(ret);
}

static unsigned char read_byte(std::istream* in)
{
  unsigned char ret = input->get();
  return ret;
}

static unsigned int read_bytes(std::istream* in, int n)
{
  assert(n>0 && n<5);

  unsigned char ret[4] = {0};
  in->read(ret, n);
  return *reinterpret_cast<unsigned int*>(ret);
}

namespace AnnexB
{

/// Parse an AVC AnnexB bytestream, in to extract a single nal unit while
/// accumulating bytestream statistics into stats.
///
/// Returns false if EOF is reached otherwise true.
bool bytestream_to_NAL_unit(std::istream* in,
                            std::deque<unsigned char* nal_data,
                            AnnexBStats* stats)
{
  bool ret = false;




  stats->num_bytes_in_nal_unit = nal_data.size();
  return ret;
}

} // namespace AnnexB

} // namespace HEVC