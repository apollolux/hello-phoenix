#if defined(PHOENIX_WINDOWS)
  #include "../windows/header.hpp"
#elif defined(PHOENIX_QT)
  #include "../qt/header.hpp"
#elif defined(PHOENIX_GTK)
  #include "../gtk/header.hpp"
#elif defined(PHOENIX_COCOA)
  #include "../cocoa/header.hpp"
#elif defined(PHOENIX_REFERENCE)
  #include "../reference/header.hpp"
#endif

#include "core.hpp"
using namespace nall;

namespace phoenix {
  #include "state.hpp"
  #include "layout/fixed-layout.cpp"
  #include "layout/horizontal-layout.cpp"
  #include "layout/vertical-layout.cpp"
}

#if defined(PHOENIX_WINDOWS)
  #include "../windows/platform.cpp"
#elif defined(PHOENIX_QT)
  #include "../qt/platform.cpp"
#elif defined(PHOENIX_GTK)
  #include "../gtk/platform.cpp"
#elif defined(PHOENIX_COCOA)
  #include "../cocoa/platform.cpp"
#elif defined(PHOENIX_REFERENCE)
  #include "../reference/platform.cpp"
#endif

namespace phoenix {

//Application
//===========

function<void ()> Application::main;

function<void ()> Application::Windows::onModalBegin;
function<void ()> Application::Windows::onModalEnd;

function<void ()> Application::Cocoa::onAbout;
function<void ()> Application::Cocoa::onActivate;
function<void ()> Application::Cocoa::onPreferences;
function<void ()> Application::Cocoa::onQuit;

void Application::run() {
  return pApplication::run();
}

bool Application::pendingEvents() {
  return pApplication::pendingEvents();
}

void Application::processEvents() {
  return pApplication::processEvents();
}

void Application::quit() {
  applicationState.quit = true;
  return pApplication::quit();
}

void Application::setName(const string& name) {
  applicationState.name = name;
}

void Application::initialize() {
  static bool initialized = false;
  if(initialized == false) {
    initialized = true;
    return pApplication::initialize();
  }
}

//Color
//=====

uint32_t Color::rgb() const {
  return (255 << 24) + (red << 16) + (green << 8) + (blue << 0);
}

uint32_t Color::rgba() const {
  return (alpha << 24) + (red << 16) + (green << 8) + (blue << 0);
}

//Geometry
//========

Position Geometry::position() const {
  return {x, y};
}

Size Geometry::size() const {
  return {width, height};
}

string Geometry::text() const {
  return {x, ",", y, ",", width, ",", height};
}

Geometry::Geometry(const string& text) {
  lstring part = text.split(",");
  x = integer(part(0, "256"));
  y = integer(part(1, "256"));
  width = decimal(part(2, "256"));
  height = decimal(part(3, "256"));
}

//Font
//====

string Font::serif(unsigned size, const string& style) {
  return pFont::serif(size, style);
}

string Font::sans(unsigned size, const string& style) {
  return pFont::sans(size, style);
}

string Font::monospace(unsigned size, const string& style) {
  return pFont::monospace(size, style);
}

Size Font::size(const string& font, const string& text) {
  return pFont::size(font, text);
}

//Desktop
//=======

Size Desktop::size() {
  return pDesktop::size();
}

Geometry Desktop::workspace() {
  return pDesktop::workspace();
}

//Keyboard
//========

bool Keyboard::pressed(Keyboard::Scancode scancode) {
  return pKeyboard::pressed(scancode);
}

bool Keyboard::released(Keyboard::Scancode scancode) {
  return !pressed(scancode);
}

vector<bool> Keyboard::state() {
  return pKeyboard::state();
}

//Mouse
//=====

Position Mouse::position() {
  return pMouse::position();
}

bool Mouse::pressed(Mouse::Button button) {
  return pMouse::pressed(button);
}

bool Mouse::released(Mouse::Button button) {
  return !pressed(button);
}

//BrowserWindow
//=============

string BrowserWindow::directory() {
  return pBrowserWindow::directory(state);
}

string BrowserWindow::open() {
  return pBrowserWindow::open(state);
}

string BrowserWindow::save() {
  return pBrowserWindow::save(state);
}

BrowserWindow& BrowserWindow::setFilters(const lstring& filters) {
  state.filters = filters;
  return *this;
}

BrowserWindow& BrowserWindow::setParent(Window& parent) {
  state.parent = &parent;
  return *this;
}

BrowserWindow& BrowserWindow::setPath(const string& path) {
  state.path = path;
  return *this;
}

BrowserWindow& BrowserWindow::setTitle(const string& title) {
  state.title = title;
  return *this;
}

BrowserWindow::BrowserWindow():
state(*new State) {
}

BrowserWindow::~BrowserWindow() {
  delete &state;
}

//MessageWindow
//=============

MessageWindow::Response MessageWindow::error(MessageWindow::Buttons buttons) {
  state.buttons = buttons;
  return pMessageWindow::error(state);
}

MessageWindow::Response MessageWindow::information(MessageWindow::Buttons buttons) {
  state.buttons = buttons;
  return pMessageWindow::information(state);
}

MessageWindow::Response MessageWindow::question(MessageWindow::Buttons buttons) {
  state.buttons = buttons;
  return pMessageWindow::question(state);
}

MessageWindow& MessageWindow::setParent(Window& parent) {
  state.parent = &parent;
  return *this;
}

MessageWindow& MessageWindow::setText(const string& text) {
  state.text = text;
  return *this;
}

MessageWindow& MessageWindow::setTitle(const string& title) {
  state.title = title;
  return *this;
}

MessageWindow::Response MessageWindow::warning(MessageWindow::Buttons buttons) {
  state.buttons = buttons;
  return pMessageWindow::warning(state);
}

MessageWindow::MessageWindow(const string& text):
state(*new State) {
  state.text = text;
}

MessageWindow::~MessageWindow() {
  delete &state;
}

//Object
//======

Object::Object(pObject& p):
p(p) {
  Application::initialize();
  p.constructor();
}

Object::~Object() {
  p.destructor();
  delete &p;
}

//Timer
//=====

bool Timer::enabled() const {
  return state.enabled;
}

unsigned Timer::interval() const {
  return state.interval;
}

void Timer::setEnabled(bool enabled) {
  state.enabled = enabled;
  return p.setEnabled(enabled);
}

void Timer::setInterval(unsigned interval) {
  state.interval = interval;
  return p.setInterval(interval);
}

Timer::Timer():
state(*new State),
base_from_member<pTimer&>(*new pTimer(*this)),
Object(base_from_member<pTimer&>::value),
p(base_from_member<pTimer&>::value) {
  p.constructor();
}

Timer::~Timer() {
  p.destructor();
  delete &state;
}

//Window
//======

void Window::append(Layout& layout) {
  if(state.layout.append(layout)) {
    ((Sizable&)layout).state.window = this;
    ((Sizable&)layout).state.layout = nullptr;
    p.append(layout);
    layout.synchronizeLayout();
  }
}

void Window::append(Menu& menu) {
  if(state.menu.append(menu)) {
    ((Action&)menu).state.window = this;
    p.append(menu);
  }
}

void Window::append(Widget& widget) {
  if(state.widget.append(widget)) {
    ((Sizable&)widget).state.window = this;
    p.append(widget);
    widget.synchronizeLayout();
  }
}

Color Window::backgroundColor() {
  return p.backgroundColor();
}

bool Window::droppable() const {
  return state.droppable;
}

Geometry Window::frameGeometry() {
  Geometry geometry = p.geometry();
  Geometry margin = p.frameMargin();
  return {
    geometry.x - margin.x, geometry.y - margin.y,
    geometry.width + margin.width, geometry.height + margin.height
  };
}

Geometry Window::frameMargin() {
  return p.frameMargin();
}

bool Window::focused() {
  return p.focused();
}

bool Window::fullScreen() const {
  return state.fullScreen;
}

Geometry Window::geometry() {
  return p.geometry();
}

string Window::menuFont() const {
  return state.menuFont;
}

bool Window::menuVisible() const {
  return state.menuVisible;
}

bool Window::modal() const {
  return state.modal;
}

void Window::remove(Layout& layout) {
  if(state.layout.remove(layout)) {
    p.remove(layout);
    ((Sizable&)layout).state.window = nullptr;
  }
}

void Window::remove(Menu& menu) {
  if(state.menu.remove(menu)) {
    p.remove(menu);
    ((Action&)menu).state.window = nullptr;
  }
}

void Window::remove(Widget& widget) {
  if(state.widget.remove(widget)) {
    p.remove(widget);
    ((Sizable&)widget).state.window = nullptr;
  }
}

bool Window::resizable() const {
  return state.resizable;
}

void Window::setBackgroundColor(Color color) {
  state.backgroundColorOverride = true;
  state.backgroundColor = color;
  return p.setBackgroundColor(color);
}

void Window::setDroppable(bool droppable) {
  state.droppable = droppable;
  return p.setDroppable(droppable);
}

void Window::setFrameGeometry(Geometry geometry) {
  Geometry margin = p.frameMargin();
  return setGeometry({
    geometry.x + margin.x, geometry.y + margin.y,
    geometry.width - margin.width, geometry.height - margin.height
  });
}

void Window::setFocused() {
  return p.setFocused();
}

void Window::setFullScreen(bool fullScreen) {
  state.fullScreen = fullScreen;
  return p.setFullScreen(fullScreen);
}

void Window::setGeometry(Geometry geometry) {
  state.geometry = geometry;
  return p.setGeometry(geometry);
}

void Window::setMenuFont(const string& font) {
  state.menuFont = font;
  return p.setMenuFont(font);
}

void Window::setMenuVisible(bool visible) {
  state.menuVisible = visible;
  return p.setMenuVisible(visible);
}

void Window::setModal(bool modal) {
  state.modal = modal;
  return p.setModal(modal);
}

void Window::setResizable(bool resizable) {
  state.resizable = resizable;
  return p.setResizable(resizable);
}

void Window::setStatusFont(const string& font) {
  state.statusFont = font;
  return p.setStatusFont(font);
}

void Window::setStatusText(const string& text) {
  state.statusText = text;
  return p.setStatusText(text);
}

void Window::setStatusVisible(bool visible) {
  state.statusVisible = visible;
  return p.setStatusVisible(visible);
}

void Window::setTitle(const string& text) {
  state.title = text;
  return p.setTitle(text);
}

void Window::setVisible(bool visible) {
  state.visible = visible;
  synchronizeLayout();
  return p.setVisible(visible);
}

void Window::setWidgetFont(const string& font) {
  state.widgetFont = font;
  return p.setWidgetFont(font);
}

void Window::setWindowGeometry(Geometry geometry) {
  Geometry margin = p.frameMargin();
  return setGeometry({
    geometry.x + margin.x, geometry.y + margin.y,
    geometry.width, geometry.height
  });
}

string Window::statusFont() const {
  return state.statusFont;
}

string Window::statusText() const {
  return state.statusText;
}

bool Window::statusVisible() const {
  return state.statusVisible;
}

void Window::synchronizeLayout() {
  if(visible() && applicationState.quit == false) setGeometry(geometry());
}

string Window::title() const {
  return state.title;
}

bool Window::visible() const {
  return state.visible;
}

string Window::widgetFont() const {
  return state.widgetFont;
}

Window::Window():
state(*new State),
base_from_member<pWindow&>(*new pWindow(*this)),
Object(base_from_member<pWindow&>::value),
p(base_from_member<pWindow&>::value) {
  p.constructor();
}

Window::~Window() {
  p.destructor();
  delete &state;
}

//Action
//======

bool Action::enabled() const {
  return state.enabled;
}

void Action::setEnabled(bool enabled) {
  state.enabled = enabled;
  return p.setEnabled(enabled);
}

void Action::setVisible(bool visible) {
  state.visible = visible;
  return p.setVisible(visible);
}

bool Action::visible() const {
  return state.visible;
}

Action::Action(pAction& p):
state(*new State),
Object(p),
p(p) {
  p.constructor();
}

Action::~Action() {
  p.destructor();
  delete &state;
}

//Menu
//====

void Menu::append(const group<Action>& list) {
  for(auto& action : list) {
    if(state.action.append(action)) {
      action.state.menu = this;
      p.append(action);
    }
  }
}

image Menu::image() const {
  return state.image;
}

void Menu::remove(const group<Action>& list) {
  for(auto& action : list) {
    if(state.action.remove(action)) {
      action.state.menu = nullptr;
      return p.remove(action);
    }
  }
}

void Menu::setImage(const nall::image& image) {
  state.image = image;
  return p.setImage(image);
}

void Menu::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string Menu::text() const {
  return state.text;
}

Menu::Menu():
state(*new State),
base_from_member<pMenu&>(*new pMenu(*this)),
Action(base_from_member<pMenu&>::value),
p(base_from_member<pMenu&>::value) {
  p.constructor();
}

Menu::~Menu() {
  p.destructor();
  delete &state;
}

//Separator
//=========

Separator::Separator():
base_from_member<pSeparator&>(*new pSeparator(*this)),
Action(base_from_member<pSeparator&>::value),
p(base_from_member<pSeparator&>::value) {
  p.constructor();
}

Separator::~Separator() {
  p.destructor();
}

//Item
//====

image Item::image() const {
  return state.image;
}

void Item::setImage(const nall::image& image) {
  state.image = image;
  return p.setImage(image);
}

void Item::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string Item::text() const {
  return state.text;
}

Item::Item():
state(*new State),
base_from_member<pItem&>(*new pItem(*this)),
Action(base_from_member<pItem&>::value),
p(base_from_member<pItem&>::value) {
  p.constructor();
}

Item::~Item() {
  p.destructor();
  delete &state;
}

//CheckItem
//=========

bool CheckItem::checked() const {
  return state.checked;
}

void CheckItem::setChecked(bool checked) {
  state.checked = checked;
  return p.setChecked(checked);
}

void CheckItem::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string CheckItem::text() const {
  return state.text;
}

CheckItem::CheckItem():
state(*new State),
base_from_member<pCheckItem&>(*new pCheckItem(*this)),
Action(base_from_member<pCheckItem&>::value),
p(base_from_member<pCheckItem&>::value) {
  p.constructor();
}

CheckItem::~CheckItem() {
  p.destructor();
  delete &state;
}

//RadioItem
//=========

void RadioItem::group(const nall::group<RadioItem>& list) {
  for(auto& item : list) item.p.setGroup(item.state.group = list);
  if(list.size()) list.first().setChecked();
}

bool RadioItem::checked() const {
  return state.checked;
}

void RadioItem::setChecked() {
  for(auto& item : state.group) item.state.checked = false;
  state.checked = true;
  return p.setChecked();
}

void RadioItem::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string RadioItem::text() const {
  return state.text;
}

RadioItem::RadioItem():
state(*new State),
base_from_member<pRadioItem&>(*new pRadioItem(*this)),
Action(base_from_member<pRadioItem&>::value),
p(base_from_member<pRadioItem&>::value) {
  p.constructor();
}

RadioItem::~RadioItem() {
  for(auto& item : state.group) {
    if(&item != this) item.state.group.remove(*this);
  }
  p.destructor();
  delete &state;
}

//Sizable
//=======

Layout* Sizable::layout() const {
  return state.layout;
}

Window* Sizable::window() const {
  if(state.layout) return state.layout->window();
  return state.window;
}

Sizable::Sizable(pSizable& p):
state(*new State),
Object(p),
p(p) {
  p.constructor();
}

Sizable::~Sizable() {
  if(layout()) layout()->remove(*this);
  p.destructor();
  delete &state;
}

//Layout
//======

void Layout::append(Sizable& sizable) {
  sizable.state.layout = this;
  sizable.state.window = nullptr;

  if(dynamic_cast<Layout*>(&sizable)) {
    Layout& layout = (Layout&)sizable;
    layout.synchronizeLayout();
  }

  if(dynamic_cast<Widget*>(&sizable)) {
    Widget& widget = (Widget&)sizable;
    if(sizable.window()) sizable.window()->append(widget);
  }
}

void Layout::remove(Sizable& sizable) {
  if(dynamic_cast<Widget*>(&sizable)) {
    Widget& widget = (Widget&)sizable;
    if(sizable.window()) sizable.window()->remove(widget);
  }

  sizable.state.layout = nullptr;
  sizable.state.window = nullptr;
}

void Layout::reset() {
}

Layout::Layout():
state(*new State),
base_from_member<pLayout&>(*new pLayout(*this)),
Sizable(base_from_member<pLayout&>::value),
p(base_from_member<pLayout&>::value) {
}

Layout::Layout(pLayout& p):
state(*new State),
base_from_member<pLayout&>(p),
Sizable(p),
p(p) {
}

Layout::~Layout() {
  if(layout()) layout()->remove(*this);
  else if(window()) window()->remove(*this);
  p.destructor();
  delete &state;
}

//Widget
//======

bool Widget::enabled() const {
  return state.enabled;
}

bool Widget::focused() {
  return p.focused();
}

string Widget::font() const {
  return state.font;
}

Geometry Widget::geometry() const {
  return state.geometry;
}

Size Widget::minimumSize() {
  return p.minimumSize();
}

void Widget::setEnabled(bool enabled) {
  state.enabled = enabled;
  return p.setEnabled(enabled);
}

void Widget::setFocused() {
  return p.setFocused();
}

void Widget::setFont(const string& font) {
  state.font = font;
  return p.setFont(font);
}

void Widget::setGeometry(Geometry geometry) {
  state.geometry = geometry;
  return p.setGeometry(geometry);
}

void Widget::setVisible(bool visible) {
  state.visible = visible;
  return p.setVisible(visible);
}

void Widget::synchronizeLayout() {
}

bool Widget::visible() const {
  return state.visible;
}

Widget::Widget():
state(*new State),
base_from_member<pWidget&>(*new pWidget(*this)),
Sizable(base_from_member<pWidget&>::value),
p(base_from_member<pWidget&>::value) {
  state.abstract = true;
  p.constructor();
}

Widget::Widget(pWidget& p):
state(*new State),
base_from_member<pWidget&>(p),
Sizable(base_from_member<pWidget&>::value),
p(base_from_member<pWidget&>::value) {
  p.constructor();
}

Widget::~Widget() {
  p.destructor();
  delete &state;
}

//Button
//======

image Button::image() const {
  return state.image;
}

Orientation Button::orientation() const {
  return state.orientation;
}

void Button::setImage(const nall::image& image, Orientation orientation) {
  state.image = image;
  state.orientation = orientation;
  return p.setImage(image, orientation);
}

void Button::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

nall::string Button::text() const {
  return state.text;
}

Button::Button():
state(*new State),
base_from_member<pButton&>(*new pButton(*this)),
Widget(base_from_member<pButton&>::value),
p(base_from_member<pButton&>::value) {
  p.constructor();
}

Button::~Button() {
  p.destructor();
  delete &state;
}

//Canvas
//======

const uint32_t* Canvas::data() const {
  return state.data;
}

uint32_t* Canvas::data() {
  return state.data;
}

void Canvas::fill(Color a) {
  uint32_t* buffer = data();
  for(unsigned y = 0; y < state.height; y++) {
    for(unsigned x = 0; x < state.width; x++) {
      *buffer++ = a.rgb();
    }
  }
  return update();
}

void Canvas::fill(Color a, Color b, Color c, Color d) {
  auto interpolate = [](uint8_t a, uint8_t b, uint8_t c, uint8_t d, double t, double s) {
    return (uint8_t)(a * (1.0 - t) * (1.0 - s) + b * t * (1.0 - s) + c * (1.0 - t) * s + d * t * s);
  };

  uint32_t* buffer = data();
  for(unsigned y = 0; y < state.height; y++) {
    double s = (double)y / (double)(state.height - 1);
    for(unsigned x = 0; x < state.width; x++) {
      double t = (double)x / (double)(state.width - 1);
      uint8_t alpha = interpolate(a.alpha, b.alpha, c.alpha, d.alpha, t, s);
      uint8_t red = interpolate(a.red, b.red, c.red, d.red, t, s);
      uint8_t green = interpolate(a.green, b.green, c.green, d.green, t, s);
      uint8_t blue = interpolate(a.blue, b.blue, c.blue, d.blue, t, s);
      *buffer++ = (alpha << 24) | (red << 16) | (green << 8) | (blue << 0);
    }
  }
  return update();
}

void Canvas::setDroppable(bool droppable) {
  state.droppable = droppable;
  return p.setDroppable(droppable);
}

bool Canvas::setImage(const nall::image& image) {
  if(image.data == nullptr || image.width == 0 || image.height == 0) return false;
  state.width = image.width;
  state.height = image.height;
  setSize({state.width, state.height});
  memcpy(state.data, image.data, state.width * state.height * sizeof(uint32_t));
  update();
  return true;
}

void Canvas::setSize(Size size) {
  state.width = size.width;
  state.height = size.height;
  delete[] state.data;
  state.data = new uint32_t[size.width * size.height];
  return p.setSize(size);
}

Size Canvas::size() const {
  return {state.width, state.height};
}

void Canvas::update() {
  return p.update();
}

Canvas::Canvas():
state(*new State),
base_from_member<pCanvas&>(*new pCanvas(*this)),
Widget(base_from_member<pCanvas&>::value),
p(base_from_member<pCanvas&>::value) {
  state.data = new uint32_t[state.width * state.height];
  p.constructor();
}

Canvas::~Canvas() {
  p.destructor();
  delete[] state.data;
  delete &state;
}

//CheckButton
//===========

bool CheckButton::checked() const {
  return state.checked;
}

image CheckButton::image() const {
  return state.image;
}

void CheckButton::setChecked(bool checked) {
  state.checked = checked;
  return p.setChecked(checked);
}

void CheckButton::setImage(const nall::image& image, Orientation orientation) {
  state.image = image;
  state.orientation = orientation;
  return p.setImage(image, orientation);
}

void CheckButton::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string CheckButton::text() const {
  return state.text;
}

CheckButton::CheckButton():
state(*new State),
base_from_member<pCheckButton&>(*new pCheckButton(*this)),
Widget(base_from_member<pCheckButton&>::value),
p(base_from_member<pCheckButton&>::value) {
  p.constructor();
}

CheckButton::~CheckButton() {
  p.destructor();
  delete &state;
}

//CheckLabel
//==========

bool CheckLabel::checked() const {
  return state.checked;
}

void CheckLabel::setChecked(bool checked) {
  state.checked = checked;
  return p.setChecked(checked);
}

void CheckLabel::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string CheckLabel::text() const {
  return state.text;
}

CheckLabel::CheckLabel():
state(*new State),
base_from_member<pCheckLabel&>(*new pCheckLabel(*this)),
Widget(base_from_member<pCheckLabel&>::value),
p(base_from_member<pCheckLabel&>::value) {
  p.constructor();
}

CheckLabel::~CheckLabel() {
  p.destructor();
  delete &state;
}


//ComboButton
//===========

void ComboButton::append(const string& text) {
  state.text.append(text);
  return p.append(text);
}

void ComboButton::remove(unsigned selection) {
  if(selection >= state.text.size()) return;
  state.text.remove(selection);
  p.remove(selection);
}

void ComboButton::reset() {
  state.selection = 0;
  state.text.reset();
  return p.reset();
}

unsigned ComboButton::rows() const {
  return state.text.size();
}

unsigned ComboButton::selection() const {
  return state.selection;
}

void ComboButton::setSelection(unsigned selection) {
  if(selection >= state.text.size()) return;
  state.selection = selection;
  return p.setSelection(selection);
}

void ComboButton::setText(unsigned selection, const string& text) {
  if(selection >= state.text.size()) return;
  state.text[selection] = text;
  p.setText(selection, text);
}

string ComboButton::text() const {
  if(state.text.empty()) return "";
  return state.text[state.selection];
}

string ComboButton::text(unsigned selection) const {
  if(selection >= state.text.size()) return "";
  return state.text[selection];
}

ComboButton::ComboButton():
state(*new State),
base_from_member<pComboButton&>(*new pComboButton(*this)),
Widget(base_from_member<pComboButton&>::value),
p(base_from_member<pComboButton&>::value) {
  p.constructor();
}

ComboButton::~ComboButton() {
  p.destructor();
  delete &state;
}

//Console
//=======

void Console::print_(const string& text) {
  return p.print(text);
}

void Console::reset() {
  return p.reset();
}

Console::Console():
state(*new State),
base_from_member<pConsole&>(*new pConsole(*this)),
Widget(base_from_member<pConsole&>::value),
p(base_from_member<pConsole&>::value) {
  p.constructor();
}

Console::~Console() {
  p.destructor();
  delete &state;
}

//Frame
//=====

void Frame::setLayout(Layout& layout) {
  state.layout = &layout;
  synchronizeLayout();
}

void Frame::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

void Frame::synchronizeLayout() {
  if(state.layout == nullptr) return;
  ((Sizable*)state.layout)->state.window = ((Sizable*)this)->state.window;
  state.layout->synchronizeLayout();
}

string Frame::text() const {
  return state.text;
}

Frame::Frame():
state(*new State),
base_from_member<pFrame&>(*new pFrame(*this)),
Widget(base_from_member<pFrame&>::value),
p(base_from_member<pFrame&>::value) {
  p.constructor();
}

Frame::~Frame() {
  p.destructor();
  delete &state;
}

//HexEdit
//=======

unsigned HexEdit::columns() const {
  return state.columns;
}

unsigned HexEdit::length() const {
  return state.length;
}

unsigned HexEdit::offset() const {
  return state.offset;
}

unsigned HexEdit::rows() const {
  return state.rows;
}

void HexEdit::setColumns(unsigned columns) {
  state.columns = columns;
  return p.setColumns(columns);
}

void HexEdit::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void HexEdit::setOffset(unsigned offset) {
  state.offset = offset;
  return p.setOffset(offset);
}

void HexEdit::setRows(unsigned rows) {
  state.rows = rows;
  return p.setRows(rows);
}

void HexEdit::update() {
  return p.update();
}

HexEdit::HexEdit():
state(*new State),
base_from_member<pHexEdit&>(*new pHexEdit(*this)),
Widget(base_from_member<pHexEdit&>::value),
p(base_from_member<pHexEdit&>::value) {
  p.constructor();
}

HexEdit::~HexEdit() {
  p.destructor();
  delete &state;
}

//HorizontalScroller
//==================

unsigned HorizontalScroller::length() const {
  return state.length;
}

unsigned HorizontalScroller::position() const {
  return state.position;
}

void HorizontalScroller::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void HorizontalScroller::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

HorizontalScroller::HorizontalScroller():
state(*new State),
base_from_member<pHorizontalScroller&>(*new pHorizontalScroller(*this)),
Widget(base_from_member<pHorizontalScroller&>::value),
p(base_from_member<pHorizontalScroller&>::value) {
  p.constructor();
}

HorizontalScroller::~HorizontalScroller() {
  p.destructor();
  delete &state;
}

//HorizontalSlider
//================

unsigned HorizontalSlider::length() const {
  return state.length;
}

unsigned HorizontalSlider::position() const {
  return state.position;
}

void HorizontalSlider::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void HorizontalSlider::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

HorizontalSlider::HorizontalSlider():
state(*new State),
base_from_member<pHorizontalSlider&>(*new pHorizontalSlider(*this)),
Widget(base_from_member<pHorizontalSlider&>::value),
p(base_from_member<pHorizontalSlider&>::value) {
  p.constructor();
}

HorizontalSlider::~HorizontalSlider() {
  p.destructor();
  delete &state;
}

//Label
//=====

void Label::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string Label::text() const {
  return state.text;
}

Label::Label():
state(*new State),
base_from_member<pLabel&>(*new pLabel(*this)),
Widget(base_from_member<pLabel&>::value),
p(base_from_member<pLabel&>::value) {
  p.constructor();
}

Label::~Label() {
  p.destructor();
  delete &state;
}

//LineEdit
//========

bool LineEdit::editable() const {
  return state.editable;
}

void LineEdit::setEditable(bool editable) {
  state.editable = editable;
  return p.setEditable(editable);
}

void LineEdit::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string LineEdit::text() {
  return p.text();
}

LineEdit::LineEdit():
state(*new State),
base_from_member<pLineEdit&>(*new pLineEdit(*this)),
Widget(base_from_member<pLineEdit&>::value),
p(base_from_member<pLineEdit&>::value) {
  p.constructor();
}

LineEdit::~LineEdit() {
  p.destructor();
  delete &state;
}

//ListView
//========

void ListView::append(const lstring& text) {
  state.checked.append(false);
  state.image.append({});
  state.text.append(text);
  return p.append(text);
}

void ListView::autoSizeColumns() {
  return p.autoSizeColumns();
}

bool ListView::checkable() const {
  return state.checkable;
}

bool ListView::checked(unsigned selection) const {
  if(selection >= state.text.size()) return false;
  return state.checked[selection];
}

unsigned ListView::columns() const {
  return max(1u, state.headerText.size());
}

bool ListView::headerVisible() const {
  return state.headerVisible;
}

image ListView::image(unsigned selection, unsigned position) const {
  if(selection >= state.text.size()) return {};
  return state.image[selection](position);
}

void ListView::remove(unsigned selection) {
  if(selection >= state.text.size()) return;
  state.checked.remove(selection);
  state.image.remove(selection);
  state.text.remove(selection);
  return p.remove(selection);
}

void ListView::reset() {
  state.checked.reset();
  state.image.reset();
  state.text.reset();
  return p.reset();
}

unsigned ListView::rows() const {
  return state.text.size();
}

bool ListView::selected() const {
  return state.selected;
}

unsigned ListView::selection() const {
  return state.selection;
}

void ListView::setCheckable(bool checkable) {
  state.checkable = checkable;
  return p.setCheckable(checkable);
}

void ListView::setChecked(unsigned selection, bool checked) {
  if(selection >= state.text.size()) return;
  state.checked[selection] = checked;
  return p.setChecked(selection, checked);
}

void ListView::setHeaderText(const lstring& text) {
  state.headerText = text;
  return p.setHeaderText(text);
}

void ListView::setHeaderVisible(bool visible) {
  state.headerVisible = visible;
  return p.setHeaderVisible(visible);
}

void ListView::setImage(unsigned selection, unsigned position, const nall::image& image) {
  if(selection >= state.text.size()) return;
  state.image[selection](position) = image;
  return p.setImage(selection, position, image);
}

void ListView::setSelected(bool selected) {
  state.selected = selected;
  return p.setSelected(selected);
}

void ListView::setSelection(unsigned selection) {
  if(selection >= state.text.size()) return;
  state.selected = true;
  state.selection = selection;
  return p.setSelection(selection);
}

void ListView::setText(unsigned selection, const lstring& text) {
  if(selection >= state.text.size()) return;
  for(unsigned position = 0; position < text.size(); position++) {
    setText(selection, position, text[position]);
  }
}

void ListView::setText(unsigned selection, unsigned position, const string& text) {
  if(selection >= state.text.size()) return;
  state.text[selection](position) = text;
  return p.setText(selection, position, text);
}

string ListView::text(unsigned selection, unsigned position) const {
  if(selection >= state.text.size()) return "";
  return state.text[selection](position);
}

ListView::ListView():
state(*new State),
base_from_member<pListView&>(*new pListView(*this)),
Widget(base_from_member<pListView&>::value),
p(base_from_member<pListView&>::value) {
  p.constructor();
}

ListView::~ListView() {
  p.destructor();
  delete &state;
}

//ProgressBar
//===========

unsigned ProgressBar::position() const {
  return state.position;
}

void ProgressBar::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

ProgressBar::ProgressBar():
state(*new State),
base_from_member<pProgressBar&>(*new pProgressBar(*this)),
Widget(base_from_member<pProgressBar&>::value),
p(base_from_member<pProgressBar&>::value) {
  p.constructor();
}

ProgressBar::~ProgressBar() {
  p.destructor();
  delete &state;
}

//RadioButton
//===========

void RadioButton::group(const nall::group<RadioButton>& list) {
  for(auto& item : list) item.p.setGroup(item.state.group = list);
  if(list.size()) list.first().setChecked();
}

bool RadioButton::checked() const {
  return state.checked;
}

image RadioButton::image() const {
  return state.image;
}

void RadioButton::setChecked() {
  for(auto& item : state.group) item.state.checked = false;
  state.checked = true;
  return p.setChecked();
}

void RadioButton::setImage(const nall::image& image, Orientation orientation) {
  state.image = image;
  state.orientation = orientation;
  return p.setImage(image, orientation);
}

void RadioButton::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string RadioButton::text() const {
  return state.text;
}

RadioButton::RadioButton():
state(*new State),
base_from_member<pRadioButton&>(*new pRadioButton(*this)),
Widget(base_from_member<pRadioButton&>::value),
p(base_from_member<pRadioButton&>::value) {
  p.constructor();
}

RadioButton::~RadioButton() {
  for(auto& item : state.group) {
    if(&item != this) item.state.group.remove(*this);
  }
  p.destructor();
  delete &state;
}

//RadioLabel
//==========

void RadioLabel::group(const nall::group<RadioLabel>& list) {
  for(auto& item : list) item.p.setGroup(item.state.group = list);
  if(list.size()) list.first().setChecked();
}

bool RadioLabel::checked() const {
  return state.checked;
}

void RadioLabel::setChecked() {
  for(auto &item : state.group) item.state.checked = false;
  state.checked = true;
  return p.setChecked();
}

void RadioLabel::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

string RadioLabel::text() const {
  return state.text;
}

RadioLabel::RadioLabel():
state(*new State),
base_from_member<pRadioLabel&>(*new pRadioLabel(*this)),
Widget(base_from_member<pRadioLabel&>::value),
p(base_from_member<pRadioLabel&>::value) {
  p.constructor();
}

RadioLabel::~RadioLabel() {
  for(auto& item : state.group) {
    if(&item != this) item.state.group.remove(*this);
  }
  p.destructor();
  delete &state;
}

//TabFrame
//========

void TabFrame::append(const string& text, const image& image) {
  state.image.append(image);
  state.layout.append(nullptr);
  state.text.append(text);
  return p.append(text, image);
}

void TabFrame::remove(unsigned selection) {
  if(selection >= state.text.size()) return;
  state.image.remove(selection);
  state.layout.remove(selection);
  state.text.remove(selection);
  return p.remove(selection);
}

unsigned TabFrame::selection() const {
  return state.selection;
}

void TabFrame::setImage(unsigned selection, const image& image) {
  if(selection >= state.text.size()) return;
  state.image[selection] = image;
  return p.setImage(selection, image);
}

void TabFrame::setLayout(unsigned selection, Layout& layout) {
  if(selection >= state.text.size()) return;
  state.layout[selection] = &layout;
  synchronizeLayout();
}

void TabFrame::setSelection(unsigned selection) {
  state.selection = selection;
  return p.setSelection(selection);
}

void TabFrame::setText(unsigned selection, const string& text) {
  if(selection >= state.text.size()) return;
  state.text[selection] = text;
  return p.setText(selection, text);
}

void TabFrame::synchronizeLayout() {
  for(auto& layout : state.layout) {
    if(layout == nullptr) continue;
    ((Sizable*)layout)->state.window = ((Sizable*)this)->state.window;
    layout->synchronizeLayout();
  }
}

unsigned TabFrame::tabs() const {
  return state.text.size();
}

string TabFrame::text(unsigned selection) const {
  if(selection >= state.text.size()) return "";
  return state.text[selection];
}

TabFrame::TabFrame():
state(*new State),
base_from_member<pTabFrame&>(*new pTabFrame(*this)),
Widget(base_from_member<pTabFrame&>::value),
p(base_from_member<pTabFrame&>::value) {
  p.constructor();
}

TabFrame::~TabFrame() {
  p.destructor();
  delete &state;
}

//TextEdit
//========

bool TextEdit::editable() const {
  return state.editable;
}

void TextEdit::setCursorPosition(unsigned position) {
  state.cursorPosition = position;
  return p.setCursorPosition(position);
}

void TextEdit::setEditable(bool editable) {
  state.editable = editable;
  return p.setEditable(editable);
}

void TextEdit::setText(const string& text) {
  state.text = text;
  return p.setText(text);
}

void TextEdit::setWordWrap(bool wordWrap) {
  state.wordWrap = wordWrap;
  return p.setWordWrap(wordWrap);
}

string TextEdit::text() {
  return p.text();
}

bool TextEdit::wordWrap() const {
  return state.wordWrap;
}

TextEdit::TextEdit():
state(*new State),
base_from_member<pTextEdit&>(*new pTextEdit(*this)),
Widget(base_from_member<pTextEdit&>::value),
p(base_from_member<pTextEdit&>::value) {
  p.constructor();
}

TextEdit::~TextEdit() {
  p.destructor();
  delete &state;
}

//VerticalScroller
//================

unsigned VerticalScroller::length() const {
  return state.length;
}

unsigned VerticalScroller::position() const {
  return state.position;
}

void VerticalScroller::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void VerticalScroller::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

VerticalScroller::VerticalScroller():
state(*new State),
base_from_member<pVerticalScroller&>(*new pVerticalScroller(*this)),
Widget(base_from_member<pVerticalScroller&>::value),
p(base_from_member<pVerticalScroller&>::value) {
  p.constructor();
}

VerticalScroller::~VerticalScroller() {
  p.destructor();
  delete &state;
}

//VerticalSlider
//==============

unsigned VerticalSlider::length() const {
  return state.length;
}

unsigned VerticalSlider::position() const {
  return state.position;
}

void VerticalSlider::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void VerticalSlider::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

VerticalSlider::VerticalSlider():
state(*new State),
base_from_member<pVerticalSlider&>(*new pVerticalSlider(*this)),
Widget(base_from_member<pVerticalSlider&>::value),
p(base_from_member<pVerticalSlider&>::value) {
  p.constructor();
}

VerticalSlider::~VerticalSlider() {
  p.destructor();
  delete &state;
}

//Viewport
//========

bool Viewport::droppable() const {
  return state.droppable;
}

uintptr_t Viewport::handle() {
  return p.handle();
}

void Viewport::setDroppable(bool droppable) {
  state.droppable = droppable;
  return p.setDroppable(droppable);
}

Viewport::Viewport():
state(*new State),
base_from_member<pViewport&>(*new pViewport(*this)),
Widget(base_from_member<pViewport&>::value),
p(base_from_member<pViewport&>::value) {
  p.constructor();
}

Viewport::~Viewport() {
  p.destructor();
  delete &state;
}

}
