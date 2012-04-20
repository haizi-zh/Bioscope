
// dvcdefs.h
// (C) DVC Company, Inc., 2002,2003  All rights reserved.

#ifndef _DVC_DEFS_H_
#define _DVC_DEFS_H_

#ifndef _DVC_C_TYPES_H_
#define _DVC_C_TYPES_H_
#define _TIFF_DATA_TYPEDEFS_	// catch tiff.h typedefs

typedef unsigned char  		UInt8, uint8, *uint8ptr, *PUInt8 ;
typedef signed char 		Int8, int8, *int8ptr, *PInt8 ;
typedef unsigned short 		UInt16, uint16, *uint16ptr, *PUInt16 ;
typedef signed short   		Int16, int16, *int16ptr, *PInt16 ;
typedef unsigned int 		UInt32, uint32, *uint32ptr, *PUInt32 ;
typedef signed int   		Int32, int32, *int32ptr, *PInt32 ;
typedef unsigned __int64 	UInt64, uint64, *uint64ptr, *PUInt64 ;
typedef signed __int64   	Int64, int64, *int64ptr, *PInt64 ;

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#endif

typedef struct CameraListStruct {
	ULONG	theGUID ;
	ULONG	model ;
	ULONG	boardId ;
	char	szDeviceName[MAX_PATH] ; // Need unique name/identifier
} CameraListStruct, *CameraListStructP ;

enum DVCInterfaceTypes 
{
	DVC_UNKNOWN_Interface = 0,
	DVC_1394_Interface    = 1,		// Firewire
	DVC_EDT_Interface	  = 2,		// Camera Link 
	DVC_Epix_Interface    = 3,		// LVDS
	DVC_NET_Interface     = 4,		// GigE
	DVC_ECL_Interface	  = 5,		// Euresys Camera Link
	DVC_MATROX_Interface  = 6		// Matrox Camera Link
} ;

typedef struct {
	ULONG	          nCameras ;
	CameraListStruct  theCameras[64] ;
} CameraListArrayStruct, *CameraListArrayStructP ;


typedef enum {
		DVC_BIN_1X1	=	0,
		DVC_BIN_1X2	=	1,
		DVC_BIN_2X2	=	2,
		DVC_BIN_4X4	=	3,
		DVC_BIN_8X8	=	4,
		DVC_BIN_3X3	=	5,	// Added for CCD_4000M, CCD_2000M
		DVC_BIN_NXM =	6	// Variable 4000 binning - use Set/GetHVBin()
} dvcBinModes ;

typedef enum {
		DVC_FORMAT_RAW = 0,
		DVC_FORMAT_RGB8,	/* Three 8-bit values per pixel */
		DVC_FORMAT_RGB16,	/* Three 16-bit values per pixel */
		DVC_FORMAT_BAYER_10_RG,
		DVC_FORMAT_BAYER_10_GR,
		DVC_FORMAT_BAYER_10_BG,
		DVC_FORMAT_BAYER_10_GB,
		DVC_FORMAT_BAYER_12_RG,
		DVC_FORMAT_BAYER_12_GR,
		DVC_FORMAT_BAYER_12_BG,
		DVC_FORMAT_BAYER_12_GB,
		DVC_FORMAT_MONO_8,
		DVC_FORMAT_MONO_10,
		DVC_FORMAT_MONO_12,
		DVC_FORMAT_MONO_16,
} dvcFORMAT ;

typedef enum {
		DVC_STATUS_ERROR		=-1,
		DVC_STATUS_NO_STREAM	=0,
		DVC_STATUS_STOPPED		=1,
		DVC_STATUS_RUNNING		=2,
		DVC_STATUS_PAUSED		=3,
		DVC_STATUS_IN_TRANSITION=4,
		DVC_STATUS_EXPOSING		=5,
		DVC_STATUS_READING		=6
} dvcStatusTypes ;

typedef enum {
	DVC_IDLE 				= 0,
	DVC_EXPOSING			= 1,
	DVC_READING_DATA		= 2,
	DVC_READ_COMPLETE		= 4,
	DVC_DMA_IN_PROGRESS		= 8,
	DVC_START_SEQUENCE		= 16,
	DVC_SEQUENCE_COMPLETE	= 32,
	DVC_SEQUENCE_ABORTED	= 64,
	DVC_SYNC_ERR			= 0x80000000,
} dvcExposeStates ;

