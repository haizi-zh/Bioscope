// dvcImage.h: interface for the CDVCImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVCIMAGE1_H__FCC5F15F_67F5_4277_A30F_930B63251C0B__INCLUDED_)
#define AFX_DVCIMAGE1_H__FCC5F15F_67F5_4277_A30F_930B63251C0B__INCLUDED_
#include "dvcMeta.h"
#ifndef _DVC_API_H_
#include "./dvcapi.h"
#endif

#include "dvcVideo.h"
#include "dvcList.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _dvcRGBMatrix {
		double rr ;
		double rg ;
		double rb ;
		double gr ;
		double gg ;
		double gb ;
		double br ;
		double bg ;
		double bb ;
} dvcRGBMatrix, *dvcRGBMatrixP ;

#define MAX_MERGE_CHANNELS	4
typedef struct _dvcMergeProp {
	int   eColorBalance ;
	int   dx[MAX_MERGE_CHANNELS],
		  dy[MAX_MERGE_CHANNELS] ;	// Offset X,Y / channel
	BOOL  bMergeImage[MAX_MERGE_CHANNELS];
	BOOL  bMergeAdd ;
	BOOL  bMergeSubtract ;
	BOOL  bMergeOpaque ;
} dvcMergeProp, *dvcMergePropP ;

class DVCAPI_API CDVCImage  : CDVCVideo
{
public:
	CDVCImage();
	CDVCImage(const CDVCImage & rImg );
	CDVCImage(PVOID pImageData, int w, int h, int bpp );
	CDVCImage( char *szFileName );
	virtual ~CDVCImage();
	CDVCImage & operator=(CDVCImage & rImg) ;
	CDVCImage & SubImage(CDVCImage & rImg, LPRECT pRect) ;
	CDVCImage & operator=(int k);
	CDVCImage & operator+=(CDVCImage & rImg) ;
	CDVCImage & operator-=(CDVCImage & rImg) ;
	CDVCImage & operator/=(CDVCImage & rImg) ;
	CDVCImage & operator*=(CDVCImage & rImg) ;
	CDVCImage & operator+=(int k) ;
	CDVCImage & operator-=(int k) ;
	CDVCImage & operator/=(int k) ;
	CDVCImage & operator/=(double k) ;
	CDVCImage & operator*=(int k) ;
	CDVCImage & operator*=(double k) ;
	
	void	SetOwner(PVOID pOwner);
	PVOID	GetOwner() ;
	BOOL	IsLocked( HANDLE hThread = NULL ) ;
	BOOL	Create(ULONG nWidth, ULONG nHeight, ULONG bytesPerPixel ) ;
	int		Lock(PVOID *pBuffer,PULONG pWidth,PULONG pHeight, PULONG pBpp );
	int		Release(void) ;
	BOOL	GetSize(PULONG ulWidth, PULONG ulHeight);
	BOOL	GetSize(int *pWidth, int *pHeight, int *pDepth) ;
	BOOL	GetCameraMeta( HANDLE hDevice, ImageMetaDataP pMeta = NULL );
	BOOL	ReadCamera( HANDLE hDevice,
					ULONG left,
					ULONG top,
					ULONG width,
					ULONG height ) ;
	HANDLE	CameraHandle(void); // return the hDevice associated w/ ReadCamera
	BOOL	OpenFile( LPSTR fileName ) ;
	BOOL	CloseFile() ;
	BOOL	ReadFile();
	BOOL	GetValue(char *szAttr, char *szValue, int vLen, char *szDefault );
	int		GetValue(char *szAttr, int nDefault);
	BOOL	GetValue(char *szAttr, bool bDefault);
	double	GetValue(char *szAttr, double dDefault);
	BOOL	SetValue(char *szAttr, char *szValue );
	BOOL	SetValue(char *szAttr, int nValue );
	BOOL	SetValue(char *szAttr, bool bValue );
	BOOL	SetValue(char *szAttr, double dValue );
	BOOL	isRGB(void);
	BOOL	isBayer(void);
	BOOL	isFile(void);
	BOOL	isFileOpening(void);
	BOOL	isImageFile(void);
	BOOL	isCamera(void);
	BOOL	isMultiFrame( int *pFrameCount ) ;
	BOOL	isReadOnly(void) ;
	int		GetBitsPerPixel(void);
	BOOL	SetRenderMethod( dvcRenderControl_t *renderOp );
	BOOL	GetRenderMethod( dvcRenderControl_t *renderOp );
	BOOL	Clear() ;
	BOOL	FillRect(RECT *pRect, ULONG ulValue) ;
	BOOL	Fill(ULONG ulValue) ;
	BOOL	ScaleLuts(int nMin=-1, int nMax=-1) ;
	BOOL	Render(PVOID pRGBBuffer,
					RECT *pSrcRect,
					ULONG bytesPerLine,
					BYTE bitsPerPixel,
					ULONG ulBufferLines = 0) ;
	BOOL	Render( LPSTR pFileName,
					RECT *pSrcRect,
					BYTE bitsPerPixel,
					int  nAppend = 0 ) ;
	int 	GetRenderLuts( PBYTE pR,
						   PBYTE pG,
						   PBYTE pB,
						   int nSize ) ;
	int 	GetLuts(   PBYTE pR,
					   PBYTE pG,
					   PBYTE pB,
					   int nSize ) ;
	int 	SetLuts(   PBYTE pR,
					   PBYTE pG,
					   PBYTE pB,
					   int nSize ) ;
	int 	CopyLuts( CDVCImage *pImage ) ;
	BOOL	HistogramStats( double *pMean,
					double *pStdDev,
					PULONG pMin,
					PULONG pMax) ;
	BOOL	HistogramMask( CDVCImage *pMask,
					double *pMean, double *pStdDev, 
					PULONG pMin, PULONG pMax) ;

