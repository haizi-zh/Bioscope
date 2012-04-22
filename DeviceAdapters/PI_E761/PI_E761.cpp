// PI_E761.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PI_E761.h"
#include "include\E7XX_GCS_DLL.h"
#include "boost/lexical_cast.hpp"

/////////////////////////// START HERE /////////////////////////////////

// External names used used by the rest of the system
// to load particular device from the "DemoCamera.dll" library
const char* g_StageDeviceName = "PI_E761_ZStage";
const char* g_XYStageDeviceName = "PI_E761_XYStage";

// The handle of PI_E761 device which is -1 if the device is closed.
int g_DeviceId = -1;
// Axis names
char g_AxisNames[64] = "123";

// Thread lock. Caution: the E761 library is NOT thread-safe
MMThreadLock g_PI_ThreadLock;

// Indicating whether the device is shut down
bool g_zShutdown = false;
bool g_xyShutdown = false;

CPI_E761_ZStage* g_pZStage = NULL;
CPI_E761_XYStage* g_pXYStage = NULL;

// All the PI stage error codes will be returned plus this.
const int g_Err_Offset = 10000;

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

/**
 * List all suppoerted hardware devices here
 * Do not discover devices at runtime.  To avoid warnings about missing DLLs, Micro-Manager
 * maintains a list of supported device (MMDeviceList.txt).  This list is generated using 
 * information supplied by this function, so runtime discovery will create problems.
 */PI_E761_API void InitializeModuleData() {
	AddAvailableDeviceName(g_StageDeviceName, "PI E761 z-axis piezo stage");
	AddAvailableDeviceName(g_XYStageDeviceName, "PI E761 xy-axis piezo stage");
}

PI_E761_API MM::Device* CreateDevice(const char* deviceName) {
	if (deviceName == 0)
		return 0;

	if (strcmp(deviceName, g_StageDeviceName) == 0)
		return new CPI_E761_ZStage();
	else if (strcmp(deviceName, g_XYStageDeviceName) == 0)
		return new CPI_E761_XYStage();
	//return 0;

	// ...supplied name not recognized
	return 0;
}

PI_E761_API void DeleteDevice(MM::Device* pDevice) {
	delete pDevice;
}

///////////////////////////////////////////////////////////////////////////////
// CPI_E761_ZStage implementation
// ~~~~~~~~~~~~~~~~~~~~~~~~~

CPI_E761_ZStage::CPI_E761_ZStage() :
		stepSizeUm_(0.001), axisLimitUm_(100.0), busy_(false), initialized_(
				false), currentAxis_(3) {
	memset(axisName_, 0, 64);
	InitializeDefaultErrorMessages();
	//SetErrorText(PI_CNTR_PARAM_SYNTAX, "Parameter syntax error");
	//SetErrorText(PI_CNTR_UNKNOWN_COMMAND, "Unknown command");
	//SetErrorText(PI_CNTR_MOVE_WITHOUT_REF_OR_NO_SERVO, "Unallowable move attempted on unreferenced axis, or move attempted with servo off");
	//SetErrorText(PI_CNTR_POS_OUT_OF_LIMITS, "Position out of limits");
	//SetErrorText(PI_CNTR_VEL_OUT_OF_LIMITS, "Velocity out of limits");
	//SetErrorText(PI_CNTR_INVALID_AXIS_IDENTIFIER, "Invalid axis identifier");
	//SetErrorText(PI_CNTR_INVALID_STAGE_NAME, "Unknown stage name");
	//SetErrorText(PI_CNTR_PARAM_OUT_OF_RANGE, "Parameter out of range");
	//SetErrorText(PI_CNTR_OPENLOOP_VALUE_SET_WHEN_SERVO_ON, "Open-loop commands (SVA, SVR) are not allowed when servo is on");
	//SetErrorText(PI_CNTR_SEND_BUFFER_OVERFLOW, "Send buffer overflow");
	//SetErrorText(PI_CNTR_VOLTAGE_OUT_OF_LIMITS, "Voltage out of limits");
	//SetErrorText(PI_CNTR_OPEN_LOOP_MOTION_SET_WHEN_SERVO_ON, "Open-loop motion attempted when servo ON");

	// create pre-initialization properties
	// ------------------------------------

	// Name
	CreateProperty(MM::g_Keyword_Name, g_StageDeviceName, MM::String, true);

	// Description
	CreateProperty(MM::g_Keyword_Description,
			"Physik Instrumente (PI) E761 Adapter / Z-Stage", MM::String, true);

	//// axis limit in um
	//pAct = new CPropertyAction (this, &CPI_E761_ZStage::OnAxisLimit);
	//CreateProperty(g_PI_ZStageAxisLimitUm, "500.0", MM::Float, false, pAct, true);
}

