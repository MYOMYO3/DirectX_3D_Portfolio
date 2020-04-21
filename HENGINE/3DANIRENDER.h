#pragma once
#include "RENDER.h"
#include "FBX.h"

class Fbx;
class Texture;
class FbxAnimation : public Renderer
{
public:
	class UserAniClip : public Ref
	{
	public:
		std::wstring Name;
		int m_Start; // fbx�� Ÿ�Ӹ�带 intȭ ��Ų��
		int m_End; // fbx�� Ÿ�Ӹ�带 intȭ ��Ų��
		float m_StartTime;
		float m_EndTime;
		float m_Speed;

		bool Loop;

		bool m_AniEnd;
		bool m_AniHalf;
		bool m_AniThreeQuarters;

		UserAniClip() : m_Speed(1.0f), m_AniHalf(false), m_AniThreeQuarters(false)
		{

		}
	};

	class ANIBONEDATA
	{
	public:
		Vector4 T;
		Vector4 S;
		Vector4 Q;
	};

	class BONEDATA
	{
	public:
		Vector4 LT;
		Vector4 LS;
		Vector4 LQ;
		Vector4 WT;
		Vector4 WS;
		Vector4 WQ;
		Matrix LMat;
		Matrix WMat;
	};

private:
	std::map<std::wstring, HPTR<UserAniClip>> m_Ani;

	int m_ClipIndex; // �ִϸ��̼���
	int m_FrameCount; // fbx�� Ÿ�Ӹ�带 intȭ ��Ų��
	
	float m_CurTime; // ���� ��� �ð�
	float m_UpdateTime; // ������Ʈ
	int StartFrame;
	int EndFrame;
	
	bool isLock;
	Vector4 seirenData;

	Fbx::FbxAniData* CurAniData;

	HPTR<Fbx> m_FBXDATA;
	HPTR<Texture> m_BoneTex;

	std::vector<ANIBONEDATA> m_ANIBONEDATA;
	std::vector<Matrix> m_OffsetAniBoneData; // �ִϸ��̼� ����� ����� ���� �����
	std::vector<Matrix> m_CalAniBoneData; // �ִϸ��̼ǵ� ���

	int iFrameIndex;
	int iNextFrameIndex;
	HPTR<UserAniClip> CurClip;


	HRENDERDATA* DATA;

public:
	bool IsLock()
	{
		return isLock;
	}

	void LockOn()
	{
		isLock = true;
	}
	void LockOff()
	{
		isLock = false;
	}

public:
	bool ChangeRockMaterialOn();
	bool ChangeRockMaterialOff();

public:
	UserAniClip* GetCurClip()
	{
		return CurClip.PTR;
	}

private:
	HPTR<UserAniClip> FindAni(std::wstring _Name);

public:
	void CreateAnimation(std::wstring _Name, int _Start, int _End, bool _Loop = false);
	void CreateAnimation(std::wstring _Name, int _Start, int _End, float _Speed ,bool _Loop = false);
	void ChangeAnimation(std::wstring _Name);


public:
	int GetFrameIndex() {
		return iFrameIndex;
	}

	float GetUpdateTime() 
	{
		return m_UpdateTime;
	}
	void SetFrameIndex(int _Number) 
	{
		m_UpdateTime = (float)_Number / (float)m_FrameCount;
	}




public:
	void FbxSetting(const wchar_t* _Name);
	void FbxEffectSetting(const wchar_t* _Name);

	BONEDATA FindBoneData(const wchar_t* _Name);

public:
	void Update() override;
	void UpdateBefore() override;
	void RenderDataUpdate(const HPTR<Camera>& _CAM, HRENDERDATA* _Data) override;
	void Render(const HPTR<Camera>& _CAM) override;


public:
	FbxAnimation();
	~FbxAnimation();
};

