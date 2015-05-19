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

//����������͡�
typedef enum DXImageFilterType
{
	DXIMAGEFILTER_NONE   = 0 ,//�޲���
	DXIMAGEFILTER_POINT  = 1 ,//�����
	DXIMAGEFILTER_LINEAR = 2 ,//���Բ���

};

typedef enum DXVerterType
{
	DX_VT_UNKNOW		= 0 , //Ĭ��
	DX_VT_POINTLIST		= 1 , //���б�
	DX_VT_LINELIST      = 2 , //���б�
	DX_VT_LINESTRIP     = 3 , //�ߴ�
	DX_VT_TRIANGLELIST  = 4 , //�������б�
	DX_VT_TRIANGLESTRIP = 5 , //�����δ�
	DX_VT_TRIANGLEFAN   = 6   //�������� 
};

typedef enum DXFillMode
{
	DX_FILL_POINT		= 1,
	DX_FILL_WIREFRAME   = 2,
	DX_FILL_SOLD        = 3,
};

typedef enum DXVertexState
{
	DX_VS_NEEDINIT     = 0 ,//û�г�ʼ��
	DX_VS_INVALID      = 1 ,//ʧ��
	DX_VS_OK           = 2  //����
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







#endif
