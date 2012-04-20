#ifndef PI_H
#define PI_H

#include <Windows.h>
#include "E7XX_GCS_DLL.h"


/////////////////////////////////////////////////////////////////////////////

// DLL initialization and comm functions
int InterfaceSetupDlg(const char* szRegKeyName) { return E7XX_InterfaceSetupDlg(szRegKeyName); }
int ConnectRS232(int iPortNumber,int iBaudRate) { return E7XX_ConnectRS232(iPortNumber, iBaudRate); }
int ConnectNIgpib(int iBoardNumber,int iDeviceAddress) { return E7XX_ConnectNIgpib(iBoardNumber, iDeviceAddress); }
int ConnectPciBoard(int iBoardNumber) { return E7XX_ConnectPciBoard(iBoardNumber); }
int ConnectPciBoardAndReboot(int iBoardNumber) { return E7XX_ConnectPciBoardAndReboot(iBoardNumber); }
int ChangeNIgpibAddress(int ID,int iDeviceAddress) { return E7XX_ChangeNIgpibAddress(ID, iDeviceAddress); }
int IsConnected(int ID) { return E7XX_IsConnected(ID); }
void CloseConnection(int ID) { return E7XX_CloseConnection(ID); }
int GetError(int ID) { return E7XX_GetError(ID); }
int SetErrorCheck(int ID,int bErrorCheck) { return E7XX_SetErrorCheck(ID, bErrorCheck); }
int TranslateError(int iErrorNumber,char* szErrorMessage,int iBufferSize) { return E7XX_TranslateError(iErrorNumber, szErrorMessage, iBufferSize); }
int CountPciBoards( void) { return E7XX_CountPciBoards(); }

/////////////////////////////////////////////////////////////////////////////

