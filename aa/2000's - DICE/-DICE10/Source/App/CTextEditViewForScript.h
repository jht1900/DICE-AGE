/* Special processing for script text edit pane.History:Copyright � 2003 John Henry Thompson. All rights reserved.2003.03.21	jht	Created.*/#pragma once#include <LTextEditView.h>class CTextEditViewForScript : public LTextEditView{public:	enum { class_ID = FOUR_CHAR_CODE('txtV') };						CTextEditViewForScript();						CTextEditViewForScript( LStream *inStream );							virtual				~CTextEditViewForScript();	virtual Boolean		HandleKeyPress(								const EventRecord	&inKeyEvent);									void				FinishCreateSelf();	protected:};