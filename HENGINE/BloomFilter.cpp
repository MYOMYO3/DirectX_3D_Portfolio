#include "BloomFilter.h"

#include "HMULTIRENDERTARGET.h"
#include "HMESH.h"
#include "RENDERPATH.h"
#include "CAMERA.h"


BloomFilter::BloomFilter()
{
}


BloomFilter::~BloomFilter()
{
}

void BloomFilter::InitFilter()
{
	m_Mesh = Resources::Find<HMESH>(L"TargetMESH");

	FilterSize.x = 64.0f;
	FilterSize.y = 64.0f;

	SFilterSize = FilterSize * 0.5f;

	{
		// ���� ������ ���� Ÿ��
		// �۰��ؼ� ������ ���̱� ���ؼ� ����Ѵ�.
		SmallTaregt = new HRENDERTARGET();
		SmallTaregt->Create(
			{ 0.0f, 0.0f, 0.0f, 0.0f } // �� Ÿ���� Ŭ����Ÿ�� �ɶ� ����
			, (UINT)FilterSize.x// �ʺ�
			, (UINT)FilterSize.y// ����
			, DXGI_FORMAT_R32G32B32A32_FLOAT // ���� ����
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	}

	{
		// ���� ������ ���� Ÿ��
		// �۰��ؼ� ������ ���̱� ���ؼ� ����Ѵ�.
		SSmallTaregt = new HRENDERTARGET();
		SSmallTaregt->Create(
			{ 0.0f, 0.0f, 0.0f, 0.0f } // �� Ÿ���� Ŭ����Ÿ�� �ɶ� ����
			, (UINT)SFilterSize.x // �ʺ�
			, (UINT)SFilterSize.y // ����
			, DXGI_FORMAT_R32G32B32A32_FLOAT // ���� ����
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	}

	{
		SpecCopyTarget = new HRENDERTARGET();
		SpecCopyTarget->Create(
			{ 0.0f, 0.0f, 0.0f, 0.0f } // �� Ÿ���� Ŭ����Ÿ�� �ɶ� ����
			, (UINT)m_Cam->CamTarget()->GetTex()->GetWidth()// �ʺ�
			, (UINT)m_Cam->CamTarget()->GetTex()->GetHeight() // ����
			, DXGI_FORMAT_R32G32B32A32_FLOAT // ���� ����
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	}


	{
		m_BloomSmallPath = Resources::Find<RenderPath>(L"BLOOMSMALLPATH");
		m_BloomSmallPath = m_BloomSmallPath->Clone();
		m_BloomSmallPath->LinkTEX(L"LightDiffTarget", m_Cam->GetLightTarget()->Target(0)->GetTex());
		m_BloomSmallPath->LinkTEX(L"LightSpecTarget", m_Cam->GetLightTarget()->Target(1)->GetTex());
		m_BloomSmallPath->LinkSMP(L"PS_LSMP", L"LSMP");
		m_BloomSmallPath->LinkCB(L"VS_FilterSize", &FilterSize);
		m_BloomSmallPath->LinkCB(L"PS_FilterSize", &FilterSize);
	}


	{
		m_BloomSmallEffectPath = Resources::Find<RenderPath>(L"BLOOMSMALLPATH");
		m_BloomSmallEffectPath = m_BloomSmallEffectPath->Clone();
		m_BloomSmallEffectPath->LinkTEX(L"LightDiffTarget", m_Cam->GetLightTarget()->Target(0)->GetTex());
		m_BloomSmallEffectPath->LinkTEX(L"LightSpecTarget", SmallTaregt->GetTex());
		m_BloomSmallEffectPath->LinkSMP(L"PS_LSMP", L"LSMP");
		m_BloomSmallEffectPath->LinkCB(L"VS_FilterSize", &SFilterSize);
		m_BloomSmallEffectPath->LinkCB(L"PS_FilterSize", &SFilterSize);
	}



	{
		m_BloomEffectPath = Resources::Find<RenderPath>(L"BLOOMEFFECTPATH");
		m_BloomEffectPath = m_BloomEffectPath->Clone();
		m_BloomEffectPath->LinkTEX(L"SmallTarget", SSmallTaregt->GetTex());
		m_BloomEffectPath->LinkTEX(L"OriTarget", SpecCopyTarget->GetTex());
		m_BloomEffectPath->LinkSMP(L"PS_LSMP", L"LSMP");
		m_BloomEffectPath->LinkCB(L"VS_FilterSize", &FilterSize);
		m_BloomEffectPath->LinkCB(L"PS_FilterSize", &FilterSize);
	}



	Template::MemZero(m_SmallViewPort);
	m_SmallViewPort.TopLeftX = 0;
	m_SmallViewPort.TopLeftY = 0;
	m_SmallViewPort.Width = FilterSize.x;
	m_SmallViewPort.Height = FilterSize.y;
	m_SmallViewPort.MinDepth = 0.0f;
	m_SmallViewPort.MaxDepth = 1.0f;

	//HTEMPLATE::MemZero(m_SSmallViewPort);
	//m_SSmallViewPort.TopLeftX = -SFilterSize.x * 0.3f;
	//m_SSmallViewPort.TopLeftY = -SFilterSize.y * 0.3f;
	//m_SSmallViewPort.Width = SFilterSize.x * 1.6f;
	//m_SSmallViewPort.Height = SFilterSize.y * 1.6f;
	//m_SSmallViewPort.MinDepth = 0.0f;
	//m_SSmallViewPort.MaxDepth = 1.0f;

	Template::MemZero(m_SSmallViewPort);
	m_SSmallViewPort.TopLeftX = 0;
	m_SSmallViewPort.TopLeftY = 0;
	m_SSmallViewPort.Width = SFilterSize.x;
	m_SSmallViewPort.Height = SFilterSize.y;
	m_SSmallViewPort.MinDepth = 0.0f;
	m_SSmallViewPort.MaxDepth = 1.0f;
}

void BloomFilter::MergeBeforeEffect()
{
	GraphicDevice::MainContext()->RSSetViewports(1, &m_SmallViewPort);

	SmallTaregt->Clear();
	SmallTaregt->OmSet(nullptr);
	m_Mesh->UpdateVB(0);
	m_Mesh->UpdateIB(0);
	m_BloomSmallPath->Update();
	m_Mesh->Render(0);

	GraphicDevice::MainContext()->RSSetViewports(1, &m_SSmallViewPort);

	SSmallTaregt->Clear();
	SSmallTaregt->OmSet(nullptr);
	m_Mesh->UpdateVB(0);
	m_Mesh->UpdateIB(0);
	m_BloomSmallEffectPath->Update();
	m_Mesh->Render(0);

	m_Cam->Update();

	// ����ŧ�� Ÿ�� ī��
	SpecCopyTarget->GetTex()->Copy(m_Cam->GetLightTarget()->Target(1)->GetTex());

	// m_Cam->LightTarget()->Target(1)->Clear();
	m_Cam->GetLightTarget()->Target(1)->OmSet(nullptr);
	m_Mesh->UpdateVB(0);
	m_Mesh->UpdateIB(0);
	m_BloomEffectPath->Update();
	m_Mesh->Render(0);
}
void BloomFilter::MergeAfterEffect()
{

}