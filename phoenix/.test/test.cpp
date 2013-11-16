#include <phoenix/phoenix.hpp>
using namespace nall;
using namespace phoenix;

struct MainWindow : Window {
  VerticalLayout layout;
  TabFrame tabFrame;
  VerticalLayout tabLayout0;
    ListView listView;
  VerticalLayout tabLayout1;
    HorizontalLayout checkLayout;
      CheckButton button1;
      CheckButton button2;
    HorizontalLayout radioLayout;
      RadioButton button3;
      RadioButton button4;
      RadioButton button5;
  VerticalLayout tabLayout2;
    Canvas canvas;
  Frame frame;
  VerticalLayout frameLayout;
    Button button;

  MainWindow() {
    setWindowGeometry({64, 64, 800, 480});
    setTitle("TabFrame Demo");

    layout.setMargin(5);

    tabFrame.append("Famicom");
    tabFrame.append("Super Famicom", {"folder.png"});
    tabFrame.append("Game Boy");
    tabFrame.append("Game Boy Color");
    tabFrame.append("Game Boy Advance");

    listView.setCheckable(true);
    listView.setHeaderVisible(true);
    listView.setHeaderText({"Name", "Region"});
    listView.append({"Chrono Trigger", "USA"});
    listView.append({"Super Metroid", "Japan"});
    listView.append({"Wanderers from Ys", "Europe"});
    listView.autoSizeColumns();
    listView.setSelection(1);
    listView.onToggle = [&](unsigned selection) { print("check: ", selection, " = ", listView.checked(selection), "\n"); };
    listView.onActivate = [&]() { print("activate: ", listView.selection(), "\n"); };
    listView.onChange = [&]() { print("change: ", listView.selection(), "\n"); };

    tabLayout0.setMargin(5);
    tabLayout0.append(listView, {~0, ~0});
    tabFrame.setLayout(0, tabLayout0);

    button1.setImage({"folder.png"}, Orientation::Vertical);
    button2.setImage({"folder.png"}, Orientation::Horizontal);
    button3.setImage({"folder.png"}, Orientation::Vertical);
    button1.setText("Button 1");
    button2.setText("Button 2");
    button3.setText("Button 3");
    button4.setText("Button 4");
    button5.setText("Button 5");
    button1.onToggle = [&]() { print(button1.checked(), "\n"); };
    button2.onToggle = [&]() { print(button2.checked(), "\n"); };
    button3.onActivate = [&]() { print("3\n"); };
    button4.onActivate = [&]() { print("4\n"); };
    button5.onActivate = [&]() { print("5\n"); };

    RadioButton::group(button3, button4, button5);

    tabLayout1.setMargin(5);
    tabLayout1.append(checkLayout, {~0, 0}, 5);
    checkLayout.append(button1, {0, 0}, 5);
    checkLayout.append(button2, {0, 0}, 5);
    tabLayout1.append(radioLayout, {~0, 0}, 5);
    radioLayout.append(button3, {0, 0}, 5);
    radioLayout.append(button4, {0, 0}, 5);
    radioLayout.append(button5, {0, 0}, 5);
    tabFrame.setLayout(1, tabLayout1);

    tabLayout2.setMargin(5);
    tabLayout2.append(canvas, {~0, ~0});
    tabFrame.setLayout(2, tabLayout2);

    canvas.onSize = [&]() {
      Geometry geometry = canvas.geometry();
      canvas.setSize(geometry.size());
      canvas.fill({255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0});
      canvas.update();
    };

    tabFrame.setSelection(1);

    layout.append(tabFrame, {~0, ~0}, 5);

    frame.setText("Information");
    button.setImage({"image.png"});
    button.setText("Button");
    frameLayout.setMargin(5);
    frameLayout.append(button, {~0, ~0});
    frame.setLayout(frameLayout);
    layout.append(frame, {~0, 100});

    append(layout);

    onClose = &Application::quit;
    setVisible();
  }
};

int main() {
  new MainWindow;
  Application::run();
  return 0;
}