// general
int qERR(int ID,int* piError) { return E7XX_qERR(ID, piError); }
int qIDN(int ID,char* szBuffer,int iBufferSize) { return E7XX_qIDN(ID, szBuffer, iBufferSize); }
int INI(int ID,const char* szAxes) { return E7XX_INI(ID, szAxes); }
int qHLP(int ID,char* szBuffer,int iBufferSize) { return E7XX_qHLP(ID, szBuffer, iBufferSize); }
int qHPA(int ID,char* szBuffer,int iBufferSize) { return E7XX_qHPA(ID, szBuffer, iBufferSize); }
int CSV(int ID,double pdCommandSyntaxVersion) { return E7XX_CSV(ID, pdCommandSyntaxVersion); }
int qCSV(int ID,double* pdCommandSyntaxVersion) { return E7XX_qCSV(ID, pdCommandSyntaxVersion); }
int qOVF(int ID,const char* szAxes,int* pbValueArray) { return E7XX_qOVF(ID, szAxes, pbValueArray); }
int RBT(int ID) { return E7XX_RBT(ID); }
int REP(int ID) { return E7XX_REP(ID); }
int qSSN(int ID, char*szSerialNumber,int iBufferSize) { return E7XX_qSSN(ID, szSerialNumber, iBufferSize); }
int qVER(int ID,char* szVersion,int iBufferSize) { return E7XX_qVER(ID, szVersion, iBufferSize); }
int CCT(int ID,int iCommandType) { return E7XX_CCT(ID, iCommandType); }
int MOV(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_MOV(ID, szAxes, pdValueArray); }
int qMOV(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qMOV(ID, szAxes, pdValueArray); }
int MVR(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_MVR(ID, szAxes, pdValueArray); }
int qPOS(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qPOS(ID, szAxes, pdValueArray); }
int IsMoving(int ID,const char* szAxes,int* pbValueArray) { return E7XX_IsMoving(ID, szAxes, pbValueArray); }
int HLT(int ID,const char* szAxes) { return E7XX_HLT(ID, szAxes); }
int qONT(int ID,const char* szAxes,int* pbValueArray) { return E7XX_qONT(ID, szAxes, pbValueArray); }
int SVA(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_SVA(ID, szAxes, pdValueArray); }
int qSVA(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qSVA(ID, szAxes, pdValueArray); }
int SVR(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_SVR(ID, szAxes, pdValueArray); }
int DFH(int ID,const char* szAxes) { return E7XX_DFH(ID, szAxes); }
int qDFH(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qDFH(ID, szAxes, pdValueArray); }
int GOH(int ID,const char* szAxes) { return E7XX_GOH(ID, szAxes); }
int DFF(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_DFF(ID, szAxes, pdValueArray); }
int qDFF(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qDFF(ID, szAxes, pdValueArray); }
int qCST(int ID,const char* szAxes,char* szNames,int iBufferSize) { return E7XX_qCST(ID, szAxes, szNames, iBufferSize); }
int CST(int ID,const char* szAxes,const char* szNames) { return E7XX_CST(ID, szAxes, szNames); }
int qVST(int ID,char* szValideStages,int iBufferSize) { return E7XX_qVST(ID, szValideStages, iBufferSize); }
int qTVI(int ID,char* szPossibleAxisCharacters,int iBufferSize) { return E7XX_qTVI(ID, szPossibleAxisCharacters, iBufferSize); }
int SVO(int ID,const char* szAxes,const int* pbValueArray) { return E7XX_SVO(ID, szAxes, pbValueArray); }
int qSVO(int ID,const char* szAxes,int* pbValueArray) { return E7XX_qSVO(ID, szAxes, pbValueArray); }
int VCO(int ID,const char* szAxes,const int* pbValueArray) { return E7XX_VCO(ID, szAxes, pbValueArray); }
int qVCO(int ID,const char* szAxes,int* pbValueArray) { return E7XX_qVCO(ID, szAxes, pbValueArray); }
int VEL(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_VEL(ID, szAxes, pdValueArray); }
int qVEL(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qVEL(ID, szAxes, pdValueArray); }
int SPA(int ID,const char* szAxes,const int* iParameterArray,const double* pdValueArray,const char* szStrings) { return E7XX_SPA(ID, szAxes, iParameterArray, pdValueArray, szStrings); }
int qSPA(int ID,const char* szAxes,int* iParameterArray,double* pdValueArray,char* szStrings,int iMaxStringSize) { return E7XX_qSPA(ID, szAxes, iParameterArray, pdValueArray, szStrings, iMaxStringSize); }
int SEP(int ID,const char* szPassword,const char* szAxes,const int* iParameterArray,const double* pdValueArray,const char* szStrings) { return E7XX_SEP(ID, szPassword, szAxes, iParameterArray, pdValueArray, szStrings); }
int qSEP(int ID,const char* szAxes,int* iParameterArray,double* pdValueArray,char* szStrings,int iMaxStringSize) { return E7XX_qSEP(ID, szAxes, iParameterArray, pdValueArray, szStrings, iMaxStringSize); }
int WPA(int ID,const char* szPassword,const char* szAxes,const int* iParameterArray) { return E7XX_WPA(ID, szPassword, szAxes, iParameterArray); }
int RPA(int ID,const char* szAxes,const int* iParameterArray) { return E7XX_RPA(ID, szAxes, iParameterArray); }
int SPA_String(int ID,const char* szAxes,const int* iParameterArray,const char* szStrings) { return E7XX_SPA_String(ID, szAxes, iParameterArray, szStrings); }
int qSPA_String(int ID,const char* szAxes,int* iParameterArray,char* szStrings,int iMaxStringSize) { return E7XX_qSPA_String(ID, szAxes, iParameterArray, szStrings, iMaxStringSize); }
int SEP_String(int ID,const char* szPassword,const char* szAxes,const int* iParameterArray,const char* szStrings) { return E7XX_SEP_String(ID, szPassword, szAxes, iParameterArray, szStrings); }
int qSEP_String(int ID,const char* szAxes,int* iParameterArray,char* szStrings,int iMaxStringSize) { return E7XX_qSEP_String(ID, szAxes, iParameterArray, szStrings, iMaxStringSize); }
int STE(int ID,char cAxis,double dStepSize) { return E7XX_STE(ID, cAxis, dStepSize); }
int qSTE(int ID,char cAxis,int iOffsetOfFirstPointInRecordTable,int iNumberOfValues,double* pdValueArray) { return E7XX_qSTE(ID, cAxis, iOffsetOfFirstPointInRecordTable, iNumberOfValues, pdValueArray); }
int IMP(int ID,char cAxis,double dImpulseSize) { return E7XX_IMP(ID, cAxis, dImpulseSize); }
int IMP_PulseWidth(int ID,char cAxis,double dImpulseSize,int iPulseWidth) { return E7XX_IMP_PulseWidth(ID, cAxis, dImpulseSize, iPulseWidth); }
int qIMP(int ID,char cAxis,int iOffsetOfFirstPointInRecordTable,int iNumberOfValues,double* pdValueArray) { return E7XX_qIMP(ID, cAxis, iOffsetOfFirstPointInRecordTable, iNumberOfValues, pdValueArray); }
int SAI(int ID,const char* szOldAxes,const char* szNewAxes) { return E7XX_SAI(ID, szOldAxes, szNewAxes); }
int qSAI(int ID,char* axes,int iBufferSize) { return E7XX_qSAI(ID, axes, iBufferSize); }
int qSAI_ALL(int ID,char* axes,int iBufferSize) { return E7XX_qSAI_ALL(ID, axes, iBufferSize); }
int CCL(int ID,int iComandLevel,const char* szPassWord) { return E7XX_CCL(ID, iComandLevel, szPassWord); }
int qCCL(int ID,int* piComandLevel) { return E7XX_qCCL(ID, piComandLevel); }
int AVG(int ID,int iAverrageTime) { return E7XX_AVG(ID, iAverrageTime); }
int qAVG(int ID,int* iAverrageTime) { return E7XX_qAVG(ID, iAverrageTime); }
int DIO(int ID,const char* szChannels,const int* pbValueArray) { return E7XX_DIO(ID, szChannels, pbValueArray); }
int qTIO(int ID,int* piInputNr,int* piOutputNr) { return E7XX_qTIO(ID, piInputNr, piOutputNr); }

