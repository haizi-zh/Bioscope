// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DVCCAMERA_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DVCCAMERA_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DVCCAMERA_EXPORTS
#define DVCCAMERA_API __declspec(dllexport)
#else
#define DVCCAMERA_API __declspec(dllimport)
#endif

#include <map>
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceThreads.h"
#include "dvcAPI.h"

// error codes
#define ERR_BUFFER_ALLOCATION_FAILED 101
#define ERR_INCOMPLETE_SNAP_IMAGE_CYCLE 102
#define ERR_INVALID_ROI 103
#define ERR_INVALID_READOUT_MODE_SETUP 104
#define ERR_CAMERA_DOES_NOT_EXIST 105
#define ERR_BUSY_ACQUIRING 106
#define ERR_INVALID_PREAMPGAIN 107
#define ERR_INVALID_VSPEED 108
#define ERR_TRIGGER_NOT_SUPPORTED 109
#define ERR_OPEN_OR_CLOSE_SHUTTER_IN_ACQUISITION_NOT_ALLOWEDD 110
#define ERR_NO_AVAIL_AMPS 111
#define ERR_SOFTWARE_TRIGGER_IN_USE 112

class DVCCamera : public CCameraBase<DVCCamera> 
{
public:
	static DVCCamera* GetInstance();
	~DVCCamera();

	// MMDevice API
	int Initialize();
	int Shutdown();
   
	void GetName(char* pszName) const;
	bool Busy() { return false; }

	// MMCamera API
	int SnapImage();
	const unsigned char* GetImageBuffer();
	long GetImageBufferSize() const { return img_.Width() * img_.Height() * GetImageBytesPerPixel(); }
	unsigned int GetImageWidth() const { return img_.Width(); }
	unsigned int GetImageHeight() const { return img_.Height(); }
	unsigned int GetImageBytesPerPixel() const { return img_.Depth(); }
	unsigned int GetBitDepth() const { return depth_; }
	int GetBinning() const { return binSize_; }
	int SetBinning(int binSize);
	double GetExposure() const;
	void SetExposure(double dExp);
	int SetROI(unsigned uX, unsigned uY, unsigned uXSize, unsigned uYSize); 
	int GetROI(unsigned& uX, unsigned& uY, unsigned& uXSize, unsigned& uYSize);
	int ClearROI();
	int IsExposureSequenceable(bool& isSequenceable) const { isSequenceable = false; return DEVICE_OK; }
	int StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow);
	int StopSequenceAcquisition(); // temporary=true
	int StopSequenceAcquisition(bool temporary); // temporary=true

private:
	DVCCamera();

	// custom interface for the thread
	int PushImage(int userBufferId);

	// Get the error code and the error message
	int processErr();

	// Calculate the readout time
	int getReadoutTime(double& time);
	// Stop acquisitions and wait
	int SetToIdle();
	bool IsAcquiring();
	bool IsCapturing(){return sequenceRunning_;};

	// Query for the last error information and log
	int logDeviceError();
	// Update the image buffer information
	int ResizeImageBuffer();
	// Internal use
	int StopCameraAcquisition();

	// Property initializers
	int createGainProp();
	int createBinProp();
	int createScanRateProp();

	// Property interface
	int OnGaindB(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnGaindBRange(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnExposure(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnActualFrameTime(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnPixelClock(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnScanRate(MM::PropertyBase* pProp, MM::ActionType eAct);	

	// a list of available cameras
	std::map<int, HANDLE> camMap_;
	int currentCam_;

	// Camera information
	HANDLE hCam_;
	std::string camName_;
	static std::map<int, std::string> camTypeMap_;
	int camType_;
	int camSSN_;
	int fullFrameX_;
	int fullFrameY_;
	int depth_;
	std::vector<std::string> scanRates_;
	ImgBuffer img_;
	static unsigned int refCount_;
	static DVCCamera* instance_;
	bool initialized_;
	bool busy_;
	double expMs_;
	bool sequenceRunning_;
	dvcBufStruct dvcBuf;
	const int bufNumber;
	unsigned short* fullFrameBuffer_;
	int binSize_;
	int sequenceLength_;
	double intervalMs_;
	int imageCounter_;
	MM::MMTime startTime_;
	dvcBufStruct userBuffers_;
	class AcqSequenceThread;
	DVCCamera::AcqSequenceThread* thd_;
	bool sequencePaused_;
	bool stopOnOverflow_;

	struct ROI {
		int x;
		int y;
		int xSize;
		int ySize;

		ROI() : x(0), y(0), xSize(0), ySize(0) {}
		~ROI() {}

		bool isEmpty() {return x==0 && y==0 && xSize==0 && ySize == 0;}
	};
	ROI roi_;

	/**
	* Acquisition thread
	*/
	class AcqSequenceThread : public BaseSequenceThread
	{
	public:
		AcqSequenceThread(DVCCamera* pCam, HANDLE h) : BaseSequenceThread(pCam),
			waitTime_(0),
			imageTimeOut_(0),
			hCam_(h),
			prevFrameIndex_(-1)
		{
			pCam_ = pCam;
		}
		void SetWaitTime(int waitTime) { waitTime_ = waitTime; }
		void SetTimeOut(int imageTimeOut) { imageTimeOut_ = imageTimeOut; }
		int svc();

	private:
		int waitTime_;
		int imageTimeOut_;
		HANDLE hCam_;
		int prevFrameIndex_;
		DVCCamera* pCam_;
	};
};

class DriverGuard
{
public:
   DriverGuard(const DVCCamera * cam);
   ~DriverGuard();
};