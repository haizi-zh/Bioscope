// DVCCamera.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DVCCamera.h"
#include <iostream>

using namespace std;

/////////////////////////// START HERE /////////////////////////////////

// All the DVC error code will be returned plus this.
const int g_Err_Offset = 10000;

// External names used used by the rest of the system
// to load particular device from the "DemoCamera.dll" library
const char* g_DVCCameraDeviceName = "DVCCamera";

// singleton instance
DVCCamera* DVCCamera::instance_ = 0;
unsigned int DVCCamera::refCount_ = 0;

// Number of user buffers
const int g_UserBufferNumber = 48;

// global Andor driver thread lock
MMThreadLock g_DVCCamDriverLock;

map<int, string> DVCCamera::camTypeMap_;

// Properties
const char g_CameraName[] = "Camera Name";
const char g_CameraType[] = "Camera Type";
const char g_SSN[] = "Camera Serial Number";
const char g_Width[] = "Camera Width";
const char g_Height[] = "Camera Height";
const char g_Depth[] = "Bit Depth";
const char g_GaindB[] = "Gain (dB)";
const char g_GaindBRange[] = "Gain Range (dB)";
const char g_Binning[] = "Binning";
const char g_hBinning[] = "hBin";
const char g_vBinning[] = "vBin";
const char g_ActualFrameTime[] = "Actual Frame Time (ms)";
const char g_PixelClock[] = "Pixel Clock Frequency (Hz)";
const char g_ScanRate[] = "Scan Rate";
const char g_Label[] = "Label";
const char g_MD_ExposeTime[] = "Exposure Time";
const char g_MD_ExposeTimestamp[] = "Exposure Timestamp";
const char g_MD_FrameTimestamp[] = "Frame Timestamp";
const char g_MD_TransferTime[] = "Transfer Time";
const char g_MD_TriggerTimestamp[] = "Trigger Timestamp";

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

/**
 * List all suppoerted hardware devices here
 * Do not discover devices at runtime.  To avoid warnings about missing DLLs, Micro-Manager
 * maintains a list of supported device (MMDeviceList.txt).  This list is generated using 
 * information supplied by this function, so runtime discovery will create problems.
 */
DVCCAMERA_API void InitializeModuleData()
{
	AddAvailableDeviceName(g_DVCCameraDeviceName, "DVC GigE Camera");
}

DVCCAMERA_API MM::Device* CreateDevice(const char* deviceName)
{
	if (deviceName == 0)
		return 0;

	if (strcmp(deviceName, g_DVCCameraDeviceName) == 0)
		return DVCCamera::GetInstance();

	// ...supplied name not recognized
	return 0;
}

DVCCAMERA_API void DeleteDevice(MM::Device* pDevice)
{
	delete pDevice;
}

DriverGuard::DriverGuard(const DVCCamera * cam)
{
    g_DVCCamDriverLock.Lock();
    //if (cam != 0 && cam->GetNumberOfWorkableCameras() > 1)
    //{
	   // // must be defined as 32bit in order to compile on 64bit systems since GetCurrentCamera 
    //    // only takes 32bit -kdb		
    //    at_32 currentCamera;
    //    GetCurrentCamera(&currentCamera);
    //    if (currentCamera != cam->GetMyCameraID())
    //    {
    //    int ret = SetCurrentCamera(cam->GetMyCameraID());
    //    if (ret != DRV_SUCCESS)
    //        printf("Error switching active camera");
    //    }
    //}
}

DriverGuard::~DriverGuard()
{
    g_DVCCamDriverLock.Unlock();
}

void DVCCamera::GetName(char* name) const 
{
	CDeviceUtils::CopyLimitedString(name, g_DVCCameraDeviceName);
}

///////////////////////////////////////////////////////////////////////////////
// Continuous acquisition
//

