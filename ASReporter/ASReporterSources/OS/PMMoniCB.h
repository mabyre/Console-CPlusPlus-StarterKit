// ===========================================================================
//	PMMoniCB.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMMoniCB_h
#define PMMoniCB_h

// ---------------------------------------------------------------------------

#include "PMStr.h"

// ===========================================================================
//	PMMonitorInfo
// ===========================================================================

PMDEFINE(PMMonitorInfo);

class PMMonitorable;

/**	Internal use.	*/
class PMMonitorInfo : public PMRC
{
public:
		/**	Internal use.	*/
	PMMonitorInfo(PMMonitorable *anObject,const PMStrRef& aText);
	
		/**	Internal use.	*/
	virtual ~PMMonitorInfo();
	
		/**	Internal use.	*/
	PMStrRef	GetText()	
		{ return itsText; }

protected:
	PMStrRef		itsText;
	PMMonitorable	*itsObject;
	
private:
	PMMonitorInfo();
};

// ===========================================================================
//	PMMonitorCallBack
// ===========================================================================

/**	Internal use.	*/
class PMMonitorCallBack
{
public:
		/**	Internal use.	*/
	PMMonitorCallBack();
	
		/**	Internal use.	*/
	virtual ~PMMonitorCallBack();
	
		/**	Internal use.	*/
	virtual void MonitorIn(const PMMonitorInfoRef &anInfo) = 0;
};

// ---------------------------------------------------------------------------

#endif	// PMMoniCB_h

