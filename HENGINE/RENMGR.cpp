#include "RENMGR.h"
#include "GRAPHICDEVICE.h"
#include "GRAPHICDEBUG.h"
#include "SCENE.h"

#include "HMESH.h"
#include "HCB.h"
#include "HVTXSHADER.h"
#include "HPIXSHADER.h"
#include "HSMP.h"
#include "SPRITE.h"

std::map<std::wstring, HPTR<Instancing>>::iterator RenderMgr::FindInstIter;
std::map<std::wstring, HPTR<Instancing>>::iterator RenderMgr::StartInst;
std::map<std::wstring, HPTR<Instancing>>::iterator RenderMgr::EndInst;
std::map<std::wstring, HPTR<Instancing>> RenderMgr::m_InstMap;

void Instancing::SetMesh(const wchar_t* _Name)
{
	m_HMESH = Resources::Find<HMESH>(_Name);

	if (nullptr == m_HMESH)
	{
		BOOM;
	}

	if (nullptr == m_HMESH->INSTBUFFER())
	{
		BOOM;
	}
}


// ��������.
// ���� ���ƽ� 
// �п� �� �������ٵ� Mfc�� ���� ����غô�.

bool(*RenderMgr::SortFunc[RenderMgr::GT_MAX])(const HPTR<Renderer>&, const HPTR<Renderer>&);

bool RenderMgr::CamSort(const HPTR<Camera>& _Left, const HPTR<Camera>& _Right)
{
	return _Left->Order() < _Right->Order();
}
bool RenderMgr::ZSort(const HPTR<Renderer>& _Left, const HPTR<Renderer>& _Right) 
{
	return _Left->GetActor()->GetTransform()->GetPosition().z > _Right->GetActor()->GetTransform()->GetPosition().z;
}
bool RenderMgr::YSort(const HPTR<Renderer>& _Left, const HPTR<Renderer>& _Right) 
{
	return _Left->GetActor()->GetTransform()->GetPosition().y > _Right->GetActor()->GetTransform()->GetPosition().y;
}
bool RenderMgr::XSort(const HPTR<Renderer>& _Left, const HPTR<Renderer>& _Right) 
{
	return _Left->GetActor()->GetTransform()->GetPosition().x > _Right->GetActor()->GetTransform()->GetPosition().x;
}

// �� �ð��� �������� �Ǹ����� �ʰ�
// ����.
// �Ƿ��� ��°ġ�� ���ο� ��������� ��.


Light::ALLLIGHTDATA RenderMgr::m_ALLLIGHTDATA;
std::vector<HPTR<HCB>> RenderMgr::m_LIGHTCBLIST;


bool m_IsLightInit = false;
void RenderMgr::LightCBInit()
{
	if (true == m_IsLightInit)
	{
		return;
	}

	m_LIGHTCBLIST.resize(5);

	// ������۰� �̹� ����� �������Ƿ� �Ǵ°�.
	m_LIGHTCBLIST[0] = Resources::Find<HCB>(L"ALLLIGHTDATAV");
	m_LIGHTCBLIST[1] = Resources::Find<HCB>(L"ALLLIGHTDATAP");
	m_LIGHTCBLIST[2] = Resources::Find<HCB>(L"ALLLIGHTDATAD");
	m_LIGHTCBLIST[3] = Resources::Find<HCB>(L"ALLLIGHTDATAG");
	m_LIGHTCBLIST[4] = Resources::Find<HCB>(L"ALLLIGHTDATAH");

	for (size_t i = 0; i < m_LIGHTCBLIST.size(); i++)
	{
		m_LIGHTCBLIST[i]->Link(&m_ALLLIGHTDATA, sizeof(Light::ALLLIGHTDATA));
	}
	m_IsLightInit = true;
}

RenderMgr::RenderMgr(Scene* _Scene) : m_Scene(_Scene)
{
	SortFunc[GT_Z] = ZSort;
	SortFunc[GT_Y] = ZSort;
	SortFunc[GT_X] = ZSort;
	SortFunc[GT_CAMZ] = nullptr;
	SortFunc[GT_CAMY] = nullptr;
	SortFunc[GT_CAMX] = nullptr;


	LightCBInit();
}


