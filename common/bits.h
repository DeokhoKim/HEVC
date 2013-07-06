#ifndef _BITS_H_
#define _BITS_H_
namespace HEVC
{

template<typename T>
inline T P2ALIGN(T x, unsigned int align)
{
  // clears all bits below align, rounding x down to the next lower multiple of
  // align
  return x & (-align);
}

template<typename T>
inline T P2PHASE(T x, unsigned int align)
{
  // clears all bits above or equal to align, getting (x%align), the phase of x
  // with regards to align
  return x & (align - 1);
}

template<typename T>
inline T P2NPHASE(T x, unsigned int align)
{
  // equal to (align - P2PHASE(x, align)) % align
  return (-x) & (align - 1);
}

template<typename T>
inline T P2ROUNDUP(T x, unsigned int align)
{
  // round x up to the next align boundary, equal to -P2ALIGN(x, align)
  return -((-x) & (-align));
}

template<typename T>
inline T P2END(T x, unsigned int align)
{
  // round up (x+1) to the next align boundary. This gets the end address of the
  // current align block.
  return -((~x) & (-align));
}

template<typename T>
inline T P2PHASEUP(T x, unsigned int align, unsigned int phase)
{
  // round up x to have a specific phase with regards to align.
  return phase - ((phase - x) & (-align));
}

template<typename T>
inline T P2CROSS(T x, unsigned int y, unsigned int align)
{
  // This just tests to see if they differ in any position above th alignment.
  return (x ^ y) > (align - 1);
}

inline unsigned int ALIGN2MASK(unsigned int align)
{
  return (1u << align) - 1;
}

} // namespace HEVC

#endif // _BITS_H_