Win::Win() : Window() {
	init();	// initialize static variables
	reflowStatic();	// initialize static layout
	reflow();	// update initial layouts
}

void Win::openFile(const string &fn) {
	prog->open(fn);
	feFile.setPath(fn);
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
	bImg.setText("");
	bImgText.setText("Icon+Text");
	bText.setText("Text");
	cnSmp.setSize({256,256});
	//cnSmp.setColor({128,0,0,0});
	//cnSmp.setColor({255,0,0,0});
	cnSmp.setData();
	cnSmp.fill_old(0xff000000);
	cnSmp.plot(64, 64, 0xffffffff);
	cnSmp.plot(128, 64, 0xff000000);
	uint32_t colT = 0xff2020ff;
	cnSmp.plot(127, 127, colT);
	cnSmp.plot(127, 128, colT);
	cnSmp.plot(127, 129, colT);
	cnSmp.plot(128, 127, colT);
	cnSmp.plot(128, 128, colT);
	cnSmp.plot(128, 129, colT);
	cnSmp.plot(129, 129, colT);
	cnSmp.plot(129, 128, colT);
	cnSmp.plot(129, 127, colT);
	//cnSmp.line(8,8, 64,192,0xff4080ff);
	cnSmp.line(10,10, 10,210,0xffffffff);
	cnSmp.line(10,10, 30,210,0xffffffff);
	cnSmp.line(10,10, 210,10,0xffffffff);
	cnSmp.line(10,10, 210,30,0xffffffff);
	setStatusText({
		"Hello world!",
		" ",
		"What's up?"
	});
	//browser.setParent(*this).setPath("").setFilters("All files (*)");
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
	cnSmp.onMousePress = [&](Mouse::Button b) {
		Position* p = &cnSmp.current;
		if (b==Mouse::Button::Left) {
			uint32_t colT = 0x20ffff20;
			uint32_t before = cnSmp.at(p->x, p->y);
			cnSmp.plot(p->x-1,p->y-1, colT);
			cnSmp.plot(p->x,p->y-1, colT);
			cnSmp.plot(p->x+1,p->y-1, colT);
			cnSmp.plot(p->x-1,p->y, colT);
			cnSmp.plot(p->x,p->y, colT);
			cnSmp.plot(p->x+1,p->y, colT);
			cnSmp.plot(p->x-1,p->y+1, colT);
			cnSmp.plot(p->x,p->y+1, colT);
			cnSmp.plot(p->x+1,p->y+1, colT);
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
			cnSmp.setData();
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
		lTabbed.setLayout(0, lTab1);
		fsT1.setText("Framed");
		fsT1.setLayout(lTab1a);
	lTabbed.append("Second");
		lTabbed.setLayout(1, lTab2);
	lTabbed.append("Third");
		lTabbed.setLayout(2, lTab3);
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
	lMain.remove(lTabbed); lMain.remove(feFile);
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
	lMain.append(feFile, {~0, 0}); feFile.reflow();
}
void Win::resize() {
	/****
	add some resize handling here
	maybe make an interface 3 columns instead of 2 if wide enough,
	or set a tab bar to align left instead of top,
	or set buttons to only use images when small and image+text when wide
	****/
}