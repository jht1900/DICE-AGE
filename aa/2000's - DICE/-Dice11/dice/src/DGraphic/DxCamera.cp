// --------------------------------------------------------------------------------
//
// Video Source Input
//
// Plan:
//		Use frame complete call back
//
// 2005.02.21 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#include "DxCamera_js.h"
#include "DxCamera.h"

#include "DxImage_js.h"
#include "DxImage.h"

#include "dicejs.h"

// --------------------------------------------------------------------------------
DxCamera::DxCamera(void *wrapperObj )
	: DxGeneric(wrapperObj )
{
	this->inputDeviceNum = 1;
	
	//this->bounds.left = 0;
	//this->bounds.right = width;
	//this->bounds.top = 0;
	//this->bounds.bottom = height;
}

// --------------------------------------------------------------------------------
DxCamera::~DxCamera()
{
	Close();
}

// --------------------------------------------------------------------------------
void DxCamera::Process( )
{
	SpendTime();
}

// --------------------------------------------------------------------------------
JSBool DxCamera::GetProp( DxPropID propId, DxPropType propType, void **outResult )
{
	return JS_FALSE;
}

// --------------------------------------------------------------------------------
JSBool DxCamera::SetProp( DxPropID propId, DxPropType propType, void *outResult )
{
	return JS_FALSE;
}

// ---------------------------------------------------------------------------------
static pascal Boolean
SeqGrabberModalFilterProc (DialogPtr theDialog, const EventRecord *theEvent,
	short *itemHit, long refCon)
{
	// Ordinarily, if we had multiple windows we cared about, we'd handle
	// updating them in here, but since we don't, we'll just clear out
	// any update events meant for us
	
	Boolean	handled = false;
	
	theDialog;
	itemHit;
	
	if ((theEvent->what == updateEvt) && 
		((WindowPtr) theEvent->message == (WindowPtr) refCon))
	{
		WindowPtr	wPtr = (WindowPtr) refCon;
		BeginUpdate (wPtr);
		EndUpdate (wPtr);
		handled = true;
	}
	
	return (handled);
}

// ---------------------------------------------------------------------------------
static void
VideoSettings( 	SeqGrabComponent SeqGrabber, SGChannel VideoChannel, WindowPtr winPtr)
{
	SGModalFilterUPP	seqGragModalFilterUPP;
	ComponentResult	err;
	
	err = SGPause (SeqGrabber, true);
	
	// Do the dialog thang
	//seqGragModalFilterUPP = (SGModalFilterUPP)NewSGModalFilterProc( &SeqGrabberModalFilterProc);
	// !!@ 2003.03.08 jht: No longer in Carbon. 
	seqGragModalFilterUPP = 0;
	
	err = SGSettingsDialog(SeqGrabber, VideoChannel, 0, 
		nil, 0L, seqGragModalFilterUPP, (long)winPtr);
		
	DisposeSGModalFilterUPP(seqGragModalFilterUPP);

	err = SGPause (SeqGrabber, false);
}

// ---------------------------------------------------------------------------------
void DxCamera::VideoSettings()
{
	
}

// ---------------------------------------------------------------------------------
void DxCamera::SetGWorldPtr( GWorldPtr gworldPtr)
{
	// gworldPtr may be 0 to turn off outputing.
	Rect bounds;
	
	this->gworldPtr = gworldPtr;
		
	if (! gworldPtr)
	{
		Close();
		return;
	}

	PixMapHandle		pixMapHd;

	pixMapHd = GetGWorldPixMap( this->gworldPtr );
	if (! pixMapHd)
	{
		// !!@ internal error
		return;
	}
	
	bounds = (**pixMapHd).bounds;
	
	if (this->seqGrabber)
	{
		//ComponentResult			result = noErr;
		
		//result = SGSetGWorld (this->seqGrabber, (CGrafPtr) this->gworldPtr, nil);
		// switching gworlds is not that simple, so rebuilt the seq grab comp
		Close();
		CreateSeqGrab( this->inputDeviceNum, bounds, this->gworldPtr );
	}
	else
	{
		CreateSeqGrab( this->inputDeviceNum, bounds, this->gworldPtr );
	}
}

