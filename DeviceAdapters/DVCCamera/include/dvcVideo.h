// dvcVideo.h: interface for the CDVCVideo class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVCVIDEO_H__3CE1A7C7_77CA_48DC_8ACF_AAA8BC86BC89__INCLUDED_)
#define AFX_DVCVIDEO_H__3CE1A7C7_77CA_48DC_8ACF_AAA8BC86BC89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _DVC_API_H_
#include "./dvcapi.h"
#endif

// Abstract class for receiving a video stream
class DVCAPI_API CDVCVideoReceiver  
{
public:
	CDVCVideoReceiver();
	virtual ~CDVCVideoReceiver();
	virtual void	streamChangeEvent( int nCamera, int nGoStop ) = 0;
	virtual void	deviceChangeEvent( int nCamera, int nStatus ) = 0;
	virtual void	frameUpdateEvent( int nCamera, int nFrame ) = 0;
	virtual void	restartVideo( int nCamera ) = 0;
	virtual void	suspend() = 0 ;
	virtual void	resume() = 0;
} ;

// Abstract class for sending a video stream

class DVCAPI_API CDVCVideo  
{
public:
	CDVCVideo();
	virtual ~CDVCVideo();
	virtual BOOL	begin() = 0 ;
	virtual BOOL	end() = 0 ;
	virtual BOOL	next() = 0 ;
	virtual BOOL	previous() = 0 ;
	virtual BOOL	last() = 0 ;
	virtual BOOL	isImageFile() =  0;
	virtual BOOL	isCamera() = 0;
	virtual BOOL	running() = 0;
	virtual void	run() = 0;
	virtual void	pause(BOOL bPause) = 0;
	virtual void	stop() = 0;
	virtual CDVCVideo *GetVideo() = 0 ;
	virtual void	addReceiver(CDVCVideoReceiver *pCRcvr ) = 0;
	virtual void	removeReceiver(CDVCVideoReceiver *pCRcvr ) = 0;
	virtual void	SendDeviceEvent() = 0;
	virtual void	SendFrameEvent() = 0;
	virtual void	SendStreamEvent() = 0;
	virtual void	SendRestart() = 0;
};


#endif // !defined(AFX_DVCVIDEO_H__3CE1A7C7_77CA_48DC_8ACF_AAA8BC86BC89__INCLUDED_)