typedef enum {
	MODE_HDL=0,
	MODE_NRR=1,
	MODE_NFR=2,
	MODE_ULT=3,
	MODE_PDX=4,
	MODE_NOR=5,
	MODE_HDO=6,
	MODE_HNL=7,
	MODE_PDI=8,
	MODE_PDP=9,
	MODE_INC=10,
	MODE_PRC=11,
	MODE_POS=12,
	MODE_KSH=13,	// Shuttered, continuous
	MODE_HDX=14,	// OneShot long
	MODE_UNKNOWN 
} dvcExposeModes ;

static char *dvcModeNames[MODE_UNKNOWN+1] = {
		"HDL", "NRR", "NFR", "ULT", "PDX", "NOR",
		"HDO", "HNL", "PDI", "PDP",
		"INC", "PRC", "POS",
		"KSH", "HDX", "???"
} ;

typedef enum {
		DVC_STREAM_CLOSED	= 0,
		DVC_STREAM_OPEN,
		DVC_STREAM_RUNNING,
		DVC_STREAM_SHARED
} dvcAPIStreamStates ;

typedef enum {
	INTEN_MODE_OFF=0,
	INTEN_MODE_SHORT=1,
	INTEN_MODE_LONG=2,
	INTEN_MODE_ON=3,
	INTEN_MODE_EXT=4,
	INTEN_MODE_AUTO=5,
	INTEN_MODE_AUTO_SHORT=6,
	INTEN_MODE_AUTO_LONG=7
} dvcIntensifierModes ;

typedef enum {
	FILTER_STOPPED = 0,
	FILTER_MOVING  = 1,
	FILTER_ERROR   = 2
} dvcFilterStates ;

typedef enum {
	CCD_UNKNOWN = 0,
	CCD_1312M,
	CCD_1312C,
	CCD_1310M,
	CCD_1310C,
	CCD_1412M,
	CCD_1412C,
	CCD_0700M,
	CCD_0700C,
	CCD_2000M,
	CCD_2000C,
	CCD_4000M,
	CCD_4000C,
	CCD_0340M,
	CCD_0340C,
	CCD_1500M,
	CCD_1500C,
	CCD_16000M,
	CCD_16000C,
	CCD_PROTO
} dvcCCDTypes ; 

static char *dvcCameraNames[CCD_PROTO+1] = {
	"UNKNOWN",
	"DVC 1312M",
	"DVC 1312C",
	"DVC 1310M",
	"DVC 1310C",
	"DVC 1412M",
	"DVC 1412C",
	"DVC 0710M",
	"DVC 0710C",
	"DVC 2000M",
	"DVC 2000C",
	"DVC 4000M",
	"DVC 4000C",
	"DVC 0340M",
	"DVC 0340C",
	"DVC 1500M",
	"DVC 1500C",
	"DVC 16000M",
	"DVC 16000C",
	"DVC PROTO"
} ;

#define DVC_MAX_HIST_SIZE 	65536
#define DVC_MAX_HIST_VALUE 	(DVC_MAX_HIST_SIZE-1)

typedef enum {
		DVC_FATAL = -1,
		DVC_WARN  =  0,
		DVC_DEBUG =  1
} dvcErrLevels ;

typedef enum {
		DVC_ERR_OK 				= 0,
		DVC_ERR_HANDLE 			= 1,
		DVC_ERR_NULL_PARAM		= 2,
		DVC_ERR_BUFFER_SIZE		= 3,
		DVC_ERR_EXCEPTION		= 4,
		DVC_ERR_IOCTL_READ		= 5,
		DVC_ERR_ILLEGAL_PARAM	= 6,
		DVC_ERR_ALLOCATION		= 7,
		DVC_ERR_CREATE_FILE		= 8,
		DVC_ERR_WRITE_FILE		= 9,
		DVC_ERR_READ_FILE		= 10,
		DVC_ERR_ILLEGAL_SIZE	= 11,
		DVC_ERR_TIMEOUT			= 12,
		DVC_ERR_SERIAL			= 13,
		DVC_ERR_MODE			= 14,
		DVC_ERR_BINNING			= 15,
		DVC_ERR_SCANRATE		= 16,
		DVC_ERR_EVENT			= 17,
		DVC_ERR_THREAD			= 18,
		DVC_ERR_START_STREAM	= 19,
		DVC_ERR_MODULE			= 20,
		DVC_ERR_SUPPORTED		= 21,
		DVC_ERR_RESOURCE		= 22,
		DVC_ERR_EPIX			= 23,
		DVC_ERR_1394			= 24,
		DVC_ERR_EDT				= 25,			// = 25
		DVC_ERR_FILTER			= 26,
		DVC_ERR_INTENSIFIER		= 27,
		DVC_ERR_ROI				= 28,
		DVC_ERR_TEMP			= 29,
		DVC_ERR_NET				= 30,
		DVC_ERR_DISCONNECT		= 31,
		DVC_ERR_ECL				= 32,
		DVC_ERR_MATROX			= 33,
		/* Must be last */
		DVC_ERR_UNKNOWN
} dvcErrCodes ;

