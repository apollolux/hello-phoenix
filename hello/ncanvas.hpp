#ifndef __N_CANVAS__
#define __N_CANVAS__

#include <nall/nall.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include <cmath>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#endif

#include "blend.h"

/****
NCanvas
An extended Canvas widget for phoenix
****/
struct NCanvas : Canvas {
	Position current;
	NCanvas() : Canvas() {
		onMouseMove = [&](Position p){
			current = p;
			//plot(p.x, p.y, 0xff00ffff);
		};
	}
	inline void fill(uint32_t col=0x00000000) {
		setColor({
			(uint8_t)((col&0xff000000)>>24),
			(uint8_t)((col&0x00ff0000)>>16),
			(uint8_t)((col&0x0000ff00)>>8),
			(uint8_t)(col&0x000000ff)
		});
	}
	inline void fill_old(uint32_t col=0x00000000) {
		Size z = size();
		uint32_t *dp = data();
		uint32_t i, j; for (i=0; i<z.height; ++i)
			for (j=0; j<z.width; ++j) *dp++ = col;
	}
	/** rect(x,y, w,h, color, overwrite) **/
	inline void rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t col, bool over=false) {
		uint32_t m = x+w, n = y+h, _x, _y; for (_y=y; _y<n; ++n) {
			for (_x=x; _x<m; ++_x) {plot(_x, _y, col, over);}
		}
	}
	//inline void rect(const Position &z, uint32_t w, uint32_t h, uint32_t col, bool over=false) {rectangle(z.x, z.y, w, h, col, over);}
	//inline void rect(const Position &z, const Size &s, uint32_t col, bool over=false) {rectangle(z.x, z.y, s.width, s.height, col, over);}
	//inline void rect(uint32_t x, uint32_t y, const Size &s, uint32_t col, bool over=false) {rectangle(x, y, s.width, s.height, col, over);}
	/** circle(x, y, radius, color, overwrite) **/
	inline void circle(uint32_t x, uint32_t y, uint32_t r, uint32_t col, bool over=false) {
		double _d, _d2, _r2 = r*r, _rz = r-1;
		uint32_t c = col,
			cc = col&0x00ffffff,
			ca = (col>>24)&0xff;
		double pi_1_2 = M_PI_2;
		uint32_t _x, _y; for (_y=0; _y<r; ++_y) {
			for (_x=0; _x<r; ++_x) {
				_d2 = (double)(_x*_x+_y*_y);
				if (_d2>_r2) continue;
				else {
					_d = sqrt(_d2);
					if (_d>_rz)
						c = ((uint32_t)((double)(ca)*sin( pi_1_2*sin(pi_1_2*(r-_d)) ))<<24)|cc;	// change alpha
					else c = col;	// leave alpha
					plot(x+_x, y+_y, c, over);
					if (_x!=0) plot(x-_x, y+_y, c, over);
					if (_x!=0&&_y!=0) plot(x-_x, y-_y, c, over);
					if (_y!=0) plot(x+_x, y-_y, c, over);
				}
			}
		}
	}
	inline void circle(const Position &z, uint32_t r, uint32_t col, bool over=false) {circle(z.x, z.y, r, col, over);}
	/** line(x1, y1, x2, y2, color, overwrite) **/
	inline void line(double x1, double y1, double x2, double y2, uint32_t col, bool over=false) {
		//Size cnSz = size();
		double _x1 = (double)x1, _x2 = (double)x2,
			_y1 = (double)y1, _y2 = (double)y2;
		double gr, ex, ey, gx, gy, b1, b2;
		unsigned ix, iy, ix1, iy1, qx, qy;
		double tmp, fx, fy;
		double dx = _x2-_x1, dy = _y2-_y1;
		bool done;
		// TODO: offload blending to plot()
		uint32_t ca = (col>>24)&0xff,
			cc = col&0x00ffffff;
		if (abs(dx)>abs(dy)) {	// skewed horizontal
			//// swap if p1 > p2
			if (_x1>_x2) {	// OPTIMIZE THIS BLOCK
				tmp = _x1; _x1 = _x2; _x2 = tmp;
				tmp = _y1; _y1 = _y2; _y2 = tmp;
				dx = _x2-_x1; dy = _y2-_y1;
			}
			gr = dy/dx;
			//// endpt 1
			tmp = _x1+0.5;
			ex = (double)((signed)tmp);
			ey = _y1+gr*(ex-_x1);
			gx = 1.0-(tmp-ex);
			ix1 = (signed)ex;
			iy1 = (signed)ey;
			b1 = gx*(1.0-(ey-iy1));
			b2 = gx*(ey-iy1);
			plot(
				ix1,iy1,
				//col*b1,over
				cc|(((uint32_t)(ca*b1))<<24),over
			);
			plot(
				ix1,iy1+1,
				//col*b2,over
				cc|(((uint32_t)(ca*b2))<<24),over
			);
			fy = ey+gr;
			qx = ix1;
			//// endpt 2
			tmp = _x2+0.5;
			ex = (double)((signed)tmp);
			ey = _y2+gr*(ex-_x2);
			tmp = _x2-0.5;
			gx = 1.0-(tmp-(signed)tmp);
			ix = (signed)ex;
			iy = (signed)ey;
			b1 = gx*(1.0-(ey-(double)iy));
			b2 = gx*(ey-(double)iy);
			plot(
				ix,iy,
				//col*b1,over
				cc|(((uint32_t)(ca*b1))<<24),over
			);
			plot(
				ix,iy+1,
				//col*b2,over
				cc|(((uint32_t)(ca*b2))<<24),over
			);
			//// main loop
			done = false;
			do {
				++qx;
				qy = (signed)fy;
				b1 = 1.0-(fy-(double)qy);
				b2 = fy-(double)qy;
				plot(
					qx,qy,
					//col*b1,over
					cc|(((uint32_t)(ca*b1))<<24),over
				);
				plot(
					qx,qy+1,
					//col*b2,over
					cc|(((uint32_t)(ca*b2))<<24),over
				);
				fy += gr;
				done = (bool)(qx==ix);
			} while (!done);
		}
		else {	// skewed vertical
			//// swap if p1 > p2
			if (_y1>_y2) {	// OPTIMIZE THIS BLOCK
				tmp = _x1; _x1 = _x2; _x2 = tmp;
				tmp = _y1; _y1 = _y2; _y2 = tmp;
				dx = _x2-_x1; dy = _y2-_y1;
			}
			gr = dx/dy;
			//// endpt 1
			tmp = _y1+0.5;
			ey = (double)((signed)tmp);
			ex = _x1+gr*(ey-_y1);
			gy = 1.0-(tmp-ey);
			ix1 = (signed)ex;
			iy1 = (signed)ey;
			b1 = gy*(1.0-(ex-ix1));
			b2 = gy*(ex-ix1);
			plot(
				ix1,iy1,
				//col*b1,over
				cc|(((uint32_t)(ca*b1))<<24),over
			);
			plot(
				ix1+1,iy1,
				//col*b2,over
				cc|(((uint32_t)(ca*b2))<<24),over
			);
			fx = ex+gr;
			qy = iy1;
			//// endpt 2
			tmp = _y2+0.5;
			ey = (double)((signed)tmp);
			ex = _x2+gr*(ey-_y2);
			tmp = _y2-0.5;
			gy = 1.0-(tmp-(signed)tmp);
			ix = (signed)ex;
			iy = (signed)ey;
			b1 = gy*(1.0-(ex-ix));
			b2 = gy*(ex-ix);
			plot(
				ix,iy,
				//col*b1,over
				cc|(((uint32_t)(ca*b1))<<24),over
			);
			plot(
				ix+1,iy,
				//col*b2,over
				cc|(((uint32_t)(ca*b2))<<24),over
			);
			//// main loop
			done = false;
			do {
				++qy;
				qx = (signed)fx;
				b1 = 1.0-(fx-(double)qx);
				b2 = fx-(double)qx;
				plot(
					qx,qy,
					//col*b1,over
					cc|(((uint32_t)(ca*b1))<<24),over
				);
				plot(
					qx+1,qy,
					//col*b2,over
					cc|(((uint32_t)(ca*b2))<<24),over
				);
				fx += gr;
				done = (bool)(qy==iy);
			} while (!done);
		}
	}
	//inline void line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t col, bool precalc=false) {line((double)x1, (double)y1, (double)x2, (double)y2, col, precalc);}
	inline void line(const Position &z1, const Position &z2, uint32_t col, bool over=false) {line((double)z1.x, (double)z1.y, (double)z2.x, (double)z2.y, col, over);}
	/** plot(x, y, color) **/
	inline void plot(const uint32_t &x, const uint32_t &y, const uint32_t &col, bool over=false) {
		Size z = size();
		if (x<z.width&&y<z.height) {
			uint32_t* cp = data();
			uint32_t* dp = cp+y*z.width+x;
			//if (col<0x01000000&&(*dp)<0x01000000) *dp = 0xff000000;	// transparent becomes black
			//else {
				uint32_t c = over?col:blend(col,*dp);//,//!precalc?col:(uint32_t)((col&0xffffff)*(double(col>>24)/255)),
					//_c = c;
				/*if (precalc) ;// print("[",x,",",y,"]=",(*dp),"+",_c,"\n");
				else {
					if ((*dp)>0) {	// TODO: alpha blend
						//_c = ((*dp)>>1)+
					}
				}*/
				*dp = c;
			//}
		}
	}
	inline void plot(const Position &p, const uint32_t &col, bool over=false) {plot(p.x, p.y, col, over);}
	/** blendAt(x, y, color) **/
	/*inline uint32_t blendAt(const uint32_t &x, const uint32_t &y, const uint32_t &col) {
		uint32_t c = at(x,y);
		return blend(col, c);
	}
	inline uint32_t blendAt(const Position &p, const uint32_t &col) {return blendAt(p.x, p.y, col);}*/
	/** blend(src, dst) **/
	inline uint32_t blend(const uint32_t &c1, const uint32_t &c2) {
		uint32_t a2 = c2 >> 24;
		uint32_t a1 = c1 >> 24;
		double t = (255-a1)/255.0;
		uint32_t a = a2==255?255:(a1+a2*t) / 255;//0xff;
		uint32_t r1 = (c1 >> 16) & 0xff, g1 = (c1 >> 8) & 0xff, b1 = (c1 >> 0) & 0xff;
		uint32_t r2 = (c2 >> 16) & 0xff, g2 = (c2 >> 8) & 0xff, b2 = (c2 >> 0) & 0xff;
		uint32_t r = (r1 * a1 + r2 * a2 * t) / 255;
		uint32_t g = (g1 * a1 + g2 * a2 * t) / 255;
		uint32_t b = (b1 * a1 + b2 * a2 * t) / 255;
		return (a << 24) | (r << 16) | (g << 8) | (b << 0);
	}
	inline Color blend(const Color &c1, const Color &c2) {
		uint32_t a2 = c2.alpha;
		uint32_t a1 = c1.alpha;
		double t = (255-a1)/255.0;
		uint32_t a = a2==255?255:(a1+a2*t) / 255;//0xff;
		uint32_t r1 = (c1.red), g1 = (c1.green), b1 = (c1.blue);
		uint32_t r2 = (c2.red), g2 = (c2.green), b2 = (c2.blue);
		uint32_t r = (r1 * a1 + r2 * a2 * t) / 255;
		uint32_t g = (g1 * a1 + g2 * a2 * t) / 255;
		uint32_t b = (b1 * a1 + b2 * a2 * t) / 255;
		return Color(a, r, g, b);
	}
	/** at(x, y) **/
	inline uint32_t at(uint32_t x, uint32_t y) {
		Size z = size();
		if (x<z.width&&y<z.height) {
			uint32_t* dp = data();
			return *(dp+y*z.width+x);
		}
		else return 0;
	}
	inline uint32_t at(const Position &p) { return at(p.x, p.y); }
};

#endif