DVCCamera::DVCCamera() :
initialized_(false),
hCam_(NULL),
busy_(false),
currentCam_(0),
camType_(0),
camSSN_(0),
fullFrameX_(0),
fullFrameY_(0),
depth_(0),
expMs_(0.0),
sequenceRunning_(false),
bufNumber(48),
fullFrameBuffer_(NULL),
binSize_(1),
sequenceLength_(0),
intervalMs_(0),
imageCounter_(0),
startTime_(0),
thd_(NULL),
sequencePaused_(false),
stopOnOverflow_(true)
{
	InitializeDefaultErrorMessages();
	// add custom messages
	SetErrorText(ERR_BUSY_ACQUIRING, "Camera Busy.  Stop camera activity first.");
	SetErrorText(ERR_NO_AVAIL_AMPS, "No available amplifiers.");
	SetErrorText(ERR_TRIGGER_NOT_SUPPORTED, "Trigger Not supported.");
	SetErrorText(ERR_INVALID_VSPEED, "Invalid Vertical Shift Speed.");
	SetErrorText(ERR_INVALID_PREAMPGAIN, "Invalid Pre-Amp Gain.");
	SetErrorText(ERR_CAMERA_DOES_NOT_EXIST, "No Camera Found.  Make sure it is connected and switched on, and try again.");
	SetErrorText(ERR_SOFTWARE_TRIGGER_IN_USE, "Only one camera can use software trigger."); 

	//seqThread_ = new AcqSequenceThread(this);

	CameraListArrayStruct cams;
	if (!dvcGetListOfCameras(&cams))
		LogMessage("No DVC camera found!");
	for (int i=0; i < (int)cams.nCameras; i++)
	{
		camMap_.insert(pair<int, HANDLE>(i+1, (HANDLE)NULL));
	}
	currentCam_ = 1;

	// Initialize the camera type map
	camTypeMap_.insert(pair<int, string>(0, "Unknow camera"));
	camTypeMap_.insert(pair<int, string>(1, "Sony 085 12 bit monochrome: 1312M"));
	camTypeMap_.insert(pair<int, string>(2, "Sony 085 12 bit color: 1312C"));
	camTypeMap_.insert(pair<int, string>(3, "Sony 085 10 bit monochrome: 1310M"));
	camTypeMap_.insert(pair<int, string>(4, "Sony 085 10 bit color: 1310C"));
	camTypeMap_.insert(pair<int, string>(5, "Sony 285 12 bit monochrome: 1412M"));
	camTypeMap_.insert(pair<int, string>(6, "Sony 285 12 bit color: 1412C"));
	camTypeMap_.insert(pair<int, string>(7, "Sony 428 10 bit monochrome: 700M"));
	camTypeMap_.insert(pair<int, string>(8, "Sony 428 10 bit color: 700C"));
	camTypeMap_.insert(pair<int, string>(9, "Kodak 2020 12 bit monochrome: 2000M"));
	camTypeMap_.insert(pair<int, string>(10, "Kodak 2020 12 bit color: 2000C"));
	camTypeMap_.insert(pair<int, string>(11, "Kodak 4021 12 bit monochrome: 4000M"));
	camTypeMap_.insert(pair<int, string>(12, "Kodak 4021 12 bit color: 4000C"));
	camTypeMap_.insert(pair<int, string>(13, "Kodak 340 12 bit monochrome: 340M"));
	camTypeMap_.insert(pair<int, string>(14, "Kodak 340 12 bit monochrome: 340C"));

}

DVCCamera::~DVCCamera()
{
   DriverGuard dg(this);

   refCount_--;
   if (refCount_ == 0) {
      // release resources
      if (initialized_) {
         //SetToIdle();
         //int ShutterMode = 2;  //0: auto, 1: open, 2: close
         //SetShutter(1, ShutterMode, 20,20);//0, 0);
      }


      //if (initialized_ && mb_canSetTemp) {
      //   CoolerOFF();  //turn off the cooler at shutdown
      //}

      if (initialized_) {
         Shutdown();
      }
	  dvcReleaseUserBuffers(&userBuffers_);

      // clear the instance pointer
      instance_ = NULL;
   }
}

DVCCamera* DVCCamera::GetInstance()
{
	instance_ = new DVCCamera();

	refCount_++;
	return instance_;
}

int DVCCamera::processErr()
{
	int err = dvcGetLastErr();
	const char* msg = dvcGetLastErrMsg();
	if (msg != NULL)
		SetErrorText(err + g_Err_Offset, msg);
	return err;
}

