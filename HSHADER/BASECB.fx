cbuffer MATDATA : register(b0)
{
    matrix Pos;
    matrix Scale;
    matrix Rot;
    matrix World;
    matrix View;
    matrix Proj;
    matrix WV;
    matrix VP;
    matrix WVP;
    float4 CAMPOS;
    float4 RENDERDATA;
    // LIGHTDATA[1000] // 0 1 2 3
    // int �� ��� ������ �޴���? // 0 1 2 3
    // int �� ������ ��� ���� ������ �޴���?
    // int[1000] // 0 3 4
}

    // �������� �÷� 4��
    // RENDERDATA.x ���ؽ� ���(��� 8 ���� ����) / �ȼ� ���(��)1
    // RENDERDATA.y ���� on1 / ���� off