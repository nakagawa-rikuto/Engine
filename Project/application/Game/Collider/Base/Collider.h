#pragma once
/// ===Include=== ///
// map
#include <map>
#include <string>
// Object3D
#include "application/Drawing/3d/Object3d.h"
// Math
#include "Engine/DataInfo/ColliderData.h"

 // ColliderType
enum class ColliderType {
    AABB,      // 軸整合バウンディングボックス（矩形）
    Sphere,    // 球体
    OBB        // 任意方向のバウンディングボックス
};

/// ===前方宣言=== ///
class Camera;

///=====================================================/// 
/// コライダー
///=====================================================///
class Collider {
public:

    Collider() = default;
    virtual ~Collider() = default;

    // 初期化
    virtual void Initialize() = 0;

public: /// ===衝突=== ///
    // 衝突時の応答処理
    virtual void OnCollision(Collider* collider) = 0;

public: /// ===Getter=== ///
    // Type
    ColliderType GetColliderType();
    // CollsisionName
    std::string GetColliderName();

public: /// ===Setter=== ///
    //NOTE:定点用のセッター
	// Object3D
    void SetTranslate(const Vector3& translate);
	void SetRotate(const Quaternion& rotate);
	void SetScale(const Vector3& scale);
	void SetColor(const Vector4& color);
    // Camera
    void SetCamera(Camera* camera);
	// Light
	void SetLight(LightType type);
	void SetLightData(LightInfo light);

protected:
    // Type
    ColliderType type_;
    // ColliderName
    std::string colliderName_;

	// Object3D
	std::unique_ptr<Object3d> object3d_;
};