int DVCCamera::Initialize()
{
	if (initialized_)
		return DEVICE_OK;

	int ret;

	const HANDLE hCam = dvcOpenCamera(currentCam_);
	if (hCam == NULL)
		return processErr();
	camMap_[currentCam_] = hCam;
	hCam_ = hCam;

	thd_ = new DVCCamera::AcqSequenceThread(this, hCam);
	dvcAllocateUserBuffers(hCam, &userBuffers_, g_UserBufferNumber);

	if (!dvcResetCamera(hCam))
		return processErr();

	// Description
	if (!HasProperty(MM::g_Keyword_Description))
	{
		ret = CreateProperty(MM::g_Keyword_Description, "DVC camera adapter", MM::String, true);
		if (ret != DEVICE_OK)
			return ret;
	}

	// Dummy property for getPixelSizeUm
	if (!HasProperty(g_Label))
	{
		ret = CreateProperty(g_Label, "Dummy", MM::String, false);
		assert(ret == DEVICE_OK);
	}

	// Camera name
	{
		char str[1024];
		camType_ = dvcGetCameraName(hCam, str, 1024);
		camName_ = str;
	}
	if (!HasProperty(g_CameraName))
	{
		ret = CreateProperty(g_CameraName, camName_.c_str(), MM::String, true);
		if (ret != DEVICE_OK)
			return ret;
	}
	if (!HasProperty(g_CameraType))
	{
		ret = CreateProperty(g_CameraType, camTypeMap_[camType_].c_str(), MM::String, true);
		if (ret != DEVICE_OK)
			return ret;
	}

	// Camera serial number
	if (!dvcGetCameraSerialNumber(hCam, &camSSN_))
	{
		return processErr();
	}
	if (!HasProperty(g_SSN))
	{
		char str[64];
		_itoa_s(camSSN_, str, 64, 10);
		ret = CreateProperty(g_SSN, str, MM::String, true);
		if (ret != DEVICE_OK)
			return ret;
	}

	fullFrameX_ = dvcGetCCDWidth(hCam);
	fullFrameY_ = dvcGetCCDHeight(hCam);
	if (!HasProperty(g_Width))
	{
		char str[64];
		_itoa_s(fullFrameX_, str, 64, 10);
		ret = CreateProperty(g_Width, str, MM::Integer, true);
		if (ret != DEVICE_OK)
			return ret;
	}
	if (!HasProperty(g_Height))
	{
		char str[64];
		_itoa_s(fullFrameY_, str, 64, 10);
		ret = CreateProperty(g_Height, str, MM::Integer, true);
		if (ret != DEVICE_OK)
			return ret;
	}
	roi_.x = 0;
	roi_.y = 0;
	roi_.xSize = fullFrameX_;
	roi_.ySize = fullFrameY_;

	depth_ = dvcGetNBits(hCam);
	if (!HasProperty(g_Depth))
	{
		char str[64];
		_itoa_s(depth_, str, 64, 10);
		ret = CreateProperty(g_Depth, str, MM::Integer, true);
		if (ret != DEVICE_OK)
			return ret;
	}

	fullFrameBuffer_ = new unsigned short[fullFrameX_ * fullFrameY_];
	ResizeImageBuffer();

	ret = createGainProp();
	if (ret != DEVICE_OK)
		return ret;

	ret = createBinProp();
	if (ret != DEVICE_OK)
		return ret;

	expMs_ = GetExposure();
	if (!HasProperty(MM::g_Keyword_Exposure))
	{
		CPropertyAction* pAct = new CPropertyAction(this, &DVCCamera::OnExposure);
		ret = CreateProperty(MM::g_Keyword_Exposure, "", MM::Float, false, pAct);
		if (ret != DEVICE_OK)
			return ret;
	}

	if (!HasProperty(g_ActualFrameTime))
	{
		CPropertyAction* pAct = new CPropertyAction(this, &DVCCamera::OnActualFrameTime);
		ret = CreateProperty(g_ActualFrameTime, "", MM::Float, true, pAct);
		if (ret != DEVICE_OK)
			return ret;
	}

	if (!HasProperty(g_PixelClock))
	{
		CPropertyAction* pAct = new CPropertyAction(this, &DVCCamera::OnPixelClock);
		ret = CreateProperty(g_PixelClock, "", MM::Float, true, pAct);
		if (ret != DEVICE_OK)
			return ret;
	}

	assert(createScanRateProp() == DEVICE_OK);

	// Set Mirror X to 1
	SetProperty(MM::g_Keyword_Transpose_MirrorX, "1");

	if (!dvcAllocateUserBuffers(hCam, &dvcBuf, bufNumber))
		return processErr();

	if (!dvcSetUserBuffers(hCam, &dvcBuf))
		return processErr();



	return DEVICE_OK;
}