// ---------------------------------------------------------------------------------
void DxCamera::CreateSeqGrab(int devNum, Rect bounds, GWorldPtr monitor)
{
	ComponentDescription	theDesc;
	ComponentResult			result = noErr;
	GrafPtr					savedPort;
	Component				sgCompID;
	
	gworldPtr = monitor;
	
	grabStarted = 0;
	if (this->seqGrabber != 0L)
	{
		result = CloseComponent (this->seqGrabber);
		this->seqGrabber = 0L;
	}
	this->videoChannel = 0;
	//SoundChannel=0;
	//ActiveVideoRect;
	//RecordVideo = 1;
	//RecordSound = 1;
	//SplitTracks = 0;

	//SoundChannel = 0L;
	
	// Find and open a sequence grabber
	theDesc.componentType = SeqGrabComponentType;
	theDesc.componentSubType = 0L;
	theDesc.componentManufacturer = 'appl';
	theDesc.componentFlags = 0L;
	theDesc.componentFlagsMask = 0L;
	sgCompID = FindNextComponent (nil, &theDesc);

	if (sgCompID != 0L)
		this->seqGrabber = OpenComponent (sgCompID);
	
	// If we got a sequence grabber, set it up
	if (this->seqGrabber == 0L)
		return;

	// Get the monitor
	// Monitor = GetMacPort();
	if (monitor == nil)
		return;
		
	// Display the monitor window
	GetPort (&savedPort);
	
	MacSetPort (monitor);
	
	// Initialize the sequence grabber
	result = SGInitialize (this->seqGrabber);
	if (result == noErr)
	{
		#if 1
		result = SGSetGWorld (this->seqGrabber, (CGrafPtr) monitor, nil);
		#endif
		
		// Get a video channel
		result = SGNewChannel (this->seqGrabber, VideoMediaType, &this->videoChannel);
		if ((this->videoChannel != nil) && (result == noErr))
		{
			#if 0
			// ActiveVideoRect not used, caller drive bounds */
			short	width;
			short	height;
			
			result = SGGetSrcVideoBounds (this->videoChannel, &ActiveVideoRect);
			width = (ActiveVideoRect.right - ActiveVideoRect.left) ;
			height = (ActiveVideoRect.bottom - ActiveVideoRect.top)  ;
			#endif
			SeqGrabDataOutputEnum	opt;
			
			opt = seqGrabPreview /* | seqGrabRecord | seqGrabPlayDuringRecord */ ;
			result = SGSetChannelUsage (this->videoChannel, opt );

			result = SGSetChannelBounds (this->videoChannel, &bounds);

			SetInputDeviceNumOnly( devNum );
		
			#if 1
			/* 2001.06.01 jht; Enable DV high quality (slow) preview */
			result = SGSetChannelPlayFlags(this->videoChannel, channelPlayHighQuality);
			#endif
		}
		else
		{
			// No video, kill seqGrabber
			result = CloseComponent (this->seqGrabber);
			this->seqGrabber = 0L;
		}
		
		#if 0
		// Get a sound channel
		result = SGNewChannel (this->seqGrabber, SoundMediaType, &SoundChannel);

		if ((SoundChannel != nil) && (result == noErr))
		{
			if (SoundChannel != nil)
			{
				result = SGSetChannelUsage (SoundChannel, seqGrabPreview | seqGrabRecord);
				
				// Set the volume low to prevent feedback when we start the preview,
				// in case the mic is anywhere near the speaker.
				result = SGSetChannelVolume (SoundChannel, 0x0010);
			}
		}
		#endif

		// Get the alignment proc (for use when dragging the monitor)
		//result = SGGetAlignmentProc (this->seqGrabber, &SeqGrabberAlignProc);
	}
	
	// Go!
	#if 0
	// 2001.05.24 jht: No don't go!
	// for some odd reason starting a preview here will wack out the ports to 
	// that alerts get draw to the off screen world.
	// defer start preview until later
	if (result == noErr)
		result = SGStartPreview (this->seqGrabber);
	#endif
		
	MacSetPort (savedPort);
}

// ---------------------------------------------------------------------------
// Select an input device for video channel,
// 1 selects the first available, 2 the second etc.
void DxCamera::SetInputDeviceNumOnly( int aDevNum)
{
	int 			devNum = aDevNum;
	SGDeviceList	devList = 0;
	Str63			devName;
	ComponentResult			result = noErr;
	
	if (! this->videoChannel)
		return;
		
	result = SGGetChannelDeviceList( this->videoChannel, 0, &devList );

	if (! devList)
		goto done;

	int		index;
	
	for (index = 0; index < (**devList).count; index++)
	{
		// Only count available devices
		if (  ((**devList).entry[index].flags & sgDeviceNameFlagDeviceUnavailable) == 0 )
		{
			devNum--;
			if (devNum <= 0)
			{
				BlockMove( &(**devList).entry[index].name, &devName, sizeof(devName));
				
				result = SGSetChannelDevice( this->videoChannel, devName );
				
				inputDeviceNum = aDevNum;
				goto done;
			}
		}
	}
	
done:;
	if (devList)
	{
		result = SGDisposeDeviceList( this->seqGrabber, devList );
	}
}

// ---------------------------------------------------------------------------
// Select an input device for video channel,
// 1 selects the first available, 2 the second etc.
// Recreates lower level QT data structures.
//
void DxCamera::SetInputDeviceNum( int aDevNum)
{
	if (this->inputDeviceNum != aDevNum)
	{
		this->inputDeviceNum = aDevNum;
		
		SetInputDeviceNumOnly( aDevNum );
	}
}

// ---------------------------------------------------------------------------------
int DxCamera::GetInputDeviceNum()
{
	if (! this->videoChannel)
		return 0;
	return inputDeviceNum;
}

// ---------------------------------------------------------------------------------
void DxCamera::SpendTime()
{
	ComponentResult	result = noErr;
	CGrafPtr	savePort;
	
	GetPort( &savePort );
	
	SetPort( gworldPtr );
	
	// 2001.05.24 jht: Do start here to prevent wacked out ports.
	if (! grabStarted)
	{
		grabStarted = 1;
		result = SGStartPreview (this->seqGrabber);
	}

	if (this->seqGrabber)
	{
		//FocusDraw();
		
		result = SGIdle (this->seqGrabber);
		
		//UpdateFromGWorld();
		
	}
	
	SetPort( savePort);
	//UpdateStatus();
}

// ---------------------------------------------------------------------------------
void DxCamera::AddRef()
{
	this->refCount++;
}

// ---------------------------------------------------------------------------------
void DxCamera::ReleaseRef()
{
	if ( --this->refCount <= 0)
	{
		delete this;
	}
}

// ---------------------------------------------------------------------------------
void DxCamera::Close()
{
	ComponentResult	result = noErr;
	
	// Clean up
	if (this->seqGrabber != 0L)
	{
		result = CloseComponent (this->seqGrabber);
		this->seqGrabber = 0L;
	}
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------