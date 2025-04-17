#pragma once
/// ===Include=== ///
// map
#include <map>
#include <string>
// Math
#include "Engine/DataInfo/ColliderData.h"

 // ColliderType
enum class ColliderType {
    AABB,      // 軸整合バウンディングボックス（矩形）
    Sphere,    // 球体
    OBB        // 任意方向のバウンディングボックス
};

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

protected:
    // Type
    ColliderType type_;
    // ColliderName
    std::string colliderName_;
};

