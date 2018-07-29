// --------------------------------------------------------------------------------
//
// QuickTime Movie player
//
// 2005.03.01 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#include "DxMovieMaker_js.h"
#include "DxMovieMaker.h"

#include "DxImage_js.h"
#include "DxImage.h"

#include "dicejs.h"

//#include <Navigation.h>
//#include <ImageCompression.h>
//#include <MacTypes.h>
//#include <Carbon/Carbon.h>
#include <stdlib.h>

//#include <stdio.h>
#include <fcntl.h>
//#include <unix.h>

// --------------------------------------------------------------------------------
static void CheckError(OSErr err, char* msg) ;

// --------------------------------------------------------------------------------
DxMovieMaker::DxMovieMaker(void *wrapperObj )
	: DxGeneric(wrapperObj )
{
	this->timeScale = 1200;
	this->frameDuration = 100;
	this->movie = 0;
	// Take bounds from first image added
	this->theTrack = 0;
	this->theMedia = 0;
	this->compressedData = 0;
	this->compressedDataPtr = 0;
	this->imageDesc	= 0;
	this->quality = codecNormalQuality;
	this->codec = kJPEGCodecType;
	
	this->refCount = 0;
}

// --------------------------------------------------------------------------------
DxMovieMaker::~DxMovieMaker()
{
	this->Finish();
}

// --------------------------------------------------------------------------------
void DxMovieMaker::Process( )
{	
	if (this->movie)
	{
		SpendTime();
	}
}

// --------------------------------------------------------------------------------
JSBool DxMovieMaker::GetProp( DxPropID propId, DxPropType propType, void **outResult )
{
	return JS_FALSE;
}

// --------------------------------------------------------------------------------
JSBool DxMovieMaker::SetProp( DxPropID propId, DxPropType propType, void *outResult )
{
	return JS_FALSE;
}

// ---------------------------------------------------------------------------------
void DxMovieMaker::SpendTime()
{	
	
exit:;

}

// ---------------------------------------------------------------------------------
void DxMovieMaker::AddRef()
{
	this->refCount++;
}

// ---------------------------------------------------------------------------------
void DxMovieMaker::ReleaseRef()
{
	if ( --this->refCount <= 0)
	{
		delete this;
	}
}

// ---------------------------------------------------------------------------------
void DxMovieMaker::SetTimeScale(long ts)
{
	this->timeScale = ts;
}

// ---------------------------------------------------------------------------------
int32 DxMovieMaker::GetTimeScale()
{
	return this->timeScale;
}

// ---------------------------------------------------------------------------------
void DxMovieMaker::SetFrameDuration(long dur)
{
	this->frameDuration = dur;
}

// ---------------------------------------------------------------------------------
int32 DxMovieMaker::GetFrameDuration()
{
	return this->frameDuration;
}

// ---------------------------------------------------------------------------------
//static void CreateMyCoolMovie (void);
static void PickFileSingle(void);

// ---------------------------------------------------------------------------------
void DxMovieMaker::PickFile()
{
	OSErr		err;
	
	this->Finish();
	
	//err = QTUSimpleGetMovie(&this->movie);
	//CreateMyCoolMovie();
	this->PickFileSingle();
}

// ---------------------------------------------------------------------------------
void DxMovieMaker::AddFrame(DxImage *image, long dur)
{
	if (! this->movie) 
	{
		this->InitForImage(image);
	}
	if (! this->movie)
	{
		// !!@ movie creation failed.
		return;
	}
	this->AddFrameSingle(image, dur);
}

// ---------------------------------------------------------------------------------
static OSErr XSFPutFile( FSSpecPtr theFSSpecPtr);

// ---------------------------------------------------------------------------------
void DxMovieMaker::PickFileSingle(void)
{
	OSErr       err = noErr;
	err = XSFPutFile ( &this->mySpec);
	// !!@ check for file pick error 
}

// ---------------------------------------------------------------------------------

