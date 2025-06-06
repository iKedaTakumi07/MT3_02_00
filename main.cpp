#include <Novice.h>
#define _USE_MATH_DEFINES
#include <assert.h>
#include <cmath>
#include <cstdint>
#include <imgui.h>
#include <math.h>

const char kWindowTitle[] = "LE2C_03_イケダ_タクミ";

struct Vector3 {
    float x;
    float y;
    float z;
};
struct Matrix4x4 {
    float m[4][4];
};

struct Line {
    Vector3 origin;
    Vector3 diff;
};
struct Sphere {
    Vector3 center;
    float radius;
};

struct Ray {
    Vector3 origin;
    Vector3 diff;
};

struct Segment {
    Vector3 origin;
    Vector3 diff;
};

Vector3 add(const Vector3& v1, const Vector3& v2)
{
    Vector3 num;
    num.x = v1.x + v2.x;
    num.y = v1.y + v2.y;
    num.z = v1.z + v2.z;
    return num;
}
Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
    Vector3 num;
    num.x = v1.x - v2.x;
    num.y = v1.y - v2.y;
    num.z = v1.z - v2.z;
    return num;
}

// 行列の積
Matrix4x4 Mulyiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 num;
    num.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
    num.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
    num.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
    num.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

    num.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
    num.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
    num.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
    num.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

    num.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
    num.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
    num.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
    num.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

    num.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
    num.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
    num.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
    num.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

    return num;
}

// 4.逆行列の積
Matrix4x4 Inverse(const Matrix4x4& m)
{
    float determinant;
    Matrix4x4 num;

    determinant = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
        - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
        - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
        + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
        + m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
        - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
        - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
        + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

    if (determinant == 0.0f) {
        return m;
    };

    num.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) / determinant;
    num.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) / determinant;
    num.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) / determinant;
    num.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) / determinant;

    num.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) / determinant;
    num.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) / determinant;
    num.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) / determinant;
    num.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) / determinant;

    num.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) / determinant;
    num.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) / determinant;
    num.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) / determinant;
    num.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) / determinant;

    num.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) / determinant;
    num.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) / determinant;
    num.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) / determinant;
    num.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) / determinant;

    return num;
}

// 1.x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian)
{
    Matrix4x4 num;
    num = { 1, 0, 0, 0,
        0, std::cos(radian), std::sin(radian), 0,
        0, std::sin(-radian), std::cos(radian), 0,
        0, 0, 0, 1 };
    return num;
}

// 2.y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian)
{
    Matrix4x4 num;
    num = { std::cos(radian), 0, std::sin(-radian), 0,
        0, 1, 0, 0,
        std::sin(radian), 0, std::cos(radian), 0,
        0, 0, 0, 1 };
    return num;
}

// 3.z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian)
{
    Matrix4x4 num;
    num = { std::cos(radian), std::sin(radian), 0, 0,
        std::sin(-radian), std::cos(radian), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 };
    return num;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
    Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
    Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
    Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);
    Matrix4x4 rotateXYZ = Mulyiply(rotateX, Mulyiply(rotateY, rotateZ));

    Matrix4x4 num;
    num.m[0][0] = scale.x * rotateXYZ.m[0][0];
    num.m[0][1] = scale.x * rotateXYZ.m[0][1];
    num.m[0][2] = scale.x * rotateXYZ.m[0][2];
    num.m[0][3] = 0.0f * 0.0f * 0.0f * 0.0f;
    num.m[1][0] = scale.y * rotateXYZ.m[1][0];
    num.m[1][1] = scale.y * rotateXYZ.m[1][1];
    num.m[1][2] = scale.y * rotateXYZ.m[1][2];
    num.m[1][3] = 0.0f * 0.0f * 0.0f * 0.0f;
    num.m[2][0] = scale.z * rotateXYZ.m[2][0];
    num.m[2][1] = scale.z * rotateXYZ.m[2][1];
    num.m[2][2] = scale.z * rotateXYZ.m[2][2];
    num.m[2][3] = 0.0f * 0.0f * 0.0f * 0.0f;
    num.m[3][0] = translate.x;
    num.m[3][1] = translate.y;
    num.m[3][2] = translate.z;
    num.m[3][3] = 1.0f;
    return num;
}