CPI_E761_ZStage::~CPI_E761_ZStage() {
	Shutdown();
}

int CPI_E761_ZStage::Initialize() {
	LogMessage("Is initialized? (Zephyre)");
	if (initialized_)
		return DEVICE_OK;
	LogMessage("Start init (Zephyre)");

	g_pZStage = this;

	MMThreadGuard guard(g_PI_ThreadLock);

	if (g_DeviceId < 0 || !E7XX_IsConnected(g_DeviceId)) {
		// Connect the device
		int id = E7XX_ConnectPciBoard(1);
		if (id == -1)
			return id;
		g_DeviceId = id;
	}

	// Get axis names
	int ret = E7XX_qSAI(g_DeviceId, g_AxisNames, 63);
	if (!ret)
		return processErr();
	axisName_[0] = g_AxisNames[currentAxis_ - 1];

	// Switch to servo mode
	BOOL svoVal = true;
	ret = E7XX_SVO(g_DeviceId, axisName_, &svoVal);
	if (!ret)
		return processErr();

	// StepSize
	CPropertyAction* pAct = new CPropertyAction(this,
			&CPI_E761_ZStage::OnStepSizeUm);
	CreateProperty("StepSizeUm", "0.001", MM::Float, false, pAct);

	// Axis name
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnAxisName);
	CreateProperty("Axis name", "3", MM::String, false, pAct);

	// Positions
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnPosition);
	CreateProperty(MM::g_Keyword_Position, "0.0", MM::Float, false, pAct);
	//SetPropertyLimits(MM::g_Keyword_Position, 0, axisLimitUm_);

	// Axis index
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnCurrentAxis);
	CreateProperty("Axis index", "3", MM::Integer, false, pAct);
	SetPropertyLimits("Axis index", 1, 3);

	// Axis Information
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnAxisInfo);
	CreateProperty("Axis info", "", MM::String, true, pAct);

	// Lower limits
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnSoftLowerLimit);
	CreateProperty("Soft lower limit", "0", MM::Float, false, pAct);

	// Higher limits
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnSoftHigherLimit);
	CreateProperty("Soft higher limit", "0", MM::Float, false, pAct);

	// Travel range
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnTravelHighEnd);
	CreateProperty("Travel range (high end)", "100", MM::Float, true, pAct);
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnTravelLowEnd);
	CreateProperty("Travel range (low end)", "0", MM::Float, true, pAct);

	// Velocity control
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnVelControl);
	CreateProperty("Velocity control", "", MM::Float, false, pAct);

	// Firmware version
	pAct = new CPropertyAction(this, &CPI_E761_ZStage::OnVersion);
	CreateProperty("Firmware version", "", MM::String, true, pAct);

	ret = UpdateStatus();
	if (ret != DEVICE_OK)
		return ret;

	initialized_ = true;
	return DEVICE_OK;
}

int CPI_E761_ZStage::Shutdown() {
	if (initialized_) {
		initialized_ = false;

		if (g_pXYStage == NULL || !g_pXYStage->IsInitialized()) {
			// Close the device
			E7XX_CloseConnection(g_DeviceId);
		}
	}
	return DEVICE_OK;
}

