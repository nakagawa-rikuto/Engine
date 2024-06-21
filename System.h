#pragma once
class System {

public:

	/// <summary>
	/// システム全体の初期化
	/// </summary>
	/// <param name="title"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="enableDebugLayer"></param>
	static void Initialize(
		const wchar_t* title, int width = 1280, int height = 720);

	/// <summary>
	/// システム全体の終了
	/// </summary>
	static void Finalize();

	/// <summary>
	/// Windowsのメッセージを処理する
	/// </summary>
	/// <returns></returns>
	static int ProcessMessage();
};

