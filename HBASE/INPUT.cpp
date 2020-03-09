#include "INPUT.h"
#include <Windows.h>

// �𸣴ϱ� hkey�� �������� �˷���� �Ѵ�.

HPTR<Input::Key> Input::NewKey = nullptr;
std::map<std::wstring, HPTR<Input::Key>> Input::m_AllKey;
std::map<std::wstring, HPTR<Input::Key>>::iterator Input::KeyStart;
std::map<std::wstring, HPTR<Input::Key>>::iterator Input::KeyEnd;

// HINPUT::Destructor HINPUT::DES;


/////////////////////////// Key

Input::Key::Key() : m_Up(false),m_UpAway(true),m_Down(false),m_Press(false)
{

}
Input::Key::~Key() {

}

void Input::Key::Update() 
{
	if (true == IsKeyCheck()) // ��Ű�� �����ִ�.
	{
		if (true == m_UpAway) // ���� �ȴ����ִ�.
		{
			// Ű�� ���ȴ°�? yes
			// ���� �����ӿ��� Ű�� �����°�? no
			m_Up = false;
			m_UpAway = false;
			m_Down = true;
			m_Press = true;
		}
		else 
		{
			// Ű�� ���ȴ°�? yes
			// ���� �����ӿ��� Ű�� �����°�? yes
			m_Up = false;
			m_UpAway = false;
			m_Down = false;
			m_Press = true;
		}
	}
	else 
	{

		if (true == m_Press) 
		{
			// Ű�� ���ȴ°�? no
			// ���� �����ӿ��� Ű�� �����°�? yes
			m_Up = true;
			m_UpAway = true;
			m_Down = false;
			m_Press = false;
		}
		else
		{
			// Ű�� ���ȴ°�? no
			// ���� �����ӿ��� Ű�� �����°�? no
			m_Up = false;
			m_UpAway = true;
			m_Down = false;
			m_Press = false;
		}
	}
}

bool Input::Key::IsKeyCheck() 
{
	for (size_t i = 0; i < m_KeyArray.size(); i++)
	{
		if (0 == GetAsyncKeyState(m_KeyArray[i]))
		{
			return false;
		}
	}

	return true;
}

////////////////////// Input



Input::Input()
{
}


Input::~Input()
{
}


HPTR<Input::Key> Input::FindKey(const wchar_t* _Name)
{
	std::map<std::wstring, HPTR<Key>>::iterator FindIter = m_AllKey.find(_Name);

	if (FindIter == m_AllKey.end())
	{
		return nullptr;
	}

	return FindIter->second;
}
//
//void HINPUT::InputEnd()
//{
//	KeyStart = m_AllKey.begin();
//	KeyEnd = m_AllKey.end();
//
//	for (; KeyStart != KeyEnd; ++KeyStart)
//	{
//		if (nullptr != KeyStart->second)
//		{
//			delete KeyStart->second;
//			KeyStart->second = nullptr;
//		}
//	}
//}


void Input::Update() 
{
	KeyStart = m_AllKey.begin();
	KeyEnd = m_AllKey.end();

	for (; KeyStart != KeyEnd; ++KeyStart)
	{
		KeyStart->second->Update();
	}
}

bool Input::Down(const wchar_t* _Name) 
{
	HPTR<Input::Key> FKey = FindKey(_Name);

	if (nullptr == FKey)
	{
		BOOM;
	}

	return FKey->m_Down;
}
bool Input::UpAway(const wchar_t* _Name) {
	HPTR<Input::Key> FKey = FindKey(_Name);

	if (nullptr == FKey)
	{
		BOOM;
	}

	return FKey->m_UpAway;
}
bool Input::Press(const wchar_t* _Name) {
	HPTR<Input::Key> FKey = FindKey(_Name);

	if (nullptr == FKey)
	{
		BOOM;
	}

	return FKey->m_Press;
}
bool Input::Up(const wchar_t* _Name) {
	HPTR<Input::Key> FKey = FindKey(_Name);

	if (nullptr == FKey)
	{
		BOOM;
	}

	return FKey->m_Up;
}

bool Input::IsKey(const wchar_t* _Name) {
	HPTR<Input::Key> FKey = FindKey(_Name);

	if (nullptr == FKey)
	{
		return false;
	}

	return true;
}