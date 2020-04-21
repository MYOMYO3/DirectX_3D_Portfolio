#include "HPIXSHADER.h"



HPIXSHADER::HPIXSHADER() : HSHADER(HSHADER::SHADERTYPE::SHT_PS), m_TargetCount(1)
{
}


HPIXSHADER::~HPIXSHADER()
{
	if (nullptr != m_pPixShader)
	{
		m_pPixShader->Release();
		m_pPixShader = nullptr;
	}
}


bool HPIXSHADER::Load(unsigned int _VH, unsigned int _VL, const char* _FuncName)
{
	m_VH = _VH;
	m_VL = _VL;

	unsigned int _Flag = D3D10_SHADER_DEBUG;

#ifdef _DEBUG 
	_Flag = D3D10_SHADER_DEBUG;
#endif

	sprintf_s(VERBUFFERTEXT, "ps_%d_%d", m_VH, m_VL);


	// ����ܰ迡������ �� ������ ����� �Ұ����� ���δ�.
	if (
		S_OK != D3DCompileFromFile
		(
			m_File.GetFullPath(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			_FuncName,
			VERBUFFERTEXT,
			_Flag, // ����׸���϶� ����� ����� ���� �˷��ش�.
			0, // ������ �𸣰���
			&m_pBlob,
			&m_pErrBlob
		)
		)
	{
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();

		Debug::ConsolePrintA(ErrorText.c_str());
		BOOM;
		return false;
	}

	if (
		GraphicDevice::MainDevice()->CreatePixelShader
		(
			m_pBlob->GetBufferPointer(),
			m_pBlob->GetBufferSize(),
			nullptr,
			&m_pPixShader
		)
		)
	{
		BOOM;
		return false;
	}

	return true;
}

void HPIXSHADER::Update()
{
	if (nullptr != m_pPixShader)
	{
		GraphicDevice::MainContext()->PSSetShader(m_pPixShader, nullptr, 0);
	}
}