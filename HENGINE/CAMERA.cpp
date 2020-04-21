#include "CAMERA.h"
#include "TRANS.h"
#include "SCENE.h"
#include "SPRITE.h"
#include "GAMEWIN.h"
#include "HRENDERTARGET.h"
#include "HDEPTHBUFFER.h"
#include "HMULTIRENDERTarget.h"
#include "LIGHT.h"

Light::ShadowData SData;
Camera* Camera::m_MainCam = nullptr;
Camera* Camera::m_UICam = nullptr;

Camera::Camera() : m_IsAllView(false), m_Order(0), m_Fov(60.0f), m_Near(1.0f), m_Far(1000.0f)
{
	
}


Camera::~Camera()
{
}

// ķ�� ���� ���ʹ� ���ο� transform�� cam

void Camera::Init() 
{
	// Ʈ���� ���� ������ ���� 
	if (nullptr == GetActor()->GetTransform())
	{
		BOOM;
	}

	if (nullptr == m_MainCam)
	{
		m_MainCam = this;
	}

	m_Height = GetActor()->GetScene()->Window()->Size().y;
	m_Width = GetActor()->GetScene()->Window()->Size().x;

	Template::MemZero(m_CamViewPort);
	m_CamViewPort.TopLeftX = 0;
	m_CamViewPort.TopLeftY = 0;
	m_CamViewPort.Width = m_Width;
	m_CamViewPort.Height = m_Height;
	m_CamViewPort.MinDepth = 0.0f;
	m_CamViewPort.MaxDepth = 1.0f;

	// ����Ÿ�� �並 � ������ �ֳ���????
	// 1�� ������ �ִ�.
	{
		m_CamTarget = new HRENDERTARGET();
		m_CamTarget->Create(
			{ 0.0f, 0.0f, 0.0f, 0.0f } // �� Ÿ���� Ŭ����Ÿ�� �ɶ� ����
			, (UINT)m_Width // �ʺ�
			, (UINT)m_Height // ����
			, DXGI_FORMAT_R32G32B32A32_FLOAT // ���� ����
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	}

	{
		m_DepthBuffer = new HDEPTHBUFFER();
		m_DepthBuffer->Create(
			(UINT)m_Width // �ʺ�
			, (UINT)m_Height
			, DXGI_FORMAT_D24_UNORM_S8_UINT// ���� ����
		);
	}

	{
		m_DEFERREDTarget = new HMULTIRENDERTarget();
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // ��ǻ��.
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // ������.
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // �븻.
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // ����.
	
		m_DEFERREDTarget->Depth(m_DepthBuffer);

		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // TEST
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // TEST2
	}

	{
		// ����������Ʈ
		m_LightTarget = new HMULTIRENDERTarget();
		m_LightTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // ��ǻ��
		m_LightTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // ����ŧ��
		m_LightTarget->Depth(m_DepthBuffer);
	}

	// m_MultiRenderTarget->Create()

	// ���� ���� ũ�� �����ؼ� ����ۿ� �������� �ȴ�.
	m_CamRenderMesh = Resources::Find<HMESH>(L"TargetMESH");
	m_CamRenderPath = Resources::Find<RenderPath>(L"CAMPATH");
	m_CamRenderPath = m_CamRenderPath->Clone();
	m_CamRenderPath->LinkSMP(L"PS_LSMP", L"LSMP");


	m_DEFERREDLightPath = Resources::Find<RenderPath>(L"DFFERDLIGHTINGPATH");
	m_DEFERREDLightPath = m_DEFERREDLightPath->Clone();
	m_DEFERREDLightPath->LinkCB(L"DEFERREDCAMPOS", &m_ViewCamPos);
	// m_DEFERREDLightPath->TEX(L"Diff", m_DEFERREDTarget->Target(0)->Tex());
	m_DEFERREDLightPath->LinkTEX(L"Pos", m_DEFERREDTarget->Target(1)->GetTex());
	m_DEFERREDLightPath->LinkTEX(L"Nomal", m_DEFERREDTarget->Target(2)->GetTex());
	m_DEFERREDLightPath->LinkTEX(L"Depth", m_DEFERREDTarget->Target(3)->GetTex());
	m_DEFERREDLightPath->LinkTEX(L"Test", m_DEFERREDTarget->Target(4)->GetTex());
	m_DEFERREDLightPath->LinkTEX(L"Test2", m_DEFERREDTarget->Target(5)->GetTex());
	m_DEFERREDLightPath->LinkSMP(L"PS_LSMP", L"LSMP");

	m_LightMergePath = Resources::Find<RenderPath>(L"DFFERDLIGHTMERGEPATH");
	m_LightMergePath = m_LightMergePath->Clone();
	m_LightMergePath->LinkTEX(L"DiffTex", m_DEFERREDTarget->Target(0)->GetTex());
	m_LightMergePath->LinkTEX(L"LightDiffTex", m_LightTarget->Target(0)->GetTex());
	m_LightMergePath->LinkTEX(L"LightSpecTex", m_LightTarget->Target(1)->GetTex());
	//m_LightMergePath->LinkTEX(L"TestTex", m_LightTarget->Target(4)->GetTex());
	m_LightMergePath->LinkSMP(L"PS_LSMP", L"LSMP");
	// m_CamRenderPath->linkcb

	// DATA = m_CamRenderPath;
}

void Camera::Order(int _Order)
{
	// ���� ���߿� �������� ��������.
	// �ߴܺ�.
	m_Order = _Order;
}

void Camera::RenderBefore() 
{
	Vector4 Pos = GetActor()->GetTransform()->GetPosition();
	Vector4 For = GetActor()->GetTransform()->LookForward();
	Vector4 Up = GetActor()->GetTransform()->LookUp();

	// Ʈ���� ���� ������ �� �ֳ׿�????
	m_VIEW.SetViewMatrix(GetActor()->GetTransform()->GetPosition(), GetActor()->GetTransform()->LookForward(), GetActor()->GetTransform()->LookUp());
	// �������� ������ ���������� �ؾ��Ѵ�.

	switch (m_Mode)
	{
	case PROJMODE::PM_PERS:
		m_PROJ.SetPerspectiveMatrix(m_Fov, m_Width, m_Height, m_Near, m_Far);
		break;
	case PROJMODE::PM_ORTH:
		m_PROJ.SetOrthgonalMatrix(m_Width, m_Height, m_Near, m_Far);
		break;
	default:
		break;
	}

	// 
	// m_MatData.ARRMAT[HMAT_VIEW].MulOne(_Cam->Trans()->WPos())
	m_ViewCamPos = m_VIEW.MulOne( Pos); // �̰͵� �Ǽ���
	m_VP = m_VIEW * m_PROJ;

}


Vector4 Camera::OrthMousePos()
{
	Vector4 MP = GameWindow::MainGameWin()->MousePos();
	MP.y *= -1.0f;
	MP.x  -= GameWindow::MainGameWin()->Size().HX();
	MP.y += GameWindow::MainGameWin()->Size().HY();

	float XRatio = m_Width/ GameWindow::MainGameWin()->Size().x;
	float YRatio = m_Height/ GameWindow::MainGameWin()->Size().y;

	// 10 / 600;

	MP.x *= XRatio;
	MP.y *= YRatio;

	MP += GetTransform()->GetPosition();

	return MP;
}

void Camera::PushRenderGroup() 
{
	GetScene()->RMGR.PushCamera(this);
}

void Camera::Update() 
{
	// ����Ÿ�ٵ� �̶� ���õȴ�.
	if (nullptr != GraphicDevice::MainContext())
	{
		// m_CamTarget->Clear();
		GraphicDevice::MainContext()->RSSetViewports(1, &m_CamViewPort);
		// �̰� ��� �ٲ���� �Ѵ�.
	}
}

void Camera::DEFERREDMerge() 
{
	if (nullptr != GraphicDevice::MainContext())
	{
		GraphicDevice::MainContext()->RSSetViewports(1, &m_CamViewPort);
	}

	// �׻� 
	MergeBefore();

	m_CamTarget->OmSet(nullptr);
	m_CamRenderMesh->UpdateVB(0);
	m_CamRenderMesh->UpdateIB(0);
	m_LightMergePath->Update();
	m_CamRenderMesh->Render(0);

	MergeAfter();
}

void Camera::CamRender() 
{
	if (nullptr != GraphicDevice::MainContext())
	{
		GraphicDevice::MainContext()->RSSetViewports(1, &m_CamViewPort);
	}


	// ķ�� �����ϱ� ���� ��� Ÿ���� ���ļ�
	// �ϳ��� ȭ������ ������ �ؾ���.
	// �����۾��ϰ�


	// ī�޶� ���� ������ ���ؼ�
	// ���� ���۸� ����.
	m_CamTarget->ShaderUpdate(HSHADER::SHADERTYPE::SHT_PS, 0);

	m_CamRenderMesh->UpdateVB(0);
	m_CamRenderMesh->UpdateIB(0);
	m_CamRenderPath->Update();
	m_CamRenderMesh->Render(0);
}

void Camera::CamClear() 
{
	m_CamTarget->Clear();
	m_DEFERREDTarget->Clear();
	m_DepthBuffer->Clear();
}


void Camera::FORWARDTargetSet()
{
	m_CamTarget->OmSet(m_DepthBuffer->DSV());
}
void Camera::DEFERREDTargetSet()
{
	m_DEFERREDTarget->OmSet();
}

void Camera::DEFERREDLightRender(HPTR<Light> _Light)
{
	if (nullptr != GraphicDevice::MainContext())
	{
		GraphicDevice::MainContext()->RSSetViewports(1, &m_CamViewPort);
	}

	if (nullptr != _Light)
	{
		Matrix Inv = m_VIEW;
		Inv.Inverse();
		_Light->m_ShadowData.CamInvView = Inv;
		_Light->m_ShadowData.CamInvView.TransPose();
		_Light->m_ShadowData.m_ShadowData.x = 1.0F;
		m_DEFERREDLightPath->LinkCB(L"SHADOWDATA", &(_Light->m_ShadowData));
		m_DEFERREDLightPath->LinkTEX(L"Shadow", _Light->m_ShadowTarget->GetTex());
	}
	else 
	{
		SData.m_ShadowData.x = 0.0f;
		m_DEFERREDLightPath->LinkCB(L"SHADOWDATA", &SData);
	}

	// ���� �̳༮�� Ŭ���� �����ϳ���? ���߳׿�.
	m_LightTarget->Clear();
	m_LightTarget->OmSet();

	m_CamRenderMesh->UpdateVB(0);
	m_CamRenderMesh->UpdateIB(0);
	m_DEFERREDLightPath->Update();
	m_CamRenderMesh->Render(0);
}

void Camera::MergeBefore()
{
	for (auto& IterFilter : m_Filters)
	{
		IterFilter->MergeBeforeEffect();
	}
}

void Camera::MergeAfter()
{
	for (auto& IterFilter : m_Filters)
	{
		IterFilter->MergeAfterEffect();
	}
}