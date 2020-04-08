#include "LIGHT.fx"
#include "BASECB.fx"
#include "BASEMATH.fx"

#define BUMPOK 1.0F

////////// VTX
struct VTX_IN
{
    // ����ø�ƽ�� �����Ѵ�.
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
    float4 vCol : COLOR; // ����
    float4 vNormal : NORMAL; // ����
    float4 vBNormal : BINORMAL; // ����
    float4 vTangent : TANGENT; // ����
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
    float4 vCol : COLOR; // ����
    float4 vViewPos : POSITION; // �ؽ�ó ��ǥ
    float4 vNormal : NORMAL; // ����
    float4 vBNormal : BINORMAL; // ����
    float4 vTangent : TANGENT; // ����};
};
// ���

// �ؽ�ó�� �Ϻκи� ������ �ϱ� ���� FLOAT4�� �ʿ�.
cbuffer CUTDATA : register(b1)
{
    float4 CutUv;
}



VTX_OUT VS_MESH(VTX_IN _in)
{
    // ���� �ϳ� ������ ����.
    // float4

    // ���� ���ؽ����� ����ϸ� ��ν��̵�

    VTX_OUT Out = (VTX_OUT) 0.0f;
    
    Out.vUv.x = (_in.vUv.x);
    Out.vUv.y = (_in.vUv.y);
    Out.vPos = mul(_in.vPos, WVP);

    Out.vViewPos = mul(_in.vPos, WV);
    Out.vNormal = normalize(mul(_in.vNormal, WV));
    Out.vTangent = normalize(mul(_in.vTangent, WV));
    Out.vBNormal = normalize(mul(_in.vBNormal, WV));
    Out.vNormal.w = Out.vTangent.w = Out.vBNormal.w = 0.0f;

    // RENDERDATA.y == 0.0f �̸� ������
    // 1�̸� ���۵��̴�.

    if (RENDERDATA.x == 0.0F )
    {
        float4 CP = mul(CAMPOS, View);
        // CP = CAMPOS;
        for (int i = 0; i < LightData.LightSettingData.x; ++i)
        {
            LIGHTRESULT LR = CalLightVTX(Out.vViewPos, Out.vNormal, CP, LightData.ArrLight[i]);
            Out.vCol += LR.vDiff + LR.vSpec + LR.vAmbi;
        }

    }
    //else if (RENDERDATA.x == 1.0F)
    //{
    //    Out.vViewPos = mul(_in.vPos, World);
    //    Out.vNormal = normalize(mul(_in.vNormal, World));
    //    Out.vTangent = normalize(mul(_in.vTangent, World));
    //    Out.vBNormal = normalize(mul(_in.vBNormal, World));
    //    Out.vNormal.w = Out.vTangent.w = Out.vBNormal.w = 0.0f;
    //}


    return Out;
}


///// PIX



struct PIX_FORWARDOUT
{
    // ������ Ÿ��
    // ����Ÿ�� �ϳ��ΰŴ�.
    float4 vOutColor : SV_Target;
    // float4 vViewPosTarget : SV_Target1;
    // float4 vNormal : SV_Target2;
    //float4 vOutColor : SV_Target2;
    //float4 vOutColor : SV_Target3;
    // float4 vOutColor2 : SV_Target1;
};

struct PIX_DEFERREDOUT
{
    // ������ Ÿ��
    // �ؽ�ó
    float4 Diff : SV_Target0;
    float4 Pos : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Depth : SV_Target3;
    float4 Test : SV_Target4;
};

// Texture2D ViewPos : register(t0);
// Texture2D Normal : register(t0);

// LightTarget���ٰ� �־���´�.

// LightTarget
// ���������� ȥ���ϸ�
// ķŸ��
// ����� Ÿ�ٿ� 

// Texture2D Diff : register(t0);
cbuffer SEIREN: register(b3)
{
    float4 serienData;
}

Texture2D Tex : register(t0);
Texture2D Bump : register(t1);
Texture2D Test : register(t2);

SamplerState LSmp : register(s0);
SamplerState PSmp : register(s1);
// SamplerState PointSmp : register(s1);



