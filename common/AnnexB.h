#ifndef _ANNEX_B_
#define _ANNEX_B_

#include "access_unit.h"

#include <deque>
#include <istream>

namespace HEVC
{

class AnnexBStats
{
public:
  AnnexBStats();
  ~AnnexBStats() {}

  long long num_leading_zero_bytes;
  long long num_zero_byte_bytes;
  long long num_startcode_prefix_bytes;
  long long num_bytes_in_nal_unit;
  long long num_trailling_zero_bytes;

  AnnexBStats& operator+=(const AnnexBStats& rhs)
  {
    this->num_leading_zero_bytes += rhs.num_leading_zero_bytes;
    this->num_zero_byte_bytes += rhs.num_zero_byte_bytes;
    this->num_startcode_prefix_bytes += rhs.num_startcode_prefix_bytes;
    this->num_bytes_in_nal_unit += rhs.num_bytes_in_nal_unit;
    this->num_trailling_zero_bytes += rhs.num_trailling_zero_bytes;
    return *this;
  }

private:
  AnnexBStats(const AnnexBStats&);
  void operator=(const AnnexBStats&);
};

namespace AnnexB
{

bool bytestream_to_NAL_unit(std::istream* bs,
                            AnnexBStats* stats,
                            std::deque<unsigned char>* nal_data);
void NAL_units_to_bytestream(std::ostream* out, const AccessUnit& au);

} // namespace AnnexB

} // namespace HEVC

#endif // _ANNEX_B_