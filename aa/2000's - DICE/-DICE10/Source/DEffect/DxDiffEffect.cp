// --------------------------------------------------------------------------------//// DiffEffect -- image difference effect//// 2005.02.26 	jht		Created.//// Copyright (c) 2005 John Henry Thompson. All rights reserved.// --------------------------------------------------------------------------------#include "dsDiffEffect.h"#include "DxDiffEffect.h"#include "dsImage.h"#include "DxImage.h"#include "dicejs.h"// --------------------------------------------------------------------------------DxDiffEffect::DxDiffEffect(void *wrapperObj )	: DxGeneric(wrapperObj ){	this->input = 0;	this->output = 0;	this->previous = 0;}// --------------------------------------------------------------------------------DxDiffEffect::~DxDiffEffect(){	Close();}// ---------------------------------------------------------------------------------void DxDiffEffect::SetInput(DxImage *newInput){	Close();		this->input = newInput;	if (! this->input)		return;		this->input->GetPixBaseInfo( 0/*basePtr*/, 0/*rowBytes*/, &this->bounds);}// ---------------------------------------------------------------------------------void DxDiffEffect::SetOutput(DxImage *newOutput){	this->output = newOutput;	if (! this->output)		return;		// !!@ Ensure same as input bounds	Rect	outRect;		this->output->GetPixBaseInfo( 0/*basePtr*/, 0/*rowBytes*/, &outRect);	if ( ! ( (outRect.right - outRect.left) == (this->bounds.right - this->bounds.left)		&& (outRect.bottom - outRect.top) == (this->bounds.bottom - this->bounds.top) ) )	{		this->output = 0;		// !!@ Need error alert here.		}}// ---------------------------------------------------------------------------------Boolean DxDiffEffect::PrepareBuffers(){	if (! this->input)		return FALSE;		if (! this->output)		return FALSE;		/* Create previous image temp */	if (! this->previous)	{				this->previous = new DxImage( 0, 32, this->bounds, 0 );	}		return TRUE;}// --------------------------------------------------------------------------------void DxDiffEffect::Process( ){	if (! PrepareBuffers())		return;		ComputeImage();}// ---------------------------------------------------------------------------------void	DxDiffEffect::ComputeImage(){	int	i;	int diffwPix;#define CODE_INNER_LOOP \				{ \					for (i = 0; i < 3; i++) \					{ \						diffwPix = sourceLinePtr[i] - previousLinePtr[i];  \						if (diffwPix < 0) \							diffwPix = 0; \						outputLinePtr[i] = diffwPix; \					} \				} \#include "DiffEffectLoop.inc"}// --------------------------------------------------------------------------------Boolean DxDiffEffect::GetProp( DxPropID propId, DxPropType propType, void **outResult ){	switch (propId)	{		case kDxProp_value:		{			if (this->output)				return this->output->GetProp(propId, propType, outResult );			break;		}	}	if (outResult)		*outResult = 0;	return FALSE;}// --------------------------------------------------------------------------------Boolean DxDiffEffect::SetProp( DxPropID propId, DxPropType propType, void *outResult ){	return FALSE;}// ---------------------------------------------------------------------------------void DxDiffEffect::Close(){	if (this->previous)	{		delete this->previous;		this->previous = 0;	}}// --------------------------------------------------------------------------------// --------------------------------------------------------------------------------