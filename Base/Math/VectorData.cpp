#include "VectorData.h"

#pragma region /* Vector2 */
// Add
Vector2 Vector2::operator+=(Vector2& obj) {

	this->x += obj.x;
	this->y += obj.y;

	return Vector2{ this->x, this->y };
}
Vector2 const Vector2::operator+(const Vector2& obj) const {

	Vector2 sc = {};
	sc.x = this->x + obj.x;
	sc.y = this->y + obj.y;

	return sc;
}

// Sub
Vector2 Vector2::operator-=(Vector2& obj) {

	this->x -= obj.x;
	this->y -= obj.y;

	return Vector2{ this->x, this->y };
}
Vector2 const Vector2::operator-(const Vector2& obj) const {

	Vector2 sc = {};
	sc.x = this->x - obj.x;
	sc.y = this->y - obj.y;

	return sc;
}

// 
Vector2 Vector2::operator*=(float& obj) {

	this->x *= obj;
	this->y *= obj;

	return Vector2{ this->x, this->y };
}
Vector2 const Vector2::operator*(const float& obj) const {

	Vector2 sc = {};
	sc.x = this->x * obj;
	sc.y = this->y * obj;

	return sc;
}
#pragma endregion
#pragma region /* Vector3 */
// Add
Vector3 Vector3::operator+=(Vector3& obj) {

	this->x += obj.x;
	this->y += obj.y;
	this->z += obj.z;

	return Vector3{ this->x, this->y, this->z };
}
Vector3 const Vector3::operator+(const Vector3& obj) const {

	Vector3 sc = {};
	sc.x = this->x + obj.x;
	sc.y = this->y + obj.y;
	sc.z = this->z + obj.z;

	return sc;
}

// Sub
Vector3 Vector3::operator-=(Vector3& obj) {

	this->x -= obj.x;
	this->y -= obj.y;
	this->z -= obj.z;

	return Vector3{ this->x, this->y, this->z };
}
Vector3 const Vector3::operator-(const Vector3& obj) const {

	Vector3 sc = {};
	sc.x = this->x - obj.x;
	sc.y = this->y - obj.y;
	sc.z = this->z - obj.z;

	return sc;
}

// 
Vector3 Vector3::operator*=(float& obj) {

	this->x *= obj;
	this->y *= obj;
	this->z *= obj;

	return Vector3{ this->x, this->y, this->z };
}
Vector3 const Vector3::operator*(const float& obj) const {
	Vector3 sc = {};
	sc.x = this->x * obj;
	sc.y = this->y * obj;
	sc.z = this->z * obj;

	return sc;
}
#pragma endregion
#pragma region /* Vector4 */
// Add
Vector4 Vector4::operator+=(Vector4& obj) {

	this->x += obj.x;
	this->y += obj.y;
	this->z += obj.z;
	this->w += obj.w;

	return Vector4{ this->x, this->y, this->z, this->w };
}
Vector4 const Vector4::operator+(const Vector4& obj) const {

	Vector4 sc = {};
	sc.x = this->x + obj.x;
	sc.y = this->y + obj.y;
	sc.z = this->z + obj.z;
	sc.w = this->w + obj.w;

	return sc;
}

// Sub
Vector4 Vector4::operator-=(Vector4& obj) {

	this->x -= obj.x;
	this->y -= obj.y;
	this->z -= obj.z;
	this->w -= obj.w;

	return Vector4{ this->x, this->y, this->z, this->w };
}
Vector4 const Vector4::operator-(const Vector4& obj) const {

	Vector4 sc = {};
	sc.x = this->x - obj.x;
	sc.y = this->y - obj.y;
	sc.z = this->z - obj.z;
	sc.w = this->w - obj.w;

	return sc;
}

// 
Vector4 Vector4::operator*=(float& obj) {

	this->x *= obj;
	this->y *= obj;
	this->z *= obj;
	this->w *= obj;

	return Vector4{ this->x, this->y, this->z, this->w };
}
Vector4 const Vector4::operator*(const float& obj) const {

	Vector4 sc = {};
	sc.x = this->x * obj;
	sc.y = this->y * obj;
	sc.z = this->z * obj;
	sc.w = this->w * obj;

	return sc;
}
#pragma endregion