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

VTX_OUT VS_CAM(VTX_IN _in)
{

    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vUv = _in.vUv;
    Out.vPos = _in.vPos;

    return Out;
}


///// PIX



struct PIX_OUT
{
    // ����ۿ� ������ �Ұ��̴�.
    float4 vOutColor : SV_Target;
};

Texture2D Tex : register(t0); 
SamplerState Smp : register(s0); 

PIX_OUT PS_CAM(VTX_OUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;
    Out.vOutColor = Tex.Sample(Smp, _in.vUv); // �̺κ�
    // Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return Out;
}