// Convolve Effect demon = 1ww = 640; wh = 480w = ww / n; h = wh / n;	// The dimension of the movie, should get from movie file laterimg0 = new Image(w, h);	// Create an image buffer to store frame from moviemov = new Camera();		// Create a Camera objectmov.output = img0;		// Movie will play into image bufferimg01 = new Image(w, h, 1);	// 1-bitproj0 = new Projector();proj0.input = img0;proj0.output = img01;proj0.inkMode = 64;	// ditherCopyimg = new Image(w, h);	proj01 = new Projector();proj01.input = img01;proj01.output = img;//mov.pickFile();		// Present a file picker dialog boximg2 = new Image(w, h);		// Create image buffer for the effectimg3 = new Image(w, h);		// Create image buffer for the effecteff = new ConvolveEffect(); // Effect: The image difference is colorizedeff.input = img2;			// The image from the movie is feed to the effecteff.output = img3;if (0) {	eff2 = new DiffEffect(); // DiffTrailEffect -- The image difference is left as a trail	eff2.input = img;	eff2.output = img2;	eff2.diffTolerance = 40}if (1) {	eff2 = new Projector();	eff2.input = img;	eff2.output = img2;}color_int = new Color(255,255,255)	// Color to apply to image differenceeff2.color = color_int;//win = new Window(2, 45, ww, wh);	// Window use to present the result of the Effect//win.show();win2 = new Window(2, 45, ww, wh);	// Window use to present the result of the Effectwin2.show();//proj = new Projector();		// Project simply copies input to output//proj.input = img;//proj.output = win;proj2 = new Projector();		// Project simply copies input to outputproj2.input = img3;proj2.output = win2;proj3 = new Projector();		// Project simply copies input to outputproj3.input = img3;proj3.output = img2;//mov.pickFile();		// Present a file picker dialog boxload("colorCycle.js");var colCycle;colCycle = setupColorCycle(color_int);load("report.js");var report_win;report_win = setupReportWindow();function processThem(){	mov.process();	proj0.process();	proj01.process();	eff2.process();	eff.process();	//proj.process();	proj2.process();	proj3.process();		stepColorCycle(colCycle);		issueReport(report_win);}function spendTime(){	processThem();}processThem();	