	ULONG	HistogramFreq( ULONG nBin ) ;
	BOOL	Add( CDVCImage *image );
	BOOL	Add( CDVCImage & image) ;
	BOOL	Add( int nConstant ) ;
	BOOL	And( CDVCImage * image) ;
	BOOL	And( int nConstant ) ;
	BOOL	Or( CDVCImage * image) ;
	BOOL	Or( int nConstant ) ;
	BOOL	Xor( CDVCImage * image) ;
	BOOL	Xor( int nConstant ) ;
	BOOL	Not( CDVCImage * image) ;
	BOOL	Not( int nConstant ) ;
	BOOL	Subtract( CDVCImage *image );
	BOOL	Subtract( CDVCImage *image, int nOffset);
	BOOL	Multiply( CDVCImage *image, double dScaleResult) ;
	BOOL	Copy( CDVCImage *image ) ;
	BOOL	Crop( CDVCImage *image, LPRECT pRect ) ;
	BOOL	SubtractDivide(CDVCImage *flat,CDVCImage *bkg, double dScaleResult, BOOL bClipTo12Bits = FALSE );
	BOOL	Divide ( CDVCImage *image, double dScaleResult, BOOL bClipTo12Bits = FALSE );
	BOOL	Divide ( int nConstant, double dScaleResult) ;
	BOOL	DivideCopy( CDVCImage *image, int nConstant, BOOL bClipTo12Bits = FALSE ) ;
	BOOL	ReadPixel(ULONG x, ULONG y, PULONG pPixData) ;
	BOOL	ReadRenderPixel(ULONG x, ULONG y, PULONG pPixData) ;
	BOOL	WritePixel(ULONG x, ULONG y, ULONG ulPixData) ;
	BOOL	ReadColumn(ULONG x, ULONG y, ULONG height, PVOID pPixData) ;
	BOOL	ReadRow(ULONG x, ULONG y, ULONG width, PVOID pPixData) ;
	BOOL	ReadRoi(ULONG x,
					ULONG y,
					ULONG width,
					ULONG height,
					PVOID pPixData);
	BOOL	ReadRect( RECT *pRect, PVOID pPixData) ;
	BOOL	SetIndex( int nIndex) ;
	BOOL	GetIndex( int *pIndex) ;
	BOOL	GetFileName( LPSTR szFileName, int nChars );
	BOOL	SetRGBBalance( double dRed, double dGreen, double dBlue ) ;
	BOOL	GetRGBBalance( double *dRed, double *dGreen, double *dBlue ) ;
	BOOL	SetRGBOffset( int nRed, int nGreen, int nBlue ) ;
	BOOL	GetRGBOffset( int *nRed, int *nGreen, int *nBlue ) ;
	BOOL	SetRGBGamma( double dRed, double dGreen, double dBlue ) ;
	BOOL	GetRGBGamma( double *dRed, double *dGreen, double *dBlue ) ;
	BOOL	GetBinning( int *pBinX, int *pBinY ) ;
	CDVCImage *GetChannel(int nChannel) ;
	CDVCImage *Zoom( PRECT pRect, int nZoomX, int nZoomy ) ;
	CDVCImage *Decimate( PRECT pRect, int nDecimateX, int nDecimatey ) ;
	BOOL	Translate( CDVCImage *pSrc, int dx, int dy, BOOL bZero = TRUE ) ;

