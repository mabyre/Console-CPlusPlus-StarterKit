// ===========================================================================
//	PMMonit.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMMonit_h
#define PMMonit_h

// ---------------------------------------------------------------------------

#include "PMMoniCB.h"

// ===========================================================================
//	PMMonitorable
// ===========================================================================

/**	Internal use.	*/
class PMMonitorable : public PMMonitorCallBack
{
public:
		/**	Internal use.	*/
	PMMonitorable(pmuint32 aClass);
	
		/**	Internal use.	*/
	virtual ~PMMonitorable();

		/**	Internal use.	*/
	virtual void MonitorIn(const PMMonitorInfoRef &anInfo);
	
		/**	Internal use.	*/
	virtual void SetCallBack(PMMonitorCallBack *aCallBack);

		/**	Internal use.	*/
	virtual pmuint32	GetClass() { return itsClass; }

		/**	Internal use.	*/
	PMMonitorInfoRef	MakeTextInfo(const PMStrRef &aText);
	
protected:
	PMMonitorCallBack	*itsCallBack;
	pmuint32			itsClass;

	virtual void MonitorOut(const PMMonitorInfoRef &anInfo);

private:
	PMMonitorable();
};

// ---------------------------------------------------------------------------

#endif	// PMMonit_h

