#ifndef __N_THINGS__
#define __N_THINGS__

#include <nall/nall.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

/****
Spritesheet
Load an image in preparation of sectioning
****/
struct Spritesheet {
	image im;
	/*Spritesheet() {}*/
	Spritesheet(const image& img = image{}) : im(img) {}
	Spritesheet(const Spritesheet& spr) {im = spr.im;}
	Spritesheet(const string& fn) {
		if (load(fn)) {}
	}
	bool load(const string& fn) {
		return im.load(fn);
	}
	image slice(const unsigned &x, const unsigned &y, const unsigned &w, const unsigned &h) {
		image ret(im.endian, im.depth, im.alpha.mask, im.red.mask, im.green.mask, im.blue.mask);
		if (w<=im.width&&h<=im.height) {
			if (x>=0&&y>=0&&x+w<=im.width&&y+h<=im.height) {
				ret.allocate(w, h); ret.clear(0);
				unsigned _x, _y;
				for (_y=0; _y<h; ++_y) {
					uint8_t *dp = ret.data + ret.pitch * _y;
					uint8_t *sp = im.data + im.pitch * (_y+y) + x*im.stride;
					for (_x=0; _x<w; ++_x) {
						uint64_t color = im.read(sp);
						sp += im.stride;
						ret.write(dp, color);
						dp += ret.stride;
					}
				}
			}
			else {ret.allocate(1,1); ret.clear(0);}
		}
		else {ret.allocate(1,1); ret.clear(0);}
		return ret;
	}
};


/****
FileEntry
A labeled text field that can be updated with a filename
****/
struct FileEntry : HorizontalLayout {
	Label label;
	LineEdit input;
	Button bBrowse;
	Button bClear;
	string filter;
	string basePath;
	function<void (const string&)> cb;
	FileEntry() : HorizontalLayout() {
		basePath = "";
		// layout
		init("File");
		reflow();
		// proc
		cb = dummy;
		bBrowse.onActivate = [&]() {
			//MessageWindow::information(Window::None, "TODO: browse for file");
			string _lastdir = dir(getPath())||basePath;
			string _file = BrowserWindow().setParent(*window()).setPath("").setFilters("All files (*)").open();
			if (!_file.empty()) {
				setPath(_file);
				cb(getPath());
			}
			//cb(getPath());	// uncomment to run on cancelled browse
		};
		bClear.onActivate = [&]() {
			input.setText("");
			cb("");
		};
	}
	void init(const string &l, const string &b="Browse...", const string &c="Clear") {
		setLabel(l);
		bBrowse.setText(b);
		bClear.setText(c);
		filter = "All files (*)";
	}
	void reflow() {
		input.setEditable(false);
		append(label, {0, 0}, 4);
		append(input, {~0, 0}, 4);
		append(bClear, {0, 0}, 2);
		append(bBrowse, {0, 0});
	}
	void setLabel(const string &l) { label.setText(l); }
	void setPath(const string &p) { input.setText(p.length() > 0 ? p : ""); }
	string getPath() { return input.text(); }
	void setBasePath(const string &p="") { basePath = p; }
	void setFilter(const string &filt) { filter = filt; }
	void setOnLoad(const function<void (const string&)>& _cb) { cb = _cb; }
	static void dummy(const string&) {}
};


#endif