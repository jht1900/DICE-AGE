// --------------------------------------------------------------------------------
//
// DiffTrailEffect -- The image difference is left as a trail
//
// 2005.02.27 	jht		Created.
//
// Copyright (c) 2005 John Henry Thompson. All rights reserved.
// --------------------------------------------------------------------------------


#include "DxDiffEffect_js.h"
#include "DxDiffTrailEffect.h"

#include "jscntxt.h"
#include "jslock.h"
#include "dicejs.h"

#include "DxGeneric_js.h"

// --------------------------------------------------------------------------------

#define DS_TYPE		DxDiffTrailEffect
#define DS_CLASS_NAME "DiffTrailEffect"

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
static JSBool ds_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSBool ds_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSBool ds_accessProperty(JSContext *cx, JSBool doSet, JSObject *obj, jsval id, jsval *vp);

// --------------------------------------------------------------------------------
static JSFunctionSpec ds_methods[] = {
    {"process",            ds_Generic_process,            0,0,0},
    {0,0,0,0,0}
};

enum ds_tinyid {
    DS_PROP_input,
    DS_PROP_ouptput,
    DS_PROP_diffTolerance,
    DS_XXX_PROP
};

// --------------------------------------------------------------------------------
static JSPropertySpec ds_props[] = {
    {"input",			DS_PROP_input,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"output",			DS_PROP_ouptput,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
    {"diffTolerance",			DS_PROP_diffTolerance,		JSPROP_ENUMERATE|JSPROP_PERMANENT,0,0},
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
JSBool ds_accessProperty(JSContext *cx, JSBool doSet, JSObject *obj, jsval id, jsval *vp)
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
    	case DS_PROP_input:
    	{
    		if (doSet)
    		{
    			DxImage	*newImage;
    			
    			newImage = (DxImage *)ds_GenericGetProp( cx, *vp,
    												kDxProp_value, kDxType_DxImage);
    			ucontrol->SetInput( newImage );
    		}
    		else
    		{
    		}
    		break;
    	}
    	case DS_PROP_ouptput:
    	{
    		if (doSet)
    		{
    			DxImage	*newImage;
    			
    			newImage = (DxImage *)ds_GenericGetProp( cx, *vp,
    												kDxProp_value, kDxType_DxImage);
    			ucontrol->SetOutput( newImage );
    		}
    		else
    		{
    		}
    		break;
    	}
    	case DS_PROP_diffTolerance:
    	{
			jsdouble			num;
			
    		if (doSet)
    		{
	    		if (!JS_ValueToNumber(cx, *vp, &num))
		    		goto exit;
	    		ucontrol->diffTolerance = (int)( num );
    		}
    		else
    		{
    			ok = JS_NewNumberValue(cx, ucontrol->diffTolerance, vp);
    		}
    		break;
    	}
    }
    ok = JS_TRUE;

exit:;
    JS_UNLOCK_OBJ(cx, obj);
exit2:;
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
JSObject *ds_DiffTrailEffect_InitClass(JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    proto = JS_InitClass(cx, obj, NULL, &ds_Class, ds_Constructor, 0,
			 ds_props, ds_methods,
			 NULL /* static_props*/, NULL /* static_method*/);

    return proto;

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
