#ifndef _NAL_H_
#define _NAL_H_

#include <deque>

namespace HEVC
{

enum NAL_UNIT_TYPE
{
  NAL_UNIT_CODED_SLICE_TRAIL_N = 0, // 0
  NAL_UNIT_CODED_SLICE_TRAIL_R,     // 1

  NAL_UNIT_CODED_SLICE_TSA_N,       // 2
  NAL_UNIT_CODED_SLICE_TLA_R,       // 3

  NAL_UNIT_CODED_SLICE_STSA_N,      // 4
  NAL_UNIT_CODED_SLICE_STSA_R,      // 5

  NAL_UNIT_CODED_SLICE_RADL_N,      // 6
  NAL_UNIT_CODED_SLICE_RADL_R,      // 7

  NAL_UNIT_CODED_SLICE_RASL_N,      // 8
  NAL_UNIT_CODED_SLICE_RASL_R,      // 9

  NAL_UNIT_RESERVED_VCL_N10,
  NAL_UNIT_RESERVED_VCL_R11,
  NAL_UNIT_RESERVED_VCL_N12,
  NAL_UNIT_RESERVED_VCL_R13,
  NAL_UNIT_RESERVED_VCL_N14,
  NAL_UNIT_RESERVED_VCL_R15,

  NAL_UNIT_CODED_SLICE_BLA_W_LP,    // 16
  NAL_UNIT_CODED_SLICE_BLA_W_RADL,  // 17
  NAL_UNIT_CODED_SLICE_BLA_N_LP,    // 18
  NAL_UNIT_CODED_SLICE_IDR_W_RADL,  // 19
  NAL_UNIT_CODED_SLICE_IDR_N_LP,    // 20
  NAL_UNIT_CODED_SLICE_CRA,         // 21
  NAL_UNIT_RESERVED_IRAP_VCL22,
  NAL_UNIT_RESERVED_IRAP_VCL23,

  NAL_UNIT_RESERVED_VCL24,
  NAL_UNIT_RESERVED_VCL25,
  NAL_UNIT_RESERVED_VCL26,
  NAL_UNIT_RESERVED_VCL27,
  NAL_UNIT_RESERVED_VCL28,
  NAL_UNIT_RESERVED_VCL29,
  NAL_UNIT_RESERVED_VCL30,
  NAL_UNIT_RESERVED_VCL31,

  NAL_UNIT_VPS,                     // 32
  NAL_UNIT_SPS,                     // 33
  NAL_UNIT_PPS,                     // 34
  NAL_UNIT_ACCESS_UNIT_DELIMITER,   // 35
  NAL_UNIT_EOS,                     // 36
  NAL_UNIT_EOB,                     // 37
  NAL_UNIT_FILLER_DATA,             // 38
  NAL_UNIT_PREFIX_SEI,              // 39
  NAL_UNIT_SUFFIX_SEI,              // 40
  NAL_UNIT_RESERVED_NVCL41,
  NAL_UNIT_RESERVED_NVCL42,
  NAL_UNIT_RESERVED_NVCL43,
  NAL_UNIT_RESERVED_NVCL44,
  NAL_UNIT_RESERVED_NVCL45,
  NAL_UNIT_RESERVED_NVCL46,
  NAL_UNIT_RESERVED_NVCL47,
  NAL_UNIT_UNSPECIFIED_48,
  NAL_UNIT_UNSPECIFIED_49,
  NAL_UNIT_UNSPECIFIED_50,
  NAL_UNIT_UNSPECIFIED_51,
  NAL_UNIT_UNSPECIFIED_52,
  NAL_UNIT_UNSPECIFIED_53,
  NAL_UNIT_UNSPECIFIED_54,
  NAL_UNIT_UNSPECIFIED_55,
  NAL_UNIT_UNSPECIFIED_56,
  NAL_UNIT_UNSPECIFIED_57,
  NAL_UNIT_UNSPECIFIED_58,
  NAL_UNIT_UNSPECIFIED_59,
  NAL_UNIT_UNSPECIFIED_60,
  NAL_UNIT_UNSPECIFIED_61,
  NAL_UNIT_UNSPECIFIED_62,
  NAL_UNIT_UNSPECIFIED_63,
  NAL_UNIT_INVALID,
};

class NALUnitHeader
{
public:
  NALUnitHeader() {}
  NALUnitHeader(NAL_UNIT_TYPE type, int temporal_id=0, int reserved_zero_6bits=0);
  virtual ~NALUnitHeader() {}

  bool is_slice() const;
  bool is_SEI() const;
  bool is_VCL() const;

  NAL_UNIT_TYPE get_type() const { return _nal_unit_type; }
  void set_type(const NAL_UNIT_TYPE type) { _nal_unit_type = type; }

  unsigned int get_temporal_id() const { return _temporal_id; }
  void set_temporal_id(unsigned int val) { _temporal_id = val; }

  unsigned int get_reserved_zero_6bits() const { return _reserved_zero_6bits; }
  void set_reserved_zero_6bits(unsigned int val) { _reserved_zero_6bits = val; }

protected:
  NAL_UNIT_TYPE _nal_unit_type;
  unsigned int _temporal_id;
  unsigned int _reserved_zero_6bits;

private:
  NALUnitHeader(const NALUnitHeader&);
  void operator=(const NALUnitHeader&);

};

class NALUnitEBSP: public NALUnitHeader
{
public:
  NALUnitEBSP() {}
  virtual ~NALUnitEBSP() {}

private:
  NALUnitEBSP(const NALUnitEBSP&);
  void operator=(const NALUnitEBSP&);

  std::deque<unsigned char> _nal_unit_data;
};

class InputNALUnit: public NALUnitEBSP
{
public:
  InputNALUnit() {}
  virtual ~InputNALUnit() {}

private:
  InputNALUnit(const InputNALUnit&);
  void operator=(const InputNALUnit&);
};

class OutputNALUnit: public NALUnitEBSP
{
public:
  OutputNALUnit() {}
  virtual ~OutputNALUnit() {}

private:
  OutputNALUnit(const OutputNALUnit&);
  void operator=(const OutputNALUnit&);
};


} // namespace HEVC

#endif // _NAL_H_
