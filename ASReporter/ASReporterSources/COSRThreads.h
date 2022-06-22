// ===========================================================================
//	COSRThreads.h
//	Copyright (C) 1997 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#if !defined(AFX_COSRTHREADS_H__28557B71_F9C6_11D0_9F12_0020AFFA70C1__INCLUDED_)
#define AFX_COSRTHREADS_H__28557B71_F9C6_11D0_9F12_0020AFFA70C1__INCLUDED_

// ---------------------------------------------------------------------------
#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

// ---------------------------------------------------------------------------
UINT OSRListenThreadProc(LPVOID pParam);
UINT OSRReadThreadProc(LPVOID pParam);

// ---------------------------------------------------------------------------
#endif // !defined(AFX_COSRTHREADS_H__28557B71_F9C6_11D0_9F12_0020AFFA70C1__INCLUDED_)
