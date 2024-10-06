#pragma once

/// <summary>
/// テクスチャマネージャ
/// </summary>
class TextureManager {

public:

private:

	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = default;
};

