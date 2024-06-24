#pragma once
#include <Windows.h>
#include <string>
#include <format>

/// <summary>
/// string->wstring
/// </summary>
/// <param name="message"></param>
void Log(const std::string& message);

/// <summary>
/// wstring->string
/// </summary>
/// <param name="message"></param>
void Log(const std::wstring& message);

/// <summary>
/// string->wstringに変換
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring ConvertString(const std::string& str);

/// <summary>
/// wstring->stringに変換
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::string ConvertString(const std::wstring& str);