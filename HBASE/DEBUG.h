#pragma once

#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <crtdbg.h>

#define BOOM assert(false)
// ����� �����Լ� �� ������ ������ ���ּ���.
class Debug
{
	// ���ڿ�
	// ���������
	// �����
public:
	static bool IsOpenConsole;
	static void End() 
	{
		if (true == IsOpenConsole)
		{
			FreeConsole();   //�ܼ��� ����
		}
	}

	static void LeakCheck() 
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	static void ConsolePrintLineArg(const wchar_t* _Text, ...)
	{
		if (false == IsOpenConsole) { return; }

		// ù�ּ�
		va_list		argp;
		va_start(argp, _Text);
		wchar_t Text[1024] = { 0, };
		swprintf_s(Text, _Text, argp);
		va_end(argp);
		std::wcout << Text << std::endl;
	}

	static void ConsolePrintArg(const wchar_t* _Text, ...)
	{
		if (false == IsOpenConsole) { return; }

		va_list		argp;
		va_start(argp, _Text);
		wchar_t Text[1024] = { 0, };
		swprintf_s(Text, _Text, argp);
		va_end(argp);

		std::wcout << _Text;
	}

	static void OutputDebugStringW(const wchar_t* _Text = nullptr)
	{
		if (nullptr == _Text)
		{
			return;
		}

		OutputDebugStringW(_Text);
	}


	static void ConsolePrintLine(const wchar_t* _Text = nullptr) 
	{
		if (false == IsOpenConsole)	{ return; }
		if (nullptr == _Text)
		{
			std::wcout << std::endl;
			return;
		}
		std::wcout << _Text << std::endl;
	}


	static void ConsolePrintA(const char* _Text)
	{
		if (false == IsOpenConsole) { return; }
		std::cout << _Text;
	}


	static void ConsolePrint(const wchar_t* _Text)
	{
		if (false == IsOpenConsole) { return; }
		std::wcout << _Text;
	}

	static void OpenConsole()
	{

		//return;
		// �ܼ��� �ϳ� �ߴ°Ű�.
		// �� �ܼ��� ����¿� ������ ������Ѵ�.
		// �ܼ����� ���� ������ �Է����� ��ɷ� �ְڴ�.
		// �ܼ����� �Է��� �������� �Է��� ��ɷ� �ְڴ�.
		if (AllocConsole())
		{
			FILE *acStreamOut;
			FILE *acStreamIn;
			freopen_s(&acStreamOut, "CONOUT$", "wt", stdout);
			freopen_s(&acStreamIn, "CONIN$", "r", stdin);
			IsOpenConsole = true;
			std::wcout.imbue(std::locale("kor"));
			std::wcout << L"Console Debug Start OK" << std::endl;
		}
	}

private:
	Debug() {}
	~Debug() {}

};