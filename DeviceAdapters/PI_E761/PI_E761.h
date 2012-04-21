// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PI_E761_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PI_E761_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PI_E761_EXPORTS
#define PI_E761_API __declspec(dllexport)
#else
#define PI_E761_API __declspec(dllimport)
#endif

///////////////////////////////////////////////////////////////////////////////
// FILE:          PI_E761.h
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   This is the device adapter for PI_E761 piezo stages.
//                
// AUTHOR:        Zephyre, haizi.zh@gmail.com, 02/04/2012
//
// COPYRIGHT:     SM4, Institute of Physics, CAS
//
// LICENSE:       This file is distributed under the BSD license.
//                License text is included with the source distribution.
//
//                This file is distributed in the hope that it will be useful,
//                but WITHOUT ANY WARRANTY; without even the implied warranty
//                of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//                IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//                CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//                INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.
//

#ifndef _PI_E761_H_
#define _PI_E761_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceThreads.h"
#include <string>
#include <map>
#include <algorithm>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// CPI_E761_ZStage class
// the z-axis stage
//////////////////////////////////////////////////////////////////////////////

class CPI_E761_ZStage : public CStageBase<CPI_E761_ZStage>
{
public:
	CPI_E761_ZStage();
	~CPI_E761_ZStage();

	bool IsInitialized() { return initialized_; }

	// Device API
	// ----------
	int Initialize();
	int Shutdown();
	bool Busy() { return busy_; }	// TODO
	void GetName(char* pszName) const;

	// Stage API
	// ---------
	int SetPositionUm(double pos);
	int GetPositionUm(double& pos);
	int SetPositionSteps(long steps);
	int GetPositionSteps(long& steps);
	int SetOrigin();
	int GetLimits(double& min, double& max);
	int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
	bool IsContinuousFocusDrive() const {return false;}
	virtual int Move(double pos) { return SetPositionUm(pos); }

	// action interface
	// ----------------
	int OnStepSizeUm(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAxisName(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnCurrentAxis(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAxisInfo(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSoftLowerLimit(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSoftHigherLimit(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnTravelHighEnd(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnTravelLowEnd(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnVelControl(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnVersion(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
	bool busy_;
	double stepSizeUm_;
	double axisLimitUm_;
	bool initialized_;
	int currentAxis_;
	char axisName_[64];
	string axisInfo_;

	void refreshPosLimit();

	int processErr();
};

//////////////////////////////////////////////////////////////////////////////
// CPI_E761_XYStage class
// the xy-axis stage
//////////////////////////////////////////////////////////////////////////////

class CPI_E761_XYStage : public CXYStageBase<CPI_E761_XYStage>
{
public:
	CPI_E761_XYStage();
	~CPI_E761_XYStage();

	static const char* PropXPos;
	static const char* PropYPos;

	bool IsInitialized() { return initialized_; }

	// Device API
	// ----------
	int Initialize();
	int Shutdown();
	bool Busy() { return busy_; }	// TODO
	void GetName(char* pszName) const;

	virtual int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
	virtual int SetPositionSteps(long x, long y);
	virtual int GetPositionSteps(long& x, long& y);
	virtual int Home();
	virtual int Stop();
	virtual int SetOrigin();
	virtual int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
	virtual double GetStepSizeXUm() { return stepSizeUm_; }
	virtual double GetStepSizeYUm() { return stepSizeUm_; }
	virtual int IsXYStageSequenceable(bool& isSequenceable) const { isSequenceable = false; return DEVICE_OK; }

	virtual int SetXPositionUm(double x);
	virtual int SetYPositionUm(double y);
	virtual int SetXPositionSteps(long xs);
	virtual int SetYPositionSteps(long ys);

	// Properties
	int OnStepSizeUm(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAxisName(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnXPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnYPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnTravelRange(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
	bool initialized_;
	bool busy_;
	double stepSizeUm_;
	int currentAxis_[2];
	char axisName_[64];
	char xaxisName_[64];
	char yaxisName_[64];
	int processErr();
};

#endif //_PI_E7611_H
