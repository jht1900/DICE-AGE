// --------------------------------------------------------------------------------
//
// DiffNoise -- difference the images and replace negative with source
//
// 2005.02.27 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------


#include "DxDiffEffect_js.h"
#include "DxDiffNoiseEffect.h"

#include "jscntxt.h"
#include "jslock.h"
#include "dicejs.h"

#include "DxGeneric_js.h"

// --------------------------------------------------------------------------------

#define DS_TYPE		DxDiffNoiseEffect
#define DS_CLASS_NAME "DiffNoiseEffect"

#if 0
// --------------------------------------------------------------------------------
#define DS_GET_PRIVATE(cx, obj, argv) \
	(DS_TYPE*) JS_GetInstancePrivate(cx, obj, &ds_Class, argv )

static DxDiffEffect *
ds_DxDiffEffect_GetPrivate(JSContext *cx, JSObject *obj, jsval *argv)
{
	return (DxDiffEffect*) JS_GetInstancePrivate(cx, obj, &ds_Class, argv );
}
#endif


// --------------------------------------------------------------------------------
static JSFunctionSpec ds_methods[] = {
    {"process",            ds_Generic_process,            0,0,0},
    {0,0,0,0,0}
};

enum ds_tinyid {
    DS_PROP_input,
    DS_PROP_ouptput,
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"input",			DS_PROP_input,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"output",			DS_PROP_ouptput,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {0,0,0,0,0}
};

// --------------------------------------------------------------------------------
static JSClass ds_Class = {
    DS_CLASS_NAME,
    JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,  JS_PropertyStub,  ds_DiffEffect_getProperty, 	ds_DiffEffect_setProperty,
    JS_EnumerateStub, JS_ResolveStub,   JS_ConvertStub,     ds_GenericFinalize,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

// --------------------------------------------------------------------------------
// DiffEffect( )
//
static JSBool
ds_Constructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool 			ok;
	DS_TYPE			*ucontrol;
	
	
	/* PushButton called as function 
	If called with new, replace with new PushButton object
	*/
    if (!(cx->fp->flags & JSFRAME_CONSTRUCTING)) 
    {
		obj = js_NewObject(cx, &ds_Class, NULL, NULL);
		if (!obj)
		    return JS_FALSE;
		*rval = OBJECT_TO_JSVAL(obj);
    }
    
    ucontrol = new DS_TYPE( obj );
    if (! ucontrol)
    {
    	ok = JS_FALSE;
    	goto exit;
    }
    
    ok = JS_SetPrivate(cx, obj, ucontrol);
 
 exit:;
    return ok;
}


// --------------------------------------------------------------------------------
JSObject * ds_DiffNoiseEffect_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, ds_Constructor, 0,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
