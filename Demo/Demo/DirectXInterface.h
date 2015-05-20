#ifndef __DIRECTX_INTERFACE_H_
#define __DIRECTX_INTERFACE_H_
#include "Common.h"
#include "DXType.h"
#include <windows.h>

class CDXResourceManager;
#define DO_OPTION_OUTOFMEMORY 1  //创建贴图失败时触发

class CDXDeviceCallBack
{
public:
	virtual void OnInitialize() = 0;
	virtual void OnInvalidate() = 0;
	virtual void OnRestore() = 0;
	virtual void OnUpdate(DWORD CurTime, BOOL * bIsDone) = 0;
	virtual void OnRender() = 0;
	virtual void OnFinalize() = 0;
	virtual void DoOption(DWORD dwCode) = 0;
};

typedef CDXDeviceCallBack * LPDeviceCallBack;

class CDXDevice
{
	virtual ~CDXDevice(){}
	virtual HRESULT CreateDevice(HWND hWnd, BOOL bWindowed, INT nWidth, INT nHeight, BOOL bVSync) = 0;
	virtual HRESULT Resize(HWND hWnd, BOOL bWindowed, INT nWidth, INT nHeight) = 0;
	virtual HRESULT Resize(HWND hWnd, INT nWidth, int nHeight) = 0;
	virtual HRESULT SetDeviceCallBack(LPDeviceCallBack pDeviceCallBack) = 0;
	virtual HRESULT CheckDeviceState() = 0;
	virtual void SetBlendOp(DWORD dwAlphaBlend, DWORD dwAlphaTest, DWORD dwBlendOp, DWORD dwSrcBlend,
							DWORD dwDestBlend, DWORD dwAlphaOp = 4, DWORD dwColorOp = 4) = 0;
	
	virtual BOOL CheckRenderState(DWORD dwState, DWORD dwValue) const = 0;
	virtual HRESULT SetRenderState(DWORD dwState, DWORD dwValue) = 0;
	virtual HRESULT GetRenderState(DWORD dwState, DWORD* pdwValue) const = 0;

	virtual BOOL CheckTextureStageState(DWORD dwStage, DWORD dwType, DWORD dwValue) const = 0;
	virtual HRESULT GetTextureStageState(DWORD dwStage, DWORD dwType, DWORD *pdwValue) const = 0;
	virtual HRESULT SetTextureStageState(DWORD dwStage, DWORD dwType, DWORD dwValue) = 0;

	virtual HRESULT SetSamplerState(DWORD dwSampler, DWORD dwType, DWORD dwValue) = 0;
	virtual HRESULT GetSamplerState(DWORD dwSampler, DWORD dwType, DWORD* pdwValue) const = 0;

	virtual HRESULT SetTextureFilter(DXImageFilterType Filter) = 0;
	virtual HRESULT GetTextureFilter(DXImageFilterType* pFilter) const = 0;

	virtual HRESULT SetTextureAddress(DXImageAddress Address) = 0;
	virtual HRESULT GetTextureAddress(DXImageAddress* pAddress) const = 0;

	virtual HRESULT SetVSync(BOOL bSync)  = 0;
	virtual BOOL GetVSync() const = 0;

	virtual BOOL GetWindowed() const = 0;
	virtual BOOL PixelShaderSupport(DWORD dwMajor, DWORD dwMinor) = 0;

	virtual HRESULT SetViewPort(const LPDXVIEWPORT ViewPort, BOOL bSetViewMatrix = FALSE) = 0;
	virtual HRESULT GetViewPort(LPDXVIEWPORT Viewport) const = 0;

	virtual BOOL SetViewPort(const RECT * lprc, BOOL bSetViewMatrix = FALSE) = 0;
	virtual BOOL GetViewPort(RECT * lprc) = 0;

	virtual void SetViewPortState(BOOL bEnabled) = 0;
	virtual void GetViewPortState() = 0;
	virtual BOOL SaveViewport() = 0;
	virtual BOOL RestoreViewport() = 0;

	virtual HRESULT GetBackTextureSize(INT* pWidth, INT* pHeight) const = 0;
	virtual HRESULT GetBackTextureSize(SIZE &szSize) const = 0;
	