int DVCCamera::createScanRateProp()
{
	if (!HasProperty(g_ScanRate))
	{
		CPropertyAction* pAct = new CPropertyAction(this, &DVCCamera::OnScanRate);
		int ret = CreateProperty(g_ScanRate, "", MM::String, false, pAct);
		assert(ret == DEVICE_OK);
		scanRates_.push_back("20 MHz");
		scanRates_.push_back("40 MHz");
		ret = SetAllowedValues(g_ScanRate, scanRates_);
		assert(ret == DEVICE_OK);
	}
	return DEVICE_OK;
}

int DVCCamera::createGainProp()
{
	int ret = 0;
	CPropertyAction* pAct = NULL;
	if (!HasProperty(g_GaindB))
	{
		pAct = new CPropertyAction(this, &DVCCamera::OnGaindB);
		ret = CreateProperty(g_GaindB, "", MM::Float, false, pAct);
		if (ret != DEVICE_OK)
			return ret;
	}
	if (!HasProperty(g_GaindBRange))
	{
		pAct = new CPropertyAction(this, &DVCCamera::OnGaindBRange);
		ret = CreateProperty(g_GaindBRange, "", MM::String, true, pAct);
		if (ret != DEVICE_OK)
			return ret;
	}
	return DEVICE_OK;
}

int DVCCamera::OnGaindB(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	DriverGuard dg(this);
	
	HANDLE hCam = camMap_[currentCam_];
	if (eAct == MM::BeforeGet)
	{
		double gain;
		if (!dvcGetGaindB(hCam, &gain))
			return processErr();
		pProp->Set(gain);
	}
	else if (eAct == MM::AfterSet)
	{
		double gain;
		pProp->Get(gain);
		if (!dvcSetGaindB(hCam, gain))
			return processErr();
		OnPropertiesChanged();
	}
	return DEVICE_OK;
}

int DVCCamera::OnActualFrameTime(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	if (eAct == MM::BeforeGet)
	{
		double lineTime, frameTime;
		DriverGuard dg(this);
		if (!dvcGetLineFrameTime(hCam_, &lineTime, &frameTime))
			return processErr();
		pProp->Set(frameTime);
	}
	return DEVICE_OK;
}

int DVCCamera::OnPixelClock(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	if (eAct == MM::BeforeGet)
	{
		DriverGuard dg(this);
		double freq = dvcGetPixelClock(hCam_);
		pProp->Set(freq);
	}
	return DEVICE_OK;
}

int DVCCamera::OnScanRate(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	if (eAct == MM::BeforeGet)
	{
		DriverGuard dg(this);
		int scan;
		if (!dvcGetScanRate(hCam_, &scan))
			return processErr();
		pProp->Set(scanRates_[scan].c_str());
	}
	else if (eAct == MM::AfterSet)
	{
		string scan;
		pProp->Get(scan);

		for (int i=0; i<scanRates_.size(); i++)
		{
			if (scan.compare(scanRates_[i]) == 0)
			{
				if (!dvcSetScanRate(hCam_, i))
					return processErr();
				OnPropertiesChanged();
				return DEVICE_OK;
			}
		}
		assert(false);
	}
	return DEVICE_OK;
}

int DVCCamera::OnGaindBRange(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	DriverGuard dg(this);
	
	HANDLE hCam = camMap_[currentCam_];
	if (eAct == MM::BeforeGet)
	{
		double min, max, interval;
		if (!dvcGetGaindBRange(hCam, &min, &max, &interval))
			return processErr();

		char str[1024];
		snprintf(str, 1024, "Range: %.1f~%.1fdB", min, max);
		pProp->Set(str);

		SetPropertyLimits(g_GaindB, min, max);
	}
	return DEVICE_OK;
}

int DVCCamera::createBinProp()
{
	DriverGuard dg(this);

	HANDLE hCam = camMap_[currentCam_];
	int ret = 0;

	int hb, vb;
	if (!dvcGetHVBin(hCam, &hb, &vb))
		return logDeviceError();
	binSize_ = hb;
	if (!dvcSetHVBin(hCam, binSize_, binSize_))
		return logDeviceError();

	CPropertyAction* pAct = NULL;
	if (!HasProperty(g_Binning))
	{
		pAct = new CPropertyAction(this, &DVCCamera::OnBinning);
		ret = CreateProperty(g_Binning, "", MM::Integer, false, pAct);
		if (ret != DEVICE_OK)
			return ret;
	}
	return DEVICE_OK;
}

