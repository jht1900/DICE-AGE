/**/#include <MacTypes.h>#include <MemoryMgr.h>#include <ToolboxUtil.h>#include <QuickDraw.h>#include <SerialDvr.h>#include <pascal.h>#include "HyperXCmd.h"#include "pioneer.h"#define NOVAL	(-1L)#define Nullp(a) ((a) == NOVAL)int 			vd;int				score;Str255			str;long			arg1num;Byte			*arg1;long			vdwait();/* ------------------------ main -------------------------------------- */pascal void main(pp)	XCmdBlockPtr	pp;	{	Byte		*op;	Handle		h;		SetUpA4();#if 0	h = RecoverHandle(* (Ptr *) 0x9CE);	if (((long) *h) & 0x40000000) SysBeep(2);#endif		vd = AoutRefNum;	if (pp->paramCount < 1)		{		SysBeep(5);		goto End;		}			op = * (pp->params[0]);		if (pp->paramCount > 1)		{		arg1 = * (pp->params[1]);		if (stcd_i(arg1, &arg1num) > 0) ;		else if (strcmp(arg1, "on") == 0)			arg1num = 1;		else if (strcmp(arg1, "off") == 0)			arg1num = 0;		}		#define Is(a) (strcmp(op, a) == 0)	if (Is("port"))		SelectPort( (int) arg1num);			else if (Is("play"))		Vd1(vd, Play);			else if (Is("search"))		Vd(vd, Search, (int)arg1num);			else if (Is("stopat"))		Vd(vd, AutoStop, (int)arg1num);			else if (Is("waitfor"))		vdwait( arg1num );	else if (Is("chargen"))		Vd1(vd, (int)arg1num? CharGenOn: CharGenOff);			else if (Is("pos"))		ReturnPos(pp);			else if (Is("frame"))		Vd(vd, DisplayFrame, (int)arg1num);			else if (Is("video"))		Vd1(vd, (int)arg1num? SetVideoOn: SetVideoOff);			else if (Is("audio1"))		Vd(vd, 0xF4, (int)arg1num);			else if (Is("audio2"))		Vd(vd, 0xFC, (int)arg1num);			else if (Is("stop"))		Vd1(vd, Stop);			else if (Is("slowfwd"))		SlowForward( (int)arg1num );			else if (Is("slowrev"))		SlowRev( (int)arg1num );		else if (Is("stepfwd"))		Vd1(vd, StepFwd);			else if (Is("steprev"))		Vd1(vd, StepRev);			else if (Is("rev"))		RevPlay();			else if (Is("ff"))		FastForward( (int)arg1num );			else if (Is("rew"))		Rewind( (int)arg1num );			else if (Is("init"))		VdOpen(0);			else SysBeep(2);End:	RestoreA4();	}	/* -------------------------------------------------------------------- */SlowRev(n)	{	Vd(vd, SlowSpeed, n);	DelayBy(2);	Vd(vd, SearchRev, 0x0000);	}	SlowForward(n)	{	Vd(vd, SlowSpeed, n);	DelayBy(2);	Vd(vd, SearchFwd, 0xFFFF);	}	RevPlay()	{	Vd(vd, SlowSpeed, 1);	DelayBy(2);	Vd(vd, SearchRev, 0x0000);	}	FastForward(n)	{	Vd(vd, FastSpeed, n);	DelayBy(2);	Vd(vd, SearchFwd, 0xFFFF);	}Rewind(n)	{	Vd(vd, FastSpeed, n);	DelayBy(2);	Vd(vd, SearchRev, 0x0000);	}	DelayBy(n)	{	long t;	Delay((long) n, &t);	}	/* ------------------------ SelectPort -------------------------------- */SelectPort(n)	int	n;	{	switch (n)		{		case 1:			n = 0x40;			break;		case 2:			n = 0xC3;			break;		case 3:			n = 0x83;			break;		case 4:			n = 0x60;		default:			n = 0x40;			break;		}	MacEnhancer(n);	}	/* ------------------------ MacEnchancer ------------------------------ */MacEnhancer(op)	int	op;	{	int		val;	Handle	macen_h;	void	(*macen_f)();			macen_h = GetResource('PACK', 128);	if (! macen_h)		{		OhShit("\pGetResource macenh failed");		}			HLock(macen_h);		macen_f = (void (*)()) (*macen_h);		val = CallPascalW(18, -1, 0L, 0L, 0L, (int)op, 1/*-Modem-*/, macen_f);		return (val);	}/* ------------------------ ReturnPos ---------------------------------- */Handle	MakeNum();Handle	MakeString();long	vdreadpos();ReturnPos(pp)	XCmdBlockPtr	pp;	{	pp->returnValue = MakeNum(vdreadpos());	}	/* -------------------- MakeNum ---------------------------------------- */Handle MakeNum(n)	long	n;	{	Byte	str[16];		stci_d(str, n, 16);		return ( MakeString(str) );	}/* ----------------------- MakeString --------------------------------- */Handle MakeString(str)	Byte *str;	{	Handle hd;		hd = NewHandle((long) strlen(str)+1);	if (! hd)		{		OhShit("\pNewString no room");		return (hd);		}	strcpy(*hd, str);	return (hd);	}	/* -------------------- vdreadpos -------------------------------------- */long vdread();long vdreadpos()	{	long	it;		Vd1(vd, GimmeFrame);		while ((it = vdread()) == NOVAL && ! Button()) ;		return (it);	}/* ------------------ vdread ------------------------------------------- */long vdread()	{	long count, n;	Byte buf[32];	int	er;		SerGetBuf(vd + 1, &count);	if (count >= 6)		{		er = FSRead(vd + 1, &count, buf);		if (er) OhShit("\pFSRead er=%d on count=%d\n", er, count);		buf[count] = 0;		stch_i(buf, &n);		/* sscanf(buf, "%lx", &n); */#if 0		printf("(@ %s n=%ld c=%ld) ", buf, n, count);#endif		return ( n );		}			return (NOVAL);	}/* -------------------------- vdwait ------------------------------------ */long vdwait(target)	long		target;	{	long	vl;	long	to = 0x10000;	vdread();	for (; !Button();)		{		Vd1(vd, GimmeFrame);		for (; !Button();)			{			vl = vdread();			if (Nullp (vl))				continue;			if (vl != target)				break;			return (NOVAL);			}		}	}	/* --------------------- OhShit --------------------------------------- */OhShit(a,b)	Ptr	a, b;	{	DebugStr(a);	}	/* --------------------------------------------------------------------- */