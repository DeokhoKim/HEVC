#ifndef _BYTE_STREAM_H_
#define _BYTE_STREAM_H_

#include <istream>

namespace HEVC
{

class NALUnit;

namespace Bytestream
{

void bytestream_to_NAL_unit(std::istream* in, NALUnit* nalu);
void NAL_units_to_bytestream(std::ostream* out, NALUnit* nalu, bool zero_byte);

} // namespace Bytestream

} // namespace HEVC

#endif // _BYTE_STREAM_H_