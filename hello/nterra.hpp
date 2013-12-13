#ifndef __N_TERRA__
#define __N_TERRA__

#include <cmath>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#endif

/****
NTerra
3D transformation and plotting onto phoenix::Canvas
****/

namespace NTerra {

//// TYPES
struct matrix_t {
	double cells[16];	// hardcoded 4*4 for 3D blitting use
	matrix_t() {zero();}
	matrix_t(const matrix_t &m) {
		for (unsigned i = 0; i<16; ++i) cells[i] = m.cells[i];
		/*cells = {
			m.cells[0], m.cells[1], m.cells[2], m.cells[3],
			m.cells[4], m.cells[5], m.cells[6], m.cells[7],
			m.cells[8], m.cells[9], m.cells[10], m.cells[11],
			m.cells[12], m.cells[13], m.cells[14], m.cells[15]
		};*/
		//memcpy(&cells, &m.cells, sizeof(cells));
	}
	void zero() {
		/*cells = {
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0
		};*/
		for (unsigned i = 0; i<16; ++i) cells[i] = 0.0;
	}
	void identity() {
		zero();
		cells[0] = 1.0;
		cells[5] = 1.0;
		cells[10] = 1.0;
		cells[15] = 1.0;
		/*cells = {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0
		};*/
	}
	void transpose() {
		double m[16] = {
			cells[0], cells[1], cells[2], cells[3],
			cells[4], cells[5], cells[6], cells[7],
			cells[8], cells[9], cells[10],cells[11],
			cells[12],cells[13],cells[14],cells[15]
		};
		/*cells = {
			m[0],m[4],m[8],m[12],
			m[1],m[5],m[9],m[13],
			m[2],m[6],m[10],m[14],
			m[3],m[7],m[11],m[15]
		};*/
		cells[0] = m[0];
		cells[1] = m[4];
		cells[2] = m[8];
		cells[3] = m[12];
		cells[4] = m[1];
		cells[5] = m[5];
		cells[6] = m[9];
		cells[7] = m[13];
		cells[8] = m[2];
		cells[9] = m[6];
		cells[10]= m[10];
		cells[11]= m[14];
		cells[12]= m[3];
		cells[13]= m[7];
		cells[14]= m[11];
		cells[15]= m[15];
	}
	bool isIdentity() {
		if (cells[0]!=1.0||cells[5]!=1.0||cells[10]!=1.0||cells[15]!=1.0) return false;
		if (cells[1]!=0.0||cells[2]!=0.0||cells[3]!=0.0||
			cells[4]!=0.0||cells[6]!=0.0||cells[7]!=0.0||
			cells[8]!=0.0||cells[9]!=0.0||cells[11]!=0.0||
			cells[12]!=0.0||cells[13]!=0.0||cells[14]!=0.0
		) return false;
		return true;
	}
	double determinant() {
		double temp1 = (cells[10] * cells[15]) - (cells[11] * cells[14]);
		double temp2 = (cells[9] * cells[15]) - (cells[11] * cells[13]);
		double temp3 = (cells[9] * cells[14]) - (cells[10] * cells[13]);
		double temp4 = (cells[8] * cells[15]) - (cells[11] * cells[12]);
		double temp5 = (cells[8] * cells[14]) - (cells[10] * cells[12]);
		double temp6 = (cells[8] * cells[13]) - (cells[9] * cells[12]);
		return ((((cells[0] * (((cells[5] * temp1) - (cells[6] * temp2)) + (cells[7] * temp3))) - (cells[1] * (((cells[4] * temp1) -
			(cells[6] * temp4)) + (cells[7] * temp5)))) + (cells[2] * (((cells[4] * temp2) - (cells[5] * temp4)) + (cells[7] * temp6)))) -
			(cells[3] * (((cells[4] * temp3) - (cells[5] * temp5)) + (cells[6] * temp6))));
	}
	//// operators
	const matrix_t& operator=(const matrix_t &rhs) {
		if (this!=&rhs) {for (unsigned i = 0; i<16; ++i) cells[i] = rhs.cells[i];}
		return *this;
	}
	matrix_t operator*(const matrix_t &m) {
		matrix_t ret;
		Matrix::Multiply(ret.cells, cells, 4, 4, m.cells, 4, 4);
		/*
		ret.cells[0] = cells[0] * m.cells[0] + cells[1] * m.cells[4] + cells[2] * m.cells[8] + cells[3] * m.cells[12];
		ret.cells[1] = cells[0] * m.cells[1] + cells[1] * m.cells[5] + cells[2] * m.cells[9] + cells[3] * m.cells[13];
		ret.cells[2] = cells[0] * m.cells[2] + cells[1] * m.cells[6] + cells[2] * m.cells[10] + cells[3] * m.cells[14];
		ret.cells[3] = cells[0] * m.cells[3] + cells[1] * m.cells[7] + cells[2] * m.cells[11] + cells[3] * m.cells[15];
		ret.cells[4] = cells[4] * m.cells[0] + cells[5] * m.cells[4] + cells[6] * m.cells[8] + cells[7] * m.cells[12];
		ret.cells[5] = cells[4] * m.cells[1] + cells[5] * m.cells[5] + cells[6] * m.cells[9] + cells[7] * m.cells[13];
		ret.cells[6] = cells[4] * m.cells[2] + cells[5] * m.cells[6] + cells[6] * m.cells[10] + cells[7] * m.cells[14];
		ret.cells[7] = cells[4] * m.cells[3] + cells[5] * m.cells[7] + cells[6] * m.cells[11] + cells[7] * m.cells[15];
		ret.cells[8] = cells[8] * m.cells[0] + cells[9] * m.cells[4] + cells[10] * m.cells[8] + cells[11] * m.cells[12];
		ret.cells[9] = cells[8] * m.cells[1] + cells[9] * m.cells[5] + cells[10] * m.cells[9] + cells[11] * m.cells[13];
		ret.cells[10] = cells[8] * m.cells[2] + cells[9] * m.cells[6] + cells[10] * m.cells[10] + cells[11] * m.cells[14];
		ret.cells[11] = cells[8] * m.cells[3] + cells[9] * m.cells[7] + cells[10] * m.cells[11] + cells[11] * m.cells[15];
		ret.cells[12] = cells[12] * m.cells[0] + cells[13] * m.cells[4] + cells[14] * m.cells[8] + cells[15] * m.cells[12];
		ret.cells[13] = cells[12] * m.cells[1] + cells[13] * m.cells[5] + cells[14] * m.cells[9] + cells[15] * m.cells[13];
		ret.cells[14] = cells[12] * m.cells[2] + cells[13] * m.cells[6] + cells[14] * m.cells[10] + cells[15] * m.cells[14];
		ret.cells[15] = cells[12] * m.cells[3] + cells[13] * m.cells[7] + cells[14] * m.cells[11] + cells[15] * m.cells[15];
		*/
		return ret;
	}
};

struct vec2_t {
	double x;
	double y;
	vec2_t() : x(0.0), y(0.0) {}
	vec2_t(const double &vx, const double &vy) : x(vx), y(vy) {}
	vec2_t(const vec2_t &v) : x(v.x), y(v.y) {}
	inline vec2_t add(const vec2_t &v) {return vec2_t(x+v.x, y+v.y);}
	inline vec2_t scale(const double &d) {return vec2_t(d*x, d*y);}
	void zero() {x = 0.0; y = 0.0;}
	inline double length2() {return (x*x+y*y);}
	inline double length() {return sqrt(x*x+y*y);}
	void normalize() {
		double l = length(), n;
		if (l!=0) {
			n = 1.0/l;
			x *= n;
			y *= n;
		}
	}
	vec2_t transform(const matrix_t &m) {
		return vec2_t(
			((x*m.cells[0]) + (y*m.cells[4])),
			((x*m.cells[1]) + (y*m.cells[5]))
		);
	}
	//// operators
	const vec2_t& operator=(const vec2_t &rhs) {
		if (this!=&rhs) {x = rhs.x; y = rhs.y;}
		return *this;
	}
	vec2_t operator+() {return vec2_t(x, y);}
	vec2_t operator-() {return vec2_t(-x, -y);}
	vec2_t operator+(const vec2_t &v) {return add(v);}
	vec2_t operator-(const vec2_t &v) {return vec2_t(x-v.x,y-v.y);}
	vec2_t operator*(const double &d) {return scale(d);}
	bool operator==(const vec2_t &v) {return x==v.x&&y==v.y;}
	bool operator!=(const vec2_t &v) {return x!=v.x||y!=v.y;}
};

struct vec3_t : vec2_t {
	double z;
	vec3_t() : vec2_t(0.0,0.0), z(0.0) {}
	vec3_t(const double &vx, const double &vy, const double &vz) :
		vec2_t(vx, vy), z(vz) {}
	vec3_t(const vec3_t &v) : vec2_t(v.x,v.y), z(v.z) {}
	vec3_t(const vec2_t &v) : vec2_t(v), z(0.0) {}
	vec3_t add(const vec3_t &v) {return vec3_t(x+v.x, y+v.y, z+v.z);}
	vec3_t add(const vec2_t &v) {return vec3_t(x+v.x, y+v.y, z);}
	vec3_t scale(const double &d) {return vec3_t(d*x, d*y, d*z);}
	vec3_t mult(const vec3_t &v) {return vec3_t(x*v.x, y*v.y, z*v.z);}
	void zero() {x = 0.0; y = 0.0; z = 0.0;}
	inline double dot(const vec3_t &v) {return (x*v.x+y*v.y+z*v.z);}
	inline vec3_t cross(const vec3_t &v) {
		return vec3_t(
			y*v.z-z*v.y,
			z*v.x-x*v.z,
			x*v.y-y*v.x
		);
	}
	inline double length2() {return (x*x+y*y+z*z);}
	inline double length() {return sqrt(x*x+y*y+z*z);}
	void normalize() {
		double l = length(), n;
		if (l!=0) {
			n = 1.0/l;
			x *= n;
			y *= n;
			z *= n;
		}
	}
	vec3_t transform(const matrix_t &m) {
		double w = (x*m.cells[3]) + (y*m.cells[7]) + (z*m.cells[11]) + m.cells[15];
		return vec3_t(
			((x*m.cells[0]) + (y*m.cells[4]) + (z*m.cells[8]) + m.cells[12])/w,
			((x*m.cells[1]) + (y*m.cells[5]) + (z*m.cells[9]) + m.cells[13])/w,
			((x*m.cells[2]) + (y*m.cells[6]) + (z*m.cells[10]) + m.cells[14])/w
		);
	}
	//// operators
	const vec3_t& operator=(const vec3_t &rhs) {
		if (this!=&rhs) {x = rhs.x; y = rhs.y; z = rhs.z;}
		return *this;
	}
	vec3_t operator+() {return vec3_t(x, y, z);}
	vec3_t operator-() {return vec3_t(-x, -y, -z);}
	vec3_t operator+(const vec3_t &v) {return add(v);}
	vec3_t operator-(const vec3_t &v) {return vec3_t(x-v.x,y-v.y,z-v.z);}
	vec3_t operator*(const double &d) {return scale(d);}
	vec3_t operator*(const vec3_t &v) {return mult(v);}
	bool operator==(const vec3_t &v) {return x==v.x&&y==v.y&&z==v.z;}
	bool operator==(const vec2_t &v) {return x==v.x&&y==v.y&&z==0.0;}
};


namespace Transform {

vec2_t Project(const vec3_t &v, const matrix_t &m, unsigned w, unsigned h) {
	vec3_t p(v);
	p = p.transform(m);
	double x = (p.x*w+w/2.0);
	double y = (-p.y*h+h/2.0);
	return vec2_t(x,y);
}

matrix_t Scale(double x, double y, double z) {
	matrix_t ret;
	ret.cells[0] = x;
	ret.cells[5] = y;
	ret.cells[10] = z;
	ret.cells[15] = 1.0;
	return ret;
}
matrix_t Translate(double x, double y, double z) {
	matrix_t ret;
	ret.identity();
	ret.cells[12] = x;
	ret.cells[13] = y;
	ret.cells[14] = z;
	return ret;
}

namespace Rotation {
matrix_t X(double a) {
	matrix_t ret;
	double s = sin(a), c = cos(a);
	ret.cells[0] = 1.0;
	ret.cells[15] = 1.0;
	ret.cells[5] = c;
	ret.cells[10] = c;
	ret.cells[9] = -s;
	ret.cells[6] = s;
	return ret;
}
matrix_t Y(double a) {
	matrix_t ret;
	double s = sin(a), c = cos(a);
	ret.cells[5] = 1.0;
	ret.cells[15] = 1.0;
	ret.cells[0] = c;
	ret.cells[10] = c;
	ret.cells[2] = -s;
	ret.cells[8] = s;
	return ret;
}
matrix_t Z(double a) {
	matrix_t ret;
	double s = sin(a), c = cos(a);
	ret.cells[10] = 1.0;
	ret.cells[15] = 1.0;
	ret.cells[0] = c;
	ret.cells[5] = c;
	ret.cells[4] = -s;
	ret.cells[1] = s;
	return ret;
}
matrix_t About(vec3_t v, double a) {
	matrix_t ret;
	double s = sin(a), c = cos(a), c1 = 1-c;
	v.normalize();
	ret.cells[0] = v.x*v.x*c1+c;
	ret.cells[1] = v.x*v.y*c1-v.z*s;
	ret.cells[2] = v.x*v.z*c1+v.y*s;
	ret.cells[3] = 0.0;
	ret.cells[4] = v.y*v.x*c1+v.z+s;
	ret.cells[5] = v.y*v.y*c1+c;
	ret.cells[6] = v.y*v.z*c1-v.x*s;
	ret.cells[7] = 0.0;
	ret.cells[8] = v.z*v.x*c1-v.y*s;
	ret.cells[9] = v.z*v.y*c1+v.x*s;
	ret.cells[10] = v.z*v.z*c1+c;
	ret.cells[11] = 0.0;
	ret.cells[15] = 1.0;
	return ret;
}
inline matrix_t YawPitchRoll(double yaw, double pitch, double roll) {
	return Z(roll)*X(pitch)*Y(yaw);
}

}

namespace LH {

matrix_t lookAt(vec3_t eye, vec3_t target, vec3_t up) {
	matrix_t ret;
	vec3_t axisX, axisY, axisZ;
	axisZ = target-eye; axisZ.normalize();
	axisX = up.cross(axisZ); axisX.normalize();
	axisY = axisZ.cross(axisX); axisY.normalize();
	double ex = -axisX.dot(eye),
		ey = -axisY.dot(eye),
		ez = -axisZ.dot(eye);
	ret.cells[0] = axisX.x;
	ret.cells[1] = axisY.x;
	ret.cells[2] = axisZ.x;
	ret.cells[3] = 0;
	ret.cells[4] = axisX.y;
	ret.cells[5] = axisY.y;
	ret.cells[6] = axisZ.y;
	ret.cells[7] = 0;
	ret.cells[8] = axisX.z;
	ret.cells[9] = axisY.z;
	ret.cells[10]= axisZ.z;
	ret.cells[11]= 0;
	ret.cells[12]= ex;
	ret.cells[13]= ey;
	ret.cells[14]= ez;
	ret.cells[15]= 1;
	return ret;
}

matrix_t Perspective(double w, double h, double znear, double zfar) {
	matrix_t m;
	m.cells[0] = (2.0 * znear) / w;
	m.cells[1] = m.cells[2] = m.cells[3] = 0.0;
	m.cells[5] = (2.0 * znear) / h;
	m.cells[4] = m.cells[6] = m.cells[7] = 0.0;
	m.cells[10] = -zfar / (znear - zfar);
	m.cells[8] = m.cells[9] = 0.0;
	m.cells[11] = 1.0;
	m.cells[12] = m.cells[13] = m.cells[15] = 0.0;
	m.cells[14] = (znear * zfar) / (znear - zfar);
	return m;
}
matrix_t PerspectiveFOV(double fov, double aspect, double znear, double zfar) {
	matrix_t m;
	double t = 1.0 / (tan(fov * 0.5));
	m.cells[0] = t / aspect;
	m.cells[1] = m.cells[2] = m.cells[3] = 0.0;
	m.cells[5] = t;
	m.cells[4] = m.cells[6] = m.cells[7] = 0.0;
	m.cells[8] = m.cells[9] = 0.0;
	m.cells[10] = -zfar / (znear - zfar);
	m.cells[11] = 1.0;
	m.cells[12] = m.cells[13] = m.cells[15] = 0.0;
	m.cells[14] = (znear * zfar) / (znear - zfar);
	return m;
}

}

}

//// PROJECTION
struct camera_t {
	vec3_t position;
	vec3_t target;
};
struct mesh_t {
	string name;
	vector<vec3_t> vertices;
	vector<vector<unsigned> > faces;
	vec3_t position;
	vec3_t rotation;
};

}

#endif