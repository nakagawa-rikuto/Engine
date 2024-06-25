#pragma once

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

	// Add
	Vector2 operator+=(Vector2& obj);
	Vector2 const operator+(const Vector2& obj) const;

	// Sub
	Vector2 operator-=(Vector2& obj);
	Vector2 const operator-(const Vector2& obj) const;

	// 
	Vector2 operator*=(float& obj);
	Vector2 const operator*(const float& obj) const;
};

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	// Add
	Vector3 operator+=(Vector3& obj);
	Vector3 const operator+(const Vector3& obj) const;

	// Sub
	Vector3 operator-=(Vector3& obj);
	Vector3 const operator-(const Vector3& obj) const;

	// 
	Vector3 operator*=(float& obj);
	Vector3 const operator*(const float& obj) const;
};

/// <summary>
/// 4次元ベクトル
/// </summary>
struct Vector4 final {
	float x;
	float y;
	float z;
	float w;

	// Add
	Vector4 operator+=(Vector4& obj);
	Vector4 const operator+(const Vector4& obj) const;

	// Sub
	Vector4 operator-=(Vector4& obj);
	Vector4 const operator-(const Vector4& obj) const;

	// 
	Vector4 operator*=(float& obj);
	Vector4 const operator*(const float& obj) const;
};
