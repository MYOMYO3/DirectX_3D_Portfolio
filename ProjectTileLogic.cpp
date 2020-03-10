#include "ProjectTileLogic.h"

#include "ChessBoard.h"
#include "StatusLogic.h"

void ProjectTileLogic::SetTarget(Actor* _Actor)
{
	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

Actor* ProjectTileLogic::FindNextTarget()
{
	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);


	BoardIndex myIdx = m_Index;
	m_Index -= 2;

	BoardIndex idx = m_Index;
	

	for (int z = 0; z < 5; z++)
	{
		for (int x = 0; x < 5; x++)
		{
			if ((idx.z + z >= 0 && idx.z + z <= 7) &&
				(idx.x + x >= 0 && idx.x + x <= 7))
			{
				if (myIdx.z == idx.z + z && myIdx.x == idx.x + x)
				{
					continue;
				}

				Actor* enemyActor = ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_Actor;
				if (enemyActor != nullptr)
				{
					StatusLogic* statusLogic = enemyActor->GetComponent<StatusLogic>();
					if (statusLogic->GetType() == PIECETYPE::ENEMY)
					{


						return enemyActor;
					}
				}
			}
		}
	}

	return nullptr;
}

//for (int z = 0; z < 3; z++)
//{
//	for (int x = 0; x < 3; x++)
//	{
//		if ((idx.z + z >= 0 && idx.z + z <= 7) &&
//			(idx.x + x >= 0 && idx.x + x <= 7))
//		{
//			Actor* target = ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_Actor;
//			if (target != nullptr)
//			{
//				StatusLogic* statusLogic = target->GetComponent<StatusLogic>();
//				if (statusLogic->GetType() == PIECETYPE::ENEMY)
//				{
//					statusLogic->AddHp(-75);
//				}
//			}
//		}
//	}
//}

void ProjectTileLogic::ParabolaCalculate(float _MaxTime, float _MaxHeight)
{
	maxHeight = _MaxHeight;
	maxTime = _MaxTime;

	endHeight = j_destPos.y - j_StartPos.y;
	height = maxHeight - j_StartPos.y;

	g = 2 * height / (maxTime * maxTime);
	vY = sqrtf(2 * g * height);

	float a = g;
	float b = -2 * vY;
	float c = 2 * endHeight;

	endTime = (-b + sqrtf(b*b - 4 * a*c)) / (2 * a);

	vX = -(j_StartPos.x - j_destPos.x) / endTime;
	vZ = -(j_StartPos.z - j_destPos.z) / endTime;
}

Vector4 ProjectTileLogic::JumpCalculate()
{
	time += Time::DeltaTime();

	if (time > endTime)
	{

		return j_destPos;
	}
	else
	{
		float x = j_StartPos.x + vX * time;
		float y = j_StartPos.y + (vY * time) - (0.5f * g * time *time);
		float z = j_StartPos.z + vZ * time;

		return Vector4(x, y, z);
	}
}

Vector4 ProjectTileLogic::Rotate(BoardIndex _DestIdx, float _Degree)
{
	Vector4 standDir = { 1.0f, 0.0f, 0.0f, 0.0f };
	Vector4 resultDir = Vector4((float)_DestIdx.x, 0.0f, (float)_DestIdx.z, 0.0f) - Vector4((float)m_Index.x, 0.0f, (float)m_Index.z, 0.0f);

	Vector4 crossVector = Vector4::CrossProduct(standDir, resultDir);

	float cosDegree = Vector4::DotProduct(resultDir.ReturnNormalization(), standDir);
	float resultDegree = acos(cosDegree) * Constant::R2D + 90.0f + _Degree + degree;

	if (_DestIdx.z > m_Index.z)
	{
		resultDegree += 180.0f;
		if (_DestIdx.x < m_Index.x)
		{
			resultDegree -= 90.0f;
		}
		else if (_DestIdx.x > m_Index.x)
		{
			int a = 0;
			resultDegree += 90.0f;
		}
		else
		{

		}
	}
	else if (_DestIdx.z < m_Index.z)
	{
		if (_DestIdx.x < m_Index.x)
		{

		}
		else if (_DestIdx.x > m_Index.x)
		{

		}
		else
		{

		}
	}
	else
	{

	}



	GetTransform()->SetLocalRotY(-resultDegree);

	Vector4 returnDir = { resultDir.x, 0.0f, -resultDir.z };

	return returnDir.ReturnNormalization();
}


void ProjectTileLogic::Jump()
{
	

}

void ProjectTileLogic::Init()
{
	
}

void ProjectTileLogic::Update()
{
	if (isRotate == false)
	{
		Rotate(moveDestIdx, -90.0f);
		isRotate = true;
	}
	
	j_destPos = moveDestPos;
	j_StartPos = ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].m_BoardPos;

	ParabolaCalculate(0.3f, 50.0f);
	Vector4 JumpResult = JumpCalculate();

	GetActor()->GetTransform()->SetLoclaPosition(JumpResult);

	if (JumpResult == j_destPos)
	{
		GetActor()->Die();
	}
}

ProjectTileLogic::ProjectTileLogic() : isRotate(false), degree(0.0f)
{
}


ProjectTileLogic::~ProjectTileLogic()
{
}
