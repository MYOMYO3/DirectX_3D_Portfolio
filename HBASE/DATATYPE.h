#pragma once
#include <typeinfo>
// #include <type_traits>

// #include <typeinfo>

// C++ 
// RTTI ���� Ÿ�� Ÿ�� ����
// RTTI ���� Ÿ�� Ÿ�� ���̵�Ƽ���̼�
// �����ڰ� �ִ�.
// type_info Data = typeid(�ڷ��� Ȥ�� �ĺ���)

class DataType
{
private:
	const type_info* m_Info;

public:
	void TypeSetting();

public:
	bool IsType(const type_info* _Info)
	{
		return m_Info == _Info;
	}

	template<typename T>
	bool IsType() 
	{
		return m_Info == &typeid(T);
	}

	bool IsType(const DataType& _Name)
	{
		return m_Info == _Name.m_Info;
	}



public:
	DataType();
	virtual ~DataType();
};