void DxMovieMaker::CreateFile(JSString *path, JSString *fileName)
{
	OSErr		err;
	//FSRef		fref;
	
	// Need 1-byte path 
	char	*path1 = JS_GetStringBytes(path);
	FSRef	parentRef;
	
	err = FSPathMakeRef( (UInt8*)path1, &parentRef, NULL);
	if (err)
	{
		return;
	}
	
	UniCharCount nameLength = JS_GetStringLength(fileName);
	const UniChar * name = JS_GetStringChars(fileName);
	FSCatalogInfoBitmap whichInfo = kFSCatInfoNone;
	const FSCatalogInfo * catalogInfo = NULL;
	FSRef * newRef = NULL;
	FSSpec * newSpec = &this->mySpec;

	err = FSCreateFileUnicode(&parentRef, nameLength, name, whichInfo, catalogInfo, newRef, newSpec);
	if (err == dupFNErr) 
	{
   		TextEncoding textEncodingHint = kTextEncodingDefaultFormat;
		FSRef	newRef2;
		
		err = FSMakeFSRefUnicode(&parentRef, nameLength, name, textEncodingHint, &newRef2);
		CheckError( err, "FSMakeFSRefUnicode" ); 
		if (! err)
		{
			err = FSGetCatalogInfo(&newRef2, kFSCatInfoNone, NULL, NULL, newSpec, NULL);
			CheckError( err, "FSGetCatalogInfo" ); 
		}
	}
	else
	{
		CheckError( err, "FSCreateFileUnicode" ); 
	}
}

// ---------------------------------------------------------------------------------
#define kMovieFileAppSig 			'????'
//#define kCodecForMovie				kJPEGCodecType

//#define kMovieVideoTimeScale 		240
//#define kMovieOneFrameDuration 		10

#define     kFix1                   0x00010000 

// ---------------------------------------------------------------------------------
void DxMovieMaker::InitForImage(DxImage *image)
{ 
	//short       resRefNum = 0; 
	//short       resId = movieInDataForkResID; 
	OSErr       err = noErr;
	long        maxCompressedSize; 

	err = CreateMovieFile (&this->mySpec, 
							kMovieFileAppSig, 
							smCurrentScript, 
							createMovieFileDeleteCurFile, 
							&this->resRefNum, 
							&this->movie ); 
	CheckError(err, "CreateMovieFile");  
	
	image->GetBounds( &this->bounds );

	PixMapHandle		pixMapHd;
	pixMapHd = GetGWorldPixMap( image->gworldPtr );
	
	LockPixels (pixMapHd); 

	err = GetMaxCompressionSize (pixMapHd, 
									&this->bounds, 
									0, /* let ICM choose depth */ 
									this->quality, 
									this->codec, 
									(CompressorComponent) anyCodec, 
									&maxCompressedSize); 
	CheckError (err, "GetMaxCompressionSize" );  
	this->compressedData = NewHandle(maxCompressedSize); 
	CheckError( MemError(), "NewHandle" ); 
	
	MoveHHi( this->compressedData ); 
	HLock( this->compressedData ); 
	this->compressedDataPtr = *compressedData ;  
	
	this->imageDesc = (ImageDescriptionHandle)NewHandle(4); 
	CheckError( MemError(), "NewHandle" ); 

	this->theTrack = NewMovieTrack( this->movie, 
									FixRatio(this->bounds.right,1), 
									FixRatio(this->bounds.bottom,1), 
									kNoVolume); 
	CheckError( GetMoviesError(), "NewMovieTrack" ); 
	
	this->theMedia = NewTrackMedia( theTrack, VideoMediaType, 
									this->timeScale , // Video Time Scale 
									NULL, 0); 
	CheckError( GetMoviesError(), "NewTrackMedia" ); 
	
	err = BeginMediaEdits( this->theMedia); 
	CheckError( err, "BeginMediaEdits" );  
}

