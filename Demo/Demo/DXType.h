#ifndef  __DXTYPE_H__
#define  __DXTYPE_H__
#include <windows.h>

typedef enum DXColorFormat
{
	DX_CF_UNKNOW = 0,
	DX_CF_R8G8B8 = 20,
	DX_CF_A8R8G8B8 = 21,
	DX_CF_X8R8G8B8 = 22,
	DX_CF_R5G6B5 = 23,
	DX_CF_X1R5G5B5 = 24,
	DX_CF_A1R5G5B5 = 25,
	DX_CF_A4R4G4B4 = 26,
	DX_CF_R3G3B2 = 27,
	DX_CF_A8 = 28,
	DX_CF_A8R3G3B2 = 29,
	DX_CF_X4R4G4B4 = 30,
	DX_CF_DXT1 = ('D') | ('X' << 8) | ('T' << 16) | ('1'<<24),
	DX_CF_DXT5 = ('D') | ('X' << 8) | ('T' << 16) | ('5' << 24)
};

const INT64 DXT1NilData = 0xFFFFFFFFFFFF0000;
const INT64 DXT5NilData = 0x24924924924900FF;
const INT64 DXT5Opaque = 0x000000000000FFFF;

typedef enum DXBlendMode
{
	DX_BLEND_NONE			= 0 ,
	DX_BLEND_TRANSPARENT	= 1 ,
	DX_BLEND_SHADOW         = 2 ,
	DX_BLEND_DODGE          = 3 ,
	DX_BLEND_LINERDODGE	    = 4 ,
	DX_BLEND_SCREEN         = 5 ,
	DX_BLEND_LIGHTEN        = 6 ,
	DX_BLEND_OVERLAY        = 7 ,
	DX_BLEND_LIGHTSCREEN    = 8 ,
	DX_BLEND_MULTI          = 9 ,
	DX_BLEND_ALPHA          = 10,
	DX_BLEND_TEST1          = 11,
};

//纹理采样类型。
typedef enum DXImageFilterType
{
	DXIMAGEFILTER_NONE   = 0 ,//无采样
	DXIMAGEFILTER_POINT  = 1 ,//点采样
	DXIMAGEFILTER_LINEAR = 2 ,//线性采样

};

typedef enum DXVerterType
{
	DX_VT_UNKNOW		= 0 , //默认
	DX_VT_POINTLIST		= 1 , //点列表
	DX_VT_LINELIST      = 2 , //线列表
	DX_VT_LINESTRIP     = 3 , //线带
	DX_VT_TRIANGLELIST  = 4 , //三角形列表
	DX_VT_TRIANGLESTRIP = 5 , //三角形带
	DX_VT_TRIANGLEFAN   = 6   //三角扇形 
};

typedef enum DXFillMode
{
	DX_FILL_POINT		= 1,
	DX_FILL_WIREFRAME   = 2,
	DX_FILL_SOLD        = 3,
};

typedef enum DXVertexState
{
	DX_VS_NEEDINIT     = 0 ,//没有初始化
	DX_VS_INVALID      = 1 ,//失败
	DX_VS_OK           = 2  //可用
};

typedef enum DXImageAddress
{
	DX_IADDR_WRAP       = 1,
	DX_IADDR_MIRROR     = 2,
	DX_IADDR_CLAMP      = 3,
	DX_IADDR_BORDER     = 4,
	DX_IADDR_MIRRORONCE = 5
};

typedef struct _DXViewPortTag
{
	DWORD X;
	DWORD Y;
	DWORD Width;
	DWORD Height;
	float MinZ;
	float MaxZ;
}DXViewPort ,* LPDXVIEWPORT;



typedef struct tagDXDisplayMode
{
	UINT Width;
	UINT Height;
	UINT RefreshRate;
	DXColorFormat Format;
} DXDisplayMode ,*LPDXDisplayMode;

typedef enum DXGUIVertexType
{
	DX_GUIVT_NONE = 0,
	DX_GUIVT_NORMAL = 1,
	DX_GUIVT_STRETCH =2,
	DX_GUIVT_SQUARES = 3,
	DX_GUIVT_TILE = 4,
	DX_GUIVT_CRICLE = 5,

};

inline int GetPixelSize(DWORD dwFormat)
{
	switch (dwFormat)
	{
	case DX_CF_R8G8B8:return 3;
	case DX_CF_A8R8G8B8 :
	case DX_CF_X8R8G8B8:return 4;
	case DX_CF_R5G6B5 :
	case DX_CF_X1R5G5B5:
	case DX_CF_A1R5G5B5:
	case DX_CF_A4R4G4B4: return 2;
	case DX_CF_A8: return 1;
	case DX_CF_DXT1:return 8;
	case DX_CF_DXT5: return 16;
	default: return 0;

	}
}

inline int GetLinePitch(DWORD dwFormat, int Width)
{
	int PixelSize = GetPixelSize(dwFormat);
	switch (dwFormat)
	{
	case DX_CF_R8G8B8 :
	case DX_CF_A8R8G8B8:
	case DX_CF_X8R8G8B8:
	case DX_CF_R5G6B5:
	case DX_CF_X1R5G5B5:
	case DX_CF_A1R5G5B5:
	case DX_CF_A4R4G4B4:
	case DX_CF_A8:
		return (Width*PixelSize + 3) & 0xFFFFFFFC;
	case DX_CF_DXT1:
	case DX_CF_DXT5:
		return Width * PixelSize / 4;

	default:
		return 0 ;
	}
}

inline int GetLinePitchNoAlign(DWORD dwFormat, int Width)
{
	int PixelSize = GetPixelSize(dwFormat);
	return Width * PixelSize;
}

typedef struct tagVector4
{
	float x, y, z, w;
}Vector4 ,*LPVECTOR4;

class POINT_F
{
public:
	Point_F() :x(0.0f), y(0.0f){}
	Point_F(float fx, float fy) :x(fx), y(fy){}
	Point_F(const POINT &pf) :x((float)pf.x), y((float)pf.y){}
	void Offset(float fx, float fy){ x += fx; y += fy; }

	BOOL operator == (const Point_F& pt) const { return x == pt.x && y == pt.y; }
	BOOL operator != (const Point_F& pt) const { return x != pt.x || y != pt.y; }
public :
	float x, y;
};

typedef POINT_F * LPPOINT_F;





#endif