static char *dvcErrMessages[] = {
		"No Error",
		"Invalid Handle",
		"NULL parameter passed",
		"Illegal buffer size",
		"Memory Exception",
		"Driver I/O Error",				// = 5
		"Illegal parameter",	
		"Memory Allocation Failed",
		"Create File Failed",
		"Write File Failed",
		"Read File Failed",				// = 10
		"Illegal Image Size",
		"Operation timed out",
		"RS232 Communication Error",
		"Illegal mode operation",
		"Illegal Binning operation",	// = 15
		"Illegal Scanrate operation",
		"Illegal Event Handle",
		"Illegal Thread Handle",
		"Error Starting Stream",
		"Illegal Module Operation",		// = 20
		"Unsupported Operation",
		"Resource Busy",
		"Epix Error",
		"1394 Error",
		"Edt Error",					// = 25
		"Filterwheel Error",
		"Intensifier Error",
		"Illegal ROI Error",
		"Temperature Error",
		"Network Error",				// = 30
		"Disconnect Error",
		"Euresys Multicam Error",
		"Matrox Error",
		/* Must be last */
		"Unknown Error"
	} ;

#define DVCError(err) dvcSetErrMsg(DVC_FATAL,err,"Error",__FILE__,__LINE__)
#define DVCWarn(err)  dvcSetErrMsg(DVC_WARN,err,"Warning",__FILE__,__LINE__)
#define DVCDebug(err) dvcSetErrMsg(DVC_DEBUG,err,"Debug",__FILE__,__LINE__)

typedef struct _dvcHistogramData {
		int    Size ;
		double dSum ;
		double dSumSqr ;
		ULONG  ulPixels ;
		ULONG  ulMin ;
		ULONG  ulMax ;
		ULONG  ulFreq[DVC_MAX_HIST_SIZE] ;
} dvcHist, *dvcHistP ;

typedef enum _dvcRenderMethods {
	DVC_RENDER_RGB = 0,
	DVC_RENDER_TIFF,
	DVC_RENDER_BMP,
	DVC_RENDER_JPG,
	DVC_RENDER_JP2,
	DVC_RENDER_PNG,
	// Insert new types here..
	DVC_RENDER_UNKNOWN
} dvcRenderModes ;

typedef struct _dvcRenderControl_t {
	USHORT redMask, redBits, redShift ;
	USHORT greenMask, greenBits, greenShift ;
	USHORT blueMask,  blueBits,  blueShift ;
	int	nLutSize ;
	int	oLutBits ;	// if !8, cast the following to PUSHORT..
	PBYTE  pRedLut ;
	PBYTE  pGreenLut ;
	PBYTE  pBlueLut ;
	int eMethod ;
 } dvcRenderControl_t;

typedef struct _ImageMetaData_ 
{
	double dFrameTime ;
	double dExposeTime ;
	double dTransferTime ;
	ULONG ulFrameCount ; 
	ULONG ulStreamCount ; 
	int nBinning ;
	int nGain ;
	int nOffset ;
	RECT camRect ;
	int nFilterPos ;
	int nIntensifierGain ;
	int nIntensifierMode ;
	int nIntensifierDuration ;
	int hBin ;
	int vBin ;
	ULONG ulExposeCount ;
	ULONG ulTriggerCount ;
	ULONG ulRingBuffer ;
	ULONG ulBufferReadCount ;
	ULONG ulBufferWriteCount ;
	double dExposeTimeStamp ;
	double dTriggerTimeStamp ;
	int _nPad[2];
} ImageMetaData, *ImageMetaDataP ;


