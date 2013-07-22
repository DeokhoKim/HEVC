#include "annexE.h"
#include "bitstream.h"
#include "syntax_element.h"

namespace HEVC
{

using namespace SyntaxElement;

void HRD::CPRMS::read(InputBitstream* bs)
{
  assert(bs!=NULL);

  nal_hrd_parameters_present = read_flag(bs); // u(1)
  vcl_hrd_parameters_present = read_flag(bs); // u(1)
  if(nal_hrd_parameters_present || vcl_hrd_parameters_present)
  {
    sub_pic_hrd_params_present = read_flag(bs); // u(1)
    if(sub_pic_hrd_params_present)
    {
      tick_divisor = read_code(bs, 8) + 2; // u(8) + 2
      du_cpb_removal_delay_increment_length = read_code(bs, 5) + 1; // u(5) + 1
      sub_pic_cpb_params_in_pic_timing_sei_flag = read_flag(bs); // u(1)
      dpb_output_delay_du_length = read_code(bs, 5) + 1; // u(5) + 1
    }
    bit_rate_scale = read_code(bs, 4); // u(4)
    cpb_size_scale = read_code(bs, 4); // u(4)
    if(sub_pic_hrd_params_present)
      cpb_size_du_scale = read_code(bs, 4); // u(4)
    initial_cpb_removal_delay_length = read_code(bs, 5) + 1; // u(5) + 1
    au_cpb_removal_delay_length = read_code(bs, 5) + 1; // u(5) + 1
    dpb_output_delay_length = read_code(bs, 5) + 1; // u(5) + 1
  }
}

void HRD::CPRMS::write(OutputBitstream* bs)
{
  assert(bs!=NULL);

  write_flag(bs, nal_hrd_parameters_present);
  write_flag(bs, vcl_hrd_parameters_present);
  if(nal_hrd_parameters_present || vcl_hrd_parameters_present)
  {
    write_flag(bs, sub_pic_hrd_params_present);
    if(sub_pic_hrd_params_present)
    {
      write_code(bs, tick_divisor - 2, 8);
      write_code(bs, du_cpb_removal_delay_increment_length - 1, 5);
      write_flag(bs, sub_pic_cpb_params_in_pic_timing_sei_flag);
      write_code(bs, dpb_output_delay_length - 1, 5);
    }
    write_code(bs, bit_rate_scale, 4);
    write_code(bs, cpb_size_scale, 4);
    if(sub_pic_hrd_params_present)
      write_code(bs, cpb_size_du_scale, 4);
    write_code(bs, initial_cpb_removal_delay_length - 1, 5);
    write_code(bs, au_cpb_removal_delay_length - 1, 5);
    write_code(bs, dpb_output_delay_length - 1, 5);
  }
}

void HRD::read(InputBitstream* bs, bool cprms_present, int max_sub_layers)
{
  if(cprms_present)
  {
    _cprms.read(bs);
  }

  // _fixed_pic_rate_general_flag.resize(max_sub_layers);
  // _fixed_pic_rate_within_cvs_flag.resize(max_sub_layers);
  // _elemental_duration_in_tc.resize(max_sub_layers);
  // _low_delay_hrd_flag.resize(max_sub_layers);
  // _cpb_cnt.resize(max_sub_layers);

  // for(int i=0; i<max_sub_layers; i++)
  // {
  //   _fixed_pic_rate_general_flag[i] = read_flag(bs); // u(1)
  //   if(!_fixed_pic_rate_general_flag[i])
  //       _fixed_pic_rate_within_cvs_flag[i] = read_flag(bs); // u(1)
  //   if(_fixed_pic_rate_within_cvs_flag[i])
  //     _elemental_duration_in_tc[i] = read_uvlc(bs) + 1; // ue(v) + 1
  //   else
  //     _low_delay_hrd_flag[i] = read_flag(bs); // u(1)
  //   if(!_low_delay_hrd_flag[i])
  //       _cpb_cnt[i] = read_uvlc(bs) + 1; // ue(v) + 1

  // }
}

void HRD::write(OutputBitstream* bs, bool cprms_present)
{
  if(cprms_present)
  {
    _cprms.write(bs);
  }

  // for(auto i=0u; i<_fixed_pic_rate_general_flag.size(); i++)
  // {
  //   write_flag(bs, _fixed_pic_rate_general_flag[i]);
  //   if(!_fixed_pic_rate_general_flag[i])
  //     write_flag(bs, _fixed_pic_rate_within_cvs_flag[i]);
  //   if(_fixed_pic_rate_within_cvs_flag[i])
  //     write_uvlc(bs, _elemental_duration_in_tc[i] - 1);
  //   else
  //     write_flag(bs, _low_delay_hrd_flag[i]);
  //   if(!_low_delay_hrd_flag[i])
  //     write_uvlc(bs, _cpb_cnt[i] - 1);
  // }
}

} // namespace HEVC