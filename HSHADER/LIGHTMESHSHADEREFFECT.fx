#include "LIGHT.fx"
#include "BASECB.fx"
#include "BASEMATH.fx"

#define BUMPOK 1.0F

////////// VTX
struct VTX_IN
{
    // 전용시맨틱이 존재한다.
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
    float4 vNormal : NORMAL; // 색깔
    float4 vBNormal : BINORMAL; // 색깔
    float4 vTangent : TANGENT; // 색깔
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
    float4 vViewPos : POSITION; // 텍스처 좌표
    float4 vNormal : NORMAL; // 색깔
    float4 vBNormal : BINORMAL; // 색깔
    float4 vTangent : TANGENT; // 색깔};
};
// 행렬

// 텍스처중 일부분만 렌더링 하기 위한 FLOAT4가 필요.
cbuffer CUTDATA : register(b1)
{
    float4 CutUv;
}



VTX_OUT VS_MESH(VTX_IN _in)
{
    // 빛을 하나 정의해 보자.
    // float4

    // 빛을 버텍스에서 계산하면 고로쉐이딩

    VTX_OUT Out = (VTX_OUT) 0.0f;
    
    Out.vUv.x = (_in.vUv.x);
    Out.vUv.y = (_in.vUv.y);
    Out.vPos = mul(_in.vPos, WVP);

    Out.vViewPos = mul(_in.vPos, WV);
    Out.vNormal = normalize(mul(_in.vNormal, WV));
    Out.vTangent = normalize(mul(_in.vTangent, WV));
    Out.vBNormal = normalize(mul(_in.vBNormal, WV));
    Out.vNormal.w = Out.vTangent.w = Out.vBNormal.w = 0.0f;

    // RENDERDATA.y == 0.0f 이면 포워드
    // 1이면 디퍼드이다.

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

    
    //float checkCos = dot(_vViewNormal.xyz, vE.xyz);
    //float degree = acos(checkCos) * (180.0f / 3.141592f);
    
    //if (75.0f < degree && 100.0f > degree)
    //{
    //    RLR.vDiff.xyz *= 1.3f;
    //}
    

    return Out;
}


///// PIX



struct PIX_FORWARDOUT
{
    // 포워드 타겟
    // 랜더타겟 하나인거다.
    float4 vOutColor : SV_Target;
    // float4 vViewPosTarget : SV_Target1;
    // float4 vNormal : SV_Target2;
    //float4 vOutColor : SV_Target2;
    //float4 vOutColor : SV_Target3;
    // float4 vOutColor2 : SV_Target1;
};

struct PIX_DEFERREDOUT
{
    // 포워드 타겟
    // 텍스처
    float4 Diff : SV_Target0;
    float4 Pos : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Depth : SV_Target3;
};

// Texture2D ViewPos : register(t0);
// Texture2D Normal : register(t0);

// LightTarget에다가 넣어놓는다.

// LightTarget
// 실제색깔을 혼합하면
// 캠타겟
// 백버퍼 타겟에 

// Texture2D Diff : register(t0);

Texture2D Tex : register(t0);
Texture2D Bump : register(t1);

SamplerState LSmp : register(s0);
SamplerState PSmp : register(s1);
// SamplerState PointSmp : register(s1);

PIX_DEFERREDOUT PS_EFFECTMESH(VTX_OUT _in)
{
    PIX_DEFERREDOUT Out = (PIX_DEFERREDOUT) 0.0f;
    Out.Diff = Tex.Sample(LSmp, _in.vUv); // 이부분
    // 깊이를 같이 저장할수가 없다.
    // Out.Pos = _in.vPos; // 실수함.
    Out.Pos = _in.vViewPos; // 이부분 float4개

    if (RENDERDATA.y == BUMPOK)
    {
        CalBump(Out.Normal, Bump, PSmp, _in.vUv.xy, _in.vNormal, _in.vTangent, _in.vBNormal);
    }
    else
    {
        Out.Normal = _in.vNormal; // 이부분 float4개
    }

    Out.Depth.x = _in.vViewPos.z; // 이부분 float4개
    // Out.Depth.y = 1.0f;

    // 안해주면 타겟에 안보임
    Out.Pos.w = 1.0f;
    Out.Normal.w = 1.0f;
    Out.Depth.w = 1.0f;

    //Out.Diff.a = 0.5f;
    
    
    if (Out.Diff.a == 0.0f)
    {
        clip(-1);
    }
    
    //Out.Diff.a = 0.1f;
    return Out;
}
