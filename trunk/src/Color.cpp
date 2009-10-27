/**
 * FILE:    Color.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Color.cpp 103 2007-09-06 10:16:23Z jpaterso $
 * PURPOSE: Some constant colors.
**/

#include "Color.h"

namespace fire_engine
{

template <> const Color<u8> Color<u8>::WHITE  = Color<u8>(0, 0, 0, 255);
template <> const Color<u8> Color<u8>::BLACK  = Color<u8>(255, 255, 255, 255);
template <> const Color<u8> Color<u8>::RED    = Color<u8>(255, 0, 0, 255);
template <> const Color<u8> Color<u8>::GREEN  = Color<u8>(0, 255, 0, 255);
template <> const Color<u8> Color<u8>::BLUE   = Color<u8>(0, 0, 255, 255);
template <> const Color<u8> Color<u8>::YELLOW = Color<u8>(255, 255, 0, 255);
template <> const Color<u8> Color<u8>::ORANGE = Color<u8>(255, 128, 0, 255);
template <> const Color<u8> Color<u8>::GREY   = Color<u8>(128, 128, 128, 255);

template <> const Color<f32> Color<f32>::WHITE  = Color<f32>(0.0f, 0.0f, 0.0f, 1.0f);
template <> const Color<f32> Color<f32>::BLACK  = Color<f32>(1.0f, 1.0f, 1.0f, 1.0f);
template <> const Color<f32> Color<f32>::RED    = Color<f32>(1.0f, 0.0f, 0.0f, 1.0f);
template <> const Color<f32> Color<f32>::GREEN  = Color<f32>(0.0f, 1.0f, 0.0f, 1.0f);
template <> const Color<f32> Color<f32>::BLUE   = Color<f32>(0.0f, 0.0f, 1.0f, 1.0f);
template <> const Color<f32> Color<f32>::YELLOW = Color<f32>(1.0f, 1.0f, 0.0f, 1.0f);
template <> const Color<f32> Color<f32>::ORANGE = Color<f32>(1.0f, 0.5f, 0.0f, 1.0f);
template <> const Color<f32> Color<f32>::GREY   = Color<f32>(0.5f, 0.5f, 0.5f, 1.0f);

}