	BOOL	begin();
	BOOL	end();
	BOOL	next();
	BOOL	previous();
	BOOL	last();
	void	run();
	void	pause(BOOL bPause);
	BOOL	running();
	void	stop();
	CDVCVideo *GetVideo() ;

	void	addReceiver( CDVCVideoReceiver *pRcvr ) ;
	void	removeReceiver( CDVCVideoReceiver *pRcvr ) ;
	void	SendDeviceEvent();
	void	SendFrameEvent();
	void	SendStreamEvent();
	void	SendRestart();

	int		ReadProfile(int x1, int y1, int x2, int y2,  PVOID pReturnData, int nReturnSize) ;

	BOOL	GetMetaData(ImageMetaDataP pMeta) ;
	BOOL	SetMetaData(ImageMetaDataP pMeta) ;
	CDVCMeta& GetMeta();
	HBITMAP	GetDIB(HDC hDC);

	BOOL	GetRGBMatrix(dvcRGBMatrixP rgbMat);
	BOOL	SetRGBMatrix(dvcRGBMatrixP rgbMat);
	void	DataNeedsRefresh();
	void	ResetGlobalAttr();

	BOOL	GetCameraRect(LPRECT pr);
	BOOL	MergeLuts(
				CDVCImage *pr,
				CDVCImage *pg,
				CDVCImage *pb,
				CDVCImage *pw,
				BOOL bOpaque,
				BOOL bSubtract,
				BOOL bAdd
					) ;

	BOOL	Merge(
				CDVCImage *pr,
				CDVCImage *pg,
				CDVCImage *pb,
				CDVCImage *pw,
				dvcMergePropP  pMerge
				 );

	BOOL	Merge(
				CDVCImage *pr,
				CDVCImage *pg,
				CDVCImage *pb,
				CDVCImage *pw,
				int nWhiteBalance
				 );

protected:
	ULONG	m_nMyID ;
	BOOL	CheckThis() ;
	BOOL	ResetLut(int nLutSize);
	BOOL	SetRenderLut(void);
	void	initData(void);
	void	init(void);
	BOOL	FreeBuffer(void) ;
	VOID *	pBuffer ;
	ULONG	bytesAllocated ;
	ULONG	ulWidth ;
	ULONG	ulHeight ;
	ULONG	ulTop ;
	ULONG	ulLeft ;
	USHORT	usBinX ;
	USHORT	usBinY ;
	BOOL	bIsColor ;
	BOOL	bIsBayer ;
	BOOL	bIsFile ;
	BOOL	bOwnsBuffer ;
	BOOL	bHistogramUptoDate ;
	BOOL	bFileModified ;
	BOOL	bFileOpening ;
	BYTE	bytesPerPixel ;	// 1, 2, 3, 4, ...
	BYTE	nBitsPerPixel ; // 8, 10, 12, ...
	int		nLocks ;
	dvcRenderControl_t	renderMethod ;
	dvcHistP pHistData ;
	PBYTE  	pRedLut ;
	PBYTE  	pGreenLut ;
	PBYTE  	pBlueLut ;
	PBYTE  	pRenderRedLut ;		// Color balanced and Gamma set
	PBYTE  	pRenderGreenLut ;
	PBYTE  	pRenderBlueLut ;
	int		nLutSize ;
	PVOID	pTiff ;
	HANDLE  hDVCDevice ;
	char	sTiffFileName[256] ;
	int		nImagesInFile ;
	int		nImageIndex ;
	PUSHORT	pRatioLut ;
	double  dRatioScale ;
	ULONG   nRatioLutSize ;
	double  dRedBalance, dGreenBalance, dBlueBalance ;
	int     nRedOffset, nGreenOffset, nBlueOffset ;
	double  dRedGamma, dGreenGamma, dBlueGamma ;
	ImageMetaData camMetaData ;
	dvcRGBMatrix  m_rgbMat ;

	CDVCMeta  m_Meta ;
	PVOID  pImageOwner ;
	CDVCList lReceivers ;

	CRITICAL_SECTION m_BufferMutex ;	// Locks allocated image buffer
	CRITICAL_SECTION m_LutMutex ;		// Locks allocated look-up tables
	CRITICAL_SECTION m_TiffMutex ;		// Locks opened TIFF file
	HANDLE hLockThread ;
	char *szGlobalAttr ;
};

#endif // !defined(AFX_DVCIMAGE1_H__FCC5F15F_67F5_4277_A30F_930B63251C0B__INCLUDED_)
