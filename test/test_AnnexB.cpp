#include "common/AnnexB.h"

#include <cstdio>
#include <cstdlib>
#include <deque>
#include <fstream>

int main(int argc, const char* argv[])
{
  if(argc < 2)
  {
    fprintf(stderr, "Insert an input sequence\n");
    return EXIT_FAILURE;
  }

  std::ifstream input;
  input.open(argv[1]);

  if(!input.is_open())
  {
    fprintf(stderr, "Fail to open an input sequence\n");
    return EXIT_FAILURE;
  }

  HEVC::AnnexBStats stats;
  std::deque<unsigned char> data;
  if (HEVC::AnnexB::bytestream_to_NAL_unit(&input, &stats, &data))
  {
    fprintf(stderr, "Fail to bytestream_to_NAL_unit\n");
    input.close();
    return EXIT_FAILURE;
  }

  fprintf(stdout, "AnnexBStats::num_leading_zero_bytes %lld\n",
                  stats.num_leading_zero_bytes);
  fprintf(stdout, "AnnexBStats::num_zero_byte_bytes %lld\n",
                  stats.num_zero_byte_bytes);
  fprintf(stdout, "AnnexBStats::num_startcode_prefix_bytes %lld\n",
                  stats.num_startcode_prefix_bytes);
  fprintf(stdout, "AnnexBStats::num_bytes_in_nal_unit %lld\n",
                  stats.num_bytes_in_nal_unit);
  fprintf(stdout, "AnnexBStats::num_trailling_zero_bytes %lld\n",
                  stats.num_trailling_zero_bytes);

  input.close();

  return EXIT_SUCCESS;
}