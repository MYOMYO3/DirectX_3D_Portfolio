#pragma once
#include "HSHADER.h"

class HPIXSHADER : public HSHADER
{
	// Ÿ�� ������ ������ �ش�.
private:
	ID3D11PixelShader* m_pPixShader;
	int m_TargetCount;

public:
	void TargetCount(int _Count) 
	{
		m_TargetCount = _Count;
	}

public:
	bool Load(unsigned int _VH, unsigned int _VL, const char* _FuncName);
	void Update() override;

public:
	HPIXSHADER();
	~HPIXSHADER();
};

