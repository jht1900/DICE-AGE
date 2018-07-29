// --------------------------------------------------------------------------------
//
// QuickTime Movie player
//
// 2006.08.09 	jht		Created.
//
// Copyright (c) 2006 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------


#include "DxMovieMaker_js.h"
#include "DxMovieMaker.h"
#include "DxGeneric_js.h"
#include "DxImage_js.h"

#include "jscntxt.h"
#include "jslock.h"
#include "dicejs.h"

// --------------------------------------------------------------------------------

#define DS_CLASS_NAME "MovieMaker"
#define DS_TYPE DxMovieMaker
#define DS_GET_PRIVATE ds_##DS_TYPE##_GetPrivate

// --------------------------------------------------------------------------------
static JSBool
ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSBool
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

// --------------------------------------------------------------------------------

static JSBool
ds_PickFile(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSBool
ds_CreateFile(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSBool
ds_AddFrame(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSBool
ds_Finish(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

// --------------------------------------------------------------------------------
static JSFunctionSpec ds_methods[] = {
    {"process",				ds_Generic_process,     0,0,0},
    {"pickFile",			ds_PickFile,            0,0,0},
    {"createFile",			ds_CreateFile,            0,0,0},
    {"addFrame",			ds_AddFrame,            1,0,0},
    {"finish",				ds_Finish,            	0,0,0},
    {0,0,0,0,0}
};

enum ds_tinyid {
    DS_PROP_output,
    DS_PROP_timeScale,
    DS_PROP_frameDuration,
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"output",				DS_PROP_output,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"timeScale",			DS_PROP_timeScale,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"frameDuration",		DS_PROP_frameDuration,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {0,0,0,0,0}
};

// --------------------------------------------------------------------------------
static JSClass ds_Class = {
    DS_CLASS_NAME,
    JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,  JS_PropertyStub,  ds_getProperty, 	ds_setProperty,
    JS_EnumerateStub, JS_ResolveStub,   JS_ConvertStub,     ds_GenericFinalize,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

// --------------------------------------------------------------------------------
static DS_TYPE *
DS_GET_PRIVATE(JSContext *cx, JSObject *obj, jsval *argv)
{
	return (DS_TYPE*) JS_GetInstancePrivate(cx, obj, &ds_Class, argv );
}

// --------------------------------------------------------------------------------
static JSBool
ds_CreateFile(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DS_TYPE 	*ucontrol; 
    JSString		*path;
    JSString		*fileName;
    JSBool 		ok;
    ok = JS_ConvertArguments(cx, argc, argv, "SS", &path, &fileName);
    if (! ok)
    	goto exit;
	ucontrol =  DS_GET_PRIVATE(cx, obj, argv); 
	if (! ucontrol) 
		return JS_FALSE;  
	ucontrol->CreateFile(path, fileName); 
exit:;
	return ok; 
}

// --------------------------------------------------------------------------------
static JSBool
ds_PickFile(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DS_TYPE 	*ucontrol;

	ucontrol =  DS_GET_PRIVATE(cx, obj, argv);
	if (! ucontrol)
		return JS_FALSE;
		
	ucontrol->PickFile();
	
	return JS_TRUE;
}

// --------------------------------------------------------------------------------
static JSBool
ds_AddFrame(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DS_TYPE 	*ucontrol;
	//GWorldPtr 	gworldPtr;
	JSBool		ok = JS_FALSE;
	JSObject	*imageObj;
	DxImage		*image;
	int32		dur = -1;
		
	ucontrol =  DS_GET_PRIVATE(cx, obj, argv);
	if (! ucontrol)
		return JS_FALSE;
		
    ok = JS_ConvertArguments(cx, argc, argv, "o/i", &imageObj, &dur);
    if (! ok)
    	goto exit;
    
    image = (DxImage*) ds_Image_GetPrivate(cx, imageObj, 0 );
    if (! image)
    	goto exit;
    
	ucontrol->AddFrame(image, dur);

exit:;
	return ok;
}

// --------------------------------------------------------------------------------
static JSBool
ds_Finish(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    DS_TYPE 	*ucontrol;

	ucontrol =  DS_GET_PRIVATE(cx, obj, argv);
	if (! ucontrol)
		return JS_FALSE;
		
	ucontrol->Finish();
	
	return JS_TRUE;
}


// --------------------------------------------------------------------------------
//
static JSBool
ds_Constructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSBool 				ok;
	//int32				width = 640, height = 480;
	//int32				inputNum = 1;
	DS_TYPE			*ucontrol;	
	
    if (!(cx->fp->flags & JSFRAME_CONSTRUCTING)) 
    {
		obj = js_NewObject(cx, &ds_Class, NULL, NULL);
		if (!obj)
		    return JS_FALSE;
		*rval = OBJECT_TO_JSVAL(obj);
    }
    ucontrol = new DxMovieMaker( obj );
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
static JSBool
ds_accessProperty(JSContext *cx, JSBool doSet, JSObject *obj, jsval id, jsval *vp)
{
    JSBool 			ok = JS_FALSE;
    jsint 			slot;
    DS_TYPE 	*ucontrol;

	// !!@ why return ok here??
    if (!JSVAL_IS_INT(id))
		return JS_TRUE;
		
    slot = JSVAL_TO_INT(id);
		
    JS_LOCK_OBJ(cx, obj);
    
	ucontrol = (DS_TYPE*) JS_GetPrivate(cx, obj);
	if (! ucontrol)
		goto exit;
		
    switch (slot)
    {
    #if 0
   	case DS_PROP_output:
    	{
    		if (doSet)
    		{
    			GWorldPtr	newGWorldPtr;
    			
    			newGWorldPtr = (GWorldPtr)ds_GenericGetProp( cx, *vp,
    												kDxProp_value, kDxType_CGrafPortPtr);
    		
				ucontrol->SetGWorldPtr(newGWorldPtr );
    		}
    		else
    		{
    		}
    		break;
    	}
    #endif
		case DS_PROP_timeScale:
		{
			jsdouble			num;
    		if (doSet)
    		{
	    		if (!JS_ValueToNumber(cx, *vp, &num))
		    		goto exit;
	    		ucontrol->SetTimeScale( num );
    		}
    		else
    		{
    			ok = JS_NewNumberValue(cx, ucontrol->GetTimeScale(), vp);
    		}
    		break;
		}
		case DS_PROP_frameDuration:
		{
			jsdouble			num;
    		if (doSet)
    		{
	    		if (!JS_ValueToNumber(cx, *vp, &num))
		    		goto exit;
	    		ucontrol->SetFrameDuration( num );
    		}
    		else
    		{
    			ok = JS_NewNumberValue(cx, ucontrol->GetFrameDuration(), vp);
    		}
    		break;
		}
    }
    ok = JS_TRUE;

exit:;
    JS_UNLOCK_OBJ(cx, obj);
    return ok;
}

// --------------------------------------------------------------------------------
static JSBool
ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	return ds_accessProperty( cx, FALSE/*doSet*/, obj, id, vp );
}

// --------------------------------------------------------------------------------
static JSBool
ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	return ds_accessProperty( cx, TRUE/*doSet*/, obj, id, vp );
}

// --------------------------------------------------------------------------------
JSObject *ds_MovieMaker_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, ds_Constructor, 0,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------