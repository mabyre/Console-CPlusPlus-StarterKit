//=======================================================================
// LBitmap.h
// Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// Description:
//   Attributes:
//      IMAGE = ID [, n, [r,g,b]] (n = nombre d'images, r,g,b :transcolor for this image)
//      NOFRAME
//      FRAME
//      3DFRAME
//      HCENTERIMAGE (center the bitmap in the control if if is bigger than the bitmap)
//      VCENTERIMAGE
//      AUTOSIZE
//      TRANSCOLOR = r,g,b (color to use for transparency. MUST be specified before any IMAGE attribute)
//      TRANSBKGND transparent background for the control itself
//		ANIMATED = delay;
//		ANIMNOERASE = prevents from erasing the background between each step of animation
//		COLORBITS = nb color bits (0,1,4,8,16,24,32: See LImageList) (MUST be specified before any IMAGE attribute
//		USEPALETTE
//
// Revision history :
//
//=======================================================================

#ifndef LBITMAP_H
#define LBITMAP_H

//-----------------------------------------------------------------------
#include "LControl.h"
#include "LImgList.h"

#define OVM_ANIM_STARTSTOP (WM_USER+4099)

//-----------------------------------------------------------------------
class LBitmap : 
	public CWnd,
	public LOVControl
{
public:
	virtual ~LBitmap();

	enum eFrameType
	{
		kNoFrame = 0,
		kFrame,
		k3DFrame
	};
		
	virtual BOOL AddBitmap(UINT anID, UINT aNb = 1, COLORREF aMaskColor = (COLORREF)-1);
	virtual void Reset();

	virtual void AdjustSize();
	virtual void SetDrawingStyle(int aStyle, BOOL afRedraw = TRUE);

	virtual BOOL SetCurrentBmp(UINT anIndex = 0);
	virtual void SetNextBmp();
	virtual void SetPrevBmp();
	virtual UINT GetCurrentBmp();

	virtual void StartAnim(UINT aDelay);
	virtual void StopAnim();

	virtual LImageList*	GetImageList();

protected:
	LImageList*	itsImageList;
	eFrameType	itsFrameType;
	int			itsFrameSize;
	int			itsDrawingStyle;
	UINT		itsCurrentBmp;
	UINT		itsDelay;


	LBitmap();

	virtual void 	Init();
	virtual void	DoAttribute(char* anAttribute, char* aValue);

	virtual void 	InternalInit();
	virtual void	DrawFrame(CDC* aDC);

	afx_msg void 	OnPaint();
	afx_msg BOOL 	OnEraseBkgnd(CDC* aDC);
	afx_msg void	OnTimer(UINT anIDEvent);
	afx_msg LRESULT	OnStartStop(WPARAM aWParam, LPARAM aLParam);
	afx_msg LRESULT	OnSendToParent(WPARAM aWParam, LPARAM aLParam);

	DECLARE_OV_BASECONTROL(LBitmap)
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------
#endif // LBITMAP_H
