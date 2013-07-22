#include "VPS.h"
#include "common/annexE.h"
#include "common/bitstream.h"
#include "common/PTL.h"
#include "common/syntax_element.h"

#include <cstdlib>

namespace HEVC
{

using namespace SyntaxElement;

VPS::VPS()
{
  _profile_tier_level = NULL;
  _layer_id_included_flag = NULL;
}

VPS::~VPS()
{
  if(_profile_tier_level!=NULL) delete _profile_tier_level;

  for(auto it=_sub_layer_ordering_info.begin();
           it!=_sub_layer_ordering_info.end(); it++)
  {
    if(*it!=NULL) delete *it;
  }

  if(_layer_id_included_flag!=NULL) delete[] _layer_id_included_flag;
}

void VPS::SubLayerOrderingInfo::read(InputBitstream* bs)
{
  assert(bs!=NULL);
  max_dec_pic_buffering = read_uvlc(bs) + 1; // ue(v) + 1
  max_num_reorder_pics = read_uvlc(bs); // ue(v)
  max_latency_increase = read_uvlc(bs) - 1; // ue(v) - 1
}

void VPS::SubLayerOrderingInfo::write(OutputBitstream* bs)
{
  assert(bs!=NULL);
  write_uvlc(bs, max_dec_pic_buffering - 1);
  write_uvlc(bs, max_num_reorder_pics);
  write_uvlc(bs, max_latency_increase + 1);
}

void VPS::TimingInfo::read(InputBitstream* bs, int max_sub_layers)
{
  assert(bs!=NULL);
  num_units_in_tick = read_code(bs, 32); // u(32)
  time_scale = read_code(bs, 32); // u(32)
  poc_proportional_to_timing_flag = read_flag(bs); // u(1)

  if(poc_proportional_to_timing_flag)
    num_ticks_poc_diff_one = read_uvlc(bs) + 1; // ue(v) + 1

  num_hrd_parameters = read_uvlc(bs); // ue(v)

  hrd_layer_set_idx.resize(num_hrd_parameters);
  cprms_present.assign(num_hrd_parameters, true);
  hrd_parameters.resize(num_hrd_parameters);

  for(int i=0; i<num_hrd_parameters; i++)
  {
    hrd_layer_set_idx[i] = read_uvlc(bs); // ue(v)
    if(i>0) cprms_present[i] = read_flag(bs); // u(1)
    hrd_parameters[i] = new HRD;
    hrd_parameters[i]->read(bs, cprms_present[i], max_sub_layers);
    assert(0); // Not yet tested.
  }

}

void VPS::TimingInfo::write(OutputBitstream* bs)
{
  assert(bs!=NULL);

  write_code(bs, num_units_in_tick, 32);
  write_code(bs, time_scale, 32);
  write_flag(bs, poc_proportional_to_timing_flag);
  if(poc_proportional_to_timing_flag)
    write_uvlc(bs, num_ticks_poc_diff_one - 1);
  write_uvlc(bs, num_hrd_parameters);

  for(int i=0; i<num_hrd_parameters; i++)
  {
    write_uvlc(bs, hrd_layer_set_idx[i]);
    if(i>0) write_flag(bs, cprms_present[i]);
    // hrd_parameters( cprms_present[i], max_sub_layers_minus1)
  }
}

void VPS::read(InputBitstream* bs)
{
  assert(bs!=NULL);

  unsigned int code;
  _vps_id = read_code(bs, 4); // u(4)
  code = read_code(bs, 2); assert(code==3u); // vps_reserved_three_2bits.
  _max_layers = read_code(bs, 6) + 1; // u(6) + 1
  _max_sub_layers = read_code(bs, 3) + 1; // u(3) + 1
  _temporal_id_nesting_flag = read_flag(bs); // u(1)
  assert(_max_sub_layers>1 || _temporal_id_nesting_flag);
  code = read_code(bs, 16); assert(code==0xFFFF);
  _profile_tier_level = new PTL(_max_sub_layers);
  _profile_tier_level->read(bs);
  _sub_layer_ordering_info_present = read_flag(bs); // u(1)

  const int sub_layers = _sub_layer_ordering_info_present ? _max_sub_layers : 1;

  _sub_layer_ordering_info.resize(sub_layers, NULL);
  for(int i=0; i<sub_layers; i++)
  {
    _sub_layer_ordering_info[i] = new SubLayerOrderingInfo;
    _sub_layer_ordering_info[i]->read(bs);
  }

  _max_layer_id = read_code(bs, 6); // u(6)
  _num_layer_sets = read_uvlc(bs) + 1; // ue(v) + 1

  _layer_id_included_flag =
    reinterpret_cast<bool(*)[MAX_VPS_NUH_RESERVED_ZERO_LAYER_ID_PLUS1]>
    (new bool[_num_layer_sets*(_max_layer_id+1)]);

  for(int i=1; i<_num_layer_sets; i++)
    for(int j=0; j<_max_layer_id+1; j++)
    {
      _layer_id_included_flag[i][j] = read_flag(bs);
      assert(0); // Not yet tested.
    }

  _timing_info_present = read_flag(bs); // u(1)
  if(_timing_info_present)
  {
    _timing_info = new TimingInfo;
    _timing_info->read(bs, _max_sub_layers);
    assert(0); // Not yet tested.
  }
  _extension_flag = read_flag(bs);
  if(_extension_flag)
  {
    assert(0); // Not yey tested.
  }
  code = read_flag(bs); assert(code==1u); // rbsp_stop_one_bit
}

void VPS::write(OutputBitstream* bs)
{
  assert(bs!=NULL);

  write_code(bs, _vps_id, 4);
  write_code(bs, 3u, 2);
  write_code(bs, _max_layers - 1, 6);
  write_code(bs, _max_sub_layers - 1, 3);
  write_flag(bs, _temporal_id_nesting_flag);
  write_code(bs, 0x0FFFF, 16);
  _profile_tier_level->write(bs);
  write_flag(bs, _sub_layer_ordering_info_present);

  for(auto it=_sub_layer_ordering_info.begin();
           it!=_sub_layer_ordering_info.end(); it++)
  {
    (*it)->write(bs);
  }

  write_code(bs, _max_layer_id, 6);
  write_uvlc(bs, _num_layer_sets - 1);

  for(int i=1; i<_num_layer_sets; i++)
    for(int j=0; j<_max_layer_id+1; j++)
    {
      write_flag(bs, _layer_id_included_flag[i][j]);
      assert(0);
    }

  write_flag(bs, _timing_info_present);
  if(_timing_info_present)
  {
    _timing_info->write(bs);
    assert(0);
  }

  write_flag(bs, _extension_flag);
  if(_extension_flag)
    assert(0);
  write_flag(bs, 1u); // rbsp_stop_one_bit
  bs->write_align_zero();
}

} // namespace HEVC