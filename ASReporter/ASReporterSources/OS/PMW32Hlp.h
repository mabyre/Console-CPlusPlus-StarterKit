// ===========================================================================
//	PMW32Hlp.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMW32Hlp_h
#define PMW32Hlp_h

// ---------------------------------------------------------------------------
#include "PMOSHelp.h"
#include "PMWinInc.h"
#include "PMError.h"
#include "PMStr.h"

// ---------------------------------------------------------------------------
#ifndef PMFinalRelease
	/**
		Available in debug version of ObjectSet only.
		Use it to temporarily disable memory leaks tracking to allocate a memory block 
		that will not be reported as leaking when exiting.
		This should not be used unless your are quite sure of what you are doing.
	*/
	extern BOOL EnableMemoryTracking(BOOL afEnable);
#endif

// ---------------------------------------------------------------------------
PMDEFINE( PMOSWin32Helper );

// ---------------------------------------------------------------------------
/** OSHelper for Win32 platforms. */

class PMOSWin32Helper : public PMOSHelper
{
public:
	PMOSWin32Helper();

		/**	Gets current OS helper. */
	static PMOSWin32HelperRef GetCurrent();

		/**	
		Initializes ObjectSet. If afInitTCP is false, TCP/IP features 
		will not be enabled until you call \Ref{InitTCP}. If you do not need TCP/IP 
		features then you do not need to call InitTCP.
		*/
	static PMError		Init(HINSTANCE anInstance, pmbool afInitTCP = pmtrue);
		/**	Initializes ObjectSet TCP/IP features. */
	static PMError		InitTCP();
		/**	Finalizes ObjectSet. Make sure not to use ObjectSet in any way after calling this method. */
	static void			Close(void* aData = 0);

		/** Returns the HINSTANCE of your application that you passed to \Ref{Init} */
	static HINSTANCE	GetInstanceHandle();

		/**
		Specifies whether or not to use long file names in various functions.
		Until you call this method, long file names are not used.
		*/
	virtual pmbool		UseLongFileNames(pmbool afUseLongFileNames = pmtrue);

		/**
		Validates the length of the file name and the characters used in it.
		Relies on the value passed to \Ref{UseLongFileNames}.
		*/
	virtual PMStrRef	ValidateFileName(const PMStrRef& aFileName);

		/**	Creates a random file name. Will not create the same name twice for a given process. */
	virtual PMStrRef CreateRandomFileName();

		/**	Sets the type of temporary media to create. */
	virtual void SetTemporaryMediaType(PMMediaTemporary_eType aType);

		/**	Returns the platform charset. */
	virtual PMCharacterSet_eCode GetPlatformCharset();

		/**	Returns the platform end-of-line code. */
	virtual PMEndOfLine_eCode GetPlatformEndOfLine();

		/**	Returns the hostname. */
	virtual PMStrRef GetHostName();

		/**	Converts a PMError to a Windows-like error. */
	virtual DWORD PMErrorToWinError(const PMError& anError);
	
		/**	Returns a readable string for anError. */
	virtual PMStrRef GetErrorString(const PMError& anError);

		/**	Returns a readable string for anError returned by PMErrorToWinError. */
	virtual PMStrRef GetErrorString(DWORD anError);

		/** Displays an assertion in the Windows fashion. Used by all Windows tracers. */
	virtual void	DisplayAssert(const char* aTest, PMTraceSendParams* aParams);

		/**	Retrieves the MIME type associated with the given file extension. */
	virtual	PMStrRef		ExtensionToMimeType(const PMStrRef& anExtension);
		/**	Retrieves the MIME type associated with the given file extension. */
	virtual PMStrRef		FileNameToMimeType(const PMStrRef& aFileName);
		/**	Retrieves the file extension associated with the given MIME type. */
	virtual PMStrArrayRef	MimeTypeToExtension(const PMStrRef& aMimeType, pmbool afReturnDefault = pmtrue);
	
		/**	
		Retrieves the Icon associated with the given file extension.
		If afLarge is false, it returns the small icon (ignored on NT3.51).
		*/
	virtual HICON			ExtensionToIcon(const PMStrRef& anExtension, pmbool afLarge = pmtrue);
		/**	
		Retrieves the Icon associated with the given file extension.
		If afLarge is false, it returns the small icon (ignored on NT3.51).
		*/
	virtual HICON			FileNameToIcon(const PMStrRef& aFileName, pmbool afLarge = pmtrue, pmbool* afIsDefault = 0);

		/**	
		Returns the file extension for an unknown MIME Type. This method is called internally
		and you can override it to return something other than the default value (.txt).
		*/
	virtual PMStrRef		ExtensionForUnknownMimeType(const PMStrRef& aMimeType);
		/**	
		Returns the default MIME Type to use when you cannot choose another that woudld be better.
		This method is called internally and you can override it to return something other  
		than the default value ("application/octet-stream").
		*/
	virtual PMStrRef		DefaultMimeType();

//	virtual UINT			NeedAssociation(HWND hwnd, LPCTSTR lpszFileName, LPCTSTR lpszMIMEType, LPTSTR lpExecutable, UINT uSize);

		/**	
		Resets the various icon caches used by previous methods.
		You can call it upon notification that the registry has changed	
		*/
	virtual void			RegistryChanged();

protected:
	static HINSTANCE	chInstance, chRCInstance;

	pmbool				itsfUseLongFileNames;
};

// ---------------------------------------------------------------------------

#endif // PMW32Hlp_h