/////////////////////////////////////////////////////////////////////////////

// String commands.
int E7XXSendString(int ID,const char* szString) { return E7XX_E7XXSendString(ID, szString); }
int E7XXGetLineSize(int ID,int* iLineSize) { return E7XX_E7XXGetLineSize(ID, iLineSize); }
int E7XXReadLine(int ID,char* szString,int iBufferSize) { return E7XX_E7XXReadLine(ID, szString, iBufferSize); }
int GcsCommandset(int ID,const char* szCommand) { return E7XX_GcsCommandset(ID, szCommand); }
int GcsGetAnswer(int ID,char* szAnswer,int iBufferSize) { return E7XX_GcsGetAnswer(ID, szAnswer, iBufferSize); }
int GcsGetAnswerSize(int ID,int* iAnswerSize) { return E7XX_GcsGetAnswerSize(ID, iAnswerSize); }

/////////////////////////////////////////////////////////////////////////////

// limits
int ATZ(int ID,const char* szAxes,const double* pdLowVoltageArray,const int* pfUseDefaultArray) { return E7XX_ATZ(ID, szAxes, pdLowVoltageArray, pfUseDefaultArray); }
int qTMN(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qTMN(ID, szAxes, pdValueArray); }
int qTMX(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qTMX(ID, szAxes, pdValueArray); }
int NLM(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_NLM(ID, szAxes, pdValueArray); }
int qNLM(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qNLM(ID, szAxes, pdValueArray); }
int PLM(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_PLM(ID, szAxes, pdValueArray); }
int qPLM(int ID,const char* szAxes,double* pdValueArray) { return E7XX_qPLM(ID, szAxes, pdValueArray); }
int GetRefResult(int ID,const char* szAxes,int* piReferenceResult) { return E7XX_GetRefResult(ID, szAxes, piReferenceResult); }

/////////////////////////////////////////////////////////////////////////////

