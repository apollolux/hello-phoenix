Win::Win() : Window() {
	init();	// initialize static variables
	reflowStatic();	// initialize static layout
	reflow();	// update initial layouts
}

void Win::openFile(const string &fn) {
	prog->open(fn);
	//feFile.setPath(fn);
}
void Win::init() {
	/** init settings **/
	setWindowGeometry({32,32,640,480});
	mFile.setText("File");
		mFileOpen.setText("Open");
		mFileQuit.setText("Quit");
	mHelp.setText("Help");
		mHelpAbout.setText("About...");
	lMain.setMargin(5);
	lTab1.setMargin(5);
		lTab1a.setMargin(5);
	lTab2.setMargin(5);
	lTab3.setMargin(5);
	lblText.setText("Hello world!");
	lblT2.setText("Hello tabs!");
	lblT3.setText("Hello phoenix!");
	bImg.setText(""); clicks = 0;
	bImgText.setText("Icon+Text");
	bText.setText("Text");
	cnSmp.setSize({256,256});
	//bzc.setCurve(Position(15,10),Position(130,10),Position(50,100),Position(150,190),slBz.position());
	bzc.setCurve(FPPosition(10,10),FPPosition(10,210),FPPosition(210,230),FPPosition(210,30),6);//slBz.position());
	mesh.name = "cube";
	mesh.vertices.append(NTerra::vec3_t(-1,1,1));
	mesh.vertices.append(NTerra::vec3_t(1,1,1));
	mesh.vertices.append(NTerra::vec3_t(-1,-1,1));
	mesh.vertices.append(NTerra::vec3_t(1,-1,1));
	mesh.vertices.append(NTerra::vec3_t(-1,1,-1));
	mesh.vertices.append(NTerra::vec3_t(1,1,-1));
	mesh.vertices.append(NTerra::vec3_t(1,-1,-1));
	mesh.vertices.append(NTerra::vec3_t(-1,-1,-1));
	mesh.faces.append({0,1,2});
	mesh.faces.append({1,2,3});
	mesh.faces.append({1,3,6});
	mesh.faces.append({1,5,6});
	mesh.faces.append({0,1,4});
	mesh.faces.append({1,4,5});
	mesh.faces.append({2,3,7});
	mesh.faces.append({3,6,7});
	mesh.faces.append({0,2,7});
	mesh.faces.append({0,4,7});
	mesh.faces.append({4,5,6});
	mesh.faces.append({4,6,7});
	mesh.rotation.x = 0.0;
	mesh.rotation.y = 0.0;
	dev.camera.position = {0,0,10};
	dev.camera.target = {0,0,0};
	dev.setView();
	dev.setProjection(cnSmp);
	//cnSmp.setColor({128,0,0,0});
	//cnSmp.setColor({255,0,0,0});
	cnSmp.setData();
	repaint();
	setStatusText({
		"Hello world!",
		" ",
		"What's up?"
	});
	//browser.setParent(*this).setPath("").setFilters("All files (*)");
	initProc();
	tUpdater.setEnabled();
}
void Win::repaint() {
	cnSmp.fill_old(0xff000000);
	uint32_t colT = 0xff2020ff;
	//cnSmp.rect(144,144, 10, 10, 0xa0e020ff);
	/*cnSmp.circle(192,192, 32, 0xa0ffe020);
	cnSmp.plot(64, 64, 0xffffffff);
	cnSmp.plot(128, 64, 0xff000000);*/
	/*cnSmp.plot(127, 127, colT);
	cnSmp.plot(127, 128, colT);
	cnSmp.plot(127, 129, colT);
	cnSmp.plot(128, 127, colT);
	cnSmp.plot(128, 128, colT);
	cnSmp.plot(128, 129, colT);
	cnSmp.plot(129, 129, colT);
	cnSmp.plot(129, 128, colT);
	cnSmp.plot(129, 127, colT);*/
	//cnSmp.line(8,8, 64,192,0xff4080ff);
	//cnSmp.line(10,10, 10,210,0xffffffff);
	cnSmp.line(10,10, 30,210,0xffffffff);
	cnSmp.line(10,10, 210,10,0xffff4020);
	cnSmp.line(10,10, 210,30,0xffff4020);
	//bzc.printCurve();
	bool isDot = false, isDrawn = bzc.draw(cnSmp, colT, isDot);//!cbBz.checked());
	//if (!isDrawn) MessageWindow().setTitle("Bezier Warning").setText("Couldn't update canvas!").warning();
	dev.render(cnSmp, mesh);
	cnSmp.setData();
}
void Win::initProc() {
	/** menu methods **/
	mFileOpen.onActivate = [&] {
		string fn = BrowserWindow().setParent(*this).setPath("").setFilters("All files (*)").open();
		//string fn = browser.open();
		if (!fn.empty()) openFile(fn);
	};
	mFileQuit.onActivate = &Application::quit;
	mHelpAbout.onActivate = [&] {
		// todo: open about dialog
		MessageWindow()
		    .setTitle({"About ", prog->name})
		    .setText({
		      prog->name, " v", prog->version, "\n",
		      "Author: ", "apollolux", "\n",
		      "License: ", "ISC"
		    })
    		.information();
	};
	/** widget methods **/
	tUpdater.onActivate = [&](){
		mesh.rotation.x += 0.01;
		mesh.rotation.y += 0.01;
		repaint();
	};
	//tUpdater.setInterval(16);
	bImg.onActivate = [&]() {
		lblT2.setText({"Clicks: ", ++clicks});
		string cmd = {"echo \"hello clicks (",clicks,")\""};
		_wsystem(utf16_t(cmd));
	};
	cnSmp.onMousePress = [&](Mouse::Button b) {
		Position* p = &cnSmp.current;
		if (b==Mouse::Button::Left) {
			uint32_t colT = 0x20ffff20;
			uint32_t before = cnSmp.at(p->x, p->y);
			/*cnSmp.plot(p->x-1,p->y-1, colT);
			cnSmp.plot(p->x,p->y-1, colT);
			cnSmp.plot(p->x+1,p->y-1, colT);
			cnSmp.plot(p->x-1,p->y, colT);
			cnSmp.plot(p->x,p->y, colT);
			cnSmp.plot(p->x+1,p->y, colT);
			cnSmp.plot(p->x-1,p->y+1, colT);
			cnSmp.plot(p->x,p->y+1, colT);
			cnSmp.plot(p->x+1,p->y+1, colT);*/
			cnSmp.circle(p->x, p->y, 5, colT);
			/*Size z = cnSmp.size();
			uint32_t* d = cnSmp.data();
			uint32_t* q = d+p->y*z.width+p->x;*/
			setStatusText({
				"PLOT [",
					p->x,",",
					p->y,":",
					hex<8,'0'>(before),"->",hex<8,'0'>(cnSmp.at(p->x, p->y)),
				"]"
			});
			//cnSmp.setData();
		}
		else setStatusText({
			"Button:",(unsigned)b,
			" P[",cnSmp.current.x,",",cnSmp.current.y,"]"
		});
	};
	/** window methods **/
	onClose = &Application::quit;
	onSize = {&Win::resize, this};
	/**** handle "drag & drop onto window" events ****/
	setDroppable();
	onDrop = [&](lstring fns) {
		if (fns.size() && file::exists(fns[0])) {
			openFile(fns[0]);
			setStatusText({
				"Opened ",
				fns[0]
			});
			setFocused();
		}
		else {
			MessageWindow()
				.setTitle({"Drop failed - ", prog->name})
				.setText({
				  "Couldn't open!"
				})
    			.information();
		}
	};
}