int DVCCamera::getReadoutTime(double& time)
{
	DriverGuard dg(this);

	HANDLE hCam = camMap_[currentCam_];
	double line, frame;
	if (!dvcGetLineFrameTime(hCam, &line, &frame))
	{
		return processErr();
	}
	time = expMs_ + frame;
	return DEVICE_OK;
}

//added to use RTA
/**
* Acquires a single frame.
* Micro-Manager expects that this function blocks the calling thread until the exposure phase is over.
* This wait is implemented by sleeping ActualInterval_ms_ - ReadoutTime_ + 0.99 ms.
* Note that this is likely not long enough when using internal triggering.
*/
int DVCCamera::SnapImage()
{
	DriverGuard dg(this);

	if (sequenceRunning_)   // If we are in the middle of a SequenceAcquisition
		return ERR_BUSY_ACQUIRING;

	HANDLE hCam = camMap_[currentCam_];
	if (!dvcTakePicture(hCam))
	{
		return processErr();
	}
	return DEVICE_OK;
}

/**
* Deactivate the camera, reverse the initialization process.
*/
int DVCCamera::Shutdown()
{
    DriverGuard dg(this);

    HANDLE hCam = camMap_[currentCam_];
    if (initialized_)
    {
		SetToIdle();
		dvcSetUserBuffers(hCam, NULL);
		dvcReleaseUserBuffers(&dvcBuf);
		dvcCloseCamera(hCam);
		delete fullFrameBuffer_;
    }

    initialized_ = false;
    return DEVICE_OK;
}

bool DVCCamera::IsAcquiring()
{
	DriverGuard dg(this);

	HANDLE hCam = camMap_[currentCam_];
	return (dvcGetStatus(hCam) != DVC_STATUS_STOPPED);
}

int DVCCamera::SetToIdle()
{
	DriverGuard dg(this);

	if (!initialized_ || !IsAcquiring())
		return DEVICE_OK;

	HANDLE hCam = camMap_[currentCam_];
	if (!dvcStopSequence(hCam))
		return processErr();

	int status = DVC_STATUS_RUNNING;
	while (status != DVC_STATUS_STOPPED)
	{
		status = dvcGetStatus(hCam);
	}

	return DEVICE_OK;
}

const unsigned char* DVCCamera::GetImageBuffer()
{
	DriverGuard dg(this);

	HANDLE hCam = camMap_[currentCam_];
	int nBytes = dvcGetImageBytes(hCam);
	assert(nBytes <= fullFrameX_ * fullFrameY_ * 2);

	int w = dvcGetXDim(hCam);
	int h = dvcGetYDim(hCam);

	BOOL ret = dvcReadImage(hCam, fullFrameBuffer_, 0, 0, w, h);
	if (!ret)
	{
		logDeviceError();
		return NULL;
	}
	return (unsigned char*)fullFrameBuffer_;
}

int DVCCamera::logDeviceError()
{
	DriverGuard dg(this);
	int err = dvcGetLastErr();
	char* msg = dvcGetLastErrMsg();
	char str[1024];
	snprintf(str, 1024, "Error code %d: %s", err, msg);
	LogMessage(str, false);
	return err;
}

int DVCCamera::OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	if (eAct == MM::BeforeGet)
	{
		pProp->Set((long)binSize_);
	}
	else if (eAct == MM::AfterSet)
	{
		long bin;
		pProp->Get(bin);
		int ret = SetBinning(bin);
		return ret;
	}
	return DEVICE_OK;
}

int DVCCamera::SetBinning(int bin)
{
	{
		DriverGuard dg(this);
		
		if (sequenceRunning_)
			return ERR_BUSY_ACQUIRING;

		//added to use RTA
		SetToIdle();
		if (!dvcSetHVBin(hCam_, bin, bin))
			return processErr();
	}
	binSize_ = bin;
	ResizeImageBuffer();
	OnPropertiesChanged();
	return DEVICE_OK;
}

int DVCCamera::OnExposure(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	if (eAct == MM::BeforeGet)
	{
		pProp->Set(expMs_);
	}
	else if (eAct == MM::AfterSet)
	{
		double exp;
		pProp->Get(exp);

		{
			DriverGuard dg(this);
			HANDLE hCam = camMap_[currentCam_];
			double interval;

			if (!dvcGetExposeInterval(hCam, -1, -1, -1, &interval))
				return processErr();

			if (fabs(exp - expMs_) < interval)
				return DEVICE_OK;
		}
		SetExposure(exp);

		//bool acquiring = sequenceRunning_;
		//if (acquiring)
		//	StopSequenceAcquisition(true);
		//if (sequenceRunning_)
		//	return ERR_BUSY_ACQUIRING;
             
		//OnPropertiesChanged();
	}
	return DEVICE_OK;
}

