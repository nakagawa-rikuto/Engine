#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
// waveの読み込み
#include <xaudio2.h> 
// mp3の読み込み
#include <mfapi.h>
#include <mfobjects.h>
#include <mfidl.h>
#include <mfreadwrite.h>
// リンク
#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

// 音声データ
struct SoundData {
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファの先頭アドレス
	BYTE* pBuffer;
	// バッファのサイズ
	unsigned int bufferSize;
};

///=====================================================/// 
/// オーディオ
///=====================================================///
class Audio {
public:
	Audio() = default;
	~Audio();

	// 初期化
	void Initialze();

public:/// ===関数=== ///

	// 音声データの読み込み
	SoundData LoadWave(const char* filename);
	SoundData LoadMP3(const char* filename);
	// 音声データの解放
	void SoundUnload(SoundData* soundData);
	// サウンドの再生
	void SoundPlayWave(const SoundData& soundData, bool loop = false);
	// サウンドの停止
	void SoundStopWave();
	// 音量の設定
	void SetVolume(float volume);
	// 再生速度の設定
	void setPitch(float pitch);
	// 音の反響効果
	void InitializeReverbEffect();
	// 

private: /// ===変数=== ///
	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;
	IXAudio2SourceVoice* sourceVoice_ = nullptr;
};


