/**/#include "cGlob.h"#include <setjmp.h>static jmp_buf			cTop;/* ---------------------------------------------------------------------------- */cEnv 		ce;short		cTraceLevel = 0;/* ---------------------------------------------------------------------------- */cTest()	{	Ent		*tok;			cEnvInit();			cOpenFile("\ptestscan");		if (setjmp(cTop) == 0)		{				xprintf("------------\n");		for (;;)			{			tok = cGetToken();			if (tok == EofTok)				break;			cUngetToken(tok);					cGetGlobalDef();			}		}		cCloseFile();		xprintf("===========\n");	cShowWorld();	cEnvClose();	}/* ---------------------------------------------------------------------------- */cTraceOn()	{	cTraceLevel = 1;	}	cTraceOff()	{	cTraceLevel = 0;	}	/* ---------------------------------------------------------------------------- */cGoTop()	{	longjmp(cTop, 1);	}	/* ---------------------------------------------------------------------------- */