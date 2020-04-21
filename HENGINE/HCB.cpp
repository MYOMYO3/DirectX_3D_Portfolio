#include "HCB.h"


HCB::HCB() : m_uiReg(0), m_bLink(false)
{
}


HCB::~HCB()
{

	if (m_bLink == false && nullptr != m_Data)
	{
		delete[](char*)m_Data;
		m_Data = nullptr;
	}

}

// ���ÿ� �ΰ��� ��������� �Ѵٸ�????
bool HCB::Create(HSHADER::SHADERTYPE _Shader, size_t _Byte, UINT _Reg)
{
	m_uiReg = _Reg;

	switch (_Shader)
	{
	case HSHADER::SHT_VS:
		ShUpdateFunc = &HCB::VSUpdate;
		break;
	case HSHADER::SHT_HS:
		ShUpdateFunc = &HCB::HSUpdate;
		break;
	case HSHADER::SHT_DS:
		ShUpdateFunc = &HCB::DSUpdate;
		break;
	case HSHADER::SHT_GS:
		ShUpdateFunc = &HCB::GSUpdate;
		break;
	case HSHADER::SHT_PS:
		ShUpdateFunc = &HCB::PSUpdate;
		break;
	default:
		break;
	}

	// �����͸� �����Ҵ����� ������ �ִ´�.
	m_Desc.ByteWidth = (UINT)_Byte;
	m_Data = new char[_Byte];
	// �̰� �׷���ī�尡 �����ұ� �ƴϸ� CPU�� �Ҵ��ұ� �� �̷��͵�
	// �׸��� �����Ҷ� �����Ҽ� �ְ� �Ұž�????
	m_Desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

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

// ����� ���⿡ ���´�.
void HCB::DataSetting(void* _pData, UINT _Size) {

	// ���� �ٲܲ���.
	GraphicDevice::MainContext()->Map(m_Buffer,
		0, D3D11_MAP_WRITE_DISCARD, 0, &m_tMapSub);
	// m_tMapSub.pData �׷���ī��� ���ϴ� ��θ� ���´�.

	// ���� �ٲٴ� �۾�
	memcpy_s(m_tMapSub.pData, m_Desc.ByteWidth, _pData, _Size);

	// ������ �ݴ´�.
	GraphicDevice::MainContext()->Unmap(m_Buffer, 0);
}

void HCB::DataSetting() 
{
	if (false == m_bLink)
	{
		BOOM;
	}

	GraphicDevice::MainContext()->Map(m_Buffer,
		0, D3D11_MAP_WRITE_DISCARD, 0, &m_tMapSub);
	// m_tMapSub.pData �׷���ī��� ���ϴ� ��θ� ���´�.

	// ���� �ٲٴ� �۾�
	memcpy_s(m_tMapSub.pData, m_Desc.ByteWidth, m_Data, BufferSize);

	// ������ �ݴ´�.
	GraphicDevice::MainContext()->Unmap(m_Buffer, 0);
}

void HCB::Update()
{
	(this->*ShUpdateFunc)();
}

void HCB::VSUpdate() { GraphicDevice::MainContext()->VSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }
void HCB::HSUpdate() { GraphicDevice::MainContext()->HSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }
void HCB::DSUpdate() { GraphicDevice::MainContext()->DSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }
void HCB::GSUpdate() { GraphicDevice::MainContext()->GSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }
void HCB::PSUpdate() { GraphicDevice::MainContext()->PSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }


void HCB::Link(void* _Data, unsigned int _Size)
{
	if (nullptr != m_Data)
	{
		delete[] m_Data;
	}

	if (0 >= _Size)
	{
		BOOM;
	}

	if (m_Desc.ByteWidth != _Size)
	{
		// ���� �������� �Ѵ�.
		BOOM;
	}

	m_Data = _Data;
	m_Desc.ByteWidth = _Size;
	BufferSize = _Size;
	m_bLink = true;
}