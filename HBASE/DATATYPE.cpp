#include "DATATYPE.h"
#include <iostream>


DataType::DataType() : m_Info(nullptr)
{
}


DataType::~DataType()
{
}

// ����.. 
void DataType::TypeSetting()
{
	
	// ���������� ������ 
	m_Info = &typeid(*this);




	std::cout << m_Info->name() << std::endl;
}