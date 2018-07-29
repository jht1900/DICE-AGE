//////////
//
//	File:		QTFrameStepper.h
//
//	Contains:	Functions to step frame-by-frame through a QuickTime movie.
//
//	Written by:	Tim Monroe
//
//	Copyright:	� 1997 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	   <1>	 	12/22/97	rtm		first file
//	   
//////////

//////////
//
// header files
//
//////////

//#ifndef __MOVIES__
//#include <Movies.h>
//#endif

//#ifndef __FIXMATH__
//#include <FixMath.h>
//#endif

#include <QuickTime/QuickTime.h>

//////////
//
// constants
//
//////////

#define kBogusStartingTime	-1			// an invalid starting time


//////////
//
// function prototypes
//
//////////

static OSErr				QTStep_GetStartTimeOfFirstVideoSample (Movie theMovie, TimeValue *theTime);
static OSErr				QTStep_DrawVideoSampleAtTime (Movie theMovie, TimeValue theTime);
static OSErr				QTStep_DrawVideoSampleNextOrPrev (Movie theMovie, Fixed theRate);

OSErr						QTStep_GoToFirstVideoSample (Movie theMovie);
OSErr						QTStep_GoToNextVideoSample (Movie theMovie);
OSErr						QTStep_GoToPrevVideoSample (Movie theMovie);

OSErr						QTStep_MCGoToFirstVideoSample (MovieController theMC);
OSErr						QTStep_MCGoToNextVideoSample (MovieController theMC);
OSErr						QTStep_MCGoToPrevVideoSample (MovieController theMC);

long						QTStep_GetFrameCount (Track theTrack);
