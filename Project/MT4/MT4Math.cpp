#include "MT4Math.h"
#include <cstdio>

///-------------------------------------------/// 
/// 任意軸回転行列の関数
///-------------------------------------------///
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
    // 回転軸を正規化
    Vector3 normalizedAxis = Normalize(axis);

    float x = normalizedAxis.x;
    float y = normalizedAxis.y;
    float z = normalizedAxis.z;

    float cosTheta = std::cos(angle);
    float sinTheta = std::sin(angle);
    float oneMinusCosTheta = 1.0f - cosTheta;

    Matrix4x4 rotationMatrix;

    // ロドリゲスの回転公式に基づく計算
    rotationMatrix.m[0][0] = cosTheta + x * x * oneMinusCosTheta;
    rotationMatrix.m[0][1] = x * y * oneMinusCosTheta - z * sinTheta;
    rotationMatrix.m[0][2] = x * z * oneMinusCosTheta + y * sinTheta;
    rotationMatrix.m[0][3] = 0.0f;

    rotationMatrix.m[1][0] = y * x * oneMinusCosTheta + z * sinTheta;
    rotationMatrix.m[1][1] = cosTheta + y * y * oneMinusCosTheta;
    rotationMatrix.m[1][2] = y * z * oneMinusCosTheta - x * sinTheta;
    rotationMatrix.m[1][3] = 0.0f;

    rotationMatrix.m[2][0] = z * x * oneMinusCosTheta - y * sinTheta;
    rotationMatrix.m[2][1] = z * y * oneMinusCosTheta + x * sinTheta;
    rotationMatrix.m[2][2] = cosTheta + z * z * oneMinusCosTheta;
    rotationMatrix.m[2][3] = 0.0f;

    rotationMatrix.m[3][0] = 0.0f;
    rotationMatrix.m[3][1] = 0.0f;
    rotationMatrix.m[3][2] = 0.0f;
    rotationMatrix.m[3][3] = 1.0f;

    return rotationMatrix;
}

///-------------------------------------------/// 
/// 行列をコンソールに描画する関数
///-------------------------------------------///
void PrintMatrix(const Vector2& position, const Matrix4x4& matrix, const std::string& name) {
    // 名前を表示
    std::printf("%s\n", name.c_str());
    for (int i = 0; i < 4; ++i) {
        std::printf("(%5.3f, %5.3f, %5.3f, %5.3f)\n",
            matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
    }
}