int CPI_E761_ZStage::OnAxisName(MM::PropertyBase* pProp, MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		char name[64];
		BOOL ret = E7XX_qSAI(g_DeviceId, name, 63);
		if (!ret)
			return processErr();
		axisName_[0] = name[currentAxis_ - 1];
		pProp->Set(axisName_);
	} else {
		string newName;
		pProp->Get(newName);
		if (newName.length() < 1)
			return DEVICE_OK;
		char tmp[] = "X";
		strncpy(tmp, newName.c_str(), 1);
		BOOL ret = E7XX_SAI(g_DeviceId, axisName_, tmp);
		if (!ret)
			return processErr();
		strncpy(axisName_, tmp, 1);
		pProp->Set(axisName_);
//		OnPropertiesChanged();
	}

	return DEVICE_OK;
}

int CPI_E761_ZStage::OnStepSizeUm(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	if (eAct == MM::BeforeGet) {
		pProp->Set(stepSizeUm_);
	} else if (eAct == MM::AfterSet) {
		pProp->Get(stepSizeUm_);
	}

	return DEVICE_OK;
}

//int CPI_E761_ZStage::OnAxisLimit(MM::PropertyBase* pProp, MM::ActionType eAct)
//{
//   //if (eAct == MM::BeforeGet)
//   //{
//   //   pProp->Set(axisLimitUm_);
//   //}
//   //else if (eAct == MM::AfterSet)
//   //{
//   //   pProp->Get(axisLimitUm_);
//   //}
//	//TODO
//
//   return DEVICE_OK;
//}

int CPI_E761_ZStage::OnAxisInfo(MM::PropertyBase* pProp, MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		char info[64];
		BOOL ret = E7XX_qCST(g_DeviceId, axisName_, info, 63);
		if (!ret)
			return processErr();
		axisInfo_ = info;
		pProp->Set(axisInfo_.c_str());
	}
	return DEVICE_OK;
}

// Refresh the soft higher and lower limit
void CPI_E761_ZStage::refreshPosLimit() {
	double low, high;
	int ret = GetLimits(low, high);
	if (ret != DEVICE_OK)
		return;

	SetPropertyLimits(MM::g_Keyword_Position, low, high);
}

int CPI_E761_ZStage::OnSoftLowerLimit(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		double val;
		BOOL ret = E7XX_qNLM(g_DeviceId, axisName_, &val);
		if (!ret)
			return processErr();
		pProp->Set(val);
	} else if (eAct == MM::AfterSet) {
		double val;
		pProp->Get(val);
		BOOL ret = E7XX_NLM(g_DeviceId, axisName_, &val);
		if (!ret)
			return processErr();

		refreshPosLimit();
//		OnPropertiesChanged();
	}
	return DEVICE_OK;
}

int CPI_E761_ZStage::OnSoftHigherLimit(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		double val;
		BOOL ret = E7XX_qPLM(g_DeviceId, axisName_, &val);
		if (!ret)
			return processErr();
		pProp->Set(val);
	} else if (eAct == MM::AfterSet) {
		double val;
		pProp->Get(val);
		BOOL ret = E7XX_PLM(g_DeviceId, axisName_, &val);
		if (!ret)
			return processErr();
		refreshPosLimit();
//		OnPropertiesChanged();
	}
	return DEVICE_OK;
}

int CPI_E761_ZStage::OnTravelHighEnd(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		double val;
		BOOL ret = E7XX_qTMX(g_DeviceId, axisName_, &val);
		if (!ret)
			return processErr();
		pProp->Set(val);
	}
	return DEVICE_OK;
}

int CPI_E761_ZStage::OnTravelLowEnd(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		double val;
		BOOL ret = E7XX_qTMN(g_DeviceId, axisName_, &val);
		if (!ret)
			return processErr();
		pProp->Set(val);
	}
	return DEVICE_OK;
}

