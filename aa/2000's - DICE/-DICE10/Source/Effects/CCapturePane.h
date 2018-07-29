/* History:Copyright (c) 2001-2003 John Henry Thompson. All rights reserved.2000.12.??	jht	Created. Trying to find my way in a digital world.*/#pragma once#include <LPane.h>#include <LListener.h>#include <QTML.h>#include <QuickTimeComponents.h>#include <Files.h>#include "CGenEffect.h"#include "CBoxWalkEffect.h"#define SCALE_FACTOR (2)#if 0#define AVG_TYPE	int#else#define AVG_TYPE	float#endif#define DIFF_MIN (30)const ResIDT	kCapturePane = 1020;typedef struct { 	int count; 	int pixel; 	} CountPixel;#define kCountPixelBucketSize	(32768)class CCapturePane : public LPane,						public LCommander,						public LPeriodical{public:	enum { class_ID = 'Capt' };						CCapturePane();						CCapturePane( const SPaneInfo &inPaneInfo );						CCapturePane( const CCapturePane &inOriginal );						CCapturePane( LStream *inStream );	virtual				~CCapturePane();	void				SetEffectNum( int n);	int					GetEffectNum();		void				SetMeuVisible( int n);	int					GetMeuVisible();	void				SetDiffTolerance( int n);	int					GetDiffTolerance();	void				SetStillFactor( int n);	int					GetStillFactor();		void				SetCaptureScale( int n);	int					GetCaptureScale();		void				SetInk( int n);	int					GetInk();	void				SetBlend( int n);	int					GetBlend();		void				SetDrawRect( Rect rect);	void				GetDrawRect(Rect &outRect);	void				SetColorCycleNum( int n);	int					GetColorCycleNum();		void				SetColorCycleCount( int n);	int					GetColorCycleCount();		void				SetDiffBackImage();	void				SetLoopBackImage( int state );	void				SetBackImagePeriod( int num );		void				SetBackImage( FSSpec &fss);		void				SetBrushImage( FSSpec &fss);		void				SetInputDeviceNum( int aDevNum);	int					GetInputDeviceNum();		void				SetLoopEffectDuration( int dur);	void				SetLoopEffects( int state);		void				SetLoopBoxesDuration( int dur);	void				SetLoopBoxes( int state);	void				SetImageBox( int state);	void				SetFullDraw();	void				SetLoopFullDraw( int state );	void				SetLoopFullDrawPeriod( int num );		void				SetBoxScale( int num);	void				SetConvolutionFactor( int num);		void				SetPaletteNum( int num);	//protected:		virtual Boolean			ObeyCommand(									CommandT			inCommand,									void*				ioParam);										virtual void			FindCommandStatus(								CommandT			inCommand,								Boolean&			outEnabled,								Boolean&			outUsesMark,								UInt16&				outMark,								Str255				outName);	virtual	void		SpendTime(									const EventRecord&	inMacEvent);	void				FinishCreateSelf();	virtual void		DrawSelf();	virtual void		ClickSelf( const SMouseDownEvent &inMouseDown );	//virtual void		AdjustCursorSelf( Point inPortPt,	//						const EventRecord &inMacEvent );	/* From HackTV QT code example */	SeqGrabComponent		SeqGrabber;	SGChannel				VideoChannel;	//SGChannel				SoundChannel;	//GrafPtr					Monitor;	//Rect					ActiveVideoRect;	//ICMAlignmentProcRecord	SeqGrabberAlignProc;	//Boolean					RecordVideo;	//Boolean					RecordSound;	//Boolean					SplitTracks;		/* ---------------------------- */		void CopyBitsEff (const BitMap *srcBits, const BitMap *dstBits, const Rect *srcRect,  const Rect *dstRect);	GWorldPtr	CreateGWorldForDepth(int depth, Rect bounds, CTabHandle ctable);		void	CreateGWorld(Rect bounds);	void	CreateSeqGrab(int devNum, Rect bounds, GWorldPtr monitor);	void	CreateAvgBuffer(Rect bounds);	void	CreateLineMap(int width );	void	CreateBuffers();	void	ReleaseBuffers();	void 	ResizeBuffers(int newCaptureScale );	void	UpdateFromGWorld();	void	UpdateStatus();	void	StepColorCycle();	void	StepEffects();	void	ImportFileIntoGWorld( ConstStr255Param fileName, GWorldPtr gworldPtr, Rect bounds);	void	ImportFileSpecIntoGWorld( FSSpec &fss, GWorldPtr gworldPtr, Rect bounds);		void	DrawLayer(char *, Rect &inRect);	void	CopyDirect();	void	ComputeAverage();	void 	ComputeMotionDiff();	void 	ComputeMotionMoWhite();	void 	ComputeMotionMoBlack();	void 	ComputeMotionNoMoBlack();	void 	ComputeMotionTrailWhite();	void 	ComputeMotionTrailImage();	void 	ComputeMotionTrailColor();	void 	ComputeMotionClampWhite();	void 	ComputeMotionClampMax();	void 	ComputeMotionDiffGray();	void 	ComputeMotionStreak();	void 	ComputeMotionStreakColor();	void 	ComputeMotionStreakColorTrail();	void	ComputeLineMap();	void	ComputeMotionPaint();	void	ComputeBlackWhiteDiffusion();	void	ComputeCovMoTrailsColor();	void	ComputeCov8MoTrailsColor();	void	ComputeMotionStreakEdge();	void	ComputeTransLayer();	void	ComputeColorMeter1();	void		ComputeAverage2();	void	ComputeColorFreq();		void	ShowHideMenu(int menuVis);		void	ResetTimers();	void 	SaveImageAsNewFile();	void 	SaveGWorldToFSpec( GWorldPtr gWorld, FSSpec &fspec );	void 	SaveImageAsNextFile();	int						convolutionFactor;		int						loopBackImage;	int						loopBackImagePeriod;	int						loopBackImageTimelast;	int						loopFullDraw;	int						loopFullDrawPeriod;	int						loopFullDrawTimeLast;		int						loopBoxes;	int						loopBoxesTimePeriod;	int						loopBoxesTimeLast;	int						loopBoxesRange;	int						imageBox;	Rect					boxRect;		int						loopEffects;	int						loopTimePeriod;	int						loopTimeLast;	int						inputDeviceNum;	int 					frameCount;	int 					startTick;	int						grabStarted;	int						colorCycleNum;	float					color[3];	int						color_int[3];	float					colorCycleDelta[3];	int						colorCycleCount;	int						colorCycleIndex;		int						effectNum;	int						menuVisible;	int						diffTolerance;	int						diffTolerance_x3; // *3 diffTolerance	int						inkMode;	int						blendLevel;	int						captureScale;	int						drawScale;		AVG_TYPE				stillScale;		Rect					captureRect;	Rect					drawRect;		GWorldPtr				gwSourcePtr;			/* Digital Captured Source */	GWorldPtr				gwPreviousPtr;			/* Previous Frame */	GWorldPtr				gwMonitorPtr;			/* Computed frame to display */		GWorldPtr				gwPrevious2Ptr;			/* Secondary Previous Frame */	GWorldPtr				gwBackPtr;				/* Background image  */	GWorldPtr				gwBrushPtr;				/* Brush image  */	GWorldPtr				gwCompositePtr;			/* Composite image  */	GWorldPtr				gwAvgPtr;				/* Frame for averged pixels */	AVG_TYPE				*avgBufferPtr;		GWorldPtr				gwOneBitPtr;			/* One bit for Black & White */	GWorldPtr				gwMonitor8Ptr;			/* 8-bit world */		GWorldPtr				gwLastOutput;			/* Last Gworld drawn to screen */	int						mapIndex;	int						mapCount;	int						mapCount2;	int						mapDirection;	int						*lineMapStart;	int						*lineMapDest;	int						*lineMap;		static FSSpec			outFSSpec;	static int				fileNum ;		CBoxWalkEffect			*boxWalkEffect;		CountPixel				*countPixelBucket;};enum{	kColorCycleNone = 0,	kColorCycleGray = 1,	kColorCycleGrayBounce = 2,	kColorCycleRandom = 3,	kColorCycleRandomBounce = 4,	kColorCycleRandomBright = 5,	kColorCycleLast};enum{	kEffectDirect = 0,	kEffectAverage = 1,	kEffectMotionDiff = 2,	kEffectMotionMoWhite = 3,	kEffectMotionMoBlack = 4,	kEffectMotionNoMoBlack = 5,	kEffectMotionTrailWhite = 6,	kEffectMotionTrailImage = 7,	kEffectMotionTrailColor = 8,	kEffectMotionClampWhite = 9,	kEffectMotionClampMax = 10,	kEffectMotionDiffGray = 11,	kEffectMotionStreak = 12,	kEffectMotionStreakColor = 13,	kEffectMotionStreakColorTrail = 14,	kEffectLineMap = 15,	kEffectMotionPaint = 16,	kEffectBlackWhiteDiffusion = 17,	kEffectCovMoTrailsColor = 18,	kEffectCov8MoTrailsColor = 19,	kEffectMotionStreakEdge = 20,	kEffectMotionTransLayer = 21,	kEffectMotionColorMeter1 = 22,	kEffectMotionColorFreq = 23,	kEffectMotionLast};