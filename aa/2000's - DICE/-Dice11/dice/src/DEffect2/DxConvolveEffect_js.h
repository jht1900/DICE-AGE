// --------------------------------------------------------------------------------
//
// ConvolveEffect - average neighbors of a pixel
//
// 2006.08.06	jht		Mod for convolve. The beat goes on.
// 2005.02.26 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------

#pragma once

#include "jsapi.h"

JS_BEGIN_EXTERN_C

// --------------------------------------------------------------------------------
JSObject *
ds_ConvolveEffect_InitClass(JSContext *cx, JSObject *obj);

JS_END_EXTERN_C

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