int CPI_E761_ZStage::OnVelControl(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		BOOL vcoSwitch;
		BOOL ret = E7XX_qVCO(g_DeviceId, axisName_, &vcoSwitch);
		if (!ret)
			return processErr();
		if (vcoSwitch) {
			// vco on
			double vel;
			ret = E7XX_qVEL(g_DeviceId, axisName_, &vel);
			if (!ret)
				return processErr();
			pProp->Set(vel);
		} else {
			// vco off
			pProp->Set(0.0);
		}
	} else if (eAct == MM::AfterSet) {
		BOOL vcoSwitch;
		double vel;
		pProp->Get(vel);
		if (vel <= 0) {
			// vco off
			vcoSwitch = FALSE;
			vel = 0;
		} else {
			// vco on
			vcoSwitch = TRUE;
		}

		BOOL ret = E7XX_VCO(g_DeviceId, axisName_, &vcoSwitch);
		if (!ret)
			return processErr();
		ret = E7XX_VEL(g_DeviceId, axisName_, &vel);
		if (!ret)
			return processErr();
//		OnPropertiesChanged();
	}
	return DEVICE_OK;
}

int CPI_E761_ZStage::OnVersion(MM::PropertyBase* pProp, MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		char str[1024];
		BOOL ret = E7XX_qVER(g_DeviceId, str, 1023);
		if (!ret)
			return processErr();
		pProp->Set(str);
	}
	return DEVICE_OK;
}

int CPI_E761_ZStage::OnCurrentAxis(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	if (eAct == MM::BeforeGet) {
		long index = currentAxis_;
		pProp->Set(index);
	} else if (eAct == MM::AfterSet) {
		long index;
		pProp->Get(index);
		currentAxis_ = index;
		UpdateStatus();
//		OnPropertiesChanged();
	}
	return DEVICE_OK;
}

int CPI_E761_ZStage::OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		double pos;
		int ret = GetPositionUm(pos);
		if (ret == DEVICE_OK) {
			pProp->Set(pos);
			LogMessage(
					"Get position: "
							+ boost::lexical_cast<std::string, float>(
									(float) pos) + "\tZephyre");
		}
		return ret;
	} else if (eAct == MM::AfterSet) {
		double pos;
		pProp->Get(pos);
		int ret = SetPositionUm(pos);
		LogMessage(
				"Set position: "
						+ boost::lexical_cast<std::string, float>((float) pos)
						+ "\tZephyre");
		OnStagePositionChanged(pos);
		return ret;
	}
	return DEVICE_OK;
}

void CPI_E761_ZStage::GetName(char* Name) const {
	CDeviceUtils::CopyLimitedString(Name, g_StageDeviceName);
}

int CPI_E761_ZStage::SetPositionUm(double pos) {
	MMThreadGuard guard(g_PI_ThreadLock);

	BOOL ret = E7XX_MOV(g_DeviceId, axisName_, &pos);
	if (!ret)
		return processErr();

//	OnPropertiesChanged();
	return DEVICE_OK;
}

int CPI_E761_ZStage::GetPositionUm(double& pos) {
	MMThreadGuard guard(g_PI_ThreadLock);

	BOOL ret = E7XX_qPOS(g_DeviceId, axisName_, &pos);
	if (!ret)
		return processErr();

	return DEVICE_OK;
}

int CPI_E761_ZStage::SetPositionSteps(long steps) {
	double pos = steps * stepSizeUm_;
	return SetPositionUm(pos);
}

int CPI_E761_ZStage::GetPositionSteps(long& steps) {
	double pos;
	BOOL ret = GetPositionUm(pos);
	if (ret != DEVICE_OK)
		return ret;

	steps = (long) (pos / stepSizeUm_);
	return DEVICE_OK;
}

int CPI_E761_ZStage::GetLimits(double& min, double& max) {
	MMThreadGuard guard(g_PI_ThreadLock);

	BOOL ret = E7XX_NLM(g_DeviceId, axisName_, &min);
	if (!ret)
		return processErr();
	ret = E7XX_qPLM(g_DeviceId, axisName_, &max);
	if (!ret)
		return processErr();

	return DEVICE_OK;
}

