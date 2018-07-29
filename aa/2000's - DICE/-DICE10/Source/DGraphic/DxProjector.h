// --------------------------------------------------------------------------------//// Projector, copy input to output//// 2005.02.21 	jht		Created.//// Copyright (c) 2005 John Henry Thompson. All rights reserved.// --------------------------------------------------------------------------------#pragma once#include "DxSurface.h"// --------------------------------------------------------------------------------class DxProjector : public DxGeneric{public:	DxProjector(void *wrapperObj);		~DxProjector();				// --------------------------------------------------------------------------------				virtual void Process();		virtual Boolean GetProp( DxPropID propId, DxPropType propType, void **outResult );		virtual Boolean SetProp( DxPropID propId, DxPropType propType, void *outResult ) ;			// --------------------------------------------------------------------------------			void 	Close();	// --------------------------------------------------------------------------------	DxSurface	*input;	DxSurface	*output;	int			inkMode;	};// --------------------------------------------------------------------------------// --------------------------------------------------------------------------------