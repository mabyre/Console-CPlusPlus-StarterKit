//=======================================================================
// LControl.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// Basic class for all OV's Controls
//=======================================================================

#ifndef LCONTROL_H
#define LCONTROL_H

#include <afxwin.h>
#include "LDefines.h"

// itsOVStyle can be used for controls OVAttributes.
// b16-b31 are reserved for common attributes
// b0-b15 can be used by controls

//-----------------------------------------------------------------------
class LOVControl
{
	friend class LOVRegister;

public:
	virtual ~LOVControl();

	virtual BOOL SetfAutoDelete(BOOL afAutoDelete = TRUE);
	virtual BOOL GetfAutoDelete();

	static const char* const GetCustomClassName();

protected:
	LOVControl();

	DWORD			itsOVStyle;

	virtual void 	DoAttribute(char* anAttribute, char* aValue);
	virtual void	Init();

private:
	LOVControl(const LOVControl&);
	LOVControl& operator=(const LOVControl&);
};

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define REGISTER_OV_CONTROL(class_name)		\
	LOVRegister::RegisterClass(class_name::cClassName, RUNTIME_CLASS(class_name))

#define UNREGISTER_OV_CONTROL(class_name)	\
	LOVRegister::UnRegisterClass(class_name::cClassName)

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define DECLARE_OV_BASECONTROL(class_name)		\
public:											\
	static const char* const cClassName;		\
												\
protected:										\
	virtual void 		PostNcDestroy();		\
												\
	DECLARE_DYNCREATE(class_name)

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define IMPLEMENT_OV_BASECONTROL(class_name, base_class_name, window_name)	\
/*----------------------------------------------------------------*/	\
const char* const class_name::cClassName = window_name;					\
/*----------------------------------------------------------------*/	\
void class_name::PostNcDestroy() {										\
/*	UnsubclassWindow();	*/ /* Too late to do it. m_hWnd == 0 */			\
	if (GetfAutoDelete())												\
		delete this;													\
}																		\
/*----------------------------------------------------------------*/	\
IMPLEMENT_DYNCREATE(class_name, base_class_name)


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define DECLARE_OV_SUBCONTROL(class_name)	\
public:										\
	static const char* const cClassName;	\
											\
protected:									\
	DECLARE_DYNCREATE(class_name)

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define IMPLEMENT_OV_SUBCONTROL(class_name, base_class_name, window_name)	\
/*----------------------------------------------------------------*/	\
const char* const class_name::cClassName = window_name;					\
/*----------------------------------------------------------------*/	\
IMPLEMENT_DYNCREATE(class_name, base_class_name)

//-----------------------------------------------------------------------
#endif // LCONTROL_H
