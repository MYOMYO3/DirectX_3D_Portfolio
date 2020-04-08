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

cbuffer TIMEDATA : register(b2)
{
    float4 TimeX;
}


float hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

float noise(float3 x)
{
    // The noise function returns a value in the range -1.0f -> 1.0f

    float3 p = floor(x);
    float3 f = frac(x);

    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + 113.0 * p.z;

    return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
                   lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
               lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
                   lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}


VTX_OUT VS_MESH(VTX_IN _in)
{
    float gWaveHeight = 50.0f;

    float gWaveFrequency = 500.0f;
    float gSpeed = 0.15f;
    // ���� �ϳ� ������ ����.
    // float4

    // ���� ���ؽ����� ����ϸ� ��ν��̵�

    VTX_OUT Out = (VTX_OUT) 0.0f;
    
    float3 bbbb = float3(TimeX.x * 2.0f + ((_in.vUv.x * CutUv.z) + CutUv.x) * gWaveFrequency, 
                            TimeX.x * 2.0f + ((_in.vUv.y * CutUv.w) + CutUv.y) * gWaveFrequency, 2.0f);
    
    //float cosTime = gWaveHeight * cos(TimeX.x * 2.0f + ((_in.vUv.x * CutUv.z) + CutUv.x) * gWaveFrequency);
    float cosTime = gWaveHeight * noise(bbbb);
    
    
    
    //float sinTime = 300.0f * sin(TimeX.x * 2.0f + ((_in.vUv.x * CutUv.z) + CutUv.x) * 1000.0f);
    
    _in.vPos.y += (cosTime);
    //_in.vPos.z += cosTime;
    //_in.vPos.z += sinTime;
    
    Out.vUv.x = (_in.vUv.x * CutUv.z) + CutUv.x + float(TimeX.x * gSpeed);
    Out.vUv.y = (_in.vUv.y * CutUv.w) + CutUv.y;
    //Out.vUv.y = _in.vUv.y;
    
    
    //Out.vUv = float2(_in.vUv.x + TimeX.x * 1.0f, _in.vUv.y);
    
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
    //Out.vCol.a = 0.5f;

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
};

Texture2D Tex : register(t0);
Texture2D Bump : register(t1);
SamplerState LSmp : register(s0);
SamplerState PSmp : register(s1);
// SamplerState PointSmp : register(s1);

PIX_FORWARDOUT PS_MESH(VTX_OUT _in)
{
    PIX_FORWARDOUT Out = (PIX_FORWARDOUT) 0.0f;
    
    
    
    
    //float2 tUv = float2(_in.vUv.x + cos(TimeX.x) * 0.3f, _in.vUv.y + cos(TimeX.x));
    float2 tUv = float2(_in.vUv.x, _in.vUv.y);
    
    Out.vOutColor = Tex.Sample(LSmp, tUv); // �̺κ�

    if (RENDERDATA.y == BUMPOK)
    {
        // ������ �ִٷ� üũ�ϰڴ�.
        CalBump(_in.vNormal, Bump, PSmp, tUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
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

    
    //if (Out.Diff.a == 0.0f)
    //{
    //  clip(-1);
    //}
    
    //Out.Diff.g += 0.5f;
    Out.Diff.a = 0.6f;
    
    return Out;
}