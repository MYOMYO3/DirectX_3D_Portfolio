#include "BASECB.fx"
////////// VTX
struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
    float4 vCol : COLOR; // ����
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
    float4 vCol : COLOR; // ����
};

// �ؽ�ó�� �Ϻκи� ������ �ϱ� ���� FLOAT4�� �ʿ�.
cbuffer CUTDATA : register(b1)
{
    float4 CutUv;
    // x y�� �������� ����.
    // z w���� ũ�⸦ �־��� ���̴�.
    // ��������?
    // 0.25 0.25 ������
    // 0.5 0.5 ũ��
}

cbuffer UIDATA : register(b2)
{
    float4 m_UIData;
    // x y�� �������� ����.
    // z w���� ũ�⸦ �־��� ���̴�.
    // ��������?
    // 0.25 0.25 ������
    // 0.5 0.5 ũ��
}


VTX_OUT VS_TEX(VTX_IN _in)
{
    // �Ϻκи� ������ �Ѵٴ� �̾߱��
    // 

    // 0 0 | 1 0
    // 0 1 | 1 1

    //   0.25 0.25   | 0.75    0.25 
    //   0.25 0.75F   | 0.75  0.75

    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vCol = _in.vCol;
    

    //           1           0.5        0.25
    Out.vUv.x = (_in.vUv.x * CutUv.z) + CutUv.x;
    //           0           0.5        0.25
    Out.vUv.y = (_in.vUv.y * CutUv.w) + CutUv.y;

    // Out.vUv = _in.vUv;

    Out.vPos = mul(_in.vPos, WVP);

    //if (m_UIData.x == 1.0f)
    //{
    //    Out.vPos.z = 0.0f;
    //}

    return Out;
}


///// PIX



struct PIX_OUT
{
    float4 vOutColor : SV_Target;
    // float4 vOutColor : SV_Target1;
    // float4 vOutColor : SV_Target2;
    // float4 vOutColor : SV_Target3;
    // float4 vOutColor : SV_Target4;
    // float4 vOutColor : SV_Target5;
    // float4 vOutColor : SV_Target6;
    // float4 vOutColor : SV_Target7;
};

Texture2D Tex : register(t0); 
SamplerState LSmp : register(s0); 
SamplerState PSmp : register(s1);

PIX_OUT PS_TEX(VTX_OUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;
    Out.vOutColor = Tex.Sample(LSmp, _in.vUv); // �̺κ�

    // Out.vOutColor.a = 0.0f;
    

    //Out.vOutColor.a = 0.6f;
    if (0.0f == Out.vOutColor.a)
    {
        // ���ȼ��� �����ض�!!!!!!
        clip(-1);
    }
    

    return Out;
}

PIX_OUT PS_HALFTRANS(VTX_OUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;
    Out.vOutColor = Tex.Sample(LSmp, _in.vUv); // �̺κ�

    // Out.vOutColor.a = 0.0f;
    

    //Out.vOutColor.a = 0.6f;
    if (0.0f == Out.vOutColor.a)
    {
        // ���ȼ��� �����ض�!!!!!!
        clip(-1);
    }
    
    Out.vOutColor.a = 0.51f;

    return Out;
}