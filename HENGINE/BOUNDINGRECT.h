#pragma once
#include "COL.h"
// 2Ʋ ���� ����������� 4�� 
// �ƹ��� ���Ƶ� ������.
// #include ""

class BoudingRect : public Collider
{
public:
	friend class BoundingCircle;

private:
	DirectX::BoundingOrientedBox m_ColliderData;
	Vector4 Rot;

public:
	void ColBefore() override;
	bool GeometryCheck(Collider* _Other) override;

public:
	BoudingRect();
	~BoudingRect();
};

