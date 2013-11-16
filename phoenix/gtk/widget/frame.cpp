namespace phoenix {

void pFrame::setGeometry(Geometry geometry) {
  pWidget::setGeometry(geometry);
  if(frame.state.layout == nullptr) return;
  Size size = pFont::size(widget.state.font, frame.state.text);
  if(frame.state.text.empty()) size.height = 8;
  geometry.x += 1, geometry.width -= 3;
  geometry.y += size.height, geometry.height -= size.height + 2;
  frame.state.layout->setGeometry(geometry);
}

void pFrame::setText(string text) {
  gtk_frame_set_label(GTK_FRAME(gtkWidget), text);
}

void pFrame::constructor() {
  gtkWidget = gtk_frame_new("");
}

void pFrame::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pFrame::orphan() {
  destructor();
  constructor();
}

}
