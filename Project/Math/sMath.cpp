#include "sMath.h"
#include <cassert>
#include <numbers>

///=====================================================///
/// π（πを使用する際に用いる関数）
///=====================================================///
float Math::Pi() { return std::numbers::pi_v<float>; }

///=====================================================///
/// クロス積の計算
///=====================================================///
Vector3 Math::Cross(const Vector3& v1, const Vector3& v2) {

    Vector3 result{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };

    return result;
}

///-------------------------------------------/// 
/// Quaternion
///-------------------------------------------///
// QuaternionのNormを返す
float Math::Norm(const Quaternion& quaternion) {
    return sqrtf(quaternion.x * quaternion.x +
        quaternion.y * quaternion.y +
        quaternion.z * quaternion.z +
        quaternion.w * quaternion.w);
}
// 単位Quaternionを返す
Quaternion Math::IdentityQuaternion() {
    return { 0.0f, 0.0f, 0.0f, 1.0f };
}
// 共役Quaternionを返す
Quaternion Math::Conjugate(const Quaternion& quaternion) {
    return {
        -quaternion.x,
        -quaternion.y,
        -quaternion.z,
        quaternion.w
    };
}
// 逆Quaternionを返す
Quaternion Math::Inverse(const Quaternion& quaternion) {
    float normSquared = quaternion.x * quaternion.x +
        quaternion.y * quaternion.y +
        quaternion.z * quaternion.z +
        quaternion.w * quaternion.w;
    if (normSquared == 0.0f) {
        // Avoid division by zero
        return IdentityQuaternion();
    }
    Quaternion conjugate = Conjugate(quaternion);
    return {
        conjugate.x / normSquared,
        conjugate.y / normSquared,
        conjugate.z / normSquared,
        conjugate.w / normSquared
    };
}
// 任意軸回転Quaternionを返す
Quaternion Math::MakeRotateAxisAngle(const Vector3& axis, float angle) {
    Quaternion result;
    float halfAngle = angle / 2.0f;
    float sinHalfAngle = sinf(halfAngle);
    result.x = axis.x * sinHalfAngle;
    result.y = axis.y * sinHalfAngle;
    result.z = axis.z * sinHalfAngle;
    result.w = cosf(halfAngle);
    return result;
}
// Quaternionの回転結果をベクトルで返す
Vector3 Math::RotateVector(const Vector3& vector, const Quaternion& quaternion) {
    Vector3 result;
    Quaternion q = quaternion;
    Quaternion v = { vector.x,vector.y,vector.z,0.0f };
    Quaternion qConjugate = Conjugate(q);
    Quaternion v1 = Multiply(q, v);
    Quaternion v2 = Multiply(v1, qConjugate);
    result.x = v2.x;
    result.y = v2.y;
    result.z = v2.z;
    return result;
}
// QuaternionからX軸の角度を取る関数
float Math::GetXAngle(const Quaternion& quaternion) {
    // 単位ベクトル (0,0,1) をクォータニオンで回転させる
    Vector3 forward = RotateVector(Vector3(0.0f, 0.0f, 1.0f), quaternion);
    // forward.y がピッチ角度（上向きか下向きか）を示す
    return std::asin(forward.y); // ラジアン角
}
// QuaternionからY軸の角度を取る関数
float Math::GetYAngle(const Quaternion& quaternion) {
    // 単位ベクトル (0,0,1) をクォータニオンで回転させる
    Vector3 forward = RotateVector(Vector3(0.0f, 0.0f, 1.0f), quaternion);
    // atan2(forward.x, forward.z) で Yaw（水平回転） を取得。
    return std::atan2(forward.x, forward.z); // Yaw 角度（ラジアン）
}
