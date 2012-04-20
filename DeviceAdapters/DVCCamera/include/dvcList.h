// dvcList.h: interface for the dvcList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_DVC_LIST_H)
#define _DVC_LIST_H

#ifndef _DVC_API_H_
#include "./dvcAPI.h"
#endif

//#include <stdlib.h>
//#include <winbase.h>



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// extern "C" {
	typedef struct dvcList_t
	{
		struct dvcList_t *next ;
		struct dvcList_t *last ;
		void *element ;
	} dvcList, *dvcListP ;
// }


class DVCAPI_API CDVCList  
{
public:
	CDVCList();
	virtual ~CDVCList();
	void	Lock() ;
	void	Unlock() ;
	BOOL Insert(int nIndex, void *item );
	BOOL Insert(void *item );
	BOOL Append(void *item );
	BOOL Append(int nIndex, void *item );
	BOOL Cut(void *item );
	BOOL Cut(int nIndex);
	int	 Index(void *item);
	void *Element(int nIndex);
	void *First() ;
	void *Last() ;
	void *Next() ;
	void *Previous() ;
	int  Length();

protected:
	dvcListP 			 head ;
	dvcListP 			 pCurrent ;
	int		 			 nElements ;
	CRITICAL_SECTION	 mutex ; 
};

#endif // !defined(_DVC_LIST_H)
