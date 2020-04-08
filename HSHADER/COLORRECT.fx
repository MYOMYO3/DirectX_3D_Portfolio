// ���ؽ� ���̴����� ���� ��ǲ
// ����ü
// : POSITION �ø�ƽ�̶�� �ϴµ�.
// �� �����͸� ��� ó���ؾ������� ���� ����.

struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
    float4 vCol : COLOR; // ����
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
	// SV�� ������ �ý��� �����μ� �ѱ��.
	// ���� �ȼ����̴��� �ѱ�� ���� �����Ͷ�� ���� �ȴ�.
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
    float4 vCol : COLOR; // ����
};

// VTX_OUT VTXFunc(VTXCOL_IN) 
// {
// }

// RenderPipe() 
// {
//    PIXFunc(VTXFunc());
// }

// ������ ���������� �ʴ´�.

// ���� ������ �ټ� �ִ� ������ ���⿡ �־� ���´�.
// matrix BASE ���� �������� �� �ִ�.

cbuffer MATDATA : register(b0)
{
    matrix WVP;
}

// static float4x4
static matrix BASE =
{
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

VTX_OUT VS_COLORRECT(VTX_IN _in)
{
	// �ȵ� ���̴�.
    // a += 1;
	// ���̴������� ����ü�� ��κ�
	// ����Ʈ ������ ó���ϴ� ����� ����ϱ� ����
    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vCol = _in.vCol;
    Out.vUv = _in.vUv;

    // �츮�� ����� ����� 
    // Out.vPos = _in.vPos;
    Out.vPos = mul(_in.vPos, WVP);
	// ������ ����
    // Out.vCol.xyz

    return Out;
}

struct PIX_OUT
{
    float4 vOutColor : SV_Target;
	// float4 vOutColor2 : SV_Target1;
	// float4 vOutColor2 : SV_Target2;
};

cbuffer COLOR : register(b1)
{
    float4 COLOR;
    // x y�� �������� ����.
    // z w���� ũ�⸦ �־��� ���̴�.
    // ��������?
    // 0.25 0.25 ������
    // 0.5 0.5 ũ��
}

PIX_OUT PS_COLORRECT(VTX_OUT _in)
{
    matrix Mtrl;

    PIX_OUT Out = (PIX_OUT) 0.0f;
    // Out.vOutColor = Out.vOutColor = float4(0.0f, 0.0f, 1.0f, 1.0f);;
    Out.vOutColor = COLOR;

    //if (_in.vUv.x < 0.1f)
    //{
    //    Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    //if (_in.vUv.x > 0.9f)
    //{
    //    Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    //if (_in.vUv.y < 0.1f)
    //{
    //    Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    //if (_in.vUv.y > 0.9f)
    //{
    //    Out.vOutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    return Out;
}