	virtual DWORD SetDiffuse(DWORD dwDiffuse) = 0;
	virtual HRESULT getDiffuse(DWORD * pdwDiffuse) const = 0;

	virtual HRESULT SetBlendMode(DWORD dwMode)  = 0;
	virtual HRESULT GetBlendMode(DWORD *dwMode) const = 0;

	virtual HRESULT SetFillMode(DXFillMode dwFillMode) = 0;
	virtual HRESULT GetFillMode(DXFillMode* dwFillMode) = 0;

	virtual HRESULT SetFVF(DWORD dwFVF) = 0;
	virtual HRESULT GetFVF(DWORD* pdwFVF) const = 0;

	virtual HRESULT SetDefaultRenderStates() = 0;
	virtual HRESULT Clear() = 0;
	virtual HRESULT BeginDraw() = 0;
	virtual HRESULT EndDraw() = 0;
	virtual HRESULT Present() = 0;
	virtual HRESULT EvictManagedResource() = 0;

	virtual void RegisterResourceManager(CDXResourceManager * pResMgr) = 0;
	virtual void UnRegisterResourceManager(CDXResourceManager * pResMgr) = 0;
	virtual BOOL GetResourceManager(INT index, CDXResourceManger **pResMgr) = 0;

	virtual void Update(DWORD CurTime, BOOL *bIsDone) = 0;
	virtual BOOL Init() = 0;
	virtual void Final() = 0;
	virtual void SetNextRender(BOOL bValue) = 0;

	virtual INT GetDisplayModeCount() const = 0;
	virtual BOOL GetDisplayMode(INT index, DXDisplayMode * pMode) const = 0;

	virtual BOOL GetPow2Conditional() const = 0;
	virtual BOOL GetMaxTextureSize(SIZE &szSize) const = 0;
	virtual BOOL CheckStatus() const = 0;
	virtual DWORD GetVideoMemory() const = 0;
	virtual DWORD GetVideoProcess(PWCHAR pValue, INT nLen) = 0;

	virtual void DoOption(DWORD dwCode) = 0;

	virtual void SetMatrix2D() = 0;
	virtual void SetMatrix3D() = 0;

	virtual BOOL GetIsSystemMemoryMode() const = 0;
	virtual void SetIsSystemMemoryMode(BOOL bValue) = 0;

	virtual void Snapshot(LPCTSTR pFileName, INT nLen) = 0;
};

typedef CDXDevice * LPDXDevice;

class CDXResourceNode;
class CDXResource
{
public:
	CDXResource(CDXResourceNode * Owner, DWORD dwKey) :m_pOwner(Owner), m_dwKey(dwKey){}
protected:
	virtual void Update(){}
	CDXResourceNode const * Owner() const { return m_pOwner; }
	DWORD GetKey() const { return m_dwKey; }
private:
	DWORD m_dwKey;
	CDXResourceNode * m_pOwner;

};
typedef CDXResource * LPDXResource;

class CDXDib :public CDXResource
{
public:
	CDXDib(CDXResourceNode* pOwner, DWORD dwKey) :CDXResource(pOwner, dwKey){}
	virtual HRESULT LoadFormFile(LPCTSTR FileName) = 0;
	virtual HRESULT GetDibDesc(LPDXDIBDESC pDibDesc) const = 0;
	virtual BOOL Lock(char ** ppBits, LPDXDIBDESC pDibDesc) = 0;
	virtual BOOL Unlock() = 0;
	virtual BOOL ExtraBits(LPRECT src, char ** ppDestBits, LPDXBIDDESC pDibDesc) = 0;
	virtual BOOL IsPick(INT x, INT y)const = 0;
	virtual BOOL IsPickRect(int x, int y) const = 0;
	virtual void Optimize(BYTE bMode = 0) = 0;
	virtual void SetPosXY(INT x, INT y) = 0;
	virtual void Compress() = 0;
	virtual void DeCompress() = 0;
	virtual void SetReference(INT nWidth, INT nHeight, DWORD dwFormat, char* pBits, INT nSize) = 0;
	virtual BOOL IsEmpty() = 0;
};

typedef CDXDib *LPDXDIB;

