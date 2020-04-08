#include "LIGHT.fx"
#include "BASECB.fx"
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
    float4 vBlendWeight : BLENDWEIGHTS;
    float4 vBlendIndex : BLENDINDICES;

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

Texture2D AniMatTex : register(t10);

matrix TexToMat(int idx)
{
    matrix BoneMat =
    {
        AniMatTex.Load(int3((idx * 4), 0, 0)),
        AniMatTex.Load(int3((idx * 4) + 1, 0, 0)),
        AniMatTex.Load(int3((idx * 4) + 2, 0, 0)),
        AniMatTex.Load(int3((idx * 4) + 3, 0, 0)),
    };

    return BoneMat;
}

// ���۷���
void Skinning(inout float4 vPos, inout float4 vNormal, inout float4 vWeights, inout float4 vIndices)
{
    float4 CalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 CalNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.f - vWeights.x - vWeights.y - vWeights.z;

    vPos.w = 1.0F;
    vNormal.w = 0.0F;

    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = TexToMat((int) vIndices[i]);

        CalPos += fWeights[i] * mul(vPos, matBone);
        CalNormal.xyz += fWeights[i] * mul(vNormal, matBone);
    }

    CalPos.w = 1.0F;
    CalNormal.w = 0.0F;


    vPos = CalPos;


    return;
}


// �ؽ�ó�� �Ϻκи� ������ �ϱ� ���� FLOAT4�� �ʿ�.
cbuffer CUTDATA : register(b1)
{
    float4 CutUv;
}

cbuffer TIMEDATA : register(b2)
{
    float4 TimeX;
}



VTX_OUT VS_ANIMESH(VTX_IN _in)
{
    //���⼭ Ÿ�� ������۸� �޾Ƽ� �����ٰ����͵� ��鸮�� �Ҽ� �ִ�.

    VTX_OUT Out = (VTX_OUT) 0.0f;
    
    Out.vUv.x = (_in.vUv.x);
    Out.vUv.y = (_in.vUv.y);
 
    Out.vPos = mul(_in.vPos, WVP);

    Out.vViewPos = mul(_in.vPos, WV);
    Out.vNormal = normalize(mul(-_in.vNormal, WV));
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


    return Out;
}

