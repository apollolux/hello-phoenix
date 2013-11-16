Win::Win() {
	init();
	reflowStatic();
	reflow();
}

void Win::openFile(const string &fn) {
	prog->open(fn);
}
void Win::init() {
	/** init settings **/
	mFile.setText("File");
		mFileOpen.setText("Open");
		mFileQuit.setText("Quit");
	mHelp.setText("Help");
		mHelpAbout.setText("About...");
	lMain.setMargin(5);
	lblText.setText("Hello world!");
	lblT2.setText("Hello tabs!");
	lblT3.setText("Hello phoenix!");
	setStatusText({
		"Hello world!",
		" ",
		"What's up?"
	});
	//browser.setParent(*this).setPath("").setFilters("All files (*)");
	/** menu methods **/
	mFileOpen.onActivate = [&] {
		string fn = BrowserWindow().setParent(*this).setPath("./").setFilters("All files (*)").open();
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
	/** window methods **/
	onClose = &Application::quit;
	onSize = {&Win::reflow, this};
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
	lTabbed.append("Second");
	lTabbed.append("Third");
	setMenuVisible();
	setStatusVisible();
}
void Win::reflow() {
	append(lMain);
	lMain.append(lTabbed, {~0, ~0});
		lTab1.append(lblText, {~0, ~0}, 5);
		lTabbed.setLayout(0, lTab1);
		lTab2.append(lblT2, {~0, ~0}, 5);
		lTabbed.setLayout(1, lTab2);
		lTab3.append(lblT3, {~0, ~0}, 5);
		lTabbed.setLayout(2, lTab3);
}