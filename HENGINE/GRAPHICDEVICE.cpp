#include "GRAPHICDEVICE.h"
#include <JWINDOW.h>

// mgr static

GraphicDevice* GraphicDevice::m_MainDevice = nullptr;
ID3D11Device* GraphicDevice::m_pMainDevice = nullptr; // �׷���ī���� ��� �ڿ��� ���� ������ ���. (o)
ID3D11DeviceContext* GraphicDevice::m_pMainContext = nullptr; // �׷���ī���� ��� ���꿡 ���� ������ ����Ѵ�.(����) (o)
bool GraphicDevice::m_PathInit = false;

///////////////////////////// member

void GraphicDevice::MainReset() 
{
	ID3D11RenderTargetView* ArrView[16] = { nullptr };

	m_pMainContext->VSSetShader(nullptr, nullptr, 0);
	m_pMainContext->HSSetShader(nullptr, nullptr, 0);
	m_pMainContext->GSSetShader(nullptr, nullptr, 0);
	m_pMainContext->CSSetShader(nullptr, nullptr, 0);
	m_pMainContext->DSSetShader(nullptr, nullptr, 0);
	m_pMainContext->PSSetShader(nullptr, nullptr, 0);
	m_pMainContext->OMSetDepthStencilState(nullptr, 0);

	ID3D11ShaderResourceView* RTV = nullptr;

	for (UINT i = 0; i < 16; i++)
	{
		GraphicDevice::MainContext()->VSSetShaderResources(i, 1, &RTV);
		GraphicDevice::MainContext()->HSSetShaderResources(i, 1, &RTV);
		GraphicDevice::MainContext()->DSSetShaderResources(i, 1, &RTV);
		GraphicDevice::MainContext()->GSSetShaderResources(i, 1, &RTV);
		GraphicDevice::MainContext()->PSSetShaderResources(i, 1, &RTV);
	}


	m_pMainContext->OMSetRenderTargets(8, &ArrView[0], nullptr);
}

GraphicDevice::GraphicDevice() : m_pDevice(nullptr), m_pContext(nullptr), m_pSwapChain(nullptr), m_pDepthStencilView(nullptr), m_pDepthStencilTex(nullptr), m_pBackBufferView(nullptr), m_BaseInit(false)
{
}


GraphicDevice::~GraphicDevice()
{
	m_TargetWindow = nullptr;
	
	
	if (nullptr != m_pDepthStencilTex) { m_pDepthStencilTex->Release(); m_pDepthStencilTex = nullptr;};
	if (nullptr != m_pDepthStencilView) { m_pDepthStencilView->Release(); m_pDepthStencilView = nullptr; };
	if (nullptr != m_pBackBufferView) { m_pBackBufferView->Release(); m_pBackBufferView = nullptr; };
	if (nullptr != m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr;};
	if (nullptr != m_pContext) { m_pContext->Release(); m_pContext = nullptr;};
	if (nullptr != m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr;};
}


void GraphicDevice::Init(Window* _TargetWindow)
{
	// ����̽��� ������� �ϴµ�.
	int iFlag = 0;
#ifdef _DEBUG
	// ����̽��� ��尡 ����� ������� ������ ������� ��������� �Ѵ�.
	// �׸��� ����׸��� ������.
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif 
	m_TargetWindow = _TargetWindow;

	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	m_BackColor = { 4.0f / 255.0f, 104.0f / 255.0f, 216.0f / 255.0f, 1.0f };

	//m_BackColor = { 129.0f / 255.0f,193.0f / 255.0f, 71.0f / 255.0f,1.0f};
	//m_BackColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	// 
	// 
	HRESULT Check;

	Check = D3D11CreateDevice
	(
		nullptr,  // 1 ����̽� ���鶧 �ΰ������� �ֳ���? nullptr 
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, // 2 �׷���ī�尡 � ��ݿ� ���� ���ΰ���? -> �׳� �ϵ���� 
		nullptr, // ����
		iFlag, // ����׳� �������
		nullptr, // ����
		0, // ����
		D3D11_SDK_VERSION, // ���� �����쿡 ��ġ�� SDK������ ���̳�?
		&m_pDevice, // ����̽� ������
		&eLv, // ����
		&m_pContext // ���ؽ�Ʈ ���´�.
	);

	if (S_OK != Check) { BOOM; return; }

	// �̰� ������ ������� ó���ϴ� ���̱� ������ ���� ������ �Ǵ� ������
	// DXGI_FORMAT_R8G8B8A8_UNORM

	// 4��
	m_MC = 4;
	m_MQ = 0;

	D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
	Check = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MC, &m_MQ);
	if (S_OK != Check) 
	{ 
		m_MC = 1;
		m_MQ = 0;
		Check = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MC, &m_MQ);
		return; 
	}

	m_MC = 1;
	m_MQ = 0;

	// m_ML = 4;

	// ����ü���� �����.
	// ���� ������ 
	if (false == CreateSwapChain())
	{
		BOOM;
	}

	if (false == CreateDepthStencilView())
	{
		BOOM;
	}

	if (false == CreateViewPort())
	{
		BOOM;
	}

	if (nullptr == m_MainDevice)
	{
		m_MainDevice = this;
		m_pMainDevice = this->GetDevice();
		m_pMainContext = this->GetContext();
	}

	BaseResInit();
}

