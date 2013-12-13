#include <nall/nall.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include "nthings.hpp"
#include "nterra.hpp"

namespace NTerra {
struct device_t {
	camera_t camera;
	matrix_t projection;
	matrix_t view;
	/****
	TODO: set up back-buffer (copy of NCanvas.data())
	TODO: set up flip (copy back-buffer to front-buffer)
	TODO: rewrite render to copy front-buffer to NCanvas.data pointer)
	****/
	void flip() {}
	void setView() {
		view = Transform::LH::lookAt(camera.position, camera.target, vec3_t(0.0, 1.0, 0.0));
	}
	void setProjection(const NCanvas &c) {
		Size z = c.size();
		if (z.width>0&&z.height>0) projection = Transform::LH::PerspectiveFOV(0.78, z.width/z.height, 0.01, 1.0);
		else projection.identity();
	}
	void render(NCanvas &c, const mesh_t &m) {
		Size z = c.size();
		uint32_t col = 0x80ffffff, c2 = 0x408080ff, rad = 8;
		if (z.width>0&&z.height>0) {
			//matrix_t view = Transform::LH::lookAt(camera.position, camera.target, vec3_t(0.0, 1.0, 0.0));
			//matrix_t projection = Transform::LH::PerspectiveFOV(0.78, z.width/z.height, 0.01, 1.0);
			matrix_t world = Transform::Rotation::YawPitchRoll(m.rotation.y, m.rotation.x, m.rotation.z)*Transform::Translate(m.position.x, m.position.y, m.position.z);
			matrix_t xform = world*view*projection;
			for (auto &f : m.faces) {
				vec2_t p1 = Transform::Project(m.vertices[f[0]], xform, z.width, z.height);
				vec2_t p2 = Transform::Project(m.vertices[f[1]], xform, z.width, z.height);
				vec2_t p3 = Transform::Project(m.vertices[f[2]], xform, z.width, z.height);
				if (abs(p1.x-p2.x)>1.0||abs(p1.y-p2.y)>1.0) c.line(p1.x, p1.y, p2.x, p2.y, col);
				if (abs(p2.x-p3.x)>1.0||abs(p2.y-p3.y)>1.0) c.line(p2.x, p2.y, p3.x, p3.y, col);
				if (abs(p3.x-p1.x)>1.0||abs(p3.y-p1.y)>1.0) c.line(p3.x, p3.y, p1.x, p1.y, col);
			}
			for (auto &v : m.vertices) {
				vec2_t p = Transform::Project(v, xform, z.width, z.height);
				c.circle((unsigned)p.x, (unsigned)p.y, rad, c2);
			}
		}
	}
};
}

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
	//FileEntry feFile;
	Label lblText, lblT2, lblT3;
	Button bImg, bImgText, bText;
	NCanvas cnSmp;
	unsigned clicks;
	/** aux **/
	Spritesheet sprTabs;
	BrowserWindow browser;	/* file browser for "Open File" functionality */
	NTerra::device_t dev;
	NTerra::mesh_t mesh;
	Curve bzc;
	Timer tUpdater;
	/** proc **/
	void openFile(const string &fn);
	void init();
	void initProc();
	void reflowStatic();
	void reflow();
	void resize();
	/**** more proc ****/
	void repaint();
	/** constructors **/
	Win();
} *win = nullptr;
