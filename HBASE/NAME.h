#pragma once
#include <string>

// virtual�� �ϸ� �����Լ� ���̺��� ����Ƿ�
// virtual�� ���ٸ� �ϴ� �����ϴ�.
class Name
{
private:
	std::wstring m_Name;

public:
	const wchar_t* GetName() {
		return m_Name.c_str();
	}

	const std::wstring& CWStringName() {
		return m_Name;
	}

	void SetName(const wchar_t* _Name) 
	{
		m_Name = _Name;
	}

	void SetName(const std::wstring& _Name)
	{
		m_Name = _Name;
	}

protected:
	Name();
	~Name();
};

