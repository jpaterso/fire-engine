#include "quaternion.h"

namespace fire_engine
{
template <> const quaternionf quaternionf::IDENTITY_QUATERNION = quaternionf(1.0f, 0.0f, 0.0f, 0.0f);
template <> const quaterniond quaterniond::IDENTITY_QUATERNION = quaterniond(1.0, 0.0, 0.0, 0.0);
template <> const quaternionf quaternionf::ZERO_QUATERNION = quaternionf(0.0f, 0.0f, 0.0f, 0.0f);
template <> const quaterniond quaterniond::ZERO_QUATERNION = quaterniond(0.0, 0.0, 0.0, 0.0);
}
