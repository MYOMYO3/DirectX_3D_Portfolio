
struct LIGHTRESULT
{
    // ����ø�ƽ�� �����Ѵ�.
    float4 vDiff; // Ȯ�걤 ���ݻ籤
    float4 vSpec; // ���ݻ籤
    float4 vAmbi; // �ֺ��� ȯ�汤
};

struct LIGHTDATA
{
    // ���� ���δ� ǥ���Ϸ��� ��������Ƿ�
    float4 Color; // ���� ����
    float4 LightPos; // ���� ��ġ (�𷺼� ����Ʈ�� �ǹ̰� ����)
    float4 LightDir; // ���� �ٶ󺸴� ����(����Ʈ ����Ʈ�� �׶��׶����� ���� �𷺼��̸� �׻� �Ȱ���)
    // L�� �ݴ�
    float4 LightDirInv; // -LightDir (������Ʈ ������Ŀ��� L)
    float Range; // ����
    float DiffPower; // ���� ���� // ���������� �������
    float SpecPower; // ���� ���� // ���� �ٶ󺸴� ����� ���� 
    
    int Type;
    int SpecPow;
    int temp1;
    int temp2;
    int temp3;
};

struct ALLLIGHTDATA
{
    LIGHTDATA ArrLight[512];
    int4 LightSettingData;
};


// 10�� �̻��� �ý��� ������
cbuffer LIGHTSETTING : register(b10)
{
    ALLLIGHTDATA LightData;
};

// ������۴� ���߿� �����.

// in out�� 
// ����� ������ ���� �˼� �ִ�.
LIGHTRESULT CalLightVTX(
float4 _vViewPos,  // ���� ���� �ݻ��ϴ� ���� �ϳ��ε� ���� �����
float4 _vViewNormal,  // ���� ������ �ٶ󺸰� �־�
float4 _vCamPos,  // �װ� ���⼭ ���� �־�
LIGHTDATA _Light // �װ� �� ���� ���߰� �־�
)
{
    LIGHTRESULT RLR = (LIGHTRESULT) 0.0F;

    float RangeFactor = 0.0f;
    float Dis = 0.0f;

    if (0 >= _Light.Range)
    {
        _Light.Range = 1.0f;
    }
        

    //_vViewNormal.xyz = normalize(_vViewNormal.xyz);
    //_Light.LightDirInv.xyz = normalize(_Light.LightDirInv.xyz);

    // 0 DIR

    // ���ؽ��϶��� 
    switch (_Light.Type)
    {
    case 1:
       _Light.LightDirInv = normalize(_Light.LightPos - _vViewPos);

       Dis = abs(length(_vViewPos - _Light.LightPos));
       if (Dis >= _Light.Range)
       {
            // ���� �ٱ����� ������.
            RangeFactor = 0.0f;
       }
       else
       {
            // ���� �ȿ� ���Դ�.
       RangeFactor = 1.0f - (_Light.Range / Dis);
       }

       RangeFactor = abs(RangeFactor);
       if (1 <= RangeFactor)
       {
           RangeFactor = 1.0f;
       }
       break;
    }

    RLR.vDiff = max(0, dot(_vViewNormal.xyz, _Light.LightDirInv.xyz)) * _Light.DiffPower * RangeFactor;

    // ���ݻ� ���� ����Ϸ���
    // �翬�� ���� ��� ���� �ִ���
    float3 vR; // �ݻ����
    float3 vE; // �� ���� ����

    // ������ �����Ѵ�.
    // �̰͵�
    vR = normalize(2.0f * dot(_Light.LightDirInv, _vViewNormal) * _vViewNormal - _Light.LightDirInv);
    // ���� �ٶ󺸴� ����
    vE = normalize(_vCamPos.xyz - _vViewPos.xyz);

    // ���� �����ϰ� ������� �ϴ°�.
    // ���ݻ籤�� ��ħ�� �� �������� �ϴ°�. ���� ������ �κ��� �ε巯�� ����.
    // ����ŧ���� ���ߴ�.
    // �� ���� ������ �κ��� �پ��� �ȴ�.
    RLR.vSpec = dot(vE.xyz, vR.xyz) * _Light.SpecPower * RangeFactor;

    if (0 >= RLR.vSpec.x)
    {
        RLR.vSpec.xyz = 0.0f;
    }
    else
    {
        RLR.vSpec.xyz = pow(RLR.vSpec.x, _Light.SpecPow);
    }

    RLR.vAmbi = float4(0.25f, 0.25f, 0.25f, 0.0f);


    RLR.vDiff.w = RLR.vAmbi.w = RLR.vSpec.w = 1.0f;
    
    //RLR.vDiff = ceil(RLR.vDiff * 0.5f) / 0.5f;

    return RLR;
}