RenderMgr::~RenderMgr()
{
	
}

void RenderMgr::Render()
{
	// Ȥ�ó� ������ ����� ķ���� �̵���Ų��.
	// CHECKCAMORDER();

	// �⺻ ���̹��۴� �ѹ� �и���.
	//GraphicDevice::Inst()->ClearTarget();
	//GraphicDevice::Inst()->DefRenderTargetSetting();
	//GraphicDevice::Inst()->DefViewPortSetting();

	// ���⼭ Ŭ���� �ع����� �ȵȴ�.
	m_ALLCAMSTART = m_ALLCAMLIST.begin();
	m_ALLCAMEND = m_ALLCAMLIST.end();
	for (; m_ALLCAMSTART != m_ALLCAMEND; ++m_ALLCAMSTART)
	{
		(*m_ALLCAMSTART)->CamClear();
	}

	RenderGStart = m_RenderList.begin();
	RenderGEnd = m_RenderList.end();

	for (; RenderGStart != RenderGEnd; ++RenderGStart)
	{
		RenderStart = RenderGStart->second.begin();
		RenderEnd = RenderGStart->second.end();

		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			if (false == (*RenderStart)->IsUpdate())
			{
				continue;
			}

			(*RenderStart)->RenderUpdate();
		}
	}


	
	// ��� ���� �����͸� ���� �غ� �Ѵ�.
	// �� 1. ��� �����׷��� ���� �غ� �Ѵ�.
	// ������ ���� ���� �غ� �Ѵ�.
	RenderForwardGStart = m_FORWARDRenderList.begin();
	RenderForwardGEnd = m_FORWARDRenderList.end();

	// 5�� ���� �׷��� ����.
	for (; RenderForwardGStart != RenderForwardGEnd; ++RenderForwardGStart)
	{
		if (0 >= RenderForwardGStart->second.size())
		{
			continue;
		}

		CamGroupFinder = m_CAMLIST.find(RenderForwardGStart->first);
		if (CamGroupFinder == m_CAMLIST.end() || 0 >= CamGroupFinder->second.size())
		{
			continue;
		}

		CamStart = CamGroupFinder->second.begin();
		CamEnd = CamGroupFinder->second.end();

		// �� 3. ī�޶���� ���� �׷쿡 ���� ���������� ������ �����Ѵ�.
		for (; CamStart != CamEnd; ++CamStart)
		{
			GraphicDevice::MainReset();

			(*CamStart)->Update();
			(*CamStart)->FORWARDTargetSet();

			LightSetting(RenderForwardGStart->first, *CamStart);

			RenderDataStart = RenderForwardGStart->second.begin();
			RenderDataEnd = RenderForwardGStart->second.end();
			for (; RenderDataStart != RenderDataEnd; ++RenderDataStart)
			{
				if (false == (*RenderDataStart)->m_Parent->IsUpdate())
				{
					continue;
				}

				// �� �����غ���.
				(*RenderDataStart)->m_Parent->UpdateTransform();
				(*RenderDataStart)->m_Parent->UpdateSubTrans((*CamStart));
				(*RenderDataStart)->m_Parent->RenderDataUpdate((*CamStart), (*RenderDataStart));
				(*RenderDataStart)->m_Parent->m_MatData.RENDERDATA.x = (float)(*RenderDataStart)->m_Path->m_LIGHT;
				(*RenderDataStart)->m_Parent->m_MatData.RENDERDATA.y = (float)(*RenderDataStart)->m_Path->m_BUMP;



				(*RenderDataStart)->m_Mesh->UpdateVB((*RenderDataStart)->VBIndex);
				(*RenderDataStart)->m_Mesh->UpdateIB((*RenderDataStart)->IBIndex);
				(*RenderDataStart)->m_Path->Update();
				(*RenderDataStart)->m_Mesh->Render((*RenderDataStart)->IBIndex);
			}

		}
	}

	// ���۵� �� �׸���.
	RenderDeferredGStart = m_DEFERREDRenderList.begin();
	RenderDeferredGEnd = m_DEFERREDRenderList.end();
	for (; RenderDeferredGStart != RenderDeferredGEnd; ++RenderDeferredGStart)
	{
		if (0 >= RenderDeferredGStart->second.size())
		{
			continue;
		}

		CamGroupFinder = m_CAMLIST.find(RenderDeferredGStart->first);
		if (CamGroupFinder == m_CAMLIST.end() || 0 >= CamGroupFinder->second.size())
		{
			continue;
		}

		CamStart = CamGroupFinder->second.begin();
		CamEnd = CamGroupFinder->second.end();

		// �� 3. ī�޶���� ���� �׷쿡 ���� ���������� ������ �����Ѵ�.
		for (; CamStart != CamEnd; ++CamStart)
		{
			GraphicDevice::MainReset();

			(*CamStart)->Update();
			(*CamStart)->DEFERREDTargetSet();

			// LightSetting(RenderDeferredGStart->first, *CamStart);

			RenderDataStart = RenderDeferredGStart->second.begin();
			RenderDataEnd = RenderDeferredGStart->second.end();
			for (; RenderDataStart != RenderDataEnd; ++RenderDataStart)
			{
				if (false == (*RenderDataStart)->m_Parent->IsUpdate())
				{
					continue;
				}

				// �� �����غ���.
				(*RenderDataStart)->m_Parent->UpdateTransform();
				(*RenderDataStart)->m_Parent->UpdateSubTrans((*CamStart));
				(*RenderDataStart)->m_Parent->RenderDataUpdate((*CamStart), (*RenderDataStart));
				(*RenderDataStart)->m_Parent->m_MatData.RENDERDATA.x = (float)(*RenderDataStart)->m_Path->m_LIGHT;
				(*RenderDataStart)->m_Parent->m_MatData.RENDERDATA.y = (float)(*RenderDataStart)->m_Path->m_BUMP;

				(*RenderDataStart)->m_Mesh->UpdateVB((*RenderDataStart)->VBIndex);
				(*RenderDataStart)->m_Mesh->UpdateIB((*RenderDataStart)->IBIndex);
				(*RenderDataStart)->m_Path->Update();
				(*RenderDataStart)->m_Mesh->Render((*RenderDataStart)->IBIndex);
			}


			GraphicDevice::MainReset();
			ShadowRender(RenderDeferredGStart->first, *CamStart);
			LightSetting(RenderDeferredGStart->first, *CamStart);
			LightGroupFinder = m_LIGHTLIST.find(RenderDeferredGStart->first);
			HPTR<Light> ShadowLight = nullptr;
			
			if ((m_LIGHTLIST.end() != LightGroupFinder))
			{
				if (0 != LightGroupFinder->second.size())
				{
					ShadowLight = *LightGroupFinder->second.begin();
				}
			}


			//for (size_t i = 0; i < length; i++)
			//{
			//}
			// LIGHT->DEFERREDLightRender(ShadowLight);
			(*CamStart)->DEFERREDLightRender(ShadowLight);
		}
	}
	m_ALLCAMLIST.sort(CamSort);

	// �� �׷�������.

	GraphicDevice::MainReset(); // �� ����
	m_ALLCAMSTART = m_ALLCAMLIST.begin();
	m_ALLCAMEND = m_ALLCAMLIST.end();

	for (; m_ALLCAMSTART != m_ALLCAMEND; ++m_ALLCAMSTART)
	{
		(*m_ALLCAMSTART)->DEFERREDMerge();
	}
	GraphicDebug::DebugCamRender();

	// GRAPHICDEVICE::HMainDevice()->ClearTarget();
	GraphicDevice::MainReset(); // �� ����
	GraphicDevice::Inst()->ClearTarget();
	GraphicDevice::Inst()->DefRenderTargetSetting();
	GraphicDevice::Inst()->DefViewPortSetting();
	// ���̹��� ���� �׷��� �Ѵ�.

	m_ALLCAMSTART = m_ALLCAMLIST.begin();
	m_ALLCAMEND = m_ALLCAMLIST.end();

	for (; m_ALLCAMSTART != m_ALLCAMEND; ++m_ALLCAMSTART)
	{
		(*m_ALLCAMSTART)->CamRender();
	}

	// ����� ������ ���⼭ ������.

	//GraphicDevice::Inst()->DefRenderTargetSetting();
	//GraphicDevice::Inst()->DefViewPortSetting();
	m_Scene->DebugRender();
	GraphicDebug::DebugRender();

	RenderGStart = m_RenderList.begin();
	RenderGEnd = m_RenderList.end();
	for (; RenderGStart != RenderGEnd; ++RenderGStart)
	{
		RenderStart = RenderGStart->second.begin();
		RenderEnd = RenderGStart->second.end();

		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			if (false == (*RenderStart)->IsUpdate())
			{
				continue;
			}

			(*RenderStart)->TextRenderUpdate();
		}
	}

	GraphicDevice::MainReset();

	// ITERFOR(AStart, AEnd, m_ActorList, Render);
	GraphicDevice::Inst()->Present();

}


