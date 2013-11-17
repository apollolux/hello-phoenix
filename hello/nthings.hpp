#ifndef __N_THINGS__
#define __N_THINGS__

#include <nall/nall.hpp>

/****
Spritesheet
Load an image in preparation of sectioning
****/
struct Spritesheet {
	nall::image im;
	Spritesheet() {}
	Spritesheet(const string& fn) {
		if (load(fn)) {}
	}
	bool load(const string& fn) {
		return im.load(fn);
	}
	nall::image slice(const unsigned &x, const unsigned &y, const unsigned &w, const unsigned &h) {
		nall::image ret(im.endian, im.depth, im.alpha.mask, im.red.mask, im.green.mask, im.blue.mask);
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


#endif