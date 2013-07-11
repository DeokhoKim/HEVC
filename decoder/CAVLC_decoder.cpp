#include "CAVLC_decoder.h"
#include "common/bitstream.h"
#include "common/PPS.h"
#include "common/PTL.h"
#include "common/SPS.h"
#include "common/syntax_element.h"
#include "common/VPS.h"

#include <cassert>

namespace HEVC
{

using namespace SyntaxElement;

void CAVLCDecoder::decode_vps(InputBitstream* bs, VPS* vps) const
{
  assert(bs!=NULL);
  assert(vps!=NULL);

  unsigned int code;
  bool flag;
  vps->vps_id = read_code(bs, 4);
  code = read_code(bs, 2); assert(code == 3u);
  vps->max_layers = read_code(bs, 6) + 1;
  vps->max_sub_layers = read_code(bs, 3) + 1;
  vps->temporal_id_nesting_flag = read_flag(bs);
  code = read_code(bs, 16); assert(code == 0x0FFFF);
  decode_ptl(bs, &vps->profile_tier_level, vps->max_sub_layers);

  flag = read_flag(bs);
  vps->sub_layer_ordering_info_present_flag = code;

  unsigned int size = flag ? vps->max_sub_layers : 1;
  vps->max_dec_pic_buffering = new int[size];
  vps->max_num_reorder_pics = new int[size];
  vps->max_latency_increase = new int[size];

  for(int i= flag ? 0 : vps->max_sub_layers-1; i< vps->max_sub_layers; i++)
  {
    vps->max_dec_pic_buffering[i] = read_uvlc(bs);
    vps->max_num_reorder_pics[i] = read_uvlc(bs);
    vps->max_latency_increase[i] = read_uvlc(bs);
  }

  unsigned int max_layer_id = read_code(bs, 6);
  unsigned int num_layer_sets = read_uvlc(bs) + 1;

  vps->max_layer_id = max_layer_id++;
  vps->num_layer_sets = num_layer_sets;

  if(max_layer_id*num_layer_sets!=0)
  {
    vps->layer_id_included_flag = new int[max_layer_id*num_layer_sets];
  }

  for(unsigned int i=1; i<num_layer_sets; i++)
  {
    for(unsigned int j=0; j<max_layer_id; j++)
    {
      vps->layer_id_included_flag[i*max_layer_id+j] = read_flag(bs);
    }
  }

  flag = read_flag(bs);
  vps->timing_info_present_flag = flag;
  if(flag)
  {
    vps->num_units_in_tick = read_code(bs, 32);
    vps->time_scale = read_code(bs, 32);
    flag = read_flag(bs);
    vps->poc_proportional_to_timing_flag = flag;
    if(flag)
    {
      vps->num_ticks_poc_diff_one = read_uvlc(bs) + 1;
    }
    code = read_uvlc(bs);
    vps->num_hrd_parameters = code;

    if(code>0)
    {
      vps->hrd_layer_set_idx = new int[code];
      vps->cprms_present_flag = new bool[code];
    }

    for(unsigned int i=0; i<code; i++)
    {
      vps->hrd_layer_set_idx[i] = read_uvlc(bs);
      if(i>0) vps->cprms_present_flag[i] = read_flag(bs);
      // hrd_parameters(cprms_present_flag[i], vps_max_sub_layers_minus1)
    }
  }

  flag = read_flag(bs);
  vps->extension_flag = flag;
  if(flag)
  {
    // while more rbsp_data()
    // vps->extension_data_flag = read_flag(bs);
  }
}

void CAVLCDecoder::decode_sps(InputBitstream* bs, SPS* sps) const
{
  assert(bs!=NULL);
  assert(sps!=NULL);

}

void CAVLCDecoder::decode_pps(InputBitstream* bs, PPS* pps) const
{

}

void CAVLCDecoder::decode_ptl(InputBitstream* bs, PTL* ptl,
                              int max_sub_layers) const
{
  assert(bs!=NULL);
  assert(ptl!=NULL);

  unsigned int code;
  ptl->general_profile_space = read_code(bs, 2);
  ptl->general_tier_flag = read_flag(bs);
  ptl->general_profile_idc = read_code(bs, 5);

  for(int i=0; i<32; i++)
  {
    ptl->general_profile_compatibility_flag[i] = read_flag(bs);
  }

  ptl->general_progressive_source_flag = read_flag(bs);
  ptl->general_interlaced_source_flag = read_flag(bs);
  ptl->general_non_packed_constraint_flag = read_flag(bs);
  ptl->general_frame_only_constraint_flag = read_flag(bs);
  code = read_code(bs, 32); assert(code == 0);
  code = read_code(bs, 12); assert(code == 0);
  ptl->general_level_idc = read_code(bs, 8);

  if(max_sub_layers > 1)
  {
    ptl->sub_layer_profile_present_flag = new bool[max_sub_layers-1];
    ptl->sub_layer_level_present_flag = new bool[max_sub_layers-1];

    for(int i=0; i<max_sub_layers-1; i++)
    {
      ptl->sub_layer_profile_present_flag[i] = read_flag(bs);
      ptl->sub_layer_level_present_flag[i] = read_flag(bs);
    }
  }

  if(max_sub_layers > 1)
  {
    for(int i=max_sub_layers-1; i<8; i++)
    {
      code = read_code(bs, 2); assert(code == 0);
    }
  }

  if(max_sub_layers > 1)
  {
    ptl->sub_layer_profile_space = new int [max_sub_layers-1];
    ptl->sub_layer_tier_flag = new bool [max_sub_layers-1];
    ptl->sub_layer_profile_idc = new int [max_sub_layers-1];
    ptl->sub_layer_profile_compatibility_flag =
      new bool [(max_sub_layers-1)*32];
    ptl->sub_layer_progressive_source_flag = new bool [max_sub_layers-1];
    ptl->sub_layer_interlaced_source_flag = new bool [max_sub_layers-1];
    ptl->sub_layer_non_packed_constraint_flag = new bool [max_sub_layers-1];
    ptl->sub_layer_frame_only_constraint_flag = new bool [max_sub_layers-1];
    ptl->sub_layer_level_idc = new int [max_sub_layers-1];

    for(int i=0; i<max_sub_layers-1; i++)
    {
      if(ptl->sub_layer_profile_present_flag[i])
      {
        ptl->sub_layer_profile_space[i] = read_code(bs, 2);
        ptl->sub_layer_tier_flag[i] = read_flag(bs);
        ptl->sub_layer_profile_idc[i] = read_code(bs, 5);
        for(int j=0; j<32; j++)
        {
          ptl->sub_layer_profile_compatibility_flag[i*32+j] = read_flag(bs);
        }
        ptl->sub_layer_progressive_source_flag[i] = read_flag(bs);
        ptl->sub_layer_interlaced_source_flag[i]=  read_flag(bs);
        ptl->sub_layer_non_packed_constraint_flag[i] = read_flag(bs);
        ptl->sub_layer_frame_only_constraint_flag[i] = read_flag(bs);
        code = read_code(bs, 24); assert(code == 0);
        code = read_code(bs, 22); assert(code == 0);
      }

      if(ptl->sub_layer_level_present_flag[i])
        ptl->sub_layer_level_idc[i] = read_code(bs, 8);
    }
  }
}

}