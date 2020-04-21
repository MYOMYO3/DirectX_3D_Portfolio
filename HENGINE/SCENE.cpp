#include "SCENE.h"
#include "ACTOR.h"
#include "TRANS.h"
#include <JTIME.h>
#include <INPUT.h>
/////////////////////////////////// �ɹ�
void Scene::AddUpdater(HPTR<Updater> _pCom) 
{
	if (nullptr == _pCom)
	{
		BOOM;
		return;
	}
	_pCom->SetScene(this);
	m_UpdaterList.push_back(_pCom);
}

bool Scene::Updater::MouseInTheBox(Actor* _Box)
{
	if (_Box->GetTransform()->GetPosition().x - _Box->GetTransform()->GetScale().x * 0.5 < GameWindow::MainGameWin()->MousePosInt().x
		&& _Box->GetTransform()->GetPosition().x + _Box->GetTransform()->GetScale().x * 0.5 > GameWindow::MainGameWin()->MousePosInt().x
		&& _Box->GetTransform()->GetPosition().y - _Box->GetTransform()->GetScale().y * 0.5 < GameWindow::MainGameWin()->MousePosInt().y
		&& _Box->GetTransform()->GetPosition().y + _Box->GetTransform()->GetScale().y * 0.5 > GameWindow::MainGameWin()->MousePosInt().y
		&& _Box->IsUpdate() == true)
		return true;
	else
		return false;
}

Scene::Scene() : RMGR(this)
{
}


Scene::~Scene()
{
}

/////////////////////////////////////////////////// Actor

HPTR<Actor> Scene::CreateActor(const wchar_t* _Name) 
{
	// �ű�� ���� ������ ��������.
	Actor* NewActor = new Actor();
	NewActor->SetName(_Name);
	NewActor->GetScene(this);
	m_ActorList.push_back(NewActor);

	if (NewActor->AddComponent<Transform>())
	{

	}

	return NewActor;
}



void Scene::ChangeEnd() 
{
	m_UpdaterBegin = m_UpdaterList.begin();
	m_UpdaterEnd = m_UpdaterList.end();

	for (; m_UpdaterBegin != m_UpdaterEnd; ++m_UpdaterBegin)
	{
		(*m_UpdaterBegin)->ChangeEnd();
	}
}

void Scene::ChangeStart() 
{
	m_UpdaterBegin = m_UpdaterList.begin();
	m_UpdaterEnd = m_UpdaterList.end();

	for (; m_UpdaterBegin != m_UpdaterEnd; ++m_UpdaterBegin)
	{
		(*m_UpdaterBegin)->ChangeStart();
	}
}

// ������ ������ ������ �� ���Ѷ�.
void Scene::Progress() 
{
	// ������ ������Ʈ ���ִ� ��

	SceneComUpdate();

	// CBuf
	ActorCheck();
	UpdateBefore();
	Update();
	UpdateAfter();
	FinalUpdate();
	RenderBefore();
	SceneRender();
	RenderAfter();
	ColBefore();
	SceneCol();
	ColAfter();
	Release();
	ReleaseAfter();
}

void Scene::ActorCheck() 
{
	m_ActorBegin = m_ActorList.begin();
	m_ActorEnd = m_ActorList.end();

	for (; m_ActorBegin != m_ActorEnd; )
	{
		if (true == (*m_ActorBegin)->IsChildReady())
		{
			(*m_ActorBegin)->ChildReadyUpdate();
			m_ActorBegin = m_ActorList.erase(m_ActorBegin);
		}
		else {
			++m_ActorBegin;
		}
	}

	// ITERFOR(AStart, AEnd, m_ActorList, ChildReadyUpdate);
}

void Scene::SceneComUpdate()
{
	m_UpdaterBegin = m_UpdaterList.begin();
	m_UpdaterEnd = m_UpdaterList.end();

	for (; m_UpdaterBegin != m_UpdaterEnd; ++m_UpdaterBegin)
	{
		(*m_UpdaterBegin)->SceneUpdate();
	}
}

