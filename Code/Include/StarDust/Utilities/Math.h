#pragma once

#include <Universal/Math/Matrix/Matrix3x4f.h>

namespace Star::Utils
{
    Uni::Math::Matrix4x4f CreatePerspectiveProjectionMatrix(float fov, float aspectRatio,
        float near, float far);

    Uni::Math::Matrix4x4f CreateOrthographicProjectionMatrix(float left, float right,
        float bottom, float top, float near, float far);

    Uni::Math::Matrix4x4f CreateLookAtMatrix(const Uni::Math::Vector3f& position,
        const Uni::Math::Vector3f& target, const Uni::Math::Vector3f& up);
} // namespace Star::Utils