void RenderMgr::PushCamera(const HPTR<Camera>& _CAM)
{
	// ķ�׷��� ������
	// ����Ŵ�.

	//std::set<int>::iterator StartGroup = _CAM->m_GroupList.begin();
	//std::set<int>::iterator EndGroup = _CAM->m_GroupList.end();

	for (auto& var : _CAM->m_GroupList)
	{
		std::map<int, std::list<HPTR<Camera>>>::iterator FindIter
			= m_CAMLIST.find(var);

		if (FindIter == m_CAMLIST.end())
		{
			m_CAMLIST.insert(std::map<int, std::list<HPTR<Camera>>>::value_type(var, std::list<HPTR<Camera>>()));
			FindIter = m_CAMLIST.find(var);
		}

		FindIter->second.push_back(_CAM);
	}

	m_ALLCAMLIST.push_back(_CAM);
}

void RenderMgr::PushLight(const HPTR<Light>& _Light) 
{

	for (auto& var : _Light->m_GroupList)
	{
		std::map<int, std::list<HPTR<Light>>>::iterator FindIter
			= m_LIGHTLIST.find(var);

		if (FindIter == m_LIGHTLIST.end())
		{
			m_LIGHTLIST.insert(std::map<int, std::list<HPTR<Light>>>::value_type(var, std::list<HPTR<Light>>()));
			FindIter = m_LIGHTLIST.find(var);
		}

		if (LIGHTMAX <= FindIter->second.size())
		{
			BOOM;
			continue;
		}

		FindIter->second.push_back(_Light);
	}
}

