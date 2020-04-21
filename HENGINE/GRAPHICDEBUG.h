#pragma once
#include "CAMERA.h"
#include "GRAPHICDEVICE.h"
#include "HEFONT.h"

class HSMP;
class Texture;
class HMESH;
class HCB;
class HVTXSHADER;
class HPIXSHADER;
class HBLEND;
class HDSSTATE;
class HRSSTATE;
class GraphicDebug
{
private:
	enum DEBUGDRAWTYPE
	{
		RECT,
		BOX,
		OCTAHEDRON,
		SPHERE,
		CIRCLE,
		TEX,
		MAX,
	};

	class DDI// DrawDebugInfo
	{
	public:
		HPTR<Camera> m_Cam;
		DEBUGDRAWTYPE Type;
		Matrix _W; 
		Matrix _V;
		Matrix _P;
		D3D_PRIMITIVE_TOPOLOGY m_MeshDrawType;
	};

	class DDITEX// DrawDebugInfo
	{
	public:
		DEBUGDRAWTYPE Type;
		Matrix _W;
		Matrix _V;
		Matrix _P;
		HPTR<Texture> m_Tex;
		D3D_PRIMITIVE_TOPOLOGY m_MeshDrawType;
	};


	class TDDI// DrawDebugInfo
	{
	public:
		std::wstring FontName;
		std::wstring Text;
		Vector4 Color;
	};


	//class DDTI// DrawDebugTextInfo
	//{
	//public:
	//	HMAT _W;
	//	HMAT _V;
	//	HMAT _P;
	//};

	// ��� �ϸ� �������?
	// �翬�� �����̿��� �Ǿ� �ϴ°�?
	// ������Ʈ���� �����Ѵٰ� �ϸ�
	// ��� ���;� �ϴ°�?
private:
	// ��������������
	static std::vector<TDDI> m_DebugRenderText;
	static std::vector<DDI> m_DebugRenderList;
	static std::vector<DDITEX> m_DebugRenderTex;
	static int m_DrawCount;
	static int m_TexCount;
	static int m_DrawTextCount;
	static int m_DrawTexCount;

	static Vector4 StartTextPos;

	static bool IsInit;
	static bool IsDebug;
	static HPTR<HMESH> m_HMESH[DEBUGDRAWTYPE::MAX];
	static HPTR<HCB> m_HCB;
	static HPTR<HCB> m_HCBTEX;
	static HPTR<HSMP> m_TEXSMP;
	static HPTR<HVTXSHADER> m_HVTXSHADER;
	static HPTR<HPIXSHADER> m_HPIXSHADER;
	static HPTR<HVTXSHADER> m_HVTXTEXSHADER;
	static HPTR<HPIXSHADER> m_HPIXTEXSHADER;
	static HPTR<HBLEND> m_BLEND;
	static HPTR<HDSSTATE> m_DSSTATE;
	static HPTR<HDSSTATE> m_DSCAMSTATE;
	static HPTR<HDSSTATE> m_TEXDSSTATE;
	static HPTR<HRSSTATE> m_RSSTATE;
	static HPTR<HEFONT> m_HEFONT;

public:
	static void Init();

public:
	static bool IsDebugCheck() {
		return IsDebug;
	}

	static void DebugOn() {
		IsDebug = true;
	}

	static void DebugOff() {
		IsDebug = false;
	}

public:

	// ���� �Ž��� �����.
	static void DebugCamRender();
	static void DebugRender();


public:
	// ī�޶� �־���� �ϴ°��.
	static void DrawBox(HPTR<Camera> _Cam, Matrix _W);
	static void DrawOctahedron(HPTR<Camera> _Cam, Matrix _W);
	static void DrawSphere(HPTR<Camera> _Cam, Matrix _W);
	static void DrawTexture(HPTR<Texture> _Tex, Vector4 _Pos, Vector4 _Size, Vector4 _TexSize = Vector4::ZERO);

	static void DrawRect(HPTR<Camera> _Cam, Matrix _W);
	static void DrawRect(HPTR<Camera> _Cam, Vector4 _Pos, Vector4 _Rot, Vector4 _Scale, Vector4 _Color);
	static void DrawRect(Vector4 _Pos, Vector4 _Rot, Vector4 _Scale, Vector4 _Color);

	static void DrawCircle(HPTR<Camera> _Cam, Matrix _W);
	static void DrawCircle(HPTR<Camera> _Cam, Vector4 _Pos, Vector4 _Rot, Vector4 _Scale, Vector4 _Color);
	static void DrawCircle(Vector4 _Pos, Vector4 _Rot, Vector4 _Scale, Vector4 _Color);

	//static void DrawFont(HPTR<HCAM> _Cam, HVEC4 _Pos, HVEC4 _Rot, HVEC4 _Scale);
	static void DrawFontUI(const wchar_t* _Text, Vector4 COLOR);


public:
	GraphicDebug();
	~GraphicDebug();
};