PIX_FORWARDOUT PS_MESH(VTX_OUT _in)
{
    PIX_FORWARDOUT Out = (PIX_FORWARDOUT) 0.0f;
    Out.vOutColor = Tex.Sample(LSmp, _in.vUv); // �̺κ�

    if (RENDERDATA.y == BUMPOK)
    {
        // ������ �ִٷ� üũ�ϰڴ�.
        CalBump(_in.vNormal, Bump, PSmp, _in.vUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
        // �븻�� �ٲ�� ��.
        // _in.vNormal =

    }


    if (RENDERDATA.x == 0.0f)
    {
        Out.vOutColor *= _in.vCol;
    }
    else if (RENDERDATA.x == 1.0f)
    {
        // float4 CP = mul(CAMPOS, View);
        // CP = CAMPOS;
        float4 LightColor = (float4) 0.0f;

        for (int i = 0; i < LightData.LightSettingData.x; ++i)
        {
            LIGHTRESULT LR = CalLightPIX(_in.vViewPos, _in.vNormal, CAMPOS, LightData.ArrLight[i]);
            LightColor += LR.vDiff + LR.vSpec + LR.vAmbi;
        }

        // LightColor.w = 1.0f;
        // �ؽ�ó��      ����Ʈ��    ȥ����   ���������̴�.
        Out.vOutColor.xyz *= LightColor.xyz;
    }
       

    
    return Out;
}


PIX_DEFERREDOUT PS_DMESH(VTX_OUT _in)
{
 
    
    PIX_DEFERREDOUT Out = (PIX_DEFERREDOUT) 0.0f;
    Out.Diff = Tex.Sample(LSmp, _in.vUv); // �̺κ�
    // ���̸� ���� �����Ҽ��� ����.
    // Out.Pos = _in.vPos; // �Ǽ���.
    Out.Pos = _in.vViewPos; // �̺κ� float4��

    if (RENDERDATA.y == BUMPOK)
    {
        CalBump(Out.Normal, Bump, PSmp, _in.vUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
    }
    else
    {
        Out.Normal = _in.vNormal; // �̺κ� float4��
    }

    Out.Depth.x = _in.vViewPos.z; // �̺κ� float4��
    // Out.Depth.y = 1.0f;

    // �����ָ� Ÿ�ٿ� �Ⱥ���
    Out.Pos.w = 1.0f;
    Out.Normal.w = 1.0f;
    Out.Depth.w = 1.0f;

    //Out.Diff.r = 0.5f;
    
  
    
    
    if (Out.Diff.a == 0.0f)
    {
        clip(-1);
    }
    
    //Out.Diff.a = 0.1f;
    
    Out.Test = Out.Diff;
    
    
    return Out;
}

PIX_DEFERREDOUT PS_FBXDMESH(VTX_OUT _in)
{
 
    
    PIX_DEFERREDOUT Out = (PIX_DEFERREDOUT) 0.0f;
    Out.Diff = Tex.Sample(LSmp, _in.vUv); // �̺κ�
    // ���̸� ���� �����Ҽ��� ����.
    // Out.Pos = _in.vPos; // �Ǽ���.
    Out.Pos = _in.vViewPos; // �̺κ� float4��

    if (RENDERDATA.y == BUMPOK)
    {
        CalBump(Out.Normal, Bump, PSmp, _in.vUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
    }
    else
    {
        Out.Normal = _in.vNormal; // �̺κ� float4��
    }

    Out.Depth.x = _in.vViewPos.z; // �̺κ� float4��
    // Out.Depth.y = 1.0f;

    // �����ָ� Ÿ�ٿ� �Ⱥ���
    Out.Pos.w = 1.0f;
    Out.Normal.w = 1.0f;
    Out.Depth.w = 1.0f;

    //Out.Diff.a = 0.5f;
    
    float check = serienData.x;
    if (check.x == 1.0f)
    {
        Out.Diff.r = (117.0f / 255.0f);
        Out.Diff.g = (73.0f / 255.0f);
        Out.Diff.b = (41.0f / 255.0f);
        
    }
    //Out.Diff += 0.5f;
    
    
    if (Out.Diff.a == 0.0f)
    {
        clip(-1);
    }
    
    //Out.Diff += 0.5f;
    
    if (Out.Normal.x > 0.5f)
    {
        Out.Test = Out.Normal;
    }
    
    //Out.Diff += Test.Sample(LSmp, _in.vUv); // �̺κ�
    
    
    return Out;
}

PIX_FORWARDOUT SKY_MESH(VTX_OUT _in)
{
    PIX_FORWARDOUT Out = (PIX_FORWARDOUT) 0.0f;
    Out.vOutColor = Tex.Sample(LSmp, _in.vUv); // �̺κ�

    return Out;
}


PIX_DEFERREDOUT PS_EFFECTDMESH(VTX_OUT _in)
{
    PIX_DEFERREDOUT Out = (PIX_DEFERREDOUT) 0.0f;
    Out.Diff = Tex.Sample(LSmp, _in.vUv); // �̺κ�
    // ���̸� ���� �����Ҽ��� ����.
    // Out.Pos = _in.vPos; // �Ǽ���.
    Out.Pos = _in.vViewPos; // �̺κ� float4��

    if (RENDERDATA.y == BUMPOK)
    {
        CalBump(Out.Normal, Bump, PSmp, _in.vUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
    }
    else
    {
        Out.Normal = _in.vNormal; // �̺κ� float4��
    }

    Out.Depth.x = _in.vViewPos.z; // �̺κ� float4��
    // Out.Depth.y = 1.0f;

    // �����ָ� Ÿ�ٿ� �Ⱥ���
    Out.Pos.w = 1.0f;
    Out.Normal.w = 1.0f;
    Out.Depth.w = 1.0f;

    //Out.Diff.a = 0.5f;
    
    
    if (Out.Diff.a == 0.0f)
    {
        clip(-1);
    }
    Out.Diff.a = 0.7f;
    
    //Out.Diff.a = 0.1f;
    
    
    return Out;
}

PIX_DEFERREDOUT PS_FBXEFFECTDMESH(VTX_OUT _in)
{
    PIX_DEFERREDOUT Out = (PIX_DEFERREDOUT) 0.0f;
    Out.Diff = Tex.Sample(LSmp, _in.vUv); // �̺κ�
    // ���̸� ���� �����Ҽ��� ����.
    // Out.Pos = _in.vPos; // �Ǽ���.
    Out.Pos = _in.vViewPos; // �̺κ� float4��

    if (RENDERDATA.y == BUMPOK)
    {
        CalBump(Out.Normal, Bump, PSmp, _in.vUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
    }
    else
    {
        Out.Normal = _in.vNormal; // �̺κ� float4��
    }

    Out.Depth.x = _in.vViewPos.z; // �̺κ� float4��
    // Out.Depth.y = 1.0f;

    // �����ָ� Ÿ�ٿ� �Ⱥ���
    Out.Pos.w = 1.0f;
    Out.Normal.w = 1.0f;
    Out.Depth.w = 1.0f;

    //Out.Diff.a = 0.5f;
    
    
    if (Out.Diff.a == 0.0f)
    {
        clip(-1);
    }
    Out.Diff.a = 0.51f;
    
    //Out.Diff.a = 0.1f;
    return Out;
}