// 1.透明投影行列
Matrix4x4 MakePrespectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
    Matrix4x4 num;
    num = { (1 / aspectRatio) * (1 / tanf(fovY / 2)), 0, 0, 0, 0, (1 / tanf(fovY / 2)), 0, 0, 0, 0, farClip / (farClip - nearClip), 1, 0, 0, (-nearClip * farClip) / (farClip - nearClip) };
    return num;
}

// 2.正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
    Matrix4x4 num;
    num = { 2 / (right - left), 0, 0, 0, 0, 2 / (top - bottom), 0, 0, 0, 0, 1 / (farClip - nearClip), 0, (left + right) / (left - right),
        (top + bottom) / (bottom - top),
        nearClip / (nearClip - farClip), 1 };
    return num;
}

// 3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
    Matrix4x4 num;
    num = { width / 2, 0, 0, 0, 0, -(height / 2), 0, 0, 0, 0, maxDepth - minDepth, 0, left + (width / 2), top + (height / 2), minDepth, 1 };
    return num;
}

// 3.座標変換
Vector3 TransForm(const Vector3& vector, const Matrix4x4& matrix)
{
    Vector3 result;
    result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
    result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
    result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
    float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
    assert(w != 0.0f);
    result.x /= w;
    result.y /= w;
    result.z /= w;

    return result;
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
    const float kGridHalfWidth = 2.0f;
    const uint32_t kSubdivison = 10;
    const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivison);
    // 奥から手前に
    for (uint32_t xIndex = 0; xIndex <= kSubdivison; ++xIndex) {
        // 終点始点
        Vector3 numa = { -kGridHalfWidth + (kGridEvery * xIndex), 0.0f, kGridHalfWidth };
        Vector3 numb = { -kGridHalfWidth + (kGridEvery * xIndex), 0.0f, -kGridHalfWidth };
        // スクリーン座標
        Vector3 ndcnumA = TransForm(numa, viewProjectionMatrix);
        Vector3 screennumA = TransForm(ndcnumA, viewportMatrix);
        Vector3 ndcnumB = TransForm(numb, viewProjectionMatrix);
        Vector3 screennumB = TransForm(ndcnumB, viewportMatrix);

        if (xIndex != 5) {
            Novice::DrawLine((int)screennumA.x, (int)screennumA.y, (int)screennumB.x, (int)screennumB.y, 0xAAAAAAFF);
        } else {
            Novice::DrawLine((int)screennumA.x, (int)screennumA.y, (int)screennumB.x, (int)screennumB.y, 0x000000FF);
        }
    }

    for (uint32_t zIndex = 0; zIndex <= kSubdivison; ++zIndex) {
        // 終点始点
        Vector3 numa = { kGridHalfWidth, 0.0f, -kGridHalfWidth + (kGridEvery * zIndex) };
        Vector3 numb = { -kGridHalfWidth, 0.0f, -kGridHalfWidth + (kGridEvery * zIndex) };
        // スクリーン座標
        Vector3 ndcnumA = TransForm(numa, viewProjectionMatrix);
        Vector3 screennumA = TransForm(ndcnumA, viewportMatrix);
        Vector3 ndcnumB = TransForm(numb, viewProjectionMatrix);
        Vector3 screennumB = TransForm(ndcnumB, viewportMatrix);

        if (zIndex != 5) {
            Novice::DrawLine((int)screennumA.x, (int)screennumA.y, (int)screennumB.x, (int)screennumB.y, 0xAAAAAAFF);
        } else {
            Novice::DrawLine((int)screennumA.x, (int)screennumA.y, (int)screennumB.x, (int)screennumB.y, 0x000000FF);
        }
    }
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    const uint32_t kSubdivision = 20;
    const float KLatEvery = float(2 * M_PI) / (float)kSubdivision;
    const float kLonEvery = float(M_PI) / (float)kSubdivision;

    for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
        float lat = -float(M_PI) / 2.0f + KLatEvery * float(latIndex);
        for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
            float lon = lonIndex * float(kLonEvery);

            //  計算
            Vector3 a {
                sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon),
                sphere.center.y + sphere.radius * std::sinf(lat),
                sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon)
            };
            Vector3 b {
                sphere.center.x + sphere.radius * std::cosf(lat + KLatEvery) * std::cosf(lon),
                sphere.center.y + sphere.radius * std::sinf(lat + KLatEvery),
                sphere.center.z + sphere.radius * std::cosf(lat + KLatEvery) * std::sinf(lon)
            };
            Vector3 c {
                sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon + kLonEvery),
                sphere.center.y + sphere.radius * std::sinf(lat),
                sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon + kLonEvery)
            };

            Vector3 ndca, ndcb, ndcc;
            Vector3 screena, screenb, screenc;

            ndca = TransForm(a, viewProjectionMatrix);
            ndcb = TransForm(b, viewProjectionMatrix);
            ndcc = TransForm(c, viewProjectionMatrix);

            screena = TransForm(ndca, viewportMatrix);
            screenb = TransForm(ndcb, viewportMatrix);
            screenc = TransForm(ndcc, viewportMatrix);

            Novice::DrawLine(int(screena.x), int(screena.y), int(screenb.x), int(screenb.y), color);
            Novice::DrawLine(int(screena.x), int(screena.y), int(screenc.x), int(screenc.y), color);
        }
    }
}

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
    float numAB;
    numAB = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    float numB2;
    numB2 = v2.x * v2.x + v2.y * v2.y + v2.z * v2.z;

    float numAB_B2 = 0.0f;
    numAB_B2 = numAB / numB2;

    Vector3 numAB_B2_B;
    numAB_B2_B = { numAB_B2 * v2.x, numAB_B2 * v2.y, numAB_B2 * v2.z };
    return numAB_B2_B;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
    Vector3 numA;
    numA = { point.x - segment.origin.x, point.y - segment.origin.y, point.z - segment.origin.z };

    float numAB;
    numAB = numA.x * segment.diff.x + numA.y * segment.diff.y + numA.z * segment.diff.z;
    float numB2;
    numB2 = segment.diff.x * segment.diff.x + segment.diff.y * segment.diff.y + segment.diff.z * segment.diff.z;

    float numAB_B2;
    numAB_B2 = numAB / numB2;

    Vector3 numAB_B2_B;
    numAB_B2_B = { numAB_B2 * segment.diff.x, numAB_B2 * segment.diff.y, numAB_B2 * segment.diff.z };

    Vector3 numCP;
    numCP = { segment.origin.x + numAB_B2_B.x, segment.origin.y + numAB_B2_B.y, segment.origin.z + numAB_B2_B.z };

    return numCP;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // 今回使うやつ
    Segment segment { { -2.0f, -1.0f, 0.0f }, { 3.0f, 2.0f, 2.0f } };
    Vector3 point { -1.5f, 0.6f, 0.6f };
    // pointを線形に射影したベクトル
    Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
    Vector3 closestpoint = ClosestPoint(point, segment);

    Vector3 cameraTranslate { 0.0f, 1.9f, -6.49f };
    Vector3 cameraRotate { 0.26f, 0.0f, 0.0f };

    Sphere pointSphere { point, 0.01f };
    Sphere closestPointShere { closestpoint, 0.01f };

    float kWindowWidth = 1280.0f;
    float kWindowHeight = 720.0f;

    Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
    Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
    Matrix4x4 viewMatrix = Inverse(cameraMatrix);
    Matrix4x4 projectionMatrix = MakePrespectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
    Matrix4x4 worldViewProjectionMatrix = Mulyiply(worldMatrix, Mulyiply(viewMatrix, projectionMatrix));
    Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

    Vector3 start = TransForm(TransForm(segment.origin, worldViewProjectionMatrix), viewportMatrix);
    Vector3 end = TransForm(TransForm(add(segment.origin, segment.diff), worldViewProjectionMatrix), viewportMatrix);

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        ImGui::Begin("test");
        ImGui::InputFloat3("point", &point.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("segment_origin", &segment.origin.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("segment_diff", &segment.diff.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::End();

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        DrawGrid(worldViewProjectionMatrix, viewportMatrix);
        DrawSphere(pointSphere, worldViewProjectionMatrix, viewportMatrix, RED);
        DrawSphere(closestPointShere, worldViewProjectionMatrix, viewportMatrix, BLACK);
        Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);
        ///
        /// ↑描画処理ここまで
        ///

        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}
