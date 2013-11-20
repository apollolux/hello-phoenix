#include <phoenix/phoenix.hpp>
using namespace nall;
using namespace phoenix;

struct TestWindow : Window {
  VerticalLayout layout;
  Frame frame;
  VerticalLayout frameLayout;
  TextEdit label;

  TestWindow() {
    setWindowGeometry({900, 64, 480, 320});
    layout.setMargin(5);
    frame.setText("Frame");
    label.setText("Label\nText");
    frameLayout.setMargin(5);
    frameLayout.append(label, {~0, ~0});
    frame.setLayout(frameLayout);
    layout.append(frame, {~0, ~0});
    append(layout);
    setVisible();
  }
};

struct MainWindow : Window {
  VerticalLayout layout;
    TabFrame tabFrame;
      VerticalLayout tabLayout0;
        TabFrame innerFrame;
          VerticalLayout innerLayout0;
            ListView listView;
            Frame frame;
              VerticalLayout frameLayout;
                TextEdit frameLabel;
      VerticalLayout tabLayout1;
        Canvas canvas;

  MainWindow() {
    setWindowGeometry({64, 64, 800, 640});
    setTitle("TabFrame Demo");

    layout.setMargin(5);

    tabFrame.append("Famicom");
    tabFrame.append("Super Famicom");
    tabFrame.append("Game Boy");
    tabFrame.append("Game Boy Color");
    tabFrame.append("Game Boy Advance");

    tabLayout0.setMargin(5);

    innerFrame.append("First");
    innerFrame.append("Second");

    innerLayout0.setMargin(5);

    listView.append("Chrono Trigger");
    listView.append("Super Metroid");
    listView.append("Wanderers from Ys");

    frame.setText("Information");

    frameLayout.setMargin(5);

    frameLabel.setText("Title: Chrono Trigger\nPublisher: Nintendo");

    tabLayout1.setMargin(5);

    canvas.setImage({"higan.png"});
    canvas.setSize({512, 512});

    //

    innerLayout0.append(listView, {~0, ~0}, 5);
    frameLayout.append(frameLabel, {~0, ~0});
    frame.setLayout(frameLayout);
    innerLayout0.append(frame, {~0, 80});
    innerFrame.setLayout(0, innerLayout0);
    tabLayout0.append(innerFrame, {~0, ~0});
    tabFrame.setLayout(0, tabLayout0);
    tabLayout1.append(canvas, {~0, ~0});
    tabFrame.setLayout(1, tabLayout1);
    layout.append(tabFrame, {~0, ~0});
    append(layout);

    onClose = &Application::quit;
    setVisible();

    #if 0
    animation.onActivate = [&] {
      if(phase == 4) return;

      nall::image backdrop;
      backdrop.allocate(512, 512);
      backdrop.fill(0);
      nall::image logo("higan.png");

      if(phase == 0) {
        logo.crop(offset, offset, 256, 256);
        backdrop.impose(image::blend::sourceColor, offset, offset, logo, 0, 0, 256, 256);

        if(++offset == 256) phase = 1;
      } else if(phase == 1) {
        if(--offset == 128) phase = 2;

        logo.crop(offset, offset, 256, 256);
        backdrop.impose(image::blend::sourceColor, offset, offset, logo, 0, 0, 256, 256);
      } else if(phase == 2) {
        if(--offset == 0) phase = 3;

        unsigned size = (256 - offset) * 2;
        logo.crop(offset, offset, size, size);
        backdrop.impose(image::blend::sourceColor, offset, offset, logo, 0, 0, size, size);
      } else if(phase == 3) {
        backdrop.verticalGradient((offset << 24) + 255, 0);
        backdrop.impose(image::blend::sourceAlpha, 0, 0, logo, 0, 0, 512, 512);

        if(++offset == 256) phase = 4;
      }

      canvas.setImage(backdrop);
    };

    animation.setInterval(1);
    animation.setEnabled();
    #endif
  }
};

int main() {
  new MainWindow;
  new TestWindow;
  Application::run();
  return 0;
}
