#ifndef _DVC_META_H_
#define _DVC_META_H_

#ifndef _DVC_API_H_
#include "./dvcapi.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dvcList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern "C" {
typedef struct _avData_ { char *attr ; char *value ; } AV, *AVP ;
} ;

class DVCAPI_API CDVCMeta 
{
public:
	CDVCMeta() ;
	virtual ~CDVCMeta() ;
	BOOL Set(char *attr, char *value) ;
	BOOL Set(char *attr, int nValue) ;
	BOOL Set(char *attr, bool bValue) ;
	BOOL Set(char *attr, double dValue) ;
	BOOL ParseDescription( char *szDescription, BOOL bAppend ) ;
	void Reset() ;
	void Lock() ;
	void Unlock() ;
	char* Get(char *attr) ;
	BOOL Get(char *attr, char *rValue, int nLen ) ;
	BOOL Get(char *attr, int *pValue) ;
	BOOL Get(char *attr, bool *pValue) ;
	BOOL Get(char *attr, double *pValue) ;
	BOOL Remove(char *attr) ;
	void  freeMeta( char *pMeta );
	BOOL GetDescription( char *szDescription, int len, int *pReturnLen ) ;
	BOOL ParseImageMetaData( ImageMetaDataP pMeta );
	BOOL Copy( CDVCMeta& cMeta, BOOL bAppend );
	AVP First() ;
	AVP Last() ;
	AVP Next() ;
	int Length() ;
private:
	CDVCList mList ;
	char* LocalGet(char *attr) ;
} ;

#endif
