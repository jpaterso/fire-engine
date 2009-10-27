/**
 *  $Id: Math.cpp 108 2007-09-23 15:43:18Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  This file defines the functions declared in Math.h
 *  Basic mathematics functions, such as redefinitions of cos, sin, acos and
 *  asin. Other functions, like floor and ceiling are defined as well.
**/

#include "Math.h"

namespace fire_engine
{

template<> const f32         Math<f32>::PI                     = 3.14159265358979323846f;
template<> const f32         Math<f32>::PI_ON_2                = 1.57079632679489661923f;
template<> const f32         Math<f32>::PI_ON_3                = 1.04719755119659774615f;
template<> const f32         Math<f32>::PI_ON_4                = 0.78539816339744830961f;
template<> const f32         Math<f32>::PI_ON_6                = 0.52359877559829887308f;
template<> const f32         Math<f32>::TWO_PI                 = 6.28318530717958647692f;
template<> const f32         Math<f32>::TO_RAD                 = 0.01745329251994329576f;
template<> const f32         Math<f32>::TO_DEG                 = 57.2957795130823208768f;
template<> const f32         Math<f32>::SQ2                    = 1.41421356237309504880f;
template<> const f32         Math<f32>::ONE_ON_SQ2             = 0.70710678118654752440f;
template<> const f32         Math<f32>::SQ3                    = 1.73205080756887729352f;
template<> const f32         Math<f32>::TWO_SQ3                = 0.86602540378443864676f;
template<> const f32         Math<f32>::EPSILON                = 10e-5f;
template<> const f32         Math<f32>::ONE_OVER_TWO_FIVE_FIVE = 0.00392156862745098039f;
template<> const f64         Math<f64>::PI                     = 3.14159265358979323846;
template<> const f64         Math<f64>::PI_ON_2                = 1.57079632679489661923;
template<> const f64         Math<f64>::PI_ON_3                = 1.04719755119659774615;
template<> const f64         Math<f64>::PI_ON_4                = 0.78539816339744830961;
template<> const f64         Math<f64>::PI_ON_6                = 0.52359877559829887308;
template<> const f64         Math<f64>::TWO_PI                 = 6.28318530717958647692;
template<> const f64         Math<f64>::TO_RAD                 = 0.01745329251994329576;
template<> const f64         Math<f64>::TO_DEG                 = 57.2957795130823208768;
template<> const f64         Math<f64>::SQ2                    = 1.41421356237309504880;
template<> const f64         Math<f64>::ONE_ON_SQ2             = 0.70710678118654752440;
template<> const f64         Math<f64>::SQ3                    = 1.73205080756887729352;
template<> const f64         Math<f64>::TWO_SQ3                = 0.86602540378443864676;
template<> const f64         Math<f64>::EPSILON                = 10e-5;
template<> const f64         Math<f64>::ONE_OVER_TWO_FIVE_FIVE = 0.00392156862745098039;
template<>       f32 *       Math<f32>::mSineTbl               = 0;
template<>       f32 *       Math<f32>::mTanTbl                = 0;
template<>       f64 *       Math<f64>::mSineTbl               = 0;
template<>       f64 *       Math<f64>::mTanTbl                = 0;
template<>       s32         Math<f32>::mTrigTblSize           = 4096;
template<>       s32         Math<f64>::mTrigTblSize           = 4096;
template<>       Math<f32> * Math<f32>::mInstance              = 0;
template<>       Math<f64> * Math<f64>::mInstance              = 0;

template<>
f32 Math<f32>::FastInvSq(f32 val)
{
  f32 valhalf = 0.5f * val;
  s32 i = * (s32 *) &val;
  i = 0x5F3759DF - (i >> 1);
  val = * (f32 *) &i;
  val = val * (1.5f - valhalf * val * val);
  return val;
}
//------------------------------------------------------------------------------
template<>
f64 Math<f64>::FastInvSq(f64 val)
{
  f64 valhalf = 0.5 * val;
  s64 i = * (s64 *) &val;
  i = 0x5FE6EC85E7DE30DALL - (i >> 1);
  val = * (f64 *) &i;
  val = val * (1.5 - valhalf * val * val);
  return val;
}
//------------------------------------------------------------------------------
template<>
f32 Math<f32>::FastSq(f32 val)
{
  f32 valhalf = 0.5f * val;
  f32 x = val;
  s32 i = * (s32 *) &x;
  i = 0x5F3759DF - (i >> 1);
  x = * (f32 *) &i;
  x = x * (1.5f - valhalf * x * x);
  return val * x;
}
//------------------------------------------------------------------------------
template<>
f64 Math<f64>::FastSq(f64 val)
{
  f64 valhalf = 0.5 * val;
  f64 x = val;
  s64 i = * (s64 *) &x;
  i = 0x5FE6EC85E7DE30DALL - (i >> 1);
  x = * (f64 *) &i;
  x = x * (1.5 - valhalf * x * x);
  return val * x;
}
} // namespace fire_engine
