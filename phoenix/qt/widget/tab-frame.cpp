namespace phoenix {

void pTabFrame::append(string text, const image& image) {
  unsigned selection = tabFrame.state.text.size() - 1;
  QWidget* widget = new QWidget;  //addTab() requires a child widget, so give it an empty one
  qtTabFrame->addTab(widget, QString::fromUtf8(text));
  if(!image.empty()) setImage(selection, image);
}

void pTabFrame::remove(unsigned selection) {
  qtTabFrame->removeTab(selection);
}

void pTabFrame::setGeometry(Geometry geometry) {
  pWidget::setGeometry(geometry);
  geometry.x += 1, geometry.width -= 2;
  geometry.y += 26, geometry.height -= 27;
  for(auto& layout : tabFrame.state.layout) {
    if(layout == nullptr) continue;
    layout->setGeometry(geometry);
  }
  synchronizeLayout();
}

void pTabFrame::setImage(unsigned selection, const image& image) {
  qtTabFrame->setTabIcon(selection, CreateIcon(image));
}

void pTabFrame::setSelection(unsigned selection) {
  qtTabFrame->setCurrentIndex(selection);
  synchronizeLayout();
}

void pTabFrame::setText(unsigned selection, string text) {
  qtTabFrame->setTabText(selection, QString::fromUtf8(text));
}

void pTabFrame::constructor() {
  qtWidget = qtTabFrame = new QTabWidget;
  connect(qtTabFrame, SIGNAL(currentChanged(int)), SLOT(onChange(int)));

  setSelection(tabFrame.state.selection);
}

void pTabFrame::destructor() {
  delete qtTabFrame;
  qtWidget = qtTabFrame = nullptr;
}

void pTabFrame::orphan() {
  destructor();
  constructor();
}

void pTabFrame::synchronizeLayout() {
  for(unsigned n = 0; n < tabFrame.state.layout.size(); n++) {
    Layout* layout = tabFrame.state.layout[n];
    if(layout == nullptr) continue;
    layout->setVisible(n == tabFrame.state.selection);
  }
}

void pTabFrame::onChange(int selection) {
  tabFrame.state.selection = selection;
  synchronizeLayout();
  if(tabFrame.onChange) tabFrame.onChange();
}

}
