/* History:� 2001 John Henry Thompson. All rights reserved2001.05.22	jht	Split off.*/#include "CCapturePane.h"#define CODE_INNER_LOOP \	/* Show line streak on change */ \	{ \		capPix = sourceLinePtr[0] + sourceLinePtr[1] + sourceLinePtr[2];   \		diffwPix = (previousLinePtr[0] + previousLinePtr[1] + previousLinePtr[2]) - capPix;  \		if (diffwPix < -diffTolerance_x3 || diffwPix > diffTolerance_x3) \		{ \			/* There is  difference */ \			inside = ! inside; \		} \		if (inside) \		{ \			/* Show image color */ \			monitorLinePtr[0] = color[0]; \			monitorLinePtr[1] = color[1]; \			monitorLinePtr[2] = color[2]; \		} \	} \// ---------------------------------------------------------------------------------void	CCapturePane::ComputeMotionStreakColorTrail(){	int	capPix;	int diffwPix;#include "EffectsLoop1.inc"}