LIGHTRESULT CalLightPIX(
float4 _vViewPos, // ���� ���� �ݻ��ϴ� ���� �ϳ��ε� ���� �����
float4 _vViewNormal, // ���� ������ �ٶ󺸰� �־�
float4 _vCamPos, // �װ� ���⼭ ���� �־�
LIGHTDATA _Light // �װ� �� ���� ���߰� �־�
)
{
    LIGHTRESULT RLR = (LIGHTRESULT) 0.0F;

    float RangeFactor = 1.0f;
    float Dis = 0.0f;

    if (0 >= _Light.Range)
    {
        _Light.Range = 1.0f;
    }
        

    //_vViewNormal.xyz = normalize(_vViewNormal.xyz);
    //_Light.LightDirInv.xyz = normalize(_Light.LightDirInv.xyz);

    // 0 DIR

    // ���ؽ��϶��� 
    switch (_Light.Type)
    {
        case 1:
            _Light.LightDirInv.xyz = normalize(_Light.LightPos.xyz - _vViewPos.xyz);

            Dis = abs(length(_vViewPos.xyz - _Light.LightPos.xyz));
            if (Dis >= _Light.Range)
            {
            // ���� �ٱ����� ������.
                RangeFactor = 0.0f;
            }
            else
            {
            // ���� �ȿ� ���Դ�.
                RangeFactor = 1.0f - (_Light.Range / Dis);
            }

            RangeFactor = abs(RangeFactor);
            if (1 <= RangeFactor)
            {
                RangeFactor = 1.0f;
            }
            break;
    }

    RLR.vDiff = max(0, dot(_vViewNormal.xyz, _Light.LightDirInv.xyz)) * _Light.DiffPower * RangeFactor;

    // ���ݻ� ���� ����Ϸ���
    // �翬�� ���� ��� ���� �ִ���
    float3 vR; // �ݻ����
    float3 vE; // �� ���� ����

    // ������ �����Ѵ�.
    // �̰͵�
    vR = normalize(2.0f * _vViewNormal.xyz * dot(_Light.LightDirInv.xyz, _vViewNormal.xyz) - _Light.LightDirInv.xyz);
    // ���� �ٶ󺸴� ����
    vE = normalize(_vCamPos.xyz - _vViewPos.xyz);

    // ���� �����ϰ� ������� �ϴ°�.
    // ���ݻ籤�� ��ħ�� �� �������� �ϴ°�. ���� ������ �κ��� �ε巯�� ����.
    // ����ŧ���� ���ߴ�.
    // �� ���� ������ �κ��� �پ��� �ȴ�.
    RLR.vSpec = dot(vE.xyz, vR.xyz) * _Light.SpecPower * RangeFactor;

    if (0 >= RLR.vSpec.x)
    {
        RLR.vSpec.xyz = 0.0f;
    }
    else
    {
        RLR.vSpec.xyz = pow(RLR.vSpec.x, _Light.SpecPow);
    }

    
    
    
    RLR.vAmbi = float4(0.25f, 0.25f, 0.25f, 0.0f);
    
    
    //�ܰ���
    //float checkCos = dot(_vViewNormal.xyz, vE.xyz);
    //float degree = acos(checkCos) * (180.0f / 3.141592f);
    //if (85.0f < degree && 95.0f > degree)
    //{
    //    RLR.vDiff.xyz = float3(0.0f, 0.0f, 0.0f);
    //}
    
    //if (checkCos < 0.1f && checkCos > 0.0f)
    //{
    //    RLR.vDiff.xyz = float3(0.0f, 0.0f, 0.0f);
    //}
    
    RLR.vDiff.w = RLR.vAmbi.w = RLR.vSpec.w = 1.0f;
        
    //RLR.vDiff.xyz = ceil(RLR.vDiff.xyz * 2.0f) / 2.0f;
    
    return RLR;
}