void DVCCamera::SetExposure(double exp)
{
	bool acquiring = sequenceRunning_;
	if (acquiring)
		StopSequenceAcquisition(true);

	//if (sequenceRunning_)
	//	return ERR_BUSY_ACQUIRING;

	DriverGuard dg(this);
	HANDLE hCam = camMap_[currentCam_];
	expMs_ = dvcSetExposeMsec(hCam, exp);

	if (acquiring)
		StartSequenceAcquisition(sequenceLength_ - imageCounter_, intervalMs_, stopOnOverflow_);
}

double DVCCamera::GetExposure() const
{
	DriverGuard dg(this);
	return dvcGetExposeMsec(hCam_);
}

int DVCCamera::GetROI(unsigned& uX, unsigned& uY, unsigned& uXSize, unsigned& uYSize)
{
	uX = roi_.x / binSize_;
	uY = roi_.y / binSize_;
	uXSize = roi_.xSize / binSize_;
	uYSize = roi_.ySize / binSize_;

	return DEVICE_OK;
}

int DVCCamera::SetROI(unsigned uX, unsigned uY, unsigned uXSize, unsigned uYSize)
{
	DriverGuard dg(this);
	HANDLE hCam = camMap_[currentCam_];

	if (Busy())
		return ERR_BUSY_ACQUIRING;

	//added to use RTA
	SetToIdle();

	ROI oldRoi = roi_;

	roi_.x = uX * binSize_;
	roi_.y = uY * binSize_;
	roi_.xSize = uXSize * binSize_;
	roi_.ySize = uYSize * binSize_;

	if (roi_.x + roi_.xSize > fullFrameX_ || roi_.y + roi_.ySize > fullFrameY_)
	{
		roi_ = oldRoi;
		return ERR_INVALID_ROI;
	}

	// adjust image extent to conform to the bin size
	roi_.xSize -= roi_.xSize % binSize_;
	roi_.ySize -= roi_.ySize % binSize_;

	RECT rc;
	rc.left = roi_.x;
	rc.right = roi_.x + roi_.xSize;
	rc.top = roi_.y;
	rc.bottom = roi_.y + roi_.ySize;
	if (!dvcSetRoi(hCam, 1, &rc))
	{
		roi_ = oldRoi;
		return logDeviceError();
	}

	ResizeImageBuffer();
	OnPropertiesChanged();

	return DEVICE_OK;
}

int DVCCamera::ClearROI()
{
	DriverGuard dg(this);
	HANDLE hCam = camMap_[currentCam_];

	if (sequenceRunning_)
		return ERR_BUSY_ACQUIRING;

	//added to use RTA
	SetToIdle();

	roi_.x = 0;
	roi_.y = 0;
	roi_.xSize = fullFrameX_;
	roi_.ySize = fullFrameY_;

	// adjust image extent to conform to the bin size
	roi_.xSize -= roi_.xSize % binSize_;
	roi_.ySize -= roi_.ySize % binSize_;

	RECT rc;
	rc.left = roi_.x;
	rc.right = roi_.x + roi_.xSize;
	rc.top = roi_.y;
	rc.bottom = roi_.y + roi_.ySize;
	if (!dvcSetRoi(hCam, 0, &rc))
		return logDeviceError();

	ResizeImageBuffer();
	OnPropertiesChanged();
	return DEVICE_OK;
}

int DVCCamera::ResizeImageBuffer()
{
    // resize internal buffers
    // NOTE: we are assuming 16-bit pixel type
	const int bpp = (int)ceil(depth_/8.0);
	img_.Resize(roi_.xSize / binSize_, roi_.ySize / binSize_, bpp);
    return DEVICE_OK;
}

