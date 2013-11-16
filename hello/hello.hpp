#include <nall/nall.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

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
	VerticalLayout lMain, lTab1, lTab2, lTab3;
	Label lblText, lblT2, lblT3;
	TabFrame lTabbed;
	/** aux **/
	BrowserWindow browser;	/* file browser for "Open File" functionality */
	/** proc **/
	void openFile(const string &fn);
	void init();
	void reflowStatic();
	void reflow();
	/** constructors **/
	Win();
} *win = nullptr;
