// dvcAPI.h
// (C) DVC Company, Inc., 2002,  All rights reserved.

#ifndef _DVC_API_H_
#define _DVC_API_H_

#include "./dvcdefs.h"


#ifdef DVCAPI_EXPORTS
#define DVCAPI_API __declspec(dllexport)
#else
#define DVCAPI_API __declspec(dllimport)
#endif

// Initialization

#if defined(__cplusplus)
extern "C" {
#endif

DVCAPI_API HANDLE	dvcOpenCamera(ULONG id);
DVCAPI_API BOOL		dvcCloseCamera(HANDLE hDevice);
DVCAPI_API BOOL		dvcAutoDetectComPort(BOOL bOnOff);
DVCAPI_API BOOL		dvcGetHandleIndex(HANDLE hDevice, int *index);
DVCAPI_API BOOL		dvcGetListOfCameras(CameraListArrayStructP pCameras);

DVCAPI_API int		dvcGetNBits(HANDLE hDevice) ;
DVCAPI_API BOOL		dvcHasIntensifier( HANDLE hDevice );
DVCAPI_API BOOL		dvcHasFilterWheel( HANDLE hDevice );

DVCAPI_API int		dvcGetCameraType( HANDLE hDevice ) ;
DVCAPI_API int		dvcGetCameraName( HANDLE hDevice, char *szName, int len ) ;
DVCAPI_API BOOL		dvcGetCameraSerialNumber( HANDLE hDevice, int *pSN ) ;
// Depends on the interface and supporting hardware's device driver
DVCAPI_API int		dvcGetDriverVersion( HANDLE hDevice ) ;
// returns the firmware version of the camera
DVCAPI_API int		dvcGetFirmwareVersion( HANDLE hDevice ) ;
// Returns the API version id as 
// (major >> 16) & 0xff
// (minor >> 8 ) & 0xff  
// (build ) & 0xff
DVCAPI_API int		dvcGetAPIVersion() ;
// fills dateStr with the date and time of the build
// returns the number of characters in the string
// dateStr can be NULL to check for the size of the array needed.
DVCAPI_API int		dvcGetAPIDate( char *dateStr, int nStrLen ) ;


DVCAPI_API int		dvcGetCCDWidth(HANDLE hDevice) ;
DVCAPI_API int		dvcGetCCDHeight(HANDLE hDevice) ;

DVCAPI_API BOOL		dvcResetCamera( HANDLE hDevice ) ;

DVCAPI_API BOOL		dvcCommIsConnected( HANDLE hDevice) ;
DVCAPI_API BOOL		dvcCommReConnect( HANDLE hDevice, int nPort) ;
DVCAPI_API BOOL		dvcCommDisConnect( HANDLE hDevice) ;
DVCAPI_API BOOL		dvcSetDisconnectEvent(HANDLE hDevice, HANDLE hEvent);
DVCAPI_API BOOL		dvc1394Device(HANDLE hDevice);
DVCAPI_API BOOL		dvcEdtDevice(HANDLE hDevice);
DVCAPI_API BOOL		dvcEclDevice(HANDLE hDevice);
DVCAPI_API BOOL		dvcMatroxDevice(HANDLE hDevice);
DVCAPI_API BOOL		dvcEpixDevice(HANDLE hDevice);
DVCAPI_API BOOL		dvcNetDevice(HANDLE hDevice);

DVCAPI_API double	dvcGetMaxPixelClock( HANDLE hDevice ) ;

// Parameter I/O

DVCAPI_API BOOL		dvcGetBinning (HANDLE hDevice, int *pBinning ) ;
DVCAPI_API BOOL		dvcSetBinning (HANDLE hDevice, int nBinning ) ;
DVCAPI_API BOOL		dvcGetHVBin(HANDLE hDevice, int *phBin, int *pvBin ) ;
DVCAPI_API BOOL		dvcSetHVBin(HANDLE hDevice, int hBin, int vBin ) ;
DVCAPI_API BOOL		dvcDisconnectBinningExpose (HANDLE hDevice, BOOL bOnOff ) ;
DVCAPI_API BOOL		dvcSetGain( HANDLE hDevice, int nGain ) ;
DVCAPI_API BOOL		dvcGetGain(HANDLE hDevice, int *pGain) ;
DVCAPI_API BOOL		dvcSetOffset( HANDLE hDevice, int nOffset) ;
DVCAPI_API BOOL		dvcGetOffset(HANDLE hDevice, int *pOffset) ;
DVCAPI_API BOOL 	dvcSetGaindB(HANDLE hDevice, double dGain) ;
DVCAPI_API BOOL 	dvcGetGaindB(HANDLE hDevice, double *pGaindB ) ;
DVCAPI_API BOOL 	dvcGetGaindBRange(HANDLE hDevice, double *pMin, double *pMax, double *pInterval ) ;
DVCAPI_API BOOL 	dvcSetOffsetFS(HANDLE hDevice, double dOffset) ;
DVCAPI_API BOOL 	dvcGetOffsetFS(HANDLE hDevice, double *pOffset) ;
DVCAPI_API BOOL 	dvcGetOffsetFSRange(HANDLE hDevice, double *pMin, double *pMax, double *pInterval) ;
DVCAPI_API BOOL		dvcSetGainOffset( HANDLE hDevice, int nGain, int nOffset) ;
DVCAPI_API BOOL		dvcGetGainOffset(HANDLE hDevice, int *pGain, int *pOffset) ;
DVCAPI_API BOOL		dvcGetScanRate (HANDLE hDevice, int *pSlowScan ) ;
DVCAPI_API BOOL		dvcSetScanRate (HANDLE hDevice, int nSlowScan ) ;
DVCAPI_API BOOL		dvcGetExposureMode (HANDLE hDevice, int *pMode ) ;
DVCAPI_API BOOL		dvcSetExposureMode (HANDLE hDevice, int nMode ) ;
DVCAPI_API BOOL		dvcGetExposureDuration( HANDLE hDevice, int *pExpose ) ;
DVCAPI_API BOOL		dvcSetExposureDuration( HANDLE hDevice, int nExpose ) ;
DVCAPI_API double	dvcGetExposeMsec( HANDLE hDevice ) ;
DVCAPI_API double	dvcSetExposeMsec( HANDLE hDevice, double dMilliSeconds ) ;
DVCAPI_API BOOL		dvcGetExposeInterval( HANDLE hDevice, int nMode, int nBin, int nSlowScan, double *pInterval);
DVCAPI_API BOOL		dvcGetExposeMax( HANDLE hDevice, int nMode, int nBin, int nSlowScan, double *pMax);
DVCAPI_API double	dvcGetPixelClock( HANDLE hDevice ) ;

DVCAPI_API BOOL		dvcIsColor( HANDLE hDevice ) ;
DVCAPI_API BOOL		dvcCanRoi( HANDLE hDevice ) ;

DVCAPI_API int		dvcGetXDim(HANDLE hDevice) ;
DVCAPI_API int		dvcGetYDim(HANDLE hDevice) ;
DVCAPI_API int		dvcGetImageWidth(HANDLE hDevice) ;
DVCAPI_API int		dvcGetImageHeight(HANDLE hDevice) ;
DVCAPI_API BOOL		dvcSetRoi( HANDLE hDevice, int nOnOff, RECT *prSet ) ;
DVCAPI_API BOOL		dvcGetRoi( HANDLE hDevice, int *pOnOff, RECT *prGet ) ;

DVCAPI_API BOOL		dvcGetFilterWheelStatus( HANDLE hDevice, int *pStatus ) ;
DVCAPI_API BOOL		dvcGetFilterWheelSpeed( HANDLE hDevice, int *pSpeed ) ;
DVCAPI_API BOOL		dvcGetFilterWheelPosition( HANDLE hDevice, int *pPosition ) ;
DVCAPI_API BOOL		dvcSetFilterWheelSpeed( HANDLE hDevice, int nSpeed ) ;
DVCAPI_API BOOL		dvcSetFilterWheelPosition( HANDLE hDevice, int nPosition, BOOL bWait ) ;

DVCAPI_API BOOL		dvcSetIntensifierMode( HANDLE hDevice, int nMode );
DVCAPI_API BOOL		dvcGetIntensifierMode( HANDLE hDevice, int *pMode );
DVCAPI_API BOOL		dvcSetIntensifierGain( HANDLE hDevice, int nGain );
DVCAPI_API BOOL		dvcGetIntensifierGain( HANDLE hDevice, int *pGain );
DVCAPI_API BOOL		dvcSetIntensifierDelay( HANDLE hDevice, int nDelay );
DVCAPI_API BOOL		dvcGetIntensifierDelay( HANDLE hDevice, int *pDelay );
DVCAPI_API BOOL		dvcSetIntensifierDuration( HANDLE hDevice, int nPulseWidth );
DVCAPI_API BOOL		dvcGetIntensifierDuration( HANDLE hDevice, int *pPulseWidth );


// Image Acquisition 

DVCAPI_API BOOL		dvcResetExpose(HANDLE hDevice);
DVCAPI_API BOOL		dvcStartSequence( HANDLE hDevice, ULONG frames);
DVCAPI_API BOOL		dvcStartSequenceEx( HANDLE hDevice, ULONG frames,int dvcTrig);
DVCAPI_API BOOL		dvcGetStreamState( HANDLE hDevice, int *state, ULONG *frames);
DVCAPI_API BOOL		dvcGetStreamBytes( HANDLE hDevice, int *state, ULONG *nBytes);
DVCAPI_API int		dvcGetStatus( HANDLE hDevice ) ;
DVCAPI_API BOOL		dvcStopSequence( HANDLE hDevice ) ;
DVCAPI_API BOOL		dvcGetLineFrameTime( HANDLE hDevice, double *pLine, double *pFrame ) ;
DVCAPI_API double	dvcGetTimeToReadImage( HANDLE hDevice ) ;
DVCAPI_API double	dvcGetTimeToNextExpose( HANDLE hDevice ) ;
DVCAPI_API double	dvcGetTimeToExposeComplete( HANDLE hDevice ) ;
DVCAPI_API double	dvcGetTimeToReadComplete( HANDLE hDevice ) ;
DVCAPI_API BOOL		dvcWaitImage( HANDLE hDevice, double dWaitMilliSeconds ) ;
DVCAPI_API BOOL		dvcWaitExpose( HANDLE hDevice, double dWaitMilliSeconds ) ;
DVCAPI_API BOOL		dvcSetCallback( HANDLE hDevice, dvcCallbackProcP fCallback, PVOID pUserData ) ;
DVCAPI_API BOOL		dvcHasCallback( HANDLE hDevice );
DVCAPI_API BOOL		dvcSetReadCompleteEvent( HANDLE hDevice, HANDLE hEvent ) ;
DVCAPI_API BOOL		dvcGetReadCompleteEvent( HANDLE hDevice, PHANDLE hEvent ) ;
DVCAPI_API BOOL		dvcSetFilterMoveEvent( HANDLE hDevice, HANDLE hEvent ) ;
DVCAPI_API BOOL		dvcSetExposeCompleteEvent( HANDLE hDevice, HANDLE hEvent ) ;
DVCAPI_API BOOL		dvcGetExposeCompleteEvent( HANDLE hDevice, PHANDLE hEvent) ;
DVCAPI_API void		dvcSetOpenCallback( dvcCallback2ProcP fCallback ) ;
DVCAPI_API void		updateOpenCallback(char *msg, ...);
DVCAPI_API int		dvcGetFrameCount( HANDLE hDevice ) ;
DVCAPI_API int		dvcGetStreamCount( HANDLE hDevice ) ;
DVCAPI_API int		dvcGetExposeCount( HANDLE hDevice ) ;
DVCAPI_API double	dvcGetStreamTime( HANDLE hDevice ) ;
DVCAPI_API double	dvcGetFrameRate( HANDLE hDevice ) ;

DVCAPI_API ULONG	dvcGetImageBytes( HANDLE hDevice ) ;
DVCAPI_API BOOL		dvcTakePicture( HANDLE hDevice ) ;

DVCAPI_API BOOL		dvcReadMeta( HANDLE hDevice, ImageMetaDataP pMeta);
DVCAPI_API BOOL		dvcGetDriverBuffers( HANDLE hDevice, int *pNBuffers, int *pLastBuffer);
DVCAPI_API BOOL		dvcReadDriverBuffer( HANDLE hDevice, int nBuffer, PUSHORT buffer, ULONG left, ULONG top, ULONG width, ULONG height) ;
DVCAPI_API BOOL		dvcReadDriverBufferEx( HANDLE hDevice, int nBuffer, PUSHORT buffer, ULONG left, ULONG top, ULONG width, ULONG height, ImageMetaDataP pMeta) ;
DVCAPI_API BOOL		dvcReadImageAndMeta( HANDLE hDevice, PUSHORT buffer, ULONG left, ULONG top, ULONG width, ULONG height, ImageMetaDataP pMeta) ;
DVCAPI_API BOOL		dvcReadImage( HANDLE hDevice, PUSHORT buffer, ULONG left, ULONG top, ULONG width, ULONG height) ;

DVCAPI_API BOOL		dvcReadImageEx( HANDLE hDevice, PUSHORT buffer, ULONG left, ULONG top, ULONG width, ULONG height, ULONG lineWidth ) ;
DVCAPI_API BOOL	dvcOverlappedReadImage( HANDLE hDevice, PUSHORT buffer, ULONG left, ULONG top, ULONG width, ULONG height) ;
DVCAPI_API BOOL	dvcOverlappedReadImageEx( HANDLE hDevice, PUSHORT buffer, ULONG left, ULONG top, ULONG width, ULONG height, ImageMetaDataP pMeta) ;
DVCAPI_API BOOL		dvcReadRGB32( HANDLE hDevice, PULONG buffer, ULONG left, ULONG top, ULONG width, ULONG height, ULONG lineWidth, BOOL bWhiteBalance ) ;
DVCAPI_API BOOL		dvcReadImageRGB48( HANDLE hDevice, PUSHORT rgbBuffer, ULONG left, ULONG top, ULONG width, ULONG height) ;
DVCAPI_API BOOL		dvcReadLine( HANDLE hDevice, ULONG imageX, ULONG imageY, ULONG width, PUSHORT pixelData) ;
DVCAPI_API BOOL		dvcReadPixel( HANDLE hDevice, ULONG imageX, ULONG imageY, int *pixelData) ;

DVCAPI_API BOOL		dvcSetUserBuffers(HANDLE hDevice, dvcBufStructP pBufStruct);
DVCAPI_API BOOL		dvcAllocateUserBuffersEx(HANDLE hDevice, dvcBufStructP pBufStruct, int nBuffers, int width, int height ) ;
DVCAPI_API BOOL		dvcAllocateUserBuffers(HANDLE hDevice, dvcBufStructP pBufStruct, int nBuffers) ;
DVCAPI_API BOOL		dvcReleaseUserBuffers( dvcBufStructP pBufStruct );
DVCAPI_API BOOL		dvcGetUserBufferId( HANDLE hDevice, int *pID) ;

// Image Utilities

DVCAPI_API HBITMAP	dvcImageToDIB( HDC hDC, ULONG ulX, ULONG ulY, ULONG ulWidth, ULONG ulHeight, PUSHORT imageBuffer, BYTE *rlut, BYTE *glut, BYTE *blut, ULONG lutSize, BOOL isColor) ;
DVCAPI_API BOOL		dvcWriteBmpFile( char *fname, ULONG ulX, ULONG ulY, ULONG width, ULONG height, USHORT *buffer, BYTE *rlut, BYTE *glut, BYTE *blut, ULONG lutSize, BOOL isColor) ;
DVCAPI_API BOOL		dvcWriteRGBBmpFile( char *fname, ULONG width, ULONG height, ULONG *buffer, ULONG ulBitsPerPixel) ; 
DVCAPI_API BOOL		dvcWriteTifImage( char *name, USHORT *buffer, ULONG ulX, ULONG ulY, ULONG width, ULONG height, int bitsPerPixel, int format, BYTE *rlut, BYTE *glut, BYTE *blut, ULONG lutSize) ;
DVCAPI_API BOOL		dvcAppendTifImage( PVOID pTiff, USHORT *buffer, ULONG ulX, ULONG ulY, ULONG width, ULONG height, int bitsPerPixel, int format, BYTE *rlut, BYTE *glut, BYTE *blut, ULONG lutSize) ;
DVCAPI_API BOOL		dvcWriteTifBuffer( PVOID pTiff, USHORT *buffer, ULONG ulX, ULONG ulY, ULONG width, ULONG height, int bitsPerPixel, int format, BOOL bIsBayer, BYTE *rlut, BYTE *glut, BYTE *blut, ULONG lutSize) ;
DVCAPI_API BOOL		dvcWriteTifHeader( PVOID pTiff, char *szTimeStamp, char *szDescription, char *szModel);

DVCAPI_API BOOL		dvcSaveTifColormap( BOOL bOn );

//
// Pass 16 bit RGB color info
//
DVCAPI_API BOOL		dvcRGB16Info( USHORT redMask, USHORT redBits, USHORT redShift, USHORT greenMask, USHORT greenBits, USHORT greenShift, USHORT blueMask, USHORT blueBits, USHORT blutShift) ; 

DVCAPI_API BOOL		dvcCopyBufferToRGB16( 
	USHORT *rawDataBuffer,				// 16 bits/pixel input buffer
	ULONG ulX,
	ULONG ulY,					// left,top of input image
	ULONG width,
	ULONG height,				// dimensions of input image
	USHORT *aRGBOutputBuffer,			// 16 bits/pixel output buffer
	ULONG ulBytesPerRow,		// number of bytes between vertical pix
	BYTE *rlut,							// 12 to 8 red LUT
	BYTE *glut,							// 12 to 8 green LUT
	BYTE *blut,							// 12 to 8 blue LUT
	ULONG lutSize,						// size of red,green,blue luts
	BOOL  bInputIsBayerMask
	) ;

DVCAPI_API BOOL		dvcCopyBufferToRGB24( 
	USHORT *rawDataBuffer,				// 16 bits/pixel input buffer
	ULONG ulX,
	ULONG ulY,					// left,top of input image
	ULONG width,
	ULONG height,				// dimensions of input image
	BYTE *aRGBOutputBuffer,				// 24 bits/pixel output buffer
	ULONG ulBytesPerRow,		// number of bytes between vertical pix
	BYTE *rlut,							// 12 to 8 red LUT
	BYTE *glut,							// 12 to 8 green LUT
	BYTE *blut,							// 12 to 8 blue LUT
	ULONG lutSize,						// size of red,green,blue luts
	BOOL  bInputIsBayerMask
	) ;

DVCAPI_API BOOL		dvcSetRGBCrossTalkMatrix( 
				double rg, double rb, double gr,
				double gb, double br, double bg ) ;

DVCAPI_API BOOL		dvcCopyBufferToRGB32( 
	USHORT *rawDataBuffer,				// 16 bits/pixel input buffer
	ULONG ulX,
	ULONG ulY,					// left,top of input image
	ULONG width,
	ULONG height,				// dimensions of input image
	ULONG *aRGBOutputBuffer,	//32 bits/pixel output buffer
	ULONG ulBytesPerRow,		// number of bytes between vertical pix
	BYTE *rlut,							// 12 to 8 red LUT
	BYTE *glut,							// 12 to 8 green LUT
	BYTE *blut,							// 12 to 8 blue LUT
	ULONG lutSize,						// size of red,green,blue luts
	BOOL  bInputIsBayerMask
	) ;

DVCAPI_API BOOL		dvcCMYToRGB16( 
	USHORT *rawDataBuffer,				// 16 bits/pixel input buffer
	ULONG ulX,
	ULONG ulY,					// left,top of input image
	ULONG width,
	ULONG height,				// dimensions of input image
	USHORT *aRGBOutputBuffer,			// 16 bits/pixel output buffer
	ULONG ulBytesPerRow,		// number of bytes between vertical pix
	BYTE *rlut,							// 12 to 8 red LUT
	BYTE *glut,							// 12 to 8 green LUT
	BYTE *blut,							// 12 to 8 blue LUT
	ULONG lutSize,						// size of red,green,blue luts
	BOOL bInterlaced
	) ;

DVCAPI_API BOOL		dvcCMYToRGB24( 
	USHORT *rawDataBuffer,				// 16 bits/pixel input buffer
	ULONG ulX,
	ULONG ulY,					// left,top of input image
	ULONG width,
	ULONG height,				// dimensions of input image
	BYTE *aRGBOutputBuffer,				// 24 bits/pixel output buffer
	ULONG ulBytesPerRow,		// number of bytes between vertical pix
	BYTE *rlut,							// 12 to 8 red LUT
	BYTE *glut,							// 12 to 8 green LUT
	BYTE *blut,							// 12 to 8 blue LUT
	ULONG lutSize,						// size of red,green,blue luts
	BOOL bInterlaced
	) ;

DVCAPI_API BOOL		dvcCMYToRGB32( 
	USHORT *rawDataBuffer,				// 16 bits/pixel input buffer
	ULONG ulX,
	ULONG ulY,					// left,top of input image
	ULONG width,
	ULONG height,				// dimensions of input image
	ULONG *aRGBOutputBuffer,	//32 bits/pixel output buffer
	ULONG ulBytesPerRow,		// number of bytes between vertical pix
	BYTE *rlut,							// 12 to 8 red LUT
	BYTE *glut,							// 12 to 8 green LUT
	BYTE *blut,							// 12 to 8 blue LUT
	ULONG lutSize,						// size of red,green,blue luts
	BOOL bInterlaced
	) ;

DVCAPI_API BOOL		dvcSetSharpness( HANDLE hDevice, int nOn ) ;
DVCAPI_API BOOL		dvcGetSharpness( HANDLE hDevice, int *pOn ) ;
DVCAPI_API BOOL		dvcSetWhiteBalance( HANDLE hDevice, int nOn ) ;
DVCAPI_API BOOL		dvcGetWhiteBalance( HANDLE hDevice, int *pOn ) ;

DVCAPI_API BOOL		dvcGetLutSize( HANDLE hDevice, ULONG *lutSize ) ;
DVCAPI_API BOOL		dvcResetRGBLuts( HANDLE hDevice ) ;
DVCAPI_API BYTE *	dvcGetRedLut( HANDLE hDevice, ULONG *lutSize ) ;
DVCAPI_API BYTE *	dvcGetGreenLut( HANDLE hDevice, ULONG *lutSize ) ;
DVCAPI_API BYTE *	dvcGetBlueLut( HANDLE hDevice, ULONG *lutSize ) ;
DVCAPI_API BOOL 	dvcReleaseLut( PBYTE pLut ) ;
DVCAPI_API BOOL		dvcSetRedLut( HANDLE hDevice, PBYTE pLut, ULONG lutSize ) ;
DVCAPI_API BOOL		dvcSetGreenLut( HANDLE hDevice, PBYTE pLut, ULONG lutSize ) ;
DVCAPI_API BOOL		dvcSetBlueLut( HANDLE hDevice, PBYTE pLut, ULONG lutSize ) ;
DVCAPI_API BOOL		dvcSetRGBLuts( HANDLE hDevice, PBYTE rLut, PBYTE gLut, PBYTE bLut, ULONG lutSize ) ;
DVCAPI_API BOOL		dvcGetRGBLuts( HANDLE hDevice, PBYTE rLut, PBYTE gLut, PBYTE bLut, ULONG lutSize ) ;

DVCAPI_API BOOL		dvcSaveAsBitMap( HANDLE hDevice, LPSTR szPath ) ;
DVCAPI_API BOOL		dvcShowImage( HANDLE hDevice, HDC hDC, int x, int y, int width, int height) ;


DVCAPI_API BOOL		dvcCopyShortImageData ( HANDLE hDevice, USHORT *pBuffer ) ;
DVCAPI_API BOOL		dvcCopyCharImageData( HANDLE hDevice, BYTE *pBuffer, BYTE *lut) ;

DVCAPI_API dvcHistP	dvcAllocateHistogram(void) ;
DVCAPI_API BOOL		dvcReleaseHistogram(dvcHist *pHist) ;
DVCAPI_API BOOL		dvcComputeHistogram( dvcHistP pHist, USHORT *pImageBuffer, ULONG ulWidth, ULONG ulHeight, ULONG ulDX, ULONG ulDY, BOOL bZeroHistogramData) ;
DVCAPI_API BOOL		dvcComputeRGBHistogram( dvcHistP pHist, ULONG *pImageBuffer, ULONG ulWidth, ULONG ulHeight, ULONG ulDX, ULONG ulDY, BOOL bZeroHistogramData) ;
DVCAPI_API BOOL		dvcComputeHistogramMask( dvcHistP pHist, USHORT *pImageBuffer, USHORT *pImageMask, ULONG ulWidth, ULONG ulHeight, ULONG ulDX, ULONG ulDY, BOOL bZeroHistogramData) ;
DVCAPI_API BOOL		dvcComputeRGBHistogramMask( dvcHistP pHist, ULONG *pImageBuffer, USHORT *pImageMask, ULONG ulWidth, ULONG ulHeight, ULONG ulDX, ULONG ulDY, BOOL bZeroHistogramData) ;
DVCAPI_API BOOL		dvcGetMeanVariance(dvcHistP pHist, double *pMean, double *pVariance);
DVCAPI_API BOOL		dvcGetHistogramMode( dvcHistP pHist, ULONG *pMode, ULONG *pFreq) ;


DVCAPI_API BOOL		dvcGetImageFormat(HANDLE hDevice, int nX, int nY, int *pFormat);
DVCAPI_API BOOL		dvcSnapShot(HANDLE hDevice, double dExpose, int dvcTrigger);
DVCAPI_API BOOL		dvcAutoScaleImage ( PUSHORT pImageBuffer, ULONG ulLeft, ULONG ulTop, ULONG ulWidth, ULONG ulHeight, PBYTE pRedLut, PBYTE pGreenLut, PBYTE pBlueLut, ULONG lutSize);
DVCAPI_API BOOL		dvcWhiteBalanceImage ( PUSHORT pImageBuffer, ULONG ulLeft, ULONG ulTop, ULONG ulWidth, ULONG ulHeight, PBYTE pRedLut, PBYTE pGreenLut, PBYTE pBlueLut, ULONG lutSize);
DVCAPI_API	BOOL	dvcWhiteBalanceRGB48 ( PUSHORT rBuffer, PUSHORT gBuffer, PUSHORT bBuffer, ULONG	ulLeft, ULONG	ulTop, ULONG	ulWidth, ULONG	ulHeight, PBYTE	pRedLut, PBYTE	pGreenLut, PBYTE	pBlueLut, ULONG	lutSize);
DVCAPI_API	BOOL	dvcPeakBalanceRGB48 ( PUSHORT rBuffer, PUSHORT gBuffer, PUSHORT bBuffer, ULONG	ulLeft, ULONG	ulTop, ULONG	ulWidth, ULONG	ulHeight, PBYTE	pRedLut, PBYTE	pGreenLut, PBYTE	pBlueLut, ULONG	lutSize);
DVCAPI_API BOOL		dvcFindPeakBalanceScales( PUSHORT rBuffer, PUSHORT gBuffer, PUSHORT bBuffer, ULONG ulWidth, ULONG ulHeight,  double *pRedScale, double *pGreenScale, double *pBlueScale) ;
DVCAPI_API BOOL		dvcBayerPeakBalanceScales( PUSHORT pBuffer, ULONG ulWidth, ULONG ulHeight,  double *pRedScale, double *pGreenScale, double *pBlueScale) ;
DVCAPI_API BOOL		dvcFindWhiteBalanceScales( PUSHORT rBuffer, PUSHORT gBuffer, PUSHORT bBuffer, ULONG ulWidth, ULONG ulHeight,  double *pRedScale, double *pGreenScale, double *pBlueScale) ;
DVCAPI_API BOOL		dvcBayerWhiteBalanceScales( PUSHORT pBuffer, ULONG ulWidth, ULONG ulHeight,  double *pRedScale, double *pGreenScale, double *pBlueScale) ;
DVCAPI_API BOOL		dvcCMYWhiteBalanceScales( PUSHORT pBuffer, ULONG ulWidth, ULONG ulHeight,  double *pRedScale, double *pGreenScale, double *pBlueScale) ;
DVCAPI_API	BOOL	dvcAutoScaleBayerImage ( PUSHORT pImageBuffer, ULONG	ulLeft, ULONG	ulTop, ULONG	ulWidth, ULONG	ulHeight, PBYTE	pRedLut, PBYTE	pGreenLut, PBYTE	pBlueLut, ULONG	lutSize, BOOL	bWhiteBalance);


DVCAPI_API BOOL		dvcGetRGBGamma( HANDLE hDevice, PULONG pulGamma ) ;
DVCAPI_API BOOL		dvcSetRGBGamma( HANDLE hDevice, PULONG pulGamma ) ;
DVCAPI_API BOOL		dvcGetRGBBrightness( HANDLE hDevice, PULONG pulBrightness ) ;
DVCAPI_API BOOL		dvcSetRGBBrightness( HANDLE hDevice, PULONG pulBrightness ) ;
DVCAPI_API BOOL		dvcGetRGBContrast( HANDLE hDevice, PULONG pulContrast ) ;
DVCAPI_API BOOL		dvcSetRGBContrast( HANDLE hDevice, PULONG pulContrast ) ;
DVCAPI_API BOOL		dvcGetRGBColorBalance( HANDLE hDevice, PULONG pulColorBalance ) ;
DVCAPI_API BOOL		dvcSetRGBColorBalance( HANDLE hDevice, PULONG pulColorBalance ) ;


// Diagnostics
DVCAPI_API double	dvcElapseTime( double stime ) ;
DVCAPI_API int		dvcSetDebugLevel( int level ) ;
DVCAPI_API int		dvcGetDebugLevel( void ) ;

DVCAPI_API HANDLE	dvcOpenLog(char *fmt);
DVCAPI_API BOOL		dvcCloseLog(void);
DVCAPI_API void		dvcPrintf(int level, char *fmt, ...);
DVCAPI_API void		dvcLog(char *fmt, ...);

DVCAPI_API BOOL		dvcResetErr( void  ) ;
DVCAPI_API BOOL		dvcSetErrMsg( int level, int dvcErr, char *msg,  char *file, int line  ) ;
DVCAPI_API int		dvcGetLastErr( void ) ;
DVCAPI_API char *	dvcGetLastErrMsg( void ) ;
DVCAPI_API char *	dvcGetLastErrFile( void ) ;
DVCAPI_API int		dvcGetLastErrLine( void ) ;
DVCAPI_API double	dvcGetLastErrTime( void ) ;
DVCAPI_API ULONG	dvcExceptionErr(ULONG ulCode);

DVCAPI_API BOOL 	dvcSetParm (HANDLE hDevice, ULONG ulAttribute, int nValue);
DVCAPI_API BOOL 	dvcGetParm (HANDLE hDevice, ULONG ulAttribute, int *pValue);
DVCAPI_API BOOL 	dvcGetParmRange ( HANDLE hDevice, ULONG ulAttribute,
						int *pMin, int *pMax, int *pIntveral, int *pDefault );
DVCAPI_API BOOL 	dvcGetCapability ( HANDLE hDevice, char *szCapability,
						int *pMin, int *pMax, int *pIntveral, int *pDefault );

DVCAPI_API BOOL 	dvcAutoShutterIntensifier (HANDLE hDevice, BOOL bOnOff );
DVCAPI_API BOOL 	dvcSetIntensifierExposeTime(HANDLE hDevice, double dMsec);
DVCAPI_API BOOL 	dvcGetIntensifierExposeTime(HANDLE hDevice, double *pdMsec);
DVCAPI_API BOOL 	dvcSendComm(HANDLE hDevice, char *szWrite, char *szRead, int nReadBufferSize);

// 0700 Video support
DVCAPI_API BOOL dvcHasExtVideo( HANDLE hDevice ) ;
DVCAPI_API BOOL dvcSetExtVideo( HANDLE hDevice, BOOL bOnOff ) ;
DVCAPI_API BOOL dvcGetExtVideo( HANDLE hDevice ) ;

// Switch for using HDL,HNL and NRR,NOR
// On by default,  enables HRT to async reset start of exposure
DVCAPI_API BOOL dvcSetExtReset( HANDLE hDevice, BOOL bOnOff ) ;
DVCAPI_API BOOL dvcGetExtReset( HANDLE hDevice ) ;

// Switch for setting Intensifier into PEX, external trigger mode
// Off by default,  enables AUX pin 4 to gate intensifier MCP on/off
DVCAPI_API BOOL dvcSetIntensifierExtTrigger( HANDLE hDevice, BOOL bOnOff ) ;
DVCAPI_API BOOL dvcGetIntensifierExtTrigger( HANDLE hDevice ) ;

//
// 1394 Devices only..
// Sets the priority of service threads, 
// default Capture and Expose run at THREAD_PRIORITY_NORMAL
// and Callback runs at THREAD_PRIORITY_ABOVE_NORMAL
//

DVCAPI_API BOOL dvcGetThreadPriorities( HANDLE hDevice, int *pCapture, int *pExpose, int *pCallback );
DVCAPI_API BOOL dvcSetThreadPriorities( HANDLE hDevice, int nCapture, int nExpose, int nCallback );

DVCAPI_API BOOL dvcSetSerialMode( HANDLE hSerial, int nBaud, int mData, int nStop, int nParity ) ;
DVCAPI_API BOOL dvcSetSerialTimeout( HANDLE hSerial, int nTimeOut )  ;
DVCAPI_API HANDLE dvcOpenSerial( char *szPortName, int nBaud, int nData, int nStop, int nParity ) ;
DVCAPI_API BOOL dvcCloseSerial( HANDLE hSerial ) ;
DVCAPI_API BOOL dvcReadSerial(HANDLE hSerial, char *buffer, int bytesToRead, int nTimeout);
DVCAPI_API BOOL dvcWriteSerial( HANDLE hSerial, char *szWriteBuffer, int bytesToWrite ) ;

DVCAPI_API BOOL dvcHasAuxTTL( HANDLE hDevice );
DVCAPI_API BOOL dvcSetAuxTTL( HANDLE hDevice, BOOL bManual, BOOL bHi );
DVCAPI_API BOOL dvcGetAuxTTL( HANDLE hDevice, BOOL *pManual, BOOL *pHi );

// Requires PortIOEnabled in dvcapi.ini
DVCAPI_API BOOL dvcSetPort( HANDLE hDevice, int nPortID, int nValue );
DVCAPI_API BOOL dvcGetPort( HANDLE hDevice, int nPortID, int *pValue );

DVCAPI_API BOOL dvcSetPreAmp( HANDLE hDevice, BOOL bOnOff, int nWhen );
DVCAPI_API BOOL dvcGetPreAmp( HANDLE hDevice, BOOL *pOnOff, int *pWhen );
DVCAPI_API BOOL dvcSetDefault( char *szAttr, char *szValue);
DVCAPI_API BOOL dvcSetDefaultInt( char *szAttr, int nDefault);
DVCAPI_API BOOL dvcSetDefaultDouble( char *szAttr, double dDefault);
DVCAPI_API BOOL dvcSetDefaultBool( char *szAttr, BOOL bDefault);

DVCAPI_API BOOL dvcGetDefault(char *szAttr, char *szValue, int nValueLen, char *szDefault);
DVCAPI_API int  dvcGetDefaultInt( char *szAttr, int nDefault);
DVCAPI_API double dvcGetDefaultDouble( char *szAttr, double dDefault);
DVCAPI_API BOOL dvcGetDefaultBool( char *szAttr, BOOL bDefault);

DVCAPI_API BOOL dvcGetCameraDefault(HANDLE hDevice,char *szAttr,char *szValue,int nValueLen,char *szDefault);
DVCAPI_API int dvcGetCameraDefaultInt(HANDLE hDevice,char *szAttr,int nDefault);
DVCAPI_API double dvcGetCameraDefaultDouble(HANDLE hDevice,char *szAttr,double dDefault) ;
DVCAPI_API BOOL dvcGetCameraDefaultBool(HANDLE hDevice,char *szAttr,BOOL bDefault) ;
DVCAPI_API BOOL dvcSetCameraDefault(HANDLE hDevice,char *szAttr,char *szValue);
DVCAPI_API BOOL dvcSetCameraDefaultInt(HANDLE hDevice,char *szAttr,int nValue);
DVCAPI_API BOOL dvcSetCameraDefaultDouble(HANDLE hDevice,char *szAttr,double dValue) ;
DVCAPI_API BOOL dvcSetCameraDefaultBool(HANDLE hDevice,char *szAttr,BOOL bDefault);

DVCAPI_API BOOL dvcGetAppDefault(char *szApp,char *szAttr,char *szValue,int nValueLen,char *szDefault);
DVCAPI_API int dvcGetAppDefaultInt(char *szApp,char *szAttr,int nDefault);
DVCAPI_API double dvcGetAppDefaultDouble(char *szApp,char *szAttr,double dDefault) ;
DVCAPI_API BOOL dvcGetAppDefaultBool(char *szApp,char *szAttr,BOOL bDefault) ;
DVCAPI_API BOOL dvcSetAppDefault(char *szApp,char *szAttr,char *szValue);
DVCAPI_API BOOL dvcSetAppDefaultInt(char *szApp,char *szAttr,int nValue);
DVCAPI_API BOOL dvcSetAppDefaultDouble(char *szApp,char *szAttr,double dValue) ;
DVCAPI_API BOOL dvcSetAppDefaultBool(char *szApp,char *szAttr,BOOL bDefault);

DVCAPI_API BOOL dvcSetStrobe( HANDLE hDevice, int eStrobe );
DVCAPI_API BOOL dvcGetStrobe( HANDLE hDevice, int *pStrobe );
DVCAPI_API BOOL dvcSetStrobeDelay( HANDLE hDevice, double dDelay );
DVCAPI_API BOOL dvcGetStrobeDelay( HANDLE hDevice, double *pdDelay );
DVCAPI_API BOOL dvcIsKodakDevice( HANDLE hDevice );
DVCAPI_API BOOL dvcIsSonyDevice( HANDLE hDevice );
DVCAPI_API BOOL dvcReadPromAttrString(HANDLE hDevice, char *attr, char *pValue, int nValueLen);
DVCAPI_API BOOL dvcReadPromAttrInt(HANDLE hDevice, char *attr, int *pValue);

DVCAPI_API BOOL dvcSetNonOverlappedExpose(HANDLE hDevice, BOOL bOnOff);
DVCAPI_API BOOL dvcGetNonOverlappedExpose(HANDLE hDevice, BOOL *pbOnOff);

DVCAPI_API void dvcSetWarningHandler(int dvcErr, dvcCallback2ProcP pFunc, PVOID pUserData);
DVCAPI_API void dvcSendWarning( int dvcErr, char *fmt, ... );

DVCAPI_API const char* dvcGetUserDir() ;
		// default [%HOMEDRIVE%]\DVCCo or %DVCUSER%
DVCAPI_API const char* dvcGetHomeDir() ;
		// default [%HOMEDRIVE%]\Program Files\DVCCo or %DVCHOME%
DVCAPI_API const char* dvcGetIniFile() ;
		// default [%HOMEDRIVE%]\Program Files\DVCCo\dvcapi.ini
		// or set by %DVCAPIINI%

#if defined(__cplusplus) && defined(NEEDS_DVC_IMAGE)
#include "./dvcImage.h"
#endif

// Obsolete
DVCAPI_API BOOL closeDVC(HANDLE hDevice);
DVCAPI_API HANDLE openDVC(void);
DVCAPI_API int	dvcExposing( HANDLE hDevice ) ;

DVCAPI_API BOOL dvcSetFilterWheelRampSteps( HANDLE hDevice, int nSteps ) ;
DVCAPI_API BOOL dvcGetFilterWheelRampSteps( HANDLE hDevice, int *pSteps ) ;
DVCAPI_API BOOL dvcSetFilterWheelRampTime( HANDLE hDevice, int nTime ) ;
DVCAPI_API BOOL dvcGetFilterWheelRampTime( HANDLE hDevice, int *pTime ) ;

DVCAPI_API BOOL dvcDrawRGBLine ( PBYTE pRGBBuffer, ULONG ulBytesPerRow, ULONG ulBitsPerPixel, ULONG ulStartX, ULONG ulStartY, ULONG ulEndX, ULONG ulEndY, BYTE  bRed, BYTE  bGreen, BYTE  bBlue) ;
DVCAPI_API BOOL dvcDrawRGBPixel ( PBYTE pRGBBuffer, ULONG ulBytesPerRow, ULONG ulBitsPerPixel, ULONG ulX, ULONG ulY, BYTE  bRed, BYTE  bGreen, BYTE  bBlue) ;

DVCAPI_API BOOL dvcGetCameraBaudRate(HANDLE hDevice, int *pBaudRate );
DVCAPI_API BOOL dvcSetCameraBaudRate(HANDLE hDevice, int nBaudRate );

DVCAPI_API BOOL dvcSetTDIShiftCount(HANDLE hDevice, int nCount );
DVCAPI_API BOOL dvcGetTDIShiftCount(HANDLE hDevice, int *pCount );
DVCAPI_API BOOL dvcSetTDIFrequency(HANDLE hDevice, double dHz );
DVCAPI_API BOOL dvcGetTDIFrequency(HANDLE hDevice, double *pdHz );
DVCAPI_API BOOL dvcSetTDICounter(HANDLE hDevice, int nOuterCounter, int nInnerCounter );
DVCAPI_API BOOL dvcGetTDICounter(HANDLE hDevice, int *pOuterCounter, int *pInnerCounter );

/** 
 * Return the MIL_ID's from
 * MappAlloc(), MsysAlloc() and MdigAlloc()
 */
DVCAPI_API BOOL dvcMatroxGetApplication( HANDLE hDevice, PVOID pApp );
DVCAPI_API BOOL dvcMatroxGetSystem(HANDLE hDevice, PVOID pSys);
DVCAPI_API BOOL dvcMatroxGetDigitizer(HANDLE hDevice, PVOID pDig);
/**
 * Install a callback (HOOK Function) for MdigProcess() calls
 * long (*pHookFunc)(long HookType, MIL_ID EventID, void *pUser)
 */
DVCAPI_API BOOL dvcMatroxSetProcessCB( HANDLE hDevice, PVOID pHookFunc, PVOID pUser);
DVCAPI_API BOOL dvcGetFlushInterval( HANDLE hDevice, double *pInterval);

/**
 * 1500M with Enhanced QE support
 */

DVCAPI_API BOOL dvcSetEQE(HANDLE hDevice, BOOL bOnOff);
DVCAPI_API BOOL dvcGetEQE(HANDLE hDevice, BOOL *pbOnOff);


#if defined(__cplusplus)
}
#endif

#endif	// _DVC_API_H_