// �׷� �ٲٴ°� �ظ��ϸ� �ȸ���.
// UI���ٰ� PLAY �׷������ �Ѵ�. �̷��̾߱� �ε�...
// �𸣰ڴ�. �׷����� ������.
// �̷��� ��ũ�� ���� �;�����.
void RenderMgr::PushRender(Renderer* _RENDER) 
{
	//if (_RENDER->IsDebugCheck())
	//{
	//	_RENDER->DebugCheck();
	//}
	//int a = _RENDER->Order();

	// 2�� ������ �׳� ����... 
	std::map<int, std::list<HPTR<Renderer>>>::iterator FindIter
		= m_RenderList.find(_RENDER->GetOrder());

	if (FindIter == m_RenderList.end())
	{
		m_RenderList.insert(std::map<int, std::list<HPTR<Renderer>>>::value_type(_RENDER->GetOrder(), std::list<HPTR<Renderer>>()));
		FindIter = m_RenderList.find(_RENDER->GetOrder());
		m_SORTTYPE.insert(std::map<int, GSORTTYPE>::value_type(_RENDER->GetOrder(), GSORTTYPE::GT_Z));
	}

	FindIter->second.push_back(_RENDER);
}

void RenderMgr::PushForwardender(Renderer::HRENDERDATA* _RENDER)
{
	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator FindIter
		= m_FORWARDRenderList.find(_RENDER->m_Parent->GetOrder());

	if (FindIter == m_FORWARDRenderList.end())
	{
		m_FORWARDRenderList.insert(std::map<int, std::list<Renderer::HRENDERDATA*>>::value_type(_RENDER->m_Parent->GetOrder(), std::list<Renderer::HRENDERDATA*>()));
		FindIter = m_FORWARDRenderList.find(_RENDER->m_Parent->GetOrder());
		m_SORTTYPE.insert(std::map<int, GSORTTYPE>::value_type(_RENDER->m_Parent->GetOrder(), GSORTTYPE::GT_Z));
	}

	FindIter->second.push_back(_RENDER);
}