// Wave commands.
int WAV_SIN_P(int ID,const char* szWaveTableId,int iOffsetOfFirstPointInWaveTable,int iNumberOfPoints,int iAddAppendWave,int iCenterPointOfWave,double dAmplitudeOfWave,double dOffsetOfWave,int iSegmentLength) { return E7XX_WAV_SIN_P(ID, szWaveTableId, iOffsetOfFirstPointInWaveTable, iNumberOfPoints, iAddAppendWave, iCenterPointOfWave, dAmplitudeOfWave, dOffsetOfWave, iSegmentLength); }
int WAV_LIN(int ID,const char* szWaveTableId,int iOffsetOfFirstPointInWaveTable,int iNumberOfPoints,int iAddAppendWave,int iNumberOfSpeedUpDownPointsInWave,double dAmplitudeOfWave,double dOffsetOfWave,int iSegmentLength) { return E7XX_WAV_LIN(ID, szWaveTableId, iOffsetOfFirstPointInWaveTable, iNumberOfPoints, iAddAppendWave, iNumberOfSpeedUpDownPointsInWave, dAmplitudeOfWave, dOffsetOfWave, iSegmentLength); }
int WAV_RAMP(int ID,const char* szWaveTableId,int iOffsetOfFirstPointInWaveTable,int iNumberOfPoints,int iAddAppendWave,int iCenterPointOfWave,int iNumberOfSpeedUpDownPointsInWave,double dAmplitudeOfWave,double dOffsetOfWave,int iSegmentLength) { return E7XX_WAV_RAMP(ID, szWaveTableId, iOffsetOfFirstPointInWaveTable, iNumberOfPoints, iAddAppendWave, iCenterPointOfWave, iNumberOfSpeedUpDownPointsInWave, dAmplitudeOfWave, dOffsetOfWave, iSegmentLength); }
int WAV_PNT(int ID,const char* szWaveTableId,int iOffsetOfFirstPointInWaveTable,int iNumberOfPoints,int iAddAppendWave,double* pdWavePoints) { return E7XX_WAV_PNT(ID, szWaveTableId, iOffsetOfFirstPointInWaveTable, iNumberOfPoints, iAddAppendWave, pdWavePoints); }
int qWAV(int ID,const char* szWaveTableIds,int* piParamereIdsArray,double* pdValueArray) { return E7XX_qWAV(ID, szWaveTableIds, piParamereIdsArray, pdValueArray); }
int qGWD(int ID,char cWaveTableId,int iOffsetOfFirstPointInWaveTable,int iNumberOfValues,double* pdValueArray) { return E7XX_qGWD(ID, cWaveTableId, iOffsetOfFirstPointInWaveTable, iNumberOfValues, pdValueArray); }
int WGO(int ID,const char* szWaveGeneratorIds,const int* iStartModArray) { return E7XX_WGO(ID, szWaveGeneratorIds, iStartModArray); }
int qWGO(int ID,const char* szWaveGeneratorIds,int* piValueArray) { return E7XX_qWGO(ID, szWaveGeneratorIds, piValueArray); }
int WGC(int ID,const char* szWaveGeneratorIds,const int* piNumberOfCyclesArray) { return E7XX_WGC(ID, szWaveGeneratorIds, piNumberOfCyclesArray); }
int qWGC(int ID,const char* szWaveGeneratorIds,int* piValueArray) { return E7XX_qWGC(ID, szWaveGeneratorIds, piValueArray); }
int qTNR(int ID,int* iRecordCannels) { return E7XX_qTNR(ID, iRecordCannels); }
int DRC(int ID,const int* piRecordChannelIdsArray,const char* szRecordSourceIds,const int* piRecordOptionArray,const int* piTriggerOption) { return E7XX_DRC(ID, piRecordChannelIdsArray, szRecordSourceIds, piRecordOptionArray, piTriggerOption); }
int qDRC(int ID,const int* piRecordChannelIdsArray,char* szRecordSourceIds,int* piRecordOptionArray,int* piTriggerOption,int iArraySize) { return E7XX_qDRC(ID, piRecordChannelIdsArray, szRecordSourceIds, piRecordOptionArray, piTriggerOption, iArraySize); }
int WGR(int ID) { return E7XX_WGR(ID); }
int qDRR_SYNC(int ID,int iRecordChannelId,int iOffsetOfFirstPointInRecordTable,int iNumberOfValues,double* pdValueArray) { return E7XX_qDRR_SYNC(ID, iRecordChannelId, iOffsetOfFirstPointInRecordTable, iNumberOfValues, pdValueArray); }
int RTR(int ID,int iReportTableRate) { return E7XX_RTR(ID, iReportTableRate); }
int qRTR(int ID,int* piReportTableRate) { return E7XX_qRTR(ID, piReportTableRate); }
int qTWG(int ID,int* piGenerator) { return E7XX_qTWG(ID, piGenerator); }
int WMS(int ID,const char* szWaveTablesIds,const int* iMaxWaveSize) { return E7XX_WMS(ID, szWaveTablesIds, iMaxWaveSize); }
int qWMS(int ID,const char* szWaveTablesIds,int* iMaxWaveSize) { return E7XX_qWMS(ID, szWaveTablesIds, iMaxWaveSize); }
int DTC(int ID,int iDdlTableId) { return E7XX_DTC(ID, iDdlTableId); }
int WCL(int ID,int iWaveTableId) { return E7XX_WCL(ID, iWaveTableId); }
int DDL(int ID,int iDdlTableId,int iOffsetOfFirstPointInDdlTable,int iNumberOfValues,const double* pdValueArray) { return E7XX_DDL(ID, iDdlTableId, iOffsetOfFirstPointInDdlTable, iNumberOfValues, pdValueArray); }
int qDDL(int ID,int iDdlTableId,int iOffsetOfFirstPointInDdlTable,int iNumberOfValues,double* pdValueArray) { return E7XX_qDDL(ID, iDdlTableId, iOffsetOfFirstPointInDdlTable, iNumberOfValues, pdValueArray); }
int TWS(int ID,const int* piWavePointNumberArray,const int* piTriggerLevelArray,int iNumberOfPoints) { return E7XX_TWS(ID, piWavePointNumberArray, piTriggerLevelArray, iNumberOfPoints); }
int TWC(int ID) { return E7XX_TWC(ID); }
int qTLT(int ID,int* piDdlTables) { return E7XX_qTLT(ID, piDdlTables); }
int DPO(int ID,const char* szAxes) { return E7XX_DPO(ID, szAxes); }
int IsGeneratorRunning(int ID,const char* szWaveGeneratorIds,int* pbValueArray) { return E7XX_IsGeneratorRunning(ID, szWaveGeneratorIds, pbValueArray); }

