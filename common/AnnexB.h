#ifndef _ANNEX_B_
#define _ANNEX_B_

namespace HEVC
{

class AnnexBStats
{
public:
  AnnexBStats() {}
  ~AnnexBStats() {}

  long long num_leading_zero_8bits_bytes;
  long long num_zero_byte_bytes;
  long long num_startcode_prefix_bytes;
  long long num_bytes_in_nal_unit;
  long long num_trailling_zero_8bits_bytes;

  AnnexBStats& operator+=(const AnnexBStats& rhs)
  {
    this->num_leading_zero_8bits_bytes += rhs.num_leading_zero_8bits_bytes;
    this->num_zero_byte_bytes += rhs.num_zero_byte_bytes;
    this->num_startcode_prefix_bytes += rhs.num_startcode_prefix_bytes;
    this->num_bytes_in_nal_unit += rhs.num_bytes_in_nal_unit;
    this->num_trailling_zero_8bits_bytes += rhs.num_trailling_zero_8bits_bytes;
    return *this;
  }

private:
  AnnexBStats(const AnnexBStats&);
  void operator=(const AnnexBStats&);
};

namespace AnnexB
{

bool bytestream_to_NAL_unit(std::istream* in,
                            std::deque<unsigned char* nal_data,
                            AnnexBStats* stats);

} // namespace AnnexB

} // namespace HEVC

#endif // _ANNEX_B_