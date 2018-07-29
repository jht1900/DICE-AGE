/* --------------------------------------------------------------------------------Dice General DOM functions.Keeping it simply and mapping directly to Macintosh/QuickTime APICopyright (c) 2005 John Henry Thompson. All rights reserved.History:2005.02.14	jht	Created.-------------------------------------------------------------------------------- */#include "dsDOM.h"#include "jscntxt.h"#include "jslock.h"#include "timer.h"//#include "CarbonEventsCore.h"#include "ds_NotePlayer.h"#include "dsWindow.h"#include "dsPushButton.h"#include "dsCaption.h"#include "dsTextField.h"#include "dsCamera.h"#include "dsImage.h"#include "dsProjector.h"// --------------------------------------------------------------------------------static JSBooldsGetCurrentEventTime(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval){	return JS_NewNumberValue(cx, GetCurrentEventTime(), rval);}// --------------------------------------------------------------------------------static JSBooldsMicroseconds(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval){	UnsignedWide	usec;#if 0	struct UnsignedWide {   // big-endian version	   UInt32    hi;	   UInt32    lo;	   };#endif	Microseconds( &usec );		return JS_NewNumberValue(cx, (double)usec.lo, rval);}// --------------------------------------------------------------------------------static JSBooldsGetKeys(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval){	KeyMap		keyMap;	JSObject 	*ovec = JSVAL_NULL;	jsval		elm;	long		num;		GetKeys(keyMap);		if (argc < 1)	{		// !!@ Need to store in gc root, best to take input as array		ovec = JS_NewArrayObject(cx, 0, 0);	}	else if (JSVAL_IS_OBJECT(argv[0]))	{		ovec = JSVAL_TO_OBJECT(argv[0] );		if (! JS_IsArrayObject(cx, ovec))		{			// !!@ Array expected.			return JS_FALSE;		}	}	if (ovec)	{		for (int i = 0; i < 4; i++)		{			num = keyMap[i];			if (INT_FITS_IN_JSVAL(num))			{				elm = INT_TO_JSVAL( num );			}			else			{				elm = DOUBLE_TO_JSVAL(JS_NewDouble(cx, num ));			}			JS_SetElement(cx, ovec, i, &elm);		}	}	*rval = OBJECT_TO_JSVAL(ovec);    return JS_TRUE;}// --------------------------------------------------------------------------------static JSFunctionSpec dom_functions[] = {    {"GetKeys",         		dsGetKeys,        0},    {"Microseconds",			dsMicroseconds,        0},    {"GetCurrentEventTime",		dsGetCurrentEventTime,        0},    {0}};// --------------------------------------------------------------------------------JSBool ds_ConvertStringPtrToJSValue( JSContext *cx, StringPtr strp, jsval *rval){	JSString 		*jstr ;	jstr = JS_NewStringCopyN(cx, (char*)&strp[1], strp[0]);	if ( jstr)	{		*rval = STRING_TO_JSVAL(jstr);		return JS_TRUE;	}	else	{		*rval = JSVAL_NULL;		return JS_FALSE;	}}// --------------------------------------------------------------------------------JSBool ds_ConvertJSValueToStringPtr( JSContext *cx, jsval jval, StringPtr strp){	JSString*	pjstr;	char		*ptr;		if (JSVAL_IS_STRING(jval))	{		pjstr = JS_ValueToString(cx, jval);		if (pjstr)		{			ptr = JS_GetStringBytes(pjstr);			if (ptr)			{  				CopyCStringToPascal(ptr, strp);  				return JS_TRUE;			}		}	}	return JS_FALSE;}// --------------------------------------------------------------------------------JSObject *ds_InitDOM(JSContext *cx, JSObject *obj){    if (!JS_DefineFunctions(cx, obj, dom_functions))    {    	// !!@ function def failure    }     	ds_NotePlayer_InitClass(cx, obj); 	 	ds_Window_InitClass(cx, obj);  	 	ds_PushButton_InitClass(cx, obj); 	 	ds_Caption_InitClass(cx, obj); 	 	ds_TextField_InitClass(cx, obj);	ds_Camera_InitClass(cx, obj);		ds_Image_InitClass(cx, obj);	ds_Projector_InitClass(cx, obj);	    return 0;}// --------------------------------------------------------------------------------// --------------------------------------------------------------------------------