bool GraphicDevice::CreateSwapChain() 
{
	// �̳༮�� nullptr�� �ƴϰ� ����� ��.
	// m_pSwapChain

	// DESC�ܾ� ������ ��ũ���� ����
	DXGI_SWAP_CHAIN_DESC SWINFO;

	SWINFO.BufferDesc.Width = m_TargetWindow->Size().UIX();
	SWINFO.BufferDesc.Height = m_TargetWindow->Size().UIY();

	// ������� �ִ��ػ󵵵� ���ü� �ִ�.
	// ������� �츣���� ���ü� ���� ���̴�.

	// ���� �и�
	SWINFO.BufferDesc.RefreshRate.Numerator = 60;
	// 1 / 60�� �츣�� ǥ��.
	SWINFO.BufferDesc.RefreshRate.Denominator = 1;

	SWINFO.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SWINFO.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SWINFO.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ������� �뵵
	// ȭ�鿡 ����ϴ� �뵵�� ���ڴ�. 
	SWINFO.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// ����� ����
	// ��Ƽ���ø� ����Ƽ ī��Ʈ
	// 0 �˾Ƽ� 
	// SWINFO.SampleDesc.Count = m_ML;
	// MSAA 
	
	SWINFO.SampleDesc.Count = m_MC;
	SWINFO.SampleDesc.Quality = m_MQ;

	// ������۸��� ����ѵ�.
	// 2���� �̹����� 
	SWINFO.OutputWindow = m_TargetWindow->WH();
	// ���۰��� 1 ~ 
	SWINFO.BufferCount = 2;


	// 0 -> ȭ�鿡 ������ �ִ�.
	// 1 -> �׷����� �ִ�. -> �� �׷�����.
	// ���� ������ �� �����.
	// 2���� �׸��� �׸��� �����Ҷ� ����� ���α׸���. -> �� �׷����� -> ������ 
	SWINFO.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD

	// Ǯ��ũ�� ����ΰ���?????
	SWINFO.Windowed = !m_TargetWindow->IsFull();
	SWINFO.Flags = 0;
	DXGI_SWAP_CHAIN_FLAG_HW_PROTECTED;
    // ������ ���õȰŰ�
	// ���� ����� ���� �ؾ��Ѵ�.
	// �׷��� ���̷�Ʈ�� ����ü���� ����� ���ؼ� �� ������ ������ �Ťž� �Ѵ�.

	// ���̷�Ʈ 11���ʹ� ���̵��� �������ٴ� ������
	// com��ü����̶�� ���� ����ؼ� ó���ϰ� ���������.
	// �̰� COM��ü ����̶�� ����
	// m_pDevice->createsw
	// �̳༮���� �ܼ��� ���� �Ҵ��ϰ� �����Ǵ� �༮�� �ƴ϶�� �̾߱�.
	// ���ҿ��� ���� �޸� ���� ��Ŀ� ���ؼ� �����ǰ�
	// ���ҿ����� ����� HWND
	// �׷���ī�忡 ������ ��û�ϰų� �޸𸮸� �Ҵ��ϴ� ��Ŀ� ���� �������̽���
	// new Texture();
	// �̳༮���� ���ؼ��� �׷���ī�忡 ��û�ض�.
	// ID3D11Device* m_pDevice

	IDXGIDevice* pIdxDevice = nullptr;
	IDXGIAdapter* pIdxAdapter = nullptr;
	IDXGIFactory* pIdxFactory = nullptr;
	// �̰� ������ �ʰ��� 
	// ������� �س���.
	// �׷��� ī����� ���
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pIdxDevice);
	if (nullptr == pIdxDevice)
	{
		BOOM;
		return false;
	}
	 
	// GUID�� ���ؼ� ������ ����� ����.
	pIdxDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pIdxAdapter);
	if (nullptr == pIdxAdapter)
	{
		BOOM;
		return false;
	}

	pIdxAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pIdxFactory);
	if (nullptr == pIdxFactory)
	{
		BOOM;
		return false;
	}

	if (S_OK != pIdxFactory->CreateSwapChain(m_pDevice, &SWINFO, &m_pSwapChain))
	{
		BOOM;
		return false;
	}
	

	pIdxFactory->Release();
	pIdxAdapter->Release();
	pIdxDevice->Release();

	return true;

}

