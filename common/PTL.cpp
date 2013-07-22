#include "PTL.h"
#include "common/bitstream.h"
#include "common/syntax_element.h"

#include <cstdlib>

namespace HEVC
{

using namespace SyntaxElement;

void PTL::SubLayerProfile::read(InputBitstream* bs)
{
  assert(bs!=NULL);
  unsigned int code;
  profile_space = read_code(bs, 2); // u(2)
  tier_flag = read_flag(bs); // u(1)
  profile_idc = read_code(bs, 5); // u(5)
  for(int i=0; i<32; i++)
    profile_compatibility_flag[i] = read_flag(bs); // u(1)
  progressive_source_flag = read_flag(bs); // u(1)
  interlaced_source_flag = read_flag(bs); // u(1)
  non_packed_constraint_flag = read_flag(bs); // u(1)
  frame_only_constraint_flag = read_flag(bs); // u(1)
  code = read_code(bs, 32); assert(code==0);
  code = read_code(bs, 12); assert(code==0);
}

void PTL::SubLayerProfile::write(OutputBitstream* bs)
{
  assert(bs!=NULL);
  write_code(bs, profile_space, 2);
  write_flag(bs, tier_flag);
  write_code(bs, profile_idc, 5);
  for(int i=0; i<32; i++)
    write_flag(bs, profile_compatibility_flag[i]);
  write_flag(bs, progressive_source_flag);
  write_flag(bs, interlaced_source_flag);
  write_flag(bs, non_packed_constraint_flag);
  write_flag(bs, frame_only_constraint_flag);
  write_code(bs, 0u, 32);
  write_code(bs, 0u, 12);
}

void PTL::read(InputBitstream* bs)
{
  assert(bs!=NULL);

  unsigned int code;
  _general_profile_space = read_code(bs, 2); /// u(2)
  _general_tier_flag = read_flag(bs); /// u(1)
  _general_profile_idc = read_code(bs, 5); /// u(5)
  for(int i=0; i<32; i++)
    _general_profile_compatibility_flag[i] = read_flag(bs); /// u(1)
  _general_progressive_source_flag = read_flag(bs); /// u(1)
  _general_interlaced_source_flag = read_flag(bs); /// u(1)
  _general_non_packed_constraint_flag = read_flag(bs); /// u(1)
  _general_frame_only_constraint_flag = read_flag(bs); /// u(1)
  code = read_code(bs, 32); assert(code==0);
  code = read_code(bs, 12); assert(code==0);
  _general_level_idc = read_code(bs, 8); /// u(8)

  int max_sub_layers_minus1 = _sub_layer_profile_present.size();

  for(int i=0; i<max_sub_layers_minus1; i++)
  {
    _sub_layer_profile_present[i] = read_flag(bs); /// u(1)
    _sub_layer_level_present[i] = read_flag(bs); /// u(1)
  }

  if(max_sub_layers_minus1>0)
  {
    for(int i=max_sub_layers_minus1; i<8; i++)
      code = read_code(bs, 2);
  }

  for(int i=0; i<max_sub_layers_minus1; i++)
  {
    if(_sub_layer_profile_present[i])
    {
      _sub_layer_profile[i] = new SubLayerProfile;
      _sub_layer_profile[i]->read(bs);
    }

    if(_sub_layer_level_present[i])
      _sub_layer_level_idc[i] = read_code(bs, 8);
  }
}

void PTL::write(OutputBitstream* bs)
{
  assert(bs!=NULL);

  write_code(bs, _general_profile_space, 2);
  write_flag(bs, _general_tier_flag);
  write_code(bs, _general_profile_idc, 5);
  for(int i=0; i<32; i++)
    write_flag(bs, _general_profile_compatibility_flag[i]);
  write_flag(bs, _general_progressive_source_flag);
  write_flag(bs, _general_interlaced_source_flag);
  write_flag(bs, _general_non_packed_constraint_flag);
  write_flag(bs, _general_frame_only_constraint_flag);
  write_code(bs, 0u, 32);
  write_code(bs, 0u, 12);
  write_code(bs, _general_level_idc, 8);

  int len = _sub_layer_profile_present.size();
  assert(len>=0);

  for(int i=0; i<len; i++)
  {
    write_flag(bs, _sub_layer_profile_present[i]);
    write_flag(bs, _sub_layer_level_present[i]);
  }

  if(len>0)
    for(int i=len; i<8; i++)
      write_code(bs, 0u, 2);

  for(int i=0; i<len; i++)
  {
    if(_sub_layer_profile_present[i])
      _sub_layer_profile[i]->write(bs);
    if(_sub_layer_level_present[i])
      write_code(bs, _sub_layer_level_idc[i], 8);
  }
}

}