////////// VTX
struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
    float4 vCol : COLOR; // ����

    // ��ü���� ������ �ȴ�.
    // �Ʒ��ʿ� �ִ� �༮�� ��ü�� �ϳ��� �־��ְ� �ȴ�.
    // ���ؽ� ������ ũ�Ⱑ ����Ǵ� ���� �ƴϴ�.
    // �ϳ��� �־��ְ� �˴ϴ�.
    // row_major �� ǥ���� ���� �߰� �޸� ������ ���ְ� �ѹ��� ��Ʈ�� ������ �ְ� �ȴ�.
    // ��ü�� ������ŭ �ν��Ͻ̿� ���ۿ� ��ܼ� ���´�.
    row_major matrix MATWVP : WVP;
    float4 vSpriteUv : SPRITEUV;
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // �ؽ�ó ��ǥ
    float4 vCol : COLOR; // ����
};

//// ���
//cbuffer MATDATA : register(b0)
//{
//    matrix WVP;
//}
//// �ؽ�ó�� �Ϻκи� ������ �ϱ� ���� FLOAT4�� �ʿ�.
//cbuffer CUTDATA : register(b1)
//{
//    float4 CutUv;
//    // x y�� �������� ����.
//    // z w���� ũ�⸦ �־��� ���̴�.
//    // ��������?
//    // 0.25 0.25 ������
//    // 0.5 0.5 ũ��
//}


VTX_OUT VS_TEX(VTX_IN _in)
{
    // �Ϻκи� ������ �Ѵٴ� �̾߱��
    // 

    // 0 0 | 1 0
    // 0 1 | 1 1

    //   0.25 0.25   | 0.75    0.25 
    //   0.25 0.75F   | 0.75  0.75

    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vCol = _in.vCol;
    

    //           1           0.5        0.25
    Out.vUv.x = (_in.vUv.x * _in.vSpriteUv.z) + _in.vSpriteUv.x;
    //           0           0.5        0.25
    Out.vUv.y = (_in.vUv.y * _in.vSpriteUv.w) + _in.vSpriteUv.y;

    // Out.vUv = _in.vUv;

    Out.vPos = mul(_in.vPos, _in.MATWVP);

    return Out;
}


///// PIX


