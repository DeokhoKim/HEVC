#ifndef _ACCESS_UNIT_H_
#define _ACCESS_UNIT_H_

#include "NAL.h"
#include <list>

namespace HEVC
{

class AccessUnit: public std::list<NALUnitEBSP*>
{
public:
  AccessUnit() {}
  virtual ~AccessUnit()
  {
    for(auto it = this->begin(); it != this->end(); it++)
    {
      delete *it;
      *it = NULL;
    }

    this.clear();
  }

private:
  AccessUnit(const AccessUnit&);
  void operator=(const AccessUnit&);
};


} // namespace HEVC

#endif // _ACCESS_UNIT_H_