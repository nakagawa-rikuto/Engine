#include "System.h"
#include "WinApp.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <span>

WinApp* winApp = nullptr;

void System::Initialize(const wchar_t* title, int width, int height) {

    // ゲームウィンドウの作成
    winApp = WinApp::GetInstance();
    winApp->CreateGameWindow(title, width, height);
}

void System::Finalize(){
}

int System::ProcessMessage() { return winApp->ProcessMessage();}
