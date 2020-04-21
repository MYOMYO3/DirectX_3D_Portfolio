#pragma once
#include <set>
#include <list>
#include <vector>
#include <REF.h>
#include <Name.h>

#include "UPDATEBASE.h"

class Component;
class Transform;
class Scene;
class Actor final : public Ref, public UpdateBase, public Name
{
	// �θ��ڽ��� �� �ִ� �̻�.
	// �θ��� ȸ���� �ڽĿ��� ������ �ִ°��� �׻� �����ؾ� �Ѵ�.
	// �� ����Ʈ ������� 3���� ���� �� -> ȸ�����
	// ���������� ���忡�� �̾Ƴ��°��� �����ϳ�?
	// ũ���� ���ΰ� �� ������ �༮����
	// �� ����Ʈ ������ 
	// ���� ���� �� ����Ʈ ������ => ȸ����Ŀ��� �̾Ƴ��� �ȴ�.
	// ���忡���� ���� �� ����Ʈ ������

private:
	friend Scene;
	friend Transform;

private:
	// �ѳ����� �ϴ� ���� ����Ʈ
	bool OverReady;
	std::set<std::wstring> m_OverSceneSet;

public:
	bool IsOverReady() 
	{
		return OverReady;
	}

private:
	void OverReadyOn() {
		OverReady = true;
	}

	void OverReadyOff() 
	{
		OverReady = false;
	}

public:
	inline bool OverCheck(const wchar_t* _NextScene)
	{
		return m_OverSceneSet.end() != m_OverSceneSet.find(_NextScene);
	}

	template<typename ...Rest>
	void OverScene(Rest ..._Arg)
	{
		PushOver(_Arg...);
	}

private:
	template<typename ...Rest>
	void PushOver(std::wstring _OverName, Rest ..._Arg)
	{
		m_OverSceneSet.insert(_OverName);
		PushOver(_Arg...);
	}

	void PushOver() {}


public:
	bool IsOver() {
		return 0 != m_OverSceneSet.size();
	}

private:
	// ������ �ϴ� ���Ͱ� �����.
	// �ڽ��� ���� ��� �ִ� ��.
	Scene* m_pScene;
	void GetScene(Scene* _Scene);

public:
	Scene* GetScene()
	{
		//if (nullptr != m_ParentActor)
		//{
		//	return m_ParentActor->Scene();
		//}
		return m_pScene; 
	}

private:
	Transform* m_pTrans;
	void SetTransform(Transform* _Trans) { m_pTrans = _Trans; }

public:
	Transform* GetTransform() { return m_pTrans; }


private: //////////////////////////////////////// ������Ʈ ����
	std::list<HPTR<Component>> m_ComList;
	std::list<HPTR<Component>>::iterator ComStart;
	std::list<HPTR<Component>>::iterator ComEnd;

	std::list<HPTR<Component>>::iterator FindStart;
	std::list<HPTR<Component>>::iterator FindEnd;

public:
	template<typename COM, class... Types>
	HPTR<COM> AddComponent(Types&&... _Args)
	{
		COM* NewCom = new COM();
		PushComponent(NewCom);
		NewCom->DataCreate(_Args...);
		return NewCom;
	}

	template<typename COM>
	HPTR<COM> GetComponent()
	{
		FindStart = m_ComList.begin();
		FindEnd = m_ComList.end();

		for (; FindStart != FindEnd; ++FindStart)
		{
			// ������� ����� ��κ��� ������
			// �𸥴ٰ� �ϴ°�.
			// ��������� ���� �� �θ�
			// ����� ���� ���� ����.
			// ���δ�.
			// ���ø�.

			// ���⼭ h
			// �̳༮�� �����ߴ��� �𸥴ٴ� ������� ���Ҵ�.


			// �� �۾��� �����Ϸ��� ������ �ʿ��Ѱ�? 
			// (*FindStart)->IsType<COM>()
			// ���漱�� ������ ���� �ֳ�?
			// typeid����� ������� ����Ҽ� �ִ�.
			Component* P = (*FindStart);

			if (true == CheckFunc(P, &typeid(COM)))
			{
				// �ܼ��� �������� ���԰� ���Ͽ���.
				// �̷��� �׳� ��� ����� ���� �ȴ�.
				// �׳༮�� �Լ��� ������ �˾ƾ� �ϴ� ��찡 �ƴϸ�
				// ��� ����� �ʿ����.
				// �����͸� ���ų� �����Ѵٴ� ������ 
				// ���漱�� ������ 

				// HPTR<HCOM> -> HPTR<HRENDER>
				return (COM*)P;
			}
		}
		return nullptr;
	}


	template<typename COM>
	std::vector<HPTR<COM>> GetComToList()
	{
		std::vector< HPTR<COM>> ReturnList;

		FindStart = m_ComList.begin();
		FindEnd = m_ComList.end();

		for (; FindStart != FindEnd; ++FindStart)
		{
			if (true == CheckFunc((*FindStart), &typeid(COM)))
			{
				ReturnList.push_back((COM*)(*FindStart).PTR);
			}
		}
		return ReturnList;
	}

private:
	bool CheckFunc(Component* _P, const type_info* _TI);

private:
	void PushComponent(Component* _pCom);



// �� �ѳ��ٴϴ� ���� 
// ������Ʈ ����
// ��ɰ��� ����


public:
	void UpdateAfter() override;
	void Update() override;
	void UpdateBefore() override;
	void FinalUpdate() override;
	void RenderBefore() override;
	void RenderAfter() override;
	void ColBefore() override;
	void ColAfter() override;
	void Release() override;
	void ReleaseAfter() override;

public:
	void SceneChangeStart();

////////////////// �θ��ڽ� ����.
private:
	bool							  m_IsChildReady;

	Actor*							  m_ParentActor;
	std::list<HPTR<Actor>>::iterator ChildStart;
	std::list<HPTR<Actor>>::iterator ChildEnd;
	std::list<HPTR<Actor>>			  m_ChildList;

private:
	bool IsChildReady() 
	{
		return m_IsChildReady;
	}
	void ChildReadyUpdate();

public:
	void SetParent(HPTR<Actor>	_ParentActor);

private:
	int order;

public:
	void SetOrder(int _Order)
	{
		order = _Order;
	}

private:
	int level;

public:
	void SetLevel(int _Value)
	{
		level = _Value;
	}

	int GetLevel()
	{
		return level;
	}


private:
	Actor();
	~Actor();
};

