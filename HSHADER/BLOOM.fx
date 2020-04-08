#include "BASECB.fx"
#include "LIGHT.fx"

// ����þ��� �Ҷ� ����þ� ���Ϳ� �ؽ�ó.
static float g_vGAU[25] =
{
    1.0F, 2.0f, 4.0f, 2.0f, 1.0f, // 10
    2.0F, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    4.0F, 8.0f, 16.0f, 8.0f, 4.0f, // 40
    2.0F, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    1.0F, 2.0f, 4.0f, 2.0f, 1.0f, // 10
};


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

// ���
cbuffer FilterSize : register(b0)
{
    float4 m_FilterSize;
}


// ���� ��ü ������ �ϰڴ�.
VTX_OUT VS_BLOOMSMALL(VTX_IN _in)
{
    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vUv = _in.vUv;
    Out.vPos = _in.vPos;
    return Out;
}

struct PS_OUTPUT
{
    float4 vCol : SV_Target;
};

// G����
Texture2D LightDiffTarget : register(t0);
Texture2D LightSpecTarget : register(t1);
SamplerState LSmp : register(s0);

PS_OUTPUT PS_BLOOMSMALL(VTX_OUT _in)
{
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;
    // ķŸ���� ������ ���̴�.
    // �������� ó���� �÷��� �״�� ��������.

    //_in.vUv.x *= 800 / 128.0f;
    //_in.vUv.y *= 600 / 128.0f;

    float4 SpecColor = LightSpecTarget.Sample(LSmp, _in.vUv);
    float4 DiffColor = LightDiffTarget.Sample(LSmp, _in.vUv);

    // float Slume = (Speclu.r + Speclu.g + Speclu.b) * 0.33333333334f;
    // float Dlume = (Difflu.r + Difflu.g + Difflu.b) * 0.33333333334f;

    float Lum = 0.0f;
    float PixelU = 1.0f / m_FilterSize.x;
    float PixelV = 1.0f / m_FilterSize.y;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            float2 vUv = _in.vUv + float2((i - 1) * PixelU, (j - 1) * PixelV);
            Lum += LightSpecTarget.Sample(LSmp, vUv) * g_vGAU[i * 5 + j];
        }
    }
    Lum /= 50.0f;

    if (0.8f >= Lum.x)
    {
        outData.vCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return outData;
    }
    outData.vCol = float4(Lum, Lum, Lum, Lum);

    //if (0.9f >= SpecColor.r)
    //{
    //    outData.vCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
    //    return outData;
    //}

    //outData.vCol = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // outData.vCol = SpecColor;

    // ��ǻ�� �÷��� �����Ѵ�.
    
    return outData;

}



Texture2D SmallTarget : register(t0);   //����� Ÿ��
Texture2D OriTarget : register(t1);     //���� �̹���


PS_OUTPUT PS_BLOOMEFFECT(VTX_OUT _in)
{
	// �ƿ�ǲ�� ���´�.
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;
    // ķŸ���� ������ ���̴�.
    // �������� ó���� �÷��� �״�� ��������.
    
    // getdimensions()
    // CamTarget.GetDimensions
    float4 Ori = OriTarget.Sample(LSmp, _in.vUv);


    float4 Lum = SmallTarget.Sample(LSmp, _in.vUv);
    //float4 Lum = 0.0f;
    //float PixelU = 1.0f / 64.0f;
    //float PixelV = 1.0f / 64.0f;
    //for (int i = 0; i < 5; ++i)
    //{
    //    for (int j = 0; j < 5; ++j)
    //    {
    //        float2 vUv = _in.vUv + float2((i - 1) * PixelU, (j - 1) * PixelV);
    //        Lum += SmallTarget.Sample(LSmp, vUv) * g_vGAU[i * 5 + j];
    //    }
    //}
    //Lum /= 30.0f;

    outData.vCol = Ori + Lum;
    outData.vCol.w = 1.0f;

    // ���� ��ȭ�� �ְ�

    return outData;
}