// ---------------------------------------------------------------------------------
void DxMovieMaker::Finish()
{
	OSErr       err = noErr;
	short       resId = movieInDataForkResID; 

	if (this->theMedia) 
	{
		
		err = EndMediaEdits( this->theMedia); 
		CheckError( err, "EndMediaEdits" ); 
		
		err = InsertMediaIntoTrack( this->theTrack, 
										0,  /* track start time */ 
										0,  /* media start time */ 
										GetMediaDuration( this->theMedia), 
										kFix1); 
		CheckError( err, "InsertMediaIntoTrack" ); 
	}
	
	if (this->movie)
	{
		err = AddMovieResource( this->movie, this->resRefNum, &resId, NULL); 
		CheckError(err, "AddMovieResource"); 
		
		if (this->resRefNum) 
		{
			CloseMovieFile( this->resRefNum); 
			this->resRefNum = 0;
		}
		
		DisposeMovie( this->movie );
		this->movie = 0;
		this->theMedia = 0;
		this->theTrack = 0;
	}
	
	if (this->imageDesc) 
	{
		DisposeHandle( (Handle)this->imageDesc); 
		this->imageDesc = 0;
	}
	
	if (this->compressedData) 
	{
		DisposeHandle( compressedData); 
		this->compressedData = 0;
	}
}

// ---------------------------------------------------------------------------------
void DxMovieMaker::AddFrameSingle(DxImage *image, long frameDur)
{
	OSErr		err;
	
	if (frameDur < 0)
		frameDur =  this->frameDuration;
	if (! this->movie || ! this->theMedia ) 
	{
		// !!@ init failed.
		return;
	}

	PixMapHandle		pixMapHd;
	pixMapHd = GetGWorldPixMap( image->gworldPtr );
	LockPixels (pixMapHd); 

	err = CompressImage( pixMapHd, 
								&this->bounds, 
								this->quality, 
								this->codec, 
								this->imageDesc, 
								this->compressedDataPtr ); 
	CheckError( err, "CompressImage" ); 
	
	err = AddMediaSample( this->theMedia, 
							this->compressedData, 
							0,      /* no offset in data */ 
							(**this->imageDesc).dataSize,  
							frameDur, 
							(SampleDescriptionHandle)this->imageDesc, 
							1,      /* one sample */ 
							0,      /* self-contained samples */ 
							NULL); 
	CheckError( err, "AddMediaSample" ); 
	
}

// ---------------------------------------------------------------------------------
static void CheckError(OSErr err, char* msg) 
{
	if (err) 
	{
		// !!@ Error reporting needed here
		char* msg2 = msg;
	}
}

// ---------------------------------------------------------------------------------
static OSErr XSFPutFile( FSSpecPtr theFSSpecPtr)
{
	NavReplyRecord		myReply;
	NavDialogOptions	myDialogOptions;
	NavTypeListHandle	myOpenList = NULL;
	// !!@ NavEventUPP			myEventUPP = NewNavEventUPP(QTFrame_HandleNavEvent);
	NavEventUPP			myEventUPP = 0;
	OSErr				myErr = noErr;
	
	//void *theFilterProc = NULL;
	OSType fileType = '????';
	OSType fileCreator = '????';
	
	if (theFSSpecPtr == NULL)
		return(paramErr);
	
	// deactivate any frontmost movie window
	// !!@ QTFrame_ActivateController(QTFrame_GetFrontMovieWindow(), false);

	// specify the options for the dialog box
	NavGetDefaultDialogOptions(&myDialogOptions);
	myDialogOptions.dialogOptionFlags -= kNavNoTypePopup;
	myDialogOptions.dialogOptionFlags -= kNavAllowMultipleFiles;
	// !!@ BlockMoveData(gAppName, myDialogOptions.clientName, gAppName[0] + 1);
	
	
	// prompt the user for a file
	myErr = NavPutFile(NULL, &myReply, &myDialogOptions, myEventUPP, fileType, fileCreator, NULL);

	if ((myErr == noErr) && myReply.validRecord) {
		AEKeyword		myKeyword;
		DescType		myActualType;
		Size			myActualSize = 0;
		
		// get the FSSpec for the selected file
		if (theFSSpecPtr != NULL)
			myErr = AEGetNthPtr(&(myReply.selection), 1, typeFSS, &myKeyword, &myActualType, theFSSpecPtr, sizeof(FSSpec), &myActualSize);

		NavDisposeReply(&myReply);
	}
	
	if (myOpenList != NULL) {
		HUnlock((Handle)myOpenList);
		DisposeHandle((Handle)myOpenList);
	}
	
	DisposeNavEventUPP(myEventUPP);
 
	return(myErr);
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------