#pragma once
/// ===include=== ///
#include "Engine/Particle/ParticleEmitter.h"

enum class ParticleType {
	Wind
};

///=====================================================/// 
/// ParticleManager
///=====================================================///
class ParticleManager {
public:

	ParticleManager();
	~ParticleManager();

	void Create(ParticleType type);
	void Update(ParticleType type);
	void Draw(ParticleType type);

private:

	struct Type {
		bool wind;
	};

	Type type_;
};

