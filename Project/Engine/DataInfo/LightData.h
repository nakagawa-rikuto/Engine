#pragma once
#include "Math/Vector3.h"
#include "Math/Vector4.h"

/// ===Lightの種類のenum=== ///
enum class LightType {
	Lambert,
	HalfLambert,
	PointLight,
	SpotLight,
	None
};

/// ===DirectionalLight=== ///
struct DirectionalLightInfo {
	Vector4 color;
	Vector3 direction;
	float intensity;
};
/// ===ポイントライト=== ///
struct PointLightInfo {
	Vector4 color;
	Vector3 position;
	float intensity;
	float radius;
	float decay;
};
/// ===スポットライト=== ///
struct SpotLightInfo {
	Vector4 color;
	Vector3 position;
	float intensity;
	Vector3 direction;
	float distance;
	float decay;
	float cosAngle;
};
/// ===LightInfo=== ///
struct LightInfo {
	float shininess;
	DirectionalLightInfo directional;
	PointLightInfo point;
	SpotLightInfo spot;
};