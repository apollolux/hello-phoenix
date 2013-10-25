#include "hello.hpp"
#include "win.cpp"

void Prog::open(const string &fn) {
	close();
	win->setTitle({notdir(fn)," - ", name});
}

void Prog::close() {
	win->setTitle(name);
}

void Prog::interactiveMode(const char *fn) {
	win = new Win;
	win->setVisible();
	win->setFocused();
	close();
	if (fn!=nullptr) open(fn);
	Application::run();
}

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
void Prog::init() {
	prog = this;
	name = "Hello phoenix";
}

int main(int argc, char **argv) {
	#if defined(PLATFORM_WINDOWS)
	utf8_args(argc, argv);
	#endif
	new Prog(argc, argv);
	delete prog;
	return 0;
}
