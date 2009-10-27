#include "matrix4.h"

namespace fire_engine
{
template <>
const matrix4<f32> matrix4<f32>::IDENTITY_MATRIX = matrix4<f32>(1.0f, 0.0f, 0.0f, 0.0f,
																0.0f, 1.0f, 0.0f, 0.0f,
																0.0f, 0.0f, 1.0f, 0.0f,
	                                                            0.0f, 0.0f, 0.0f, 1.0f);
template <>
const matrix4<f64> matrix4<f64>::IDENTITY_MATRIX = matrix4<f64>(1.0, 0.0, 0.0, 0.0,
	                                                            0.0, 1.0, 0.0, 0.0,
	                                                            0.0, 0.0, 1.0, 0.0,
	                                                            0.0, 0.0, 0.0, 1.0);
}
