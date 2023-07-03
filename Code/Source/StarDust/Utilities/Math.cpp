#include <StarDust/Utilities/Math.h>
#include <Universal/Math/Math.h>
#include <cmath>

namespace Star::Utils
{
    Uni::Math::Matrix4x4f CreatePerspectiveProjectionMatrix(
        float fov, float aspectRatio, float near, float far)
    {
        if (!Uni::Math::IsInRange(fov, 0.0f, Uni::Math::Constants::PI) ||
            aspectRatio == 0.0f ||
            !Uni::Math::IsInRange(aspectRatio, 0.0f, 1.0f) || near == 0.0f ||
            near == far || !Uni::Math::IsInRange(near, 0.0f, far))
        {
            throw std::invalid_argument("Invalid argument(s)");
        }

        const float Tan = std::tan(fov / 2.0f);
        const float A = -(far + near) / (far - near);
        const float B = -(2.0f * far * near) / (far - near);

        // clang-format off
        return Uni::Math::Matrix4x4f::CreateFromRows(
            { 1.0f / (Tan * aspectRatio), 0.0f,       0.0f, 0.0f },
            { 0.0f,                       1.0f / Tan, 0.0f, 0.0f },
            { 0.0f,                       0.0f,       A,    B    },
            { 0.0f,                       0.0f,      -1.0f, 0.0f });
        // clang-format on
    }

    Uni::Math::Matrix4x4f CreateOrthographicProjectionMatrix(
        float left, float right, float bottom, float top, float near, float far)
    {
        if (left == right || bottom == top || near == far)
        {
            throw std::invalid_argument("Invalid argument(s)");
        }

        left = std::min(left, right);
        right = std::max(left, right);
        bottom = std::min(bottom, top);
        top = std::max(bottom, top);
        near = std::min(near, far);
        far = std::max(near, far);

        const float TopBottomDiff = top - bottom;
        const float RightLeftDiff = right - left;
        const float NearFarDiff = far - near;

        // clang-format off
        return Uni::Math::Matrix4x4f::CreateFromRows(
            { 2.0f / RightLeftDiff, 0.0f, 0.0f,               -(right + left) / RightLeftDiff },
            { 0.0f, 2.0f / TopBottomDiff, 0.0f,               -(top + bottom) / TopBottomDiff },
            { 0.0f, 0.0f,                -2.0f / NearFarDiff, -(far + near) / NearFarDiff     },
            { 0.0f, 0.0f,                0.0f,                1.0f                            });
        // clang-format on
    }

    Uni::Math::Matrix4x4f CreateLookAtMatrix(
        const Uni::Math::Vector3f& position,
        const Uni::Math::Vector3f& target,
        const Uni::Math::Vector3f& up)
    {
        const Uni::Math::Vector3f Back = (position - target).GetNormalized();
        const Uni::Math::Vector3f Right =
            up.GetCrossProduct(Back).GetNormalized();
        const Uni::Math::Vector3f Up = Back.GetCrossProduct(Right);

        const Uni::Math::Matrix3x4f TranslationInverse =
            Uni::Math::Matrix3x4f::CreateTranslation(-position);
        const Uni::Math::Matrix3x4f RotationInverse =
            // clang-format off
            Uni::Math::Matrix3x4f::CreateFromRows(
                { Right, 0.0f },
                { Up,    0.0f },
                { Back,  0.0f });
        // clang-format on

        return (RotationInverse * TranslationInverse).ToMatrix4x4f();
    }
} // namespace Star::Utils