#pragma once
/// ===Include=== ///
// map
#include <map>
#include <string>
// Math
#include "Math/Vector3.h"

 // ColliderType
enum class ColliderType {
    AABB,      // 軸整合バウンディングボックス（矩形）
    Sphere,    // 球体
    Capsule,   // カプセル
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
    virtual void OnCollision() = 0;
    // Translateの取得
    void SetWorldTranslate(const Vector3& translate);

public: /// ===Getter=== ///
    // Translate
    Vector3 GetWorldTranslate() const;

protected:
    // Translate
    Vector3 translate_;
    // Type
    ColliderType type_;
    // ColliderName
    std::string colliderName_;
};

