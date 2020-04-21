#include "H3DBUFFER.h"



H3DBUFFER::H3DBUFFER()
{
}


H3DBUFFER::~H3DBUFFER()
{
	if (nullptr != m_Buffer)
	{
		m_Buffer->Release();
		m_Buffer = nullptr;
	}
}


bool H3DBUFFER::CreateBuffer(unsigned int _Size, D3D11_USAGE _Usage, unsigned int _BindFlag)
{
	// �����͸� �����Ҵ����� ������ �ִ´�.
	m_Desc.ByteWidth = (UINT)_Size;
	// �̰� �׷���ī�尡 �����ұ� �ƴϸ� CPU�� �Ҵ��ұ� �� �̷��͵�
	// �׸��� �����Ҷ� �����Ҽ� �ְ� �Ұž�????
	m_Desc.Usage = _Usage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_Desc.Usage)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (S_OK != GraphicDevice::MainDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer))
	{
		BOOM;
		return false;
	}

	return true;
}