void Win::reflowStatic() {
	append(mFile);
		mFile.append(mFileOpen);
		mFile.append(mFileQuit);
	append(mHelp);
		mHelp.append(mHelpAbout);
	lTabbed.append("First");
		lTabbed.setLayout(0, lTab3);
		fsT1.setText("Framed");
		fsT1.setLayout(lTab1a);
	lTabbed.append("Second");
		lTabbed.setLayout(1, lTab2);
	lTabbed.append("Third");
		lTabbed.setLayout(2, lTab1);
	if (sprTabs.load("res/ui-icons_2e83ff_256x240.png")) {
		lTabbed.setImage(0, sprTabs.slice(16,144,16,16));
		lTabbed.setImage(1, sprTabs.slice(144,128,16,16));
		lTabbed.setImage(2, sprTabs.slice(160,112,16,16));
		bImg.setImage(sprTabs.slice(240,96,16,16) );
		bImgText.setImage(sprTabs.slice(176,112,16,16) );
		//lvSw.setImage(5,0, spr.slice(144,96,16,16) );
	}
	else {
		bImg.setText("Icon (no icon)");
		bImgText.setText("Image+Text (no icon)");
		lblT2.setText("Hello tabs (no icons)!");
	}
	setMenuVisible();
	setStatusVisible();
}
void Win::reflow() {
	append(lMain);
	lMain.remove(lTabbed); //lMain.remove(feFile);
	lMain.append(lTabbed, {~0, ~0}, 5);
		lTab1.remove(fsT1); lTab1.append(fsT1, {~0, ~0});
			lTab1a.remove(lblText); lTab1a.append(lblText, {~0, 0});
		lTab2.remove(lblT2);
			lTab2.append(lblT2, {~0, 0}, 5);
			lTab2.append(lT2Btns, {~0, 0});
				lT2Btns.append(bImg, {0, 0}, 5);
				lT2Btns.append(bText, {0, ~0}, 5);
				lT2Btns.append(bImgText, {0, 0});
		lTab3.remove(lblT3);
			lTab3.append(lblT3, {~0, 0}, 5);
			lTab3.append(cnSmp, {~0, ~0});
	//lMain.append(feFile, {~0, 0}); feFile.reflow();
}
void Win::resize() {
	/****
	add some resize handling here
	maybe make an interface 3 columns instead of 2 if wide enough,
	or set a tab bar to align left instead of top,
	or set buttons to only use images when small and image+text when wide
	****/
}