#include <nall/nall.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

struct Prog {
	string name;	/* app name */
	void open(const string &fn);
	void close();
	void interactiveMode(const char *fn);
	void usage();
	void init();
	Prog(int argc, char **argv);
} *prog = nullptr;

struct Win : Window {
	/** menus **/
	Menu mFile;
		Item mFileQuit;
	Menu mHelp;
		Item mHelpAbout;
	/** layouts **/
	VerticalLayout lMain;
	Label lblText;
	void init();
	void reflowStatic();
	void reflow();
	Win();
} *win = nullptr;