class CDXImage : public CDXResource
{
public :
	CDXImage(CDXResourceNode * pOwner, DWORD dwKey) :CDXResource(pOwner, dwKey){}
	virtual HRESULT GetImageDesc(LPDXIMAGEDESC pImageDesc) const = 0;
	virtual HRESULT LoadFormDib(LPDXDIB pDib, DWORD dwColorKey, LPRECT pSrcRect, LPRECT pDescRect) = 0;
	virtual HRESULT LoadFormBuffer(const char* pSrcData, int size, int nPitch, DXColorFormat format, DWORD dwColorKey, LPRECT pSrcRect) = 0;
	virtual HRESULT LoadFormBuffer(const char* pSrcData, int SrcSize, int nSrcPitch, int nSrcHeight, DXColorFormat format) = 0;
	virtual HRESULT LoadImage(LPDXIMAGE pSrcImage, const RECT * pSrcRect, const RECT * pDestRect) = 0;
	virtual HRESULT Lock(LPDXLOCKRECT pLockRect, const Rect * pLockRect, LPDXIMAGEDESC pImageDesc, BOOL bReadOnly) = 0;
	virtual HRESULT Unlock() = 0;
	virtual void Gray(INT Value) = 0;
	virtual BOOL IsPick(INT x, INT y, BOOL bStrict = FALSE) = 0;
	virtual BOOL IsPickRect(INT x, INT y) const = 0;
	virtual BOOL GetPixel(INT x, INT y, DWORD * pdwColor) = 0;

};
typedef CDXImage * LPDXIMAGE;

class CDXFont :public CDXResource
{
public:
	CDXFont(CDXResourceNode * pOwner, DWORD dwkey) :CDXResource(pOwner, dwkey){}
	virtual void DrawText(LPCTSTR lpchText, DWORD dwColor, LPPOINT lppt, UINT format) = 0;
	virtual void DrawText(LPCTSTR lpchText, DWORD dwColor, LPRECT lprc, UINT format) = 0;
	virtual void DrawText(LPCTSTR lpchText, DWORD dwColor, LPPOINT lppt, UINT format, DWORD shadow) = 0;
	virtual void DrawText(LPCTSTR lpchText, DWORD dwColor, LPRECT lprc, UINT format, DWORD shadow) = 0;
	virtual void DrawText(LPCTSTR lpchText, DWORD dwColor, LPRECT lprc, UINT format, DWORD dwStyle) = 0;
	virtual DWORD SetBorderColor(DWORD dwColor) = 0;
	virtual BOOL CalcTextSize(LPCTSTR lpchText, LPRECT lprc, WORD wStyle = 0, UINT Format = 0) = 0;
	virtual void SetSize(INT nSize) = 0;
	virtual INT  GetSize() const = 0;

	virtual void SetName(LPCTSTR lpchName) = 0;
	virtual void GetName(LPCTSTR lpchName) const = 0;

	virtual void SetBold(BOOL bBold) = 0;
	virtual BOOL GetBold() const = 0;

	virtual void SetItalic(BOOL bItalic) = 0;
	virtual BOOL GetItalic() const = 0;

	virtual void SetUnderline(BOOL bUnderline) = 0;
	virtual BOOL GetUnderline() const = 0;

	virtual void SetFont(LPCTSTR lpchName, INT nSize, BOOL bBold, BOOL bItalic, BOOL bUnderline) = 0;

};
typedef CDXFont *LPDXFONT;

//2015年5月19日  今天写不动了，明天继续把，明天要写顶点缓冲区的类了
class CDXVertexBuffer :public CDXResource
{
public:
	CDXVertexBuffer(CDXResourceNode * pOwner, DWORD dwKey) :CDXResource(pOwner, dwKey){}
	virtual HRESULT Append(int nCount, vid **ppVertex) = 0;
	virtual void clear() = 0;
	virtual int GetBuffer(void ** Buffer) const = 0;
	virtual int GetCount() = 0;
	virtual void SetType(DWORD dwType) = 0;
	virtual DWORD GetType()const = 0;
	virtual void SetFVF() const = 0;
	virtual DWORD GetFVF() const = 0;
	virtual DXFillMode GetFillMode() const = 0;
	virtual void SetState(DXVertexState state) = 0;
	virtual DXVertexState GetState() = 0;
	virtual void Offset(float cx, float cy) = 0;
	virtual void SetPos(float cx, float cy) = 0;
	virtual void RotateAndScale(float cx, float cy, float scale, float angle) = 0;