bool GraphicDevice::CreateDepthStencilView() 
{
	ID3D11Texture2D* pBackBufferTex = nullptr;

	// �ڵ����� �ؽ�ó �ϳ��� ��������ֱ� ������ �̰� ���ü� �ִ�.
	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTex))
	{
		BOOM;
		return false;
	}

	// ���̷�Ʈ�� View��� ǥ���� ��ü����
	// �ؽ�ó�� ���� ������ ó�������� ����Ҽ� �ִ� �������̽��̴�.
	// �������� ���̿� �����ϰų� ��ɳ����� �ִ� ���� ��ü�� �������
	// ���߿� ����ۿ� ��� ����� ������ �̳༮�� ���ؼ� �����ٲ�.
	if (S_OK != m_pDevice->CreateRenderTargetView(pBackBufferTex, 0, &m_pBackBufferView))
	{
		BOOM;
		return false;
	}

	if (nullptr != pBackBufferTex)
	{
		pBackBufferTex->Release();
		pBackBufferTex = nullptr;
	}

	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.ArraySize = 1;
	tDesc.Width = m_TargetWindow->Size().UIX();
	tDesc.Height = m_TargetWindow->Size().UIY();

	// �ſ� ����ų� 
	// ���� ��Ÿ�� �ϰ� �ʹٰų�
	// ȭ���� ��� �ȼ��� ���ٽ� ���� 4�� �༮�� ���Ҳ���.
	// 3����Ʈ�� ���̰����� ����ϰ� �ڿ� 8��ư�� 0~255�� ���ٽ� ������ ����.
	//             R8G8B8A8
	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDesc.SampleDesc.Count = m_MC;
	tDesc.SampleDesc.Quality = m_MQ;
	tDesc.MipLevels = 1; // �� �����ؾ� �Ѵ�.
	// �޸� ���� �ɼ�
	// D3D11_USAGE_DEFAULT�� �׷���ī�忡�� �����ض�.
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	// �ؽ�ó�� �뵵�� ����?
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_pDevice->CreateTexture2D(&tDesc, 0, &m_pDepthStencilTex);

	if (nullptr == m_pDepthStencilTex)
	{
		BOOM;
		return false;
	}

	m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, 0, &m_pDepthStencilView);

	if (nullptr == m_pDepthStencilView)
	{
		BOOM;
		return false;
	}

	// D3D11_DEPTH_STENCIL_DESC TD;
	// m_pContext->OMSetDepthStencilState(TD, 1);

	return true;
}

bool GraphicDevice::CreateViewPort() 
{
	Template::MemZero(m_ViewPortInfo);

	m_ViewPortInfo.TopLeftX = 0;
	m_ViewPortInfo.TopLeftY = 0;

	m_ViewPortInfo.Width = m_TargetWindow->Size().x;
	m_ViewPortInfo.Height = m_TargetWindow->Size().y;
	m_ViewPortInfo.MinDepth = 0.0f;
	m_ViewPortInfo.MaxDepth = 1.0f;


	return true;
}

// ���������ϱ� ������ ���´�.
void GraphicDevice::ClearTarget() 
{
	if (nullptr != m_pContext)
	{
		m_pContext->ClearRenderTargetView(m_pBackBufferView, m_BackColor.Arr);
		m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

// ���±��� �� �׸��� ȭ�鿡 ����ض�.
void GraphicDevice::Present() 
{
	if (nullptr != m_pSwapChain)
	{
		m_pSwapChain->Present(0, 0);
	}
	
}

void GraphicDevice::DefRenderTargetSetting()
{
	if (nullptr != m_pContext && nullptr != m_pDepthStencilView)
	{
		// ���� ���۴� �׸��� ��ü�� ������ ��� ����� ����.
		// ���� �� ����Ÿ�ٰ� �� ���̹��۸� ����ؼ�
		// �׷����� ����� ����� ������ �����ϰڽ��ϴ�.
		m_pContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);
	}
}

void GraphicDevice::DefViewPortSetting() 
{
	if (nullptr != m_pContext)
	{
		m_pContext->RSSetViewports(1, &m_ViewPortInfo);
	}
}