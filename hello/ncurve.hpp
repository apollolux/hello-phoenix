#ifndef __N_CURVE__
#define __N_CURVE__

#include <nall/nall.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

/****
FPSize : Size
A Size struct with floating point width and height
****/
struct FPSize : Size {
	double width;
	double height;
	inline FPSize() : width(0), height(0) {}
	inline FPSize(double width, double height) : width(width), height(height) {}
};
/****
FPPosition : Position
A Position struct with floating point x and y
****/
struct FPPosition : Position {
	double x;
	double y;
	inline FPPosition() : x(0.0), y(0.0) {}
	inline FPPosition(double x, double y) : x(x), y(y) {}
	inline FPPosition(const Position &p) : x(p.x), y(p.y) {}
	bool operator==(const Position &p) const {return x==p.x&&y==p.y;}
	bool operator!=(const Position &p) const {return x!=p.x||y!=p.y;}
	bool operator==(const FPPosition &p) const {return x==p.x&&y==p.y;}
	bool operator!=(const FPPosition &p) const {return x!=p.x||y!=p.y;}
};

/****
Curve
A nall+phoenix implementation of a cubic Bezier curve
****/
class Curve {
	public:
		Curve() {
			//Position q1({10,10}), q2({10,11});
			//Size s = dist(q1,q2);
			//MessageWindow::information(Window::None,{"Distance: ",s.width,",",s.height});
			//print("empty curve");
		}
		Curve(Position ps, Position pcs, Position pce, Position pe, int32_t t=256) {setCurve(ps,pcs,pce,pe,t);}
		//void setCurve(Position ps, Position pcs, Position pce, Position pe, int32_t t=256) {setCurve(FPPosition(ps),FPPosition(pcs),FPPosition(pce),FPPosition(pe),t);}
		void setCurve(FPPosition ps, FPPosition pcs, FPPosition pce, FPPosition pe, int32_t t=256) {
			p1 = ps; p2 = pcs, p3 = pce; p4 = pe;
			threshold = 0.25;
			points.reset();
			//print("C [",p1.x,",",p1.y,"] [",p2.x,",",p2.y,"] [",p3.x,",",p3.y,"] [",p4.x,",",p4.y,"]");
			int32_t n = addPoints(ps, pcs, pce, pe, t);
			//MessageWindow::information(Window::None,{"Completed: ",n});
			killDuplicates();
		}
		bool draw(NCanvas &c, uint32_t col, bool dot=true) {
			if (points.size()>0) {
				Size cnSz = c.size();
				bool pcc = true, pcl = true;
				bool pcb = false;
				if (points.size()>cnSz.width*cnSz.height) {
					MessageWindow()
						.setParent(*c.window())
						.setTitle({"Bezier Warning!"})
						.setText({
						  "Too many (",points.size(),") points!"
						})
						.warning();
					return false;
				}
				//MessageWindow::information(Window::None,{"Points: ",points.size()});
				c.line(p1.x,p1.y, p2.x,p2.y, 0x4020ff10, pcl);
					c.circle(p2.x,p2.y, 6, 0xffffffff, pcc); c.circle(p2.x,p2.y, 4, 0xff2010a0, pcc);
				c.line(p4.x,p4.y, p3.x,p3.y, 0x4020ff10, pcl);
					c.circle(p3.x,p3.y, 6, 0xffffffff, pcc); c.circle(p3.x,p3.y, 4, 0xff2010a0, pcc);
				//uint32_t* cnData = c.data();
				uint32_t i = 0; if (dot) c.plot(points[i].x, points[i].y, col);
				for (i=1; i<points.size(); ++i) {
					if (dot) c.plot(points[i].x,points[i].y, col, pcb);
					else c.line(points[i-1].x,points[i-1].y, points[i].x,points[i].y, col, pcb);
				}
				//c.line(points[0], points[points.size()-1], col);
				return true;
			}
			else {
				MessageWindow()
					.setParent(*c.window())
					.setTitle({"Bezier Warning!"})
					.setText({
					  "No points!"
					})
					.warning();
				return false;
			}
		}
		void printCurve() {
			if (points.size()>0) {
				print("Bezier {");
				for (auto &p : points) {
					print("\t",p.x, ",", p.y);
				}
				print("}");
			}
			else print("Bezier: empty curve!");
		}
	private:
		FPPosition p1, p2, p3, p4;
		double threshold;
		vector<FPPosition> points;
		int32_t addPoints(FPPosition z1, FPPosition z2, FPPosition z3, FPPosition z4, int32_t n=-1) {
			int32_t r = n;
			double d1 = dsq(z1,z2), d2 = dsq(z4,z3);
			// if (d1.width>threshold||d1.height>threshold||d2.width>threshold||d2.height>threshold) {
				FPPosition m12 = mid(z1,z2), m34 = mid(z3,z4), m23 = mid(z2,z3);
				FPPosition m123 = mid(m12,m23), m234 = mid(m23,m34);
				FPPosition q = mid(m123,m234), m1a = mid(z1,m12), mb4 = mid(m34,z4);
				// "left" side
				if (n==0||d1<threshold) points.append(FPPosition{z1.x,z1.y});
				else if (n!=0)//(d1.width>threshold||d1.height>threshold))
					r = addPoints(z1,m12,m123,q,n-1);
				// "right" side
				if ((n==0||d2<threshold)&&z4!=z1) points.append(FPPosition{z4.x,z4.y});
				else if (n!=0&&z4!=z1)//(d2.width>threshold||d2.height>threshold))
					r = addPoints(q,m234,m34,z4,n-1);
			// }
			return min(r, n);
		}
		void killDuplicates() {
			uint32_t s = points.size();
			if (s>0) {
				bool cant;
				vector<FPPosition> ptmp; ptmp.append(points[0]);
				uint32_t i; for (i=1; i<points.size(); ++i) {
					cant = dsq(points[i-1],points[i])<0.5;//&&(points[i].x==points[i-1].x&&points[i].y==points[i-1].y);
					if (!cant) ptmp.append(points[i]);
				}
				points = ptmp;
			}
		}
		double dsq(FPPosition z1, FPPosition z2) {
			double dx = z2.x-z1.x, dy = z2.y-z1.y;
			return dx*dx+dy*dy;
		}
		FPSize dist(FPPosition z1, FPPosition z2) {
			double w = (z2.x>z1.x?z2.x-z1.x:z1.x-z2.x),
				h = (z2.y>z1.y?z2.y-z1.y:z1.y-z2.y);
			return FPSize(w, h);
		}
		/*Size dist(Position z1, Position z2) {
			unsigned w = (unsigned)(z2.x>z1.x?z2.x-z1.x:z1.x-z2.x),
				h = (unsigned)(z2.y>z1.y?z2.y-z1.y:z1.y-z2.y);
			return Size({w, h});
		}*/
		FPPosition mid(FPPosition z1, FPPosition z2) {
			double x = (z1.x+(z2.x-z1.x)*0.5),
				y = (z1.y+(z2.y-z1.y)*0.5);
			return FPPosition(x, y);
		}
		/*FPPosition mid(Position z1, Position z2) {
			signed x = (signed)(z1.x+(z2.x-z1.x)*0.5),
				y = (signed)(z1.y+(z2.y-z1.y)*0.5);
			return FPPosition({x, y});
		}*/
};

#endif