void RenderMgr::PushDeferredRender(Renderer::HRENDERDATA* _RENDER) 
{

	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator FindIter
		= m_DEFERREDRenderList.find(_RENDER->m_Parent->GetOrder());

	if (FindIter == m_DEFERREDRenderList.end())
	{
		m_DEFERREDRenderList.insert(std::map<int, std::list<Renderer::HRENDERDATA*>>::value_type(_RENDER->m_Parent->GetOrder(), std::list<Renderer::HRENDERDATA*>()));
		FindIter = m_DEFERREDRenderList.find(_RENDER->m_Parent->GetOrder());
		m_SORTTYPE.insert(std::map<int, GSORTTYPE>::value_type(_RENDER->m_Parent->GetOrder(), GSORTTYPE::GT_Z));
	}

	FindIter->second.push_back(_RENDER);
}

void RenderMgr::PopForwardender(Renderer::HRENDERDATA* _RENDER)
{
	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator FindIter
		= m_FORWARDRenderList.find(_RENDER->m_Parent->GetOrder());

	std::map<int, GSORTTYPE>::iterator FindSortIter 
		= m_SORTTYPE.find(_RENDER->m_Parent->GetOrder());

	if (FindIter == m_FORWARDRenderList.end())
	{
		FindIter = m_FORWARDRenderList.find(_RENDER->m_Parent->GetOrder());

		m_FORWARDRenderList.erase(FindIter);
		m_SORTTYPE.erase(FindSortIter);
	}

	//FindIter->second.push_back(_RENDER);
}

void RenderMgr::Release()
{
	// ����� ��������

	RenderForwardGStart = m_FORWARDRenderList.begin();
	RenderForwardGEnd = m_FORWARDRenderList.end();

	for (; RenderForwardGStart != RenderForwardGEnd; ++RenderForwardGStart)
	{
		RenderDataStart = RenderForwardGStart->second.begin();
		RenderDataEnd = RenderForwardGStart->second.end();

		for (; RenderDataStart != RenderDataEnd; )
		{
			if (true == (*RenderDataStart)->m_Parent->IsDeath())
			{
				RenderDataStart = RenderForwardGStart->second.erase(RenderDataStart);
			}
			else {
				++RenderDataStart;
			}

		}
	}

	RenderDeferredGStart = m_DEFERREDRenderList.begin();
	RenderDeferredGEnd = m_DEFERREDRenderList.end();

	for (; RenderDeferredGStart != RenderDeferredGEnd; ++RenderDeferredGStart)
	{
		RenderDataStart = RenderDeferredGStart->second.begin();
		RenderDataEnd = RenderDeferredGStart->second.end();

		for (; RenderDataStart != RenderDataEnd; )
		{
			if (true == (*RenderDataStart)->m_Parent->IsDeath())
			{
				RenderDataStart = RenderDeferredGStart->second.erase(RenderDataStart);
			}
			else {
				++RenderDataStart;
			}

		}
	}

	RenderGStart = m_RenderList.begin();
	RenderGEnd = m_RenderList.end();

	for (; RenderGStart != RenderGEnd; ++RenderGStart)
	{
		RenderStart = RenderGStart->second.begin();
		RenderEnd = RenderGStart->second.end();

		for (; RenderStart != RenderEnd; )
		{
			if (true == (*RenderStart)->IsDeath())
			{
				RenderStart = RenderGStart->second.erase(RenderStart);
			}
			else {
				++RenderStart;
			}
		}
	}
}

