#pragma once

#include "../valve_sdk/sdk.hpp"

#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define M_PI 3.14159265358979323846
#define PI_F	((float)(M_PI)) 

namespace Math
{
    void NormalizeAngles(QAngle& angles);
    void ClampAngles(QAngle& angles);
    void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
    void AngleVectors(const QAngle &angles, Vector& forward);
    void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
    void VectorAngles(const Vector& forward, QAngle& angles);
    bool WorldToScreen(const Vector& in, Vector& out);
}