#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

// スフィアの情報
struct Sphere {
	Vector3 center;  // 中心
	float radius;    // 半径
};

// AABB
struct AABB {
	Vector3 min; //!<最小点
	Vector3 max; //!<最大点
};

// OBB
struct OBB {
	Vector3 center;          // !<中心
	Vector3 orientations[3]; // !<座標軸。正規化・直交必須
	Vector3 size;            // !<座標軸方向の長さの半分。中心空面までの距離
};

// 三角形の情報
struct Triangle {
	Vector3 vertices[3];
};

// 直線
struct Line {
	Vector3 origin; // !< 始点
	Vector3 diff;   // !< 終点への差分ベクトル
};

// 半直線
struct Ray {
	Vector3 origin; // !< 始点
	Vector3 diff;   // !< 終点への差分ベクトル
};

// 線分
struct Segment {
	Vector3 origin; // !< 始点
	Vector3 diff;   // !< 終点への差分ベクトル
};

// 平面
struct Plane {
	Vector3 normal; // !< 法線
	float distance; // !< 距離
};

// ばね
struct Spring {
	Vector3 anchor; // アンカー
	float naturalLength; // 自然長
	float stiffness; // 剛性
	float dampingCoefficient; // 減衰係数
};

// ボール
struct Ball {
	Vector3 position; // 位置
	Vector3 velocity; // 速度
	Vector3 acceleration; // 加速度
	float mass; // 質量
	float radius; // 半径
	unsigned int color; // 色
};

// 角速度
struct Angular {
	float velocity;
	float angle;
	float radius;
};

// 振り子
struct Pendulum {
	Vector3 anchor;  // アンカーポイント
	float length;    // 紐の長さ
	float angle;     // 現在の角度
	float angleVelocity;        // 角速度w 
	float angularAcceleration;  // 角加速度
};

// 円錐振り子
struct ConicalPendulum {
	Vector3 anchor;      // アンカーポイント
	float length;        // 紐の長さ
	float halfApexAngle; // 円錐の頂角の半分
	float angle;         // 現在の角度
	float angularVelocity; // 角度速w
};

// カプセル
struct Capsule {
	Segment segment;
	float radius;
};