void RenderMgr::OverRelease() 
{
	RenderGStart = m_RenderList.begin();
	RenderGEnd = m_RenderList.end();

	for (; RenderGStart != RenderGEnd; ++RenderGStart)
	{
		RenderStart = RenderGStart->second.begin();
		RenderEnd = RenderGStart->second.end();

		for (; RenderStart != RenderEnd; )
		{
			if (true == (*RenderStart)->GetActor()->IsOverReady())
			{
				RenderStart = RenderGStart->second.erase(RenderStart);
			}
			else {
				++RenderStart;
			}
		}
	}
}

void RenderMgr::ShadowRender(int _Order, HPTR<Camera>& _Cam)
{
	LightGroupFinder = m_LIGHTLIST.find(_Order);




	HPTR<RenderPath> PATH = nullptr;
	if (LightGroupFinder != m_LIGHTLIST.end())
	{
		for (auto& CurLight : LightGroupFinder->second)
		{
			CurLight->ShadowUpdate();

			RenderDataStart = RenderDeferredGStart->second.begin();
			RenderDataEnd = RenderDeferredGStart->second.end();


			for (; RenderDataStart != RenderDataEnd; ++RenderDataStart)
			{
				if (true == (*RenderDataStart)->m_bShadow)
				{
					if ((*RenderDataStart)->m_bShadowAni == true)
					{
						PATH = Resources::Find<RenderPath>(L"SHADOWANIPATH");
					}
					else
					{
						PATH = Resources::Find<RenderPath>(L"SHADOWPATH");						
					}

					if (nullptr == PATH)
					{
						BOOM;
					}

					PATH->Init((*RenderDataStart)->m_Parent);
					(*RenderDataStart)->m_Parent->UpdateTransform();
					// ����Ʈ �������� �׸��ڸ� �׷��� �ϴϱ� ���� ������ �־��ִ°�.
					(*RenderDataStart)->m_Parent->UpdateShadowTrans(CurLight);
					(*RenderDataStart)->m_Parent->RenderDataUpdate((*CamStart), (*RenderDataStart));

					(*RenderDataStart)->m_Mesh->UpdateVB((*RenderDataStart)->VBIndex);
					(*RenderDataStart)->m_Mesh->UpdateIB((*RenderDataStart)->IBIndex);
					PATH->Update();
					(*RenderDataStart)->m_Mesh->Render((*RenderDataStart)->IBIndex);
				}
			}
		}
	}
}

void RenderMgr::LightRender(HPTR<Camera>& _Cam) 
{

}

void RenderMgr::LightSetting(int _Order, HPTR<Camera>& _Cam) 
{
	// Light�� �غ�
	LightGroupFinder = m_LIGHTLIST.find(_Order);

	if (LightGroupFinder != m_LIGHTLIST.end())
	{
		int LightCount = 0;
		for (auto& CurLight : LightGroupFinder->second)
		{
			m_ALLLIGHTDATA.ArrData[LightCount] = CurLight->m_LightData;
			m_ALLLIGHTDATA.ArrData[LightCount].LightPos = _Cam->GetViewMatrix().MulOne(m_ALLLIGHTDATA.ArrData[LightCount].LightPos);
			// m_ALLLIGHTDATA.ArrData[LightCount].LightPos.Normal3D();
			m_ALLLIGHTDATA.ArrData[LightCount].LightDir = _Cam->GetViewMatrix().MulZero(m_ALLLIGHTDATA.ArrData[LightCount].LightDir);
			m_ALLLIGHTDATA.ArrData[LightCount].LightDir.Normal3DToWZero();
			m_ALLLIGHTDATA.ArrData[LightCount].LightDirInv = _Cam->GetViewMatrix().MulZero(m_ALLLIGHTDATA.ArrData[LightCount].LightDirInv);
			m_ALLLIGHTDATA.ArrData[LightCount].LightDirInv.Normal3DToWZero();
			++LightCount;
		}

		m_ALLLIGHTDATA.LightSettingData[0] = LightCount;


		for (size_t i = 0; i < m_LIGHTCBLIST.size(); i++)
		{
			m_LIGHTCBLIST[i]->DataSetting();
			m_LIGHTCBLIST[i]->Update();
		}
	}

}