/////////////////////////////////////////////////////////////////////////////

// Piezo-Channel commands.
int VMA(int ID,const char* szPiezoChannels,const double* pdValueArray) { return E7XX_VMA(ID, szPiezoChannels, pdValueArray); }
int qVMA(int ID,const char* szPiezoChannels,double* pdValueArray) { return E7XX_qVMA(ID, szPiezoChannels, pdValueArray); }
int VMI(int ID,const char* szPiezoChannels,const double* pdValueArray) { return E7XX_VMI(ID, szPiezoChannels, pdValueArray); }
int qVMI(int ID,const char* szPiezoChannels,double* pdValueArray) { return E7XX_qVMI(ID, szPiezoChannels, pdValueArray); }
int VOL(int ID,const char* szPiezoChannels,const double* pdValueArray) { return E7XX_VOL(ID, szPiezoChannels, pdValueArray); }
int qVOL(int ID,const char* szPiezoChannels,double* pdValueArray) { return E7XX_qVOL(ID, szPiezoChannels, pdValueArray); }
int qTPC(int ID,int* iPiezoCannels) { return E7XX_qTPC(ID, iPiezoCannels); }

/////////////////////////////////////////////////////////////////////////////

// Sensor-Channel commands.
int qTAD(int ID,const char* szSensorChannels,int* piValueArray) { return E7XX_qTAD(ID, szSensorChannels, piValueArray); }
int qTNS(int ID,const char* szSensorChannels,double* pdValueArray) { return E7XX_qTNS(ID, szSensorChannels, pdValueArray); }
int qTSP(int ID,const char* szSensorChannels,double* pdValueArray) { return E7XX_qTSP(ID, szSensorChannels, pdValueArray); }
int qTSC(int ID,int* iSensorCannels) { return E7XX_qTSC(ID, iSensorCannels); }
int qTAV(int ID,const char* szAnalogInputChannels,int* piValarray) { return E7XX_qTAV(ID, szAnalogInputChannels, piValarray); }

/////////////////////////////////////////////////////////////////////////////

// Spezial
int GetSupportedFunctions(int ID,int* piComandLevelArray,int iMaxlen,char* szFunctionNames,int iMaxFunctioNamesLength) { return E7XX_GetSupportedFunctions(ID, piComandLevelArray, iMaxlen, szFunctionNames, iMaxFunctioNamesLength); }
int GetSupportedParameters(int ID,int* piParameterIdArray,int* piComandLevelArray,int* piMennoryLocationArray,int iMaxlen,char* szParameterName,int iMaxParameterNameSize) { return E7XX_GetSupportedParameters(ID, piParameterIdArray, piComandLevelArray, piMennoryLocationArray, iMaxlen, szParameterName, iMaxParameterNameSize); }
int GetSupportedControllers(char* szBuffer,int iBufferSize) { return E7XX_GetSupportedControllers(szBuffer, iBufferSize); }
int NMOV(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_NMOV(ID, szAxes, pdValueArray); }
int NMVR(int ID,const char* szAxes,const double* pdValueArray) { return E7XX_NMVR(ID, szAxes, pdValueArray); }
int TWS_EQU_DIST(int ID,int iTriggerPointStart,int iTriggerDistance,int iTriggerPointsPerPuls,int iTriggerPointsCount,int iTriggerLevel) { return E7XX_TWS_EQU_DIST(ID, iTriggerPointStart, iTriggerDistance, iTriggerPointsPerPuls, iTriggerPointsCount, iTriggerLevel); }
int WAV_LIN_SPEED_UP(int ID,int iSpeedUpDown,double rSpeed,double* rSpeedUpDown) { return E7XX_WAV_LIN_SPEED_UP(ID, iSpeedUpDown, rSpeed, rSpeedUpDown); }

#endif