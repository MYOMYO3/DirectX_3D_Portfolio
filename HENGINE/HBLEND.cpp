#include "HBLEND.h"
#include "GRAPHICDEVICE.h"


HBLEND::HBLEND() : m_Color(1.0f)
{
}


HBLEND::~HBLEND()
{
	if (nullptr != m_pState)
	{
		m_pState->Release();
		m_pState = nullptr;
	}
}


bool HBLEND::Create() 
{
	// ���� �Ұų� 
	// false�� �Ѱ��� ����ϰڴٶ�� �̾߱�
	// ���� 0���� ����ϰڴ�.
	// ����Ÿ���� �������� 0���� ���� ��ĸ��� �̿��ϰڴٴ� ����
	// ���� ������ �༮���� ������ ���ϰڴٴ� ���� �ƴϴ�.

	//BOOL BlendEnable;
	//D3D11_BLEND SrcBlend;
	//D3D11_BLEND DestBlend;
	//D3D11_BLEND_OP BlendOp;
	//D3D11_BLEND SrcBlendAlpha;
	//D3D11_BLEND DestBlendAlpha;
	//D3D11_BLEND_OP BlendOpAlpha;
	//UINT8 RenderTargetWriteMask;

	//AlphaToCoverageEnable	FALSE
	//IndependentBlendEnable	FALSE
	//RenderTarget[0].BlendEnable	FALSE
	//RenderTarget[0].SrcBlend	D3D11_BLEND_ONE
	//RenderTarget[0].DestBlend	D3D11_BLEND_ZERO
	//RenderTarget[0].BlendOp	D3D11_BLEND_OP_ADD
	//RenderTarget[0].SrcBlendAlpha	D3D11_BLEND_ONE
	//RenderTarget[0].DestBlendAlpha	D3D11_BLEND_ZERO
	//RenderTarget[0].BlendOpAlpha	D3D11_BLEND_OP_ADD
	//RenderTarget[0].RenderTargetWriteMask	D3D11_COLOR_WRITE_ENABLE_ALL

	// https://docs.microsoft.com/ko-kr/windows/desktop/api/d3d11/ne-d3d11-d3d11_blend
	//typedef enum D3D11_BLEND {
	//	D3D11_BLEND_ZERO,
	//	D3D11_BLEND_ONE,
	//	D3D11_BLEND_SRC_COLOR,
	//	D3D11_BLEND_INV_SRC_COLOR,
	//	D3D11_BLEND_SRC_ALPHA,
	//	D3D11_BLEND_INV_SRC_ALPHA,
	//	D3D11_BLEND_DEST_ALPHA,
	//	D3D11_BLEND_INV_DEST_ALPHA,
	//	D3D11_BLEND_DEST_COLOR,
	//	D3D11_BLEND_INV_DEST_COLOR,
	//	D3D11_BLEND_SRC_ALPHA_SAT,
	//	D3D11_BLEND_BLEND_FACTOR,
	//	D3D11_BLEND_INV_BLEND_FACTOR,
	//	D3D11_BLEND_SRC1_COLOR,
	//	D3D11_BLEND_INV_SRC1_COLOR,
	//	D3D11_BLEND_SRC1_ALPHA,
	//	D3D11_BLEND_INV_SRC1_ALPHA
	//};

	// m_Decs.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// rgba�� �׳� ������ ������.
		// �Ϲ����� �⺻ ���� ������� ó��.
	// ī��Ʈ�� 4�̻��� ���¿��� 

	// ����̽���
	// SWINFO.SampleDesc.Count = 4;
	// tDesc.SampleDesc.Count = 4;

	m_Decs.AlphaToCoverageEnable = true;
	m_Decs.IndependentBlendEnable = false;
	m_Decs.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_Decs.RenderTarget[0].BlendEnable = true;
	// �Ϲ��÷� ��
	m_Decs.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_Decs.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	m_Decs.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	m_Decs.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	m_Decs.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_Decs.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	//CAMBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//CAMBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//CAMBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//CAMBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//CAMBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//CAMBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	return Create(m_Decs);
}
bool HBLEND::Create(const D3D11_BLEND_DESC& _Desc) 
{
	if (&m_Decs != &_Desc)
	{
		m_Decs = _Desc;
	}

	if (S_OK != GraphicDevice::MainDevice()->CreateBlendState(&m_Decs, &m_pState))
	{
		BOOM;
		return false;
	}

	return true;
}
void HBLEND::Update() 
{
	// 0xffffffff 4���� �����̶� and �Ѵ� 0�̸� 0�ϴ� ����.
	// 255
	GraphicDevice::MainContext()->OMSetBlendState(m_pState, m_Color.Arr, 0xffffffff);
}