void Scene::UpdateAfter()
{
	m_ActorBegin = m_ActorList.begin();
	m_ActorEnd = m_ActorList.end();

	for (; m_ActorBegin != m_ActorEnd; ++m_ActorBegin)
	{
		if (false == (*m_ActorBegin)->IsUpdate())
		{
			continue;
		}
		(*m_ActorBegin)->UpdateAfter();
	}
}
void Scene::Update()
{
	m_ActorBegin = m_ActorList.begin();
	m_ActorEnd = m_ActorList.end();

	for (; m_ActorBegin != m_ActorEnd; ++m_ActorBegin)
	{
		if (false == (*m_ActorBegin)->IsUpdate())
		{
			continue;
		}
		(*m_ActorBegin)->Update();
	}
}
void Scene::UpdateBefore()
{
	ITERFOR(m_ActorBegin, m_ActorEnd, m_ActorList, UpdateBefore);
}
void Scene::FinalUpdate()
{
	ITERFOR(m_ActorBegin, m_ActorEnd, m_ActorList, FinalUpdate);
}
void Scene::RenderBefore()
{
	ITERFOR(m_ActorBegin, m_ActorEnd, m_ActorList, RenderBefore);

}
void Scene::SceneRender()
{
	RMGR.Render();
	// �����ؾ��� ����̱� ������
	// �̰͵� �����ؾ��� ����̱� ������ ���� �����Ŵ����� �Ѵ�.

}
void Scene::RenderAfter()
{
	ITERFOR(m_ActorBegin, m_ActorEnd, m_ActorList, RenderAfter);
}
void Scene::ColBefore()
{
	ITERFOR(m_ActorBegin, m_ActorEnd, m_ActorList, ColBefore);
}
void Scene::ColAfter()
{
	ITERFOR(m_ActorBegin, m_ActorEnd, m_ActorList, ColAfter);
}
void Scene::Release()
{
	RMGR.Release();
	CMGR.Release();

	m_ActorBegin = m_ActorList.begin();
	m_ActorEnd = m_ActorList.end();

	for (; m_ActorBegin != m_ActorEnd; )
	{
		(*m_ActorBegin)->Release();
		if (true == (*m_ActorBegin)->IsDeath())
		{
			m_ActorBegin = m_ActorList.erase(m_ActorBegin);
		}
		else {
			++m_ActorBegin;
		}
	}

}
void Scene::ReleaseAfter()
{
	ITERFOR(m_ActorBegin, m_ActorEnd, m_ActorList, ReleaseAfter);
}

void Scene::SceneCol() {
	CMGR.Col();
}

std::list<HPTR<Actor>> Scene::ExtractOverActor(const wchar_t* _NextSceneName) 
{
	std::list<HPTR<Actor>> OverList;

	m_ActorBegin = m_ActorList.begin();
	m_ActorEnd = m_ActorList.end();

	for (; m_ActorBegin != m_ActorEnd; )
	{
		// ���� �ѳ���¾ְ� �ƴϾ�!!!
		if (false == (*m_ActorBegin)->IsOver())
		{
			++m_ActorBegin;
			continue;
		}
		else if(true == (*m_ActorBegin)->OverCheck(_NextSceneName))
		{
			(*m_ActorBegin)->OverReadyOn();
			OverList.push_back(*m_ActorBegin);
			m_ActorBegin = m_ActorList.erase(m_ActorBegin);
		}
		else {
			++m_ActorBegin;
		}
	}

	// �������� ���ڴ�!!!!
	RMGR.OverRelease();
	CMGR.OverRelease();


	return OverList;
}

void Scene::OverActorPush(std::list<HPTR<Actor>>&& _List) 
{
	std::list<HPTR<Actor>>::iterator Start = _List.begin();
	std::list<HPTR<Actor>>::iterator End = _List.end();

	for (; Start != End; ++Start)
	{
		(*Start)->GetScene(this); // ���� ���õư�
		// �ڽ��� ���� ������Ʈ�鿡��
		(*Start)->SceneChangeStart();
		(*Start)->OverReadyOff();
		m_ActorList.push_back((*Start));
	}
}

void Scene::DebugRender()
{
	m_UpdaterBegin = m_UpdaterList.begin();
	m_UpdaterEnd = m_UpdaterList.end();

	for (; m_UpdaterBegin != m_UpdaterEnd; ++m_UpdaterBegin)
	{
		(*m_UpdaterBegin)->DebugRender();
	}
}