Win::Win() {
	init();
	reflowStatic();
	reflow();
}

void Win::init() {
	/** init settings **/
	mFile.setText("File");
		mFileQuit.setText("Quit");
	mHelp.setText("Help");
		mHelpAbout.setText("About...");
	lMain.setMargin(5);
	lblText.setText("Hello world!");
	setStatusText({
		"Hello world!",
		" ",
		"What's up?"
	});
	/** menu methods **/
	mFileQuit.onActivate = &Application::quit;
	mHelpAbout.onActivate = [&] {
		// todo: open about dialog
	};
	/** window methods **/
	onClose = &Application::quit;
	onSize = {&Win::reflow, this};
}

void Win::reflowStatic() {
	append(mFile);
		mFile.append(mFileQuit);
	append(mHelp);
		mHelp.append(mHelpAbout);
	setMenuVisible();
	setStatusVisible();
}
void Win::reflow() {
	append(lMain);
	lMain.append(lblText, {~0, ~0}, 5);
}