#pragma once
#include <LOGIC.h>

class PirateSkill :
	public Logic
{
private:
	bool isJumping;
	float particleTime;

public:
	void SetTarget(Actor* _Actor);


protected:
	bool		isRun;
	bool		isJump;
	bool		isRotate;

	BoardIndex  m_Index;
	Actor*		pAttTarget;
	Vector4		attDestPos;
	BoardIndex	attDestIdx;
	int attCnt;

	Actor*		pMoveTarget;
	Vector4		moveDestPos;
	BoardIndex	moveDestIdx;

	//
#pragma region MOVE
	Vector4 j_destPos;
	Vector4 j_StartPos;

	float vX; // x������Ǽӵ�
	float vY; // y������Ǽӵ�
	float vZ; // z������Ǽӵ�
	float g; // y������� �߷°��ӵ�
	float endTime;// ������ ���޽ð�
	float maxHeight; //�ְ��� ����
	float height; // �ְ��� - ������(����)
	float endHeight;//������ ����
	float time; //����ð�
	float maxTime; //�ְ��� ���޽ð�
#pragma endregion

	float deadTime;

	float degree;

public:
	void SetDegree(float _Degree)
	{
		degree = _Degree;
	}

public:
	Actor* FindNextTarget();


public:

	void	ParabolaCalculate(float _MaxTime, float _MaxHeight);


	Vector4 JumpCalculate();
	Vector4 Rotate(BoardIndex	_DestIdx, float _Degree);

	void Jump();

public:
	void Init() override;
	void Update() override;
public:
	PirateSkill();
	~PirateSkill();
};