int CPI_E761_ZStage::SetOrigin() {
	MMThreadGuard guard(g_PI_ThreadLock);

	BOOL ret = E7XX_DFH(g_DeviceId, axisName_);
	if (!ret)
		return processErr();

	return DEVICE_OK;
}

///////////////////////////////////////////////////////////////////////////////
// CPI_E761_XYStage implementation
// ~~~~~~~~~~~~~~~~~~~~~~~~~

const char* CPI_E761_XYStage::PropXPos = "X Position";
const char* CPI_E761_XYStage::PropYPos = "Y Position";

CPI_E761_XYStage::CPI_E761_XYStage() :
		busy_(false), initialized_(false), stepSizeUm_(0.001) {
	currentAxis_[0] = 1;
	currentAxis_[1] = 2;
	memset(axisName_, 0, 64);
	memset(xaxisName_, 0, 64);
	memset(yaxisName_, 0, 64);

	InitializeDefaultErrorMessages();

	// create pre-initialization properties
	// ------------------------------------

	// Name
	CreateProperty(MM::g_Keyword_Name, g_XYStageDeviceName, MM::String, true);

	// Description
	CreateProperty(MM::g_Keyword_Description,
			"Physik Instrumente (PI) E761 Adapter / XY-Stage", MM::String,
			true);
}

CPI_E761_XYStage::~CPI_E761_XYStage() {
	Shutdown();
}

int CPI_E761_XYStage::Initialize() {
	if (initialized_)
		return DEVICE_OK;

	g_pXYStage = this;

	MMThreadGuard guard(g_PI_ThreadLock);

	if (g_DeviceId < 0) {
		// Connect the device
		int id = E7XX_ConnectPciBoard(1);
		if (id == -1)
			return id;
		g_DeviceId = id;
	}

	// Get axis names
	int ret = E7XX_qSAI(g_DeviceId, g_AxisNames, 63);
	if (!ret)
		return processErr();
	for (int i = 0; i < 2; i++) {
		axisName_[i] = g_AxisNames[currentAxis_[i] - 1];
	}
	xaxisName_[0] = axisName_[0];
	yaxisName_[0] = axisName_[1];

	// Switch to servo mode
	BOOL svoVals[] = { true, true };
	ret = E7XX_SVO(g_DeviceId, axisName_, svoVals);
	if (!ret)
		return processErr();

	// StepSize
	CPropertyAction* pAct = new CPropertyAction(this,
			&CPI_E761_XYStage::OnStepSizeUm);
	CreateProperty("StepSizeUm", "0.001", MM::Float, false, pAct);

	// Axis name
	pAct = new CPropertyAction(this, &CPI_E761_XYStage::OnAxisName);
	CreateProperty("Axis name", "12", MM::String, false, pAct);

	// Positions
	pAct = new CPropertyAction(this, &CPI_E761_XYStage::OnXPosition);
	CreateProperty(CPI_E761_XYStage::PropXPos, "0.0", MM::Float, false, pAct);
	pAct = new CPropertyAction(this, &CPI_E761_XYStage::OnYPosition);
	CreateProperty(CPI_E761_XYStage::PropYPos, "0.0", MM::Float, false, pAct);

	// Travel range
	pAct = new CPropertyAction(this, &CPI_E761_XYStage::OnTravelRange);
	CreateProperty("Travel range", "", MM::String, true, pAct);

	return DEVICE_OK;
}

int CPI_E761_XYStage::Shutdown() {
	if (initialized_) {
		initialized_ = false;

		if (g_pZStage == NULL || !g_pZStage->IsInitialized()) {
			MMThreadGuard guard(g_PI_ThreadLock);

			// Close the device
			E7XX_CloseConnection(g_DeviceId);
		}
	}
	return DEVICE_OK;
}

