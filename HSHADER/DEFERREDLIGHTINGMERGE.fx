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
VTX_OUT VS_LIGHTMERGE(VTX_IN _in)
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
    float4 MergeColor : SV_Target0; // ���۵�� �׷��� �ֵ��� ķ Ÿ�ٿ� �ٽ� �׷��ش�.
    // float4 Ambi : SV_Target1;
};

// G����
Texture2D DiffTex : register(t0);
Texture2D LightDiffTex : register(t1);
Texture2D LightSpecTex : register(t2);
//Texture2D TestTex : register(t3);

SamplerState Smp : register(s0);

PIX_LIGHTOUT PS_LIGHTMERGE(VTX_OUT _in)
{
    PIX_LIGHTOUT m_Out = (PIX_LIGHTOUT)0.0f;
    float4 Diff = DiffTex.Sample(Smp, _in.vUv); // �̺κ�
        
    //float4 Test = TestTex.Sample(Smp, _in.vUv); // �̺κ�
    
    
    //Diff += Test;
    
    if (Diff.w == 0.0f)
    {
        clip(-1);
    }
    
    
    
    //int& num ;
    
    //int num;
    //&num ;
    
    float4 LightDiffColor = LightDiffTex.Sample(Smp, _in.vUv); // �̺κ�
    float4 LightSpecColor = LightSpecTex.Sample(Smp, _in.vUv); // �̺κ�

    //  m_Out.MergeColor = Diff + LightDiffColor + LightSpecColor;
    //  float4 Light = (LightDiffColor.xyz + LightSpecColor.xyz, 1.0f);
    
    Diff.w = 1.0f;
    m_Out.MergeColor.xyz = Diff.xyz * (LightDiffColor.xyz + LightSpecColor.xyz);
    m_Out.MergeColor.w = 1.0f;

    
   
    
    return m_Out;
}
