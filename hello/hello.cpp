#include "hello.hpp"
#include "win.cpp"

/** open a file **/
void Prog::open(const string &fn) {
	close();
	win->setTitle({notdir(fn)," - ", name});
}

/** close a file **/
void Prog::close() {
	win->setTitle(name);
}

/** open the GUI **/
void Prog::interactiveMode(const char *fn) {
	win = new Win;
	win->setVisible();
	win->setFocused();
	close();
	if (fn!=nullptr) open(fn);
	Application::run();
}

/** print out program usage **/
void Prog::usage() {
	print(
		name,"\n",
		"author: apollolux\n",
		"license: ISC\n",
		"usage:\n",
		"\thello\n",
		"\thello infile\n",
		"\n"
	);
}

/** initialize program variables **/
void Prog::init() {
	prog = this;
	name = "Hello phoenix";
	version = 1.0;
	Application::setName(name);
}

/** constructor **/
Prog::Prog(int argc, char **argv) {
	init();
	if (argc==2&&file::exists(argv[1])) {
		interactiveMode(argv[1]);
		return;
	}
	else if (argc==1) {
		interactiveMode(nullptr);
		return;
	}
	usage();
}


int main(int argc, char **argv) {
	#if defined(PLATFORM_WINDOWS)
	utf8_args(argc, argv);
	#endif
	new Prog(argc, argv);
	delete prog;
	return 0;
}
