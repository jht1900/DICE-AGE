/* Video disc player messages.*/#if 0#define BOTH	1#define MIXED	1#define NONE	1#define ENCHANCER	1#endif#define DIG		1#define ONE		1#define VID	3#define	AUD	2enum {	INIT = 0,	PLAY, STOP, SEARCH, SEARCHWAIT, POS,	STEPFWD, STEPREV, FF, REW, SLOWFWD, SLOWREV,	VIDEO, AUDIO,	FRAME, CHARGEN,	SETMARK, STOPAT,	AUDIOSQ, TEXT, CLEARTEXT, 	MAXVIDMSG	};	#define z(op, a1) (*obj[op])(a1)#define zz(op, a1, a2) (*obj[op])(arg)#define zzz(op, a1, a2, a3) (*obj[op])(a1,a2,a3)typedef int (**Object)();extern Object obj;