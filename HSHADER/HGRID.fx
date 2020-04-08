#include "BASECB.fx"

// ���



struct VTX_GRIDOUT
{
    // �ý��� �������� �⺻������ w�� ������ �Ǽ� �����°�.
    // ����ؾ� �Ѵ�.
    // �̳༮�� ������ �������� ��ϵȴ�.
    // 
    float4 vPos : SV_POSITION; //  �ִ� �װ� �ؾ��ϴ� �������Դϴ�.
    float4 vWorld : POSITION; // �ؽ�ó ��ǥ
};


VTX_GRIDOUT VS_GRID(float4 _in : POSITION)
{
    VTX_GRIDOUT Out = (VTX_GRIDOUT) 0.0f;

    Out.vPos = mul(_in, WVP);
    Out.vWorld = mul(_in, World);
    // ������ �Ǳ�� ������ �ȴ�.
    return Out;
}


///// PIX

cbuffer GRIDDATA : register(b1)
{
    // �־��ݽô�.
    float Step; // 100~ 1000 ~ 10000
    float Borader; // ����
    float MinA; // ����
    float MaxA; // ����
    float4 vColor;
    float4 vCamPos; // ķ�� Y
}


struct PIX_OUT
{
    float4 vOutColor : SV_Target;
};

PIX_OUT PS_GRID(VTX_GRIDOUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;

    // ���� ������ �ö󰥶�����.
    //// 5�� ������ 0�϶��� ĥ�ϸ� �ȴ�.
    //// �������� �Ǵ� ����???
    //// �׷���ī��� float ���꿡 ����ȭ �Ǿ��ִ�.
    //if (abs((_in.vWorld.x % Step * 10.0f)) <= 0.5f || abs((_in.vWorld.z % Step * 10.0f)) <= 0.5f)
    //{
    //    // vColor.w = Alpha;
    //    Out.vOutColor = vColor;
        
    //}
    //else 

    float MinX = abs((_in.vWorld.x % Step));
    float MinZ = abs((_in.vWorld.z % Step));

    float MaxX = abs((_in.vWorld.x % (Step * 10.0F)));
    float MaxZ = abs((_in.vWorld.z % (Step * 10.0F)));

    float Len = length(_in.vWorld.xz - vCamPos.xz);

    // 0 ~ 1.0f;
    // 0 ~ 10000.0f
    float LenA = (10000.0f - Len) / 10000.0f;
    
    if (MaxX <= Borader || MaxZ <= Borader)
    {
        // ���ܰ踦 ���� �׸���.
        // vColor.w = Alpha;
        Out.vOutColor = vColor;
        Out.vOutColor.w = 1.0F * LenA;
        return Out;
    }
    
    if (MinX <= Borader || MinZ <= Borader)
    {
        // ���ܰ踦 ���� �׸���.
        // vColor.w = Alpha;
        Out.vOutColor = vColor;
        Out.vOutColor.w = MinA * LenA;
        return Out;
    }
    

    clip(-1);

    return Out;
}