void CPI_E761_XYStage::GetName(char *Name) const {
	CDeviceUtils::CopyLimitedString(Name, g_XYStageDeviceName);
}

int CPI_E761_XYStage::GetLimitsUm(double& xMin, double& xMax, double& yMin,
		double& yMax) {
	MMThreadGuard guard(g_PI_ThreadLock);

	double lower[2];
	double higher[2];
	BOOL ret = E7XX_qNLM(g_DeviceId, axisName_, lower);
	if (!ret)
		return processErr();
	ret = E7XX_qPLM(g_DeviceId, axisName_, higher);
	if (!ret)
		return processErr();

	xMin = lower[0];
	yMin = lower[1];
	xMax = higher[0];
	yMax = higher[1];

	return DEVICE_OK;
}

int CPI_E761_XYStage::SetPositionSteps(long x, long y) {
	MMThreadGuard guard(g_PI_ThreadLock);

	double posUm[2];
	posUm[0] = x * stepSizeUm_;
	posUm[1] = y * stepSizeUm_;

	BOOL ret = E7XX_MOV(g_DeviceId, axisName_, posUm);
	if (!ret)
		return processErr();
//	OnPropertiesChanged();

	return DEVICE_OK;
}

int CPI_E761_XYStage::SetXPositionUm(double x) {
	MMThreadGuard guard(g_PI_ThreadLock);

	char axis[] = "X";
	memcpy_s(axis, 2, axisName_, 1);
	BOOL ret = E7XX_MOV(g_DeviceId, axis, &x);
	if (!ret)
		return processErr();
//	OnPropertiesChanged();

	return DEVICE_OK;
}

int CPI_E761_XYStage::SetYPositionUm(double y) {
	MMThreadGuard guard(g_PI_ThreadLock);

	char axis[] = "X";
	memcpy_s(axis, 2, axisName_ + 1, 1);
	BOOL ret = E7XX_MOV(g_DeviceId, axis, &y);
	if (!ret)
		return processErr();
//	OnPropertiesChanged();

	return DEVICE_OK;
}

int CPI_E761_XYStage::SetXPositionSteps(long xs) {
	return SetXPositionUm(xs * stepSizeUm_);
}

int CPI_E761_XYStage::SetYPositionSteps(long ys) {
	return SetYPositionUm(ys * stepSizeUm_);
}

int CPI_E761_XYStage::GetPositionSteps(long& x, long& y) {
	MMThreadGuard guard(g_PI_ThreadLock);

	double posUm[2];

	BOOL ret = E7XX_qPOS(g_DeviceId, axisName_, posUm);
	if (!ret)
		return processErr();

	x = (long) (posUm[0] / stepSizeUm_);
	y = (long) (posUm[1] / stepSizeUm_);

	return DEVICE_OK;
}

int CPI_E761_XYStage::Home() {
	MMThreadGuard guard(g_PI_ThreadLock);

	BOOL ret = E7XX_GOH(g_DeviceId, axisName_);
	if (!ret)
		return processErr();
	return DEVICE_OK;
}

int CPI_E761_XYStage::Stop() {
	MMThreadGuard guard(g_PI_ThreadLock);

	BOOL ret = E7XX_HLT(g_DeviceId, axisName_);
	if (!ret)
		return processErr();
	return DEVICE_OK;
}

int CPI_E761_XYStage::SetOrigin() {
	MMThreadGuard guard(g_PI_ThreadLock);

	BOOL ret = E7XX_DFH(g_DeviceId, axisName_);
	if (!ret)
		return processErr();

	return DEVICE_OK;
}

int CPI_E761_XYStage::GetStepLimits(long& xMin, long& xMax, long& yMin,
		long& yMax) {
	double xMinUm, xMaxUm, yMinUm, yMaxUm;
	int ret = GetLimitsUm(xMinUm, xMaxUm, yMinUm, yMaxUm);
	if (ret == DEVICE_OK) {
		xMin = (long) (xMinUm / stepSizeUm_);
		xMax = (long) (xMaxUm / stepSizeUm_);
		yMin = (long) (yMinUm / stepSizeUm_);
		yMax = (long) (yMaxUm / stepSizeUm_);
	}
	return ret;
}