int DVCCamera::AcqSequenceThread::svc()
{
	int streamState;
	unsigned long remainings;
	imageCounter_ = 0;
	prevFrameIndex_ = -1;
	do
	{
		// Get current progress
		int currentFrameIndex;
		{
			DriverGuard dg(pCam_);
			dvcGetUserBufferId(hCam_, &currentFrameIndex);
				//return pCam->processErr();
			dvcGetStreamState(hCam_, &streamState, &remainings);
		}

		if (currentFrameIndex == prevFrameIndex_ || currentFrameIndex == -1)
		{
			// No image is available yet
			CDeviceUtils::SleepMs(waitTime_);
			continue;
		}
		
		int capturedFrames;
		if (currentFrameIndex > prevFrameIndex_)
			capturedFrames = currentFrameIndex - prevFrameIndex_;
		else
			capturedFrames = g_UserBufferNumber - prevFrameIndex_ + currentFrameIndex;

		for (int i = 0; i < capturedFrames; i++)
		{
			int id = prevFrameIndex_ + i + 1;
			if (id >= g_UserBufferNumber)
				id -= g_UserBufferNumber;
			pCam_->PushImage(id);
			imageCounter_++;
		}
		prevFrameIndex_ = currentFrameIndex;
		
		CDeviceUtils::SleepMs(waitTime_);
	} while(remainings > 0 && !stop_ && streamState != DVC_IDLE);

	if (stop_)
	{
		pCam_->LogMessage("Acquisition interrupted by the user.", false);
		return DEVICE_OK;
	}

	if (imageCounter_ != numImages_)
	{
		pCam_->LogMessage("Did not get the intended number of images.", false);
		pCam_->StopCameraAcquisition();
		return DEVICE_OK;
	}
	pCam_->StopCameraAcquisition();
	return DEVICE_OK;
}

/**
* Starts continuous acquisition.
*/
int DVCCamera::StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow)
{
	DriverGuard dg(this);

	HANDLE hCam = camMap_[currentCam_];

	if (sequenceRunning_)
		return ERR_BUSY_ACQUIRING;

	stopOnOverflow_ = stopOnOverflow;
	sequenceLength_ = numImages;
	intervalMs_ = interval_ms;

	if(IsAcquiring())
	{
		SetToIdle();
	}

	ostringstream os;
	os << "Started sequence acquisition: " << numImages << "images  at " << interval_ms << " ms" << endl;
	LogMessage(os.str().c_str());
	
	// start thread
	imageCounter_ = 0;

	os.str("");
	os << "Setting thread length to " << numImages << " Images";
	LogMessage(os.str().c_str(), true);
	thd_->SetLength(numImages);

	thd_->SetWaitTime((int)(dvcGetExposeMsec(hCam) / 5));
	thd_->SetTimeOut(10000);

	// prepare the core
    int  ret = GetCoreCallback()->PrepareForAcq(this);
    if (ret != DEVICE_OK)
    {
        return ret;
    }

	LogMessage("Starting acquisition in the camera", true);
	startTime_ = GetCurrentMMTime();

	if (!dvcSetUserBuffers(hCam, NULL))
		return processErr();
	if (!dvcReleaseUserBuffers(&userBuffers_))
		return processErr();
	if (!dvcAllocateUserBuffers(hCam, &userBuffers_, g_UserBufferNumber))
		return processErr();
	if (!dvcSetUserBuffers(hCam, &userBuffers_))
		return processErr();

	if (!dvcStartSequence(hCam, numImages))
	{
		std::ostringstream os;
		os << "DVC driver returned error value: " << ret;
		LogMessage(os.str().c_str(), false);
		return processErr();
	}
	else
	{
		thd_->Start(numImages, interval_ms);
		sequenceRunning_ = true;
	}
	return DEVICE_OK;
}

/**
* Stops Sequence acquisition
* This is for external use only (if called from the sequence acquisition thread, deadlock will ensue!
*/
int DVCCamera::StopSequenceAcquisition()
{
	return StopSequenceAcquisition(false);
}

int DVCCamera::StopSequenceAcquisition(bool temporary)
{
	{
		DriverGuard dg(this);
		sequencePaused_ = temporary;
		StopCameraAcquisition();
	}

	thd_->Stop();
	thd_->wait();

	return DEVICE_OK;
}

/**
* Stop Seq sequence acquisition
* This is the function for internal use and can/should be called from the thread
*/
int DVCCamera::StopCameraAcquisition()
{
	{
		DriverGuard dg(this);
		HANDLE hCam = camMap_[currentCam_];

		if (!sequenceRunning_)
			return DEVICE_OK;

		LogMessage("Stopped sequence acquisition");
		dvcStopSequence(hCam);

		int state;
		unsigned long remaining;
		dvcGetStreamState(hCam, &state, &remaining);
		
		while (state != DVC_IDLE)
		{
			CDeviceUtils::SleepMs(100);
			dvcGetStreamState(hCam, &state, &remaining);
		}
		sequenceRunning_ = false;
	}

	MM::Core* cb = GetCoreCallback();
	if (cb)
		return cb->AcqFinished(this, 0);
	else
		return DEVICE_OK;
}

