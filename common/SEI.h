#ifndef _SEI_H_
#define _SEI_H_

namespace HEVC
{

/// JCTVC-L1003 rev. 34.
/// 7.3.2.4 Supplementall enahancement information RBSP syntax.
/// 7.3.5 Supplemental enhancement information message syntax.
class SEI
{
public:
  SEI() {}
  virtual ~SEI() {}

private:
  SEI(const SEI&);
  void operator=(const SEI&);
};

} // namespace HEVC

#endif // _SEI_H_