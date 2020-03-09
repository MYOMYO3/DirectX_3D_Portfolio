#pragma once
#include <string>
#include <vector>
#include <map>
#include "REF.h"

#include "DEBUG.h"
// ���ȴ�. �ȴ��ȴ�.
// ���������� ġ��

// ������ ����.
// bool ���� �Ѽ��� ���ȴ�. 1������ true�� �ȴ�.
// bool ��� ������ �ִ�. ���������� ������ �ִ�.

// �ڽ��� �ʿ��� �͸� �����
// bool �ȴ����ִ�. -> ���������� ������ �ִ�.
// bool ������. -> 1�����Ӹ� ����.

// bool �� ���ϵȴ�.
// 236 LP
// ����Ű�� �����ϰ�. 'A' 'B'

// if(ture == HINPU::GetUpAway(L"Fire"))
// if(ture == HINPU::GetPress(L"Fire"))
// if(ture == HINPU::GetUp(L"Fire"))
// if(ture == HINPU::GetDown(L"Fire"))
// �Ѿ� ��� �ڵ� ���

class Input
{
private:
	//class Destructor
	//{
	//public:
	//	~Destructor() {
	//		HINPUT::InputEnd();
	//	}
	//};
	//static Destructor DES;
	//friend Destructor;

private:
	class Key : public Ref
	{
	private:
		friend Input;

	public:
		// float Time ; // ���ʰ� ���ȴٸ�.
		bool m_Up;
		bool m_UpAway; // �̳༮�� true
		bool m_Down;
		bool m_Press;
		std::vector<int> m_KeyArray;

	public:
		bool IsKeyCheck(); // Űüũ�ϴ� ����.
		void Update(); // Űüũ�ϴ� ����.

	private:
		Key();
		~Key();
	};

private:
	static std::map<std::wstring, HPTR<Key>>::iterator KeyStart;
	static std::map<std::wstring, HPTR<Key>>::iterator KeyEnd;
	static std::map<std::wstring, HPTR<Key>> m_AllKey;

	// static HPOS MousePos(HWND _Window);

	// static void InputEnd();

private:
	static HPTR<Key> FindKey(const wchar_t* _Name);

public:
	static void Update();

public:
	static HPTR<Key> NewKey;

	template<typename ...Rest>
	static void CreateKey(const wchar_t* _Name, Rest ..._Arg) 
	{
		NewKey = FindKey(_Name);

		if (nullptr != NewKey)
		{
			BOOM;
		}

		NewKey = new Key();
		// ���͸� ����Ҷ��� �����곪 �������� �ؼ�
		// ���� �ƴϸ� ���� ���ƶ�.
		NewKey->m_KeyArray.reserve(sizeof...(_Arg));
		PushKey(_Arg...);

		m_AllKey.insert(std::map<std::wstring, HPTR<Key>>::value_type(_Name, NewKey));
	}

	//template<typename T, typename ...Rest>
	//static void PushKey(T _Key, Rest ..._Arg)
	//{
	//  sizeof...(_Arg) // � ó���Ǿ � ���Ҵٴ� ���� �˼� �ִ�.
	//  if(&typeid(int) == typeid(_Key))
	//	NewKey->m_KeyArray.push_back(_Key);
	//	PushKey(_Arg...);
	//}

	template<typename ...Rest>
	static void PushKey(int _Key, Rest ..._Arg)
	{
		NewKey->m_KeyArray.push_back(_Key);
		PushKey(_Arg...);
	}

	static void PushKey()
	{
	}

public:
	static bool IsKey(const wchar_t* _Name);

	static bool Down(const wchar_t* _Name);
	static bool UpAway(const wchar_t* _Name);
	static bool Press(const wchar_t* _Name);
	static bool Up(const wchar_t* _Name);


public:
	Input();
	~Input();
};