/**
* Waits for new image and inserts it in the circular buffer.
* This method is called by the acquisition thread AcqSequenceThread::svc()
* in an infinite loop.
*
* In case of error or if the sequence is finished StopSequenceAcquisition()
* is called, which will raise the stop_ flag and cause the thread to exit.
*/
int DVCCamera::PushImage(int userBufferId)
{
	// create metadata
	char label[MM::MaxStrLength];
	this->GetLabel(label);

	MM::MMTime timestamp = this->GetCurrentMMTime();
	Metadata md;
	// Copy the metadata inserted by other processes:
	std::vector<std::string> keys = metadata_.GetKeys();
	for (unsigned int i= 0; i < keys.size(); i++) {
		md.put(keys[i], metadata_.GetSingleTag(keys[i].c_str()).GetValue().c_str());
	}

	ImageMetaDataP pMd = userBuffers_.pMeta + userBufferId;
	md.put("Camera", label);
	md.put(MM::g_Keyword_Metadata_StartTime, CDeviceUtils::ConvertToString(startTime_.getMsec()));
	md.put(MM::g_Keyword_Elapsed_Time_ms, CDeviceUtils::ConvertToString((timestamp - startTime_).getMsec()));
	md.put(MM::g_Keyword_Metadata_ImageNumber, CDeviceUtils::ConvertToString((long)(pMd->ulStreamCount)));
	md.put(MM::g_Keyword_Binning, binSize_);
	md.put(g_hBinning, pMd->hBin);
	md.put(g_vBinning, pMd->vBin);
	md.put(g_MD_ExposeTime, pMd->dExposeTime);
	md.put(g_MD_ExposeTimestamp, pMd->dExposeTimeStamp);
	md.put(g_MD_FrameTimestamp, pMd->dFrameTime);
	md.put(g_MD_TransferTime, pMd->dTransferTime);
	md.put(g_MD_TriggerTimestamp, pMd->dTriggerTimeStamp);

	MetadataSingleTag mstStartTime(MM::g_Keyword_Metadata_StartTime, label, true);
	mstStartTime.SetValue(CDeviceUtils::ConvertToString(startTime_.getMsec()));
	md.SetTag(mstStartTime);

	MetadataSingleTag mst(MM::g_Keyword_Elapsed_Time_ms, label, true);
	mst.SetValue(CDeviceUtils::ConvertToString(timestamp.getMsec()));
	md.SetTag(mst);

	MetadataSingleTag mstCount(MM::g_Keyword_Metadata_ImageNumber, label, true);
	mstCount.SetValue(CDeviceUtils::ConvertToString((long)(pMd->ulStreamCount)));
	md.SetTag(mstCount);

	MetadataSingleTag mstB(MM::g_Keyword_Binning, label, true);
	mstB.SetValue(CDeviceUtils::ConvertToString(binSize_));      
	md.SetTag(mstB);

	const HANDLE hCam = camMap_[currentCam_];
	const int height = GetImageHeight();
	const int width = GetImageWidth();
	const int bytesPerPixel = GetImageBytesPerPixel();

	imageCounter_++;

	// This method inserts new image in the circular buffer (residing in MMCore)
	const int retCode = GetCoreCallback()->InsertImage(this, (const unsigned char*)(userBuffers_.pBuffers[userBufferId]),
		(unsigned int)width,
		(unsigned int)height,
		(unsigned int)bytesPerPixel,
		md.Serialize().c_str());

	if (!stopOnOverflow_ && retCode == DEVICE_BUFFER_OVERFLOW)
	{
		LogMessage("Overflow occured.");
		// do not stop on overflow - just reset the buffer
		GetCoreCallback()->ClearImageBuffer(this);
		return 	GetCoreCallback()->InsertImage(this, (const unsigned char*)(userBuffers_.pBuffers[userBufferId]),
			(unsigned int)width,
			(unsigned int)height,
			(unsigned int)bytesPerPixel,
			md.Serialize().c_str());
	}
	userBuffers_.pBufferStatus[userBufferId] = 0;

	return DEVICE_OK;
}