	virtual void BuildGuiVertex(DXGUIVertexType GUIVertexType, const RECT &DestRect, const LPRECT pSrcRect,
		int nWidth, int nHeight, int HState, int VState, int CurIndex, DWORD dwColor = 0xFFFFFFFF) = 0;

	virtual void BuildGuiVertex(DXGUIVertexType GUIVertexType, const RECT &DestRect, const LPRECT pSrcRect,
		int nWidth, int nHeight,int TexWidth, int TexHeighh, int HState, int VState, int CurIndex, DWORD dwColor = 0xFFFFFFFF) = 0;

	virtual void RotateAndScale(float scale, float angle) = 0;
	virtual DXGUIVertexType GetGUIVertextType() const = 0;


};

typedef CDXVertexBuffer *LPDXVertexBuffer;

class  CDXSurface :public CDXResource
{
public:
	CDXSurface(CDXResourceNode * pOwner, DWORD dwKey) :CDXResource(pOwner, dwKey){}
	virtual BOOL BeginDraw(const LPDXVIEWPORT pViewPort) = 0;
	virtual void Clear() = 0;
	virtual void EndDraw() = 0;
	virtual BOOL GetImage(LPDXIMAGE * pImage) = 0;

};
typedef CDXSurface * LPDXSURFACE;

class  CDXPRTS :public CDXResource
{
public:
	CDXPRTS(CDXResourceNode * pOwner, DWORD dwKey) :CDXResource(pOwner, dwKey){}
	virtual BOOL BeginDraw(const LPDXVIEWPORT pViewPort) = 0;
	virtual void Clear() = 0;
	virtual void EndDraw() = 0;
	virtual BOOL GetImage(LPDXIMAGE * pImage) = 0;

};
typedef CDXPRTS * LPDXPRTS;

class CDXEffect :public CDXResource
{
public:
	CDXEffect(CDXResourceNode *pOwner, DWORD dwKey) :CDXResource(pOwner, dwKey){}
	virtual HRESULT ApplyEffect(DWORD dwApply) = 0;
	virtual HRESULT BeginEffect(LPCSTR Technique, UINT Pass = 0) = 0;
	virtual HRESULT EndEffect() = 0;
	virtual HRESULT SetBool(LPCSTR Name, BOOL Value) = 0;
	virtual HRESULT GetBool(LPCSTR Name, BOOL *pVaule) = 0;
	virtual HRESULT SetInt(LPCSTR Name, INT n) = 0;
	virtual HRESULT GetInt(LPCSTR Name, INT *pn) = 0;
	virtual HRESULT SetFloat(LPCSTR Name, float f) = 0;
	virtual HRESULT GetFloat(LPCSTR Name, float * pf) = 0;
	virtual HRESULT SetFloatArray(LPCSTR Name, CONST float * pf, UINT Count) = 0;
	virtual HRESULT SetVector(LPCSTR Name, LPVECTOR4 pValue) = 0;
	virtual HRESULT GetVector(LPCSTR Name, LPVECTOR4 pValue) = 0;
	virtual HRESULT SetTexture(LPCSTR Name, LPDXIMAGE pTexture) = 0;

};
typedef CDXEffect * LPCDXEFFECT;

class CDXRender
{
public:
	virtual void OnInitialize() = 0;
	virtual void OnInvalidate() = 0;
	virtual void OnRestore() = 0;
	virtual void OnUpdate(DWORD dwCurTime, BOOL bLoadOnly) = 0;
	virtual void OnFinalize() = 0;
	
	CDXRender(LPDXDevice Device) :m_Device(Device){}
	virtual ~CDXRender(){}

	//绘制接口
	virtual void DrawLine(const LPPOINT_F pPoint, INT bCount, DWORD) = 0;
	virtual void DrawRectangle(const LPRE)

};




#endif