int CPI_E761_XYStage::OnStepSizeUm(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	if (eAct == MM::BeforeGet) {
		pProp->Set(stepSizeUm_);
	} else if (eAct == MM::AfterSet) {
		pProp->Get(stepSizeUm_);
	}
	return DEVICE_OK;
}

int CPI_E761_XYStage::OnAxisName(MM::PropertyBase* pProp, MM::ActionType eAct) {
	MMThreadGuard guard(g_PI_ThreadLock);

	if (eAct == MM::BeforeGet) {
		char name[64];
		BOOL ret = E7XX_qSAI(g_DeviceId, name, 63);
		if (!ret)
			return processErr();
		for (int i = 0; i < 2; i++) {
			axisName_[i] = g_AxisNames[currentAxis_[i] - 1];
		}
		pProp->Set(axisName_);
	} else if (eAct == MM::AfterSet) {
		string newName;
		pProp->Get(newName);
		if (newName.length() < 1)
			return DEVICE_OK;
		char tmp[] = "XY";
		strncpy(tmp, newName.c_str(), 2);
		BOOL ret = E7XX_SAI(g_DeviceId, axisName_, tmp);
		if (!ret)
			return processErr();
		strncpy(axisName_, tmp, 2);
		pProp->Set(axisName_);
	}

	return DEVICE_OK;
}

int CPI_E761_XYStage::OnXPosition(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	if (eAct == MM::BeforeGet) {
		double x, y;
		int ret = GetPositionUm(x, y);
		if (ret == DEVICE_OK)
			pProp->Set(x);
		return ret;
	} else if (eAct == MM::AfterSet) {
		double x;
		pProp->Get(x);
		int ret = SetXPositionUm(x);
		if (ret != DEVICE_OK)
			return ret;
	}
	return DEVICE_OK;
}

int CPI_E761_XYStage::OnYPosition(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	if (eAct == MM::BeforeGet) {
		double x, y;
		int ret = GetPositionUm(x, y);
		if (ret == DEVICE_OK)
			pProp->Set(y);
		return ret;
	} else if (eAct == MM::AfterSet) {
		double y;
		pProp->Get(y);
		int ret = SetYPositionUm(y);
		if (ret != DEVICE_OK)
			return ret;
	}
	return DEVICE_OK;
}

int CPI_E761_XYStage::OnTravelRange(MM::PropertyBase* pProp,
		MM::ActionType eAct) {
	if (eAct == MM::BeforeGet) {
		MMThreadGuard guard(g_PI_ThreadLock);

		double lowArray[2];
		double highArray[2];

		BOOL ret = E7XX_qTMN(g_DeviceId, axisName_, lowArray);
		if (!ret)
			return processErr();
		ret = E7XX_qTMX(g_DeviceId, axisName_, highArray);
		if (!ret)
			return processErr();

		char str[1024];
		snprintf(str, 1024, "Travel range: axis 1: %g~%gum / axis 2: %g~%gum",
				lowArray[0], highArray[0], lowArray[1], highArray[1]);
		pProp->Set(str);
	}

	return DEVICE_OK;
}

int CPI_E761_XYStage::processErr() {
	const int err = E7XX_GetError(g_DeviceId);
	char msg[1024];
	BOOL ret = E7XX_TranslateError(err, msg, 1024);
	if (ret)
		SetErrorText(err + g_Err_Offset, msg);

	return err;
}

int CPI_E761_ZStage::processErr() {
	const int err = E7XX_GetError(g_DeviceId);
	char msg[1024];
	BOOL ret = E7XX_TranslateError(err, msg, 1024);
	if (ret)
		SetErrorText(err + g_Err_Offset, msg);

	return err;
}
