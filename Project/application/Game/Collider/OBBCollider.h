#pragma once
/// ===Includ=== ///
#include "Collider.h"

///-------------------------------------------/// 
/// OBBCollider
///-------------------------------------------///
class OBBCollider : public Collider {
public:

	OBBCollider() = default;
	~OBBCollider() = default;

    // 初期化
    virtual void Initialize() override;
    // 更新
    virtual void Update() = 0;
    // 描画
    virtual void Draw() = 0;

public: /// ===衝突=== ///
    virtual void OnCollision(Collider* collider) override = 0;

public: /// ===Getter=== ///
    OBB GetOBB() const;

protected:
    OBB obb_;
};

