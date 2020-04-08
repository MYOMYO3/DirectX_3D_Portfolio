#include "BASECB.fx"
#include "LIGHT.fx"
////////// VTX
struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
};

// ���� ��ü ������ �ϰڴ�.
VTX_OUT VS_DLIGHTING(VTX_IN _in)
{
    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vUv = _in.vUv;
    Out.vPos = _in.vPos;
    return Out;
}

struct PIX_LIGHTOUT
{
    // ������ Ÿ��
    // �ؽ�ó
    float4 Diff : SV_Target0;
    float4 Spec : SV_Target1;
    // float4 Shadow : SV_Target2;
};

cbuffer CAMDATA : register(b1)
{
    float4 DEFERREDCAMPOS;
}

cbuffer SHADOWDATA : register(b2)
{
    matrix LightVP;
    matrix CamInvView;
    float4 ShadowData;
}

// G����
Texture2D PosTex : register(t0);
Texture2D NormalTex : register(t1);
Texture2D DepthTex : register(t2);
Texture2D ShadowTex : register(t3);
Texture2D TestTex : register(t4);

SamplerState Smp : register(s0);

//
PIX_LIGHTOUT PS_DLIGHTING(VTX_OUT _in)
{
    PIX_LIGHTOUT m_Out = (PIX_LIGHTOUT) 0.0f;
    float4 Depth = DepthTex.Sample(Smp, _in.vUv); // �̺κ�

    if (Depth.x == 0.0f)
    {
        clip(-1);
    }

    float4 Pos = PosTex.Sample(Smp, _in.vUv); // �̺κ�
    float4 Normal = NormalTex.Sample(Smp, _in.vUv); // �̺κ�
    float4 TestColor = TestTex.Sample(Smp, _in.vUv);
    // Normal.w = 0.0f; // �̰� �����ָ� w�� 1�� ���� ������ �ټ� �ִ�.

    //for (int i = 0; i < LightData.LightSettingData.x; ++i)
    //{
    //    LIGHTRESULT LR = CalLightPIX(Pos, Normal, DEFERREDCAMPOS, LightData.ArrLight[i]);
    //    m_Out.Diff += LR.vDiff + LR.vAmbi;
    //    // m_Out.Diff += LR.vAmbi;
    //    // m_Out.Diff = float4(0.0f, 0.0f, 0.0f, 1.0f);
    //    m_Out.Spec += LR.vSpec;
    //}
    
    LIGHTRESULT LR = CalLightPIX(Pos, Normal, DEFERREDCAMPOS, LightData.ArrLight[0]);
    
    //���⼭ ī�������� �־��ָ�ȴ�. 
    
    // W
    
    if (LR.vDiff.x != 0.0f && ShadowData.x != 0.0f)
    {
        // �׸��� �����ϱ�
        float4 vWorldPos = mul(float4(Pos.xyz, 1.f), CamInvView);
        float4 vShadowProj = mul(vWorldPos, LightVP);
        float fDepth = vShadowProj.z / vShadowProj.w;
        float2 vShadowUV = float2((vShadowProj.x / vShadowProj.w) * 0.5f + 0.5f
                           , (vShadowProj.y / vShadowProj.w) * -0.5f + 0.5f);
        
        if (0.01f < vShadowUV.x && vShadowUV.x < 0.99f
        && 0.01f < vShadowUV.y && vShadowUV.y < 0.99f)
        {
            float fShadowDepth = ShadowTex.Sample(Smp, vShadowUV).r;
            // �׸����� ��� ���� ��ȭ��Ų��.
            if (0.0f == fDepth)
            {
                //LR.vDiff = float4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            else if (fDepth > fShadowDepth + 0.0001f)//0.0001f
            {
             
                // ���⼭ ������. 
                //LR.vDiff = float4(1.0f, 0.0f, 0.0f, 1.0f);
                // ���� ���ϰ� �׸��°��� �ƴ� �׸��ڸ� �׸���.
                LR.vDiff *= 0.85f;
                LR.vSpec = (float4) 0.f;
            }
        }
    }
    
    //m_Out.Diff = ceil(m_Out.Diff * 0.5f) / 0.5f;
    
    m_Out.Diff += LR.vDiff + LR.vAmbi;
    m_Out.Spec += LR.vSpec;
    
    m_Out.Diff.w = m_Out.Diff.x;
    m_Out.Spec.w = m_Out.Spec.x;
    
    
    //if (TestColor.r > 0.0f)
    //{
    //    m_Out.Diff += TestColor;
    //}
    
    
    return m_Out;

}

PIX_LIGHTOUT PS_FBXDLIGHTING(VTX_OUT _in)
{
    PIX_LIGHTOUT m_Out = (PIX_LIGHTOUT) 0.0f;
    float4 Depth = DepthTex.Sample(Smp, _in.vUv); // �̺κ�

    if (Depth.x == 0.0f)
    {
        clip(-1);
    }

    float4 Pos = PosTex.Sample(Smp, _in.vUv); // �̺κ�
    float4 Normal = NormalTex.Sample(Smp, _in.vUv); // �̺κ�
    TestTex.Sample(Smp, _in.vUv);

    
    LIGHTRESULT LR = CalLightPIX(Pos, Normal, DEFERREDCAMPOS, LightData.ArrLight[0]);

    
    if (LR.vDiff.x != 0.0f && ShadowData.x != 0.0f)
    {
        // �׸��� �����ϱ�
        float4 vWorldPos = mul(float4(Pos.xyz, 1.f), CamInvView);
        float4 vShadowProj = mul(vWorldPos, LightVP);
        float fDepth = vShadowProj.z / vShadowProj.w;
        float2 vShadowUV = float2((vShadowProj.x / vShadowProj.w) * 0.5f + 0.5f
                           , (vShadowProj.y / vShadowProj.w) * -0.5f + 0.5f);
        
        if (0.01f < vShadowUV.x && vShadowUV.x < 0.99f
        && 0.01f < vShadowUV.y && vShadowUV.y < 0.99f)
        {
            float fShadowDepth = ShadowTex.Sample(Smp, vShadowUV).r;
            // �׸����� ��� ���� ��ȭ��Ų��.
            if (0.0f == fDepth)
            {
                //LR.vDiff = float4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            else if (fDepth > fShadowDepth + 0.0001f)//0.0001f
            {
             
                // ���⼭ ������. 
                //LR.vDiff = float4(1.0f, 0.0f, 0.0f, 1.0f);
                // ���� ���ϰ� �׸��°��� �ƴ� �׸��ڸ� �׸���.
                LR.vDiff *= 0.5f;
                LR.vSpec = (float4) 0.0f;
            }
        }
    }
    
    //m_Out.Diff = ceil(m_Out.Diff * 0.5f) / 0.5f;
    //m_Out.Diff = ceil(m_Out.Diff * 0.5f) / 0.5f;
    
    m_Out.Diff += LR.vDiff + LR.vAmbi;
    m_Out.Spec += LR.vSpec;
    
    m_Out.Diff.w = m_Out.Diff.x;
    m_Out.Spec.w = m_Out.Spec.x;
    
    //m_Out.Diff.w = 0.5f;
    //m_Out.Diff *= 0.5f;
    //m_Out.Diff = float4(1.0f, 0.0f, 0.0f, 1.0f);
    //m_Out.Spec = float4(1.0f, 1.0f, 1.0f, 1.0f);

    //m_Out.Diff = ceil(m_Out.Diff * 0.5f) / 0.5f;
    
    return m_Out;

}