typedef struct _dvcCallback_t {
	HANDLE	hDevice ;
	PUSHORT pImageBuffer ;
	ULONG	ulWidth ;
	ULONG	ulHeight ;
	ULONG	ulFrame ;
	double	dTimeStamp ;
	PVOID	pUserData ;
	ImageMetaData sMeta ;
} dvcCallbackData, *dvcCallbackDataP ;

typedef DWORD (*dvcCallbackProcP)(PVOID);
typedef DWORD (*dvcCallback2ProcP)(PVOID,PVOID);
typedef DWORD (*dvcCallbackNProcP)(PVOID,...);

typedef enum {
	aGain	 = 0,
	aOffset,
	aBinning,
	aScanRate,
	aExposureMode,
	aExposureDuration,
	aIntensifierMode,
	aIntensifierGain,
	aIntensifierDelay,
	aIntensifierDuration,
	aFilterWheelPosition,
	aFilterWheelSpeed,
	aExternalVideo,
	aEnableROI,
	aROITop,
	aROIBottom,
	// Insert new parameters here..
	aIllegalAttr
}  dvcCameraParameters ;

static char *dvcAttributeNames[aIllegalAttr+1] = { 
	"Gain",
	"Offset",
	"Binning",
	"ScanRate",
	"ExposureMode",
	"ExposureDuration",
	"IntensifierMode",
	"IntensifierGain",
	"IntensifierDelay",
	"IntensifierDuration",
	"FilterWheelPosition",
	"FilterWheelSpeed",
	"ExternalVideo",
	"EnableROI",
	"ROITop",
	"ROIBottom",
	"UnknownAttribute"
} ;


#define MAX_DVC_DEVICES		8

typedef enum {
	dvcTriggerTimed = 0,
	dvcTriggerBulb,
	dvcTriggerStrobe,
	dvcTriggerSequence,
	dvcTDI,
	dvcTDIExt
} dvcTriggerModes;

typedef enum {
	dvcStrobeLow = 0,
	dvcStrobeHigh,
	dvcStrobeActiveLow,
	dvcStrobeActiveHigh
} dvcStrobeStates ;

typedef struct _CameraCtl_ {
	HANDLE hDevice ;
	double dExpose ;
	int    nFilterPos ;
	int    nBin ;
	int    nGain ;
	int    nOffset ;
	int    nScanRate ;
	int    nRoiOn ;
	int    nIntensifierGain ;
	int    nIntensifierMode ;
	int    nIntensifierDuration ;
	RECT   roi ;
	PVOID  pvExt ;
	int	   nStatus ;
	int	   nTrigger ;
} CameraCtl, *CameraCtlP ;

/* Buffer structure for returning image data */
typedef struct _dvcBufStruct_ {
	int 			nBuffers ;
	int 			nXOffset, nYOffset ;
	int 			nWidth, nHeight ;
	PUSHORT 		*pBuffers ;
	PBYTE			pBufferStatus ;
	ImageMetaDataP 	pMeta ;
	int				_nAllocatedWidth, _nAllocatedHeight;
} dvcBufStruct, *dvcBufStructP ;

/*
 * API sets DVC_BUFFER_BUSY while reading
 * API sets DVC_BUFFER_FULL when finished filling a buffer
 * User APP sets DVC_BUFFER_IDLE when finished
 * API sets DVC_BUFFER_OVERFLOW	if not IDLE and reused
 */
#define DVC_BUFFER_IDLE			0
#define DVC_BUFFER_BUSY			1
#define DVC_BUFFER_FULL			2
#define DVC_BUFFER_OVERFLOW		64
#define DVC_BUFFER_ERROR		128

#ifdef VC_6
#define strncpy_s(a,b,c,d) 	strncpy(a,c,b)
#define strncat_s(a,b,c,d) 	strncat(a,c,b)
#define strcpy_s(a,b,c)		strcpy(a,c)
#define strcat_s(a,b,c)		strcat(a,c)
#define	strtok_s(a,b,c)		strtok(a,b)
#define sscanf_s			sscanf
#define sprintf_s			_snprintf
#define vsprintf_s			_vsnprintf
#endif

#endif	// _DVC_DEFS_H_

