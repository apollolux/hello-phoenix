#include <nall/nall.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include "nthings.hpp"

struct Prog {
	/** program properties **/
	string name;	/* app name */
	float version;	/* app version */
	/** program methods **/
	void open(const string &fn);	/* open a file */
	void close();	/* close a file */
	void interactiveMode(const char *fn);	/* open the GUI */
	void usage();	/* print out program usage */
	void init();	/* initialize program variables */
	/** constructors **/
	Prog(int argc, char **argv);
} *prog = nullptr;

struct Win : Window {
	/** menus **/
	Menu mFile;
		Item mFileOpen;
		Item mFileQuit;
	Menu mHelp;
		Item mHelpAbout;
	/** layouts **/
	VerticalLayout lMain, lTab1, lTab1a, lTab2, lTab3;
	HorizontalLayout lT2Btns;
	TabFrame lTabbed;
	Frame fsT1;
	/** widgets **/
	FileEntry feFile;
	Label lblText, lblT2, lblT3;
	Button bImg, bImgText, bText;
	/** aux **/
	Spritesheet sprTabs;
	BrowserWindow browser;	/* file browser for "Open File" functionality */
	/** proc **/
	void openFile(const string &fn);
	void init();
	void reflowStatic();
	void reflow();
	void resize();
	/** constructors **/
	Win();
} *win = nullptr;
