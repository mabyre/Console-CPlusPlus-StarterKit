/*	===========================================================================
	cPPP.h
	Copyright (C) 1996-98 AbyreSoft. All rights reserved.
	===========================================================================	*/

#ifndef cPPP_h
#define cPPP_h

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */

/*	---------------------------------------------------------------------------	*/
	
#include "cTypes.h"
#include "cPPPDef.h"

/*	---------------------------------------------------------------------------	*/

	/** 
	Creates a PPP object. You should only have to create one instance of
	this object.	
	*/
pPPP	PPP_New();

	/** Deletes a PPP object.	*/
void	PPP_Delete(pPPP *self);

	/** 
	Establishes the PPP connection.	You need to call this method at least
	one time. You can call it each time you need to be sure the connection is up.
	*/
pmbool	PPP_Connect(pPPP self, pmbool *afCancel);

	/** 
	Returns if the PPP connection is up. If this function returns true, the connection
	may not be up (this depends on the serial implementation). But if it returns false,  
	it is sure that the connection is down.	
	*/
pmbool	PPP_IsConnected(pPPP self);

	/**
	Performs PPP disconnection. If another process uses the stack it is not 
	disconnected.
	*/
void	PPP_Disconnect(pPPP self);

/*	---------------------------------------------------------------------------	*/

#include "cEnvPPP.h"

/*	---------------------------------------------------------------------------	*/

#ifdef __cplusplus
	}
#endif /* __cplusplus */

/*	---------------------------------------------------------------------------	*/

#endif
