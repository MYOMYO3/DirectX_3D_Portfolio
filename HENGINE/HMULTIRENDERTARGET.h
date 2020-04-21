#pragma once
#include "RES.h"
#include "HRENDERTARGET.h"
#include "HDEPTHBUFFER.h"
// ���� ���ۿ�
// ����Ÿ���� ��Ƽ�
// �������� ����Ÿ���� �����ϴ� ���� ����
// ���� ���۱��� ������ �����ϰ� ����� �������̴�.
// �ƿ�ǲ ������ �����н�()
// �װ� ���� �н��� �־��ټ��� ����.
// ķ������ �ȵȴ�.
// ķ���� ������ �ִ�.
// �̰� �ظ��ϸ� ķ�� �����ϰ�
// �׷��ٰ� �ܼ���
// ������� �뵵�θ� ����ϴ°���
// �ƴϴ�.
// �⺻�����δ� ��ó���̱� ������
// ���� ȭ�鿡�� �ٲ���?
// ������ �� ȿ���� ���� �ʴ�.
// 
class HMULTIRENDERTarget : Resources
{
private:
	int SizeX;
	int SizeY;

	std::vector<HPTR<HRENDERTARGET>> ArrRenderTarget;
	std::vector<ID3D11RenderTargetView*> ArrRTV;

	HPTR<HDEPTHBUFFER> m_DSBUFFER;
	ID3D11DepthStencilView* m_DSV;

public:
	HPTR<HRENDERTARGET> Target(int _Index) 
	{
		return ArrRenderTarget[_Index];
	}

	void Depth(HPTR<HDEPTHBUFFER> _DSBUFFER);

	void Clear();

	void CreateTarget(int _W, int _H);
	void OmSet();

public:
	HMULTIRENDERTarget();
	~HMULTIRENDERTarget();
};

