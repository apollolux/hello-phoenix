namespace phoenix {

void pTabFrame::append(string text, const image& image) {
  unsigned selection = TabCtrl_GetItemCount(hwnd);
  wchar_t wtext[] = L"";
  TCITEM item;
  item.mask = TCIF_TEXT;
  item.pszText = wtext;
  TabCtrl_InsertItem(hwnd, selection, &item);
  setText(selection, text);
  if(!image.empty()) setImage(selection, image);
}

void pTabFrame::remove(unsigned selection) {
  TabCtrl_DeleteItem(hwnd, selection);
  buildImageList();
}

void pTabFrame::setEnabled(bool enabled) {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setEnabled(layout->enabled());
  }
  pWidget::setEnabled(enabled);
}

void pTabFrame::setGeometry(Geometry geometry) {
  pWidget::setGeometry(geometry);
  geometry.x += 2, geometry.width -= 6;
  geometry.y += 22, geometry.height -= 25;
  for(auto& layout : tabFrame.state.layout) {
    if(layout == nullptr) continue;
    layout->setGeometry(geometry);
  }
}

void pTabFrame::setImage(unsigned selection, const image& image) {
  buildImageList();
}

void pTabFrame::setSelection(unsigned selection) {
  TabCtrl_SetCurSel(hwnd, selection);
  synchronizeLayout();
}

void pTabFrame::setText(unsigned selection, string text) {
  utf16_t wtext(text);
  TCITEM item;
  item.mask = TCIF_TEXT;
  item.pszText = (wchar_t*)wtext;
  TabCtrl_SetItem(hwnd, selection, &item);
}

void pTabFrame::setVisible(bool visible) {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(layout->visible());
  }
  pWidget::setVisible(visible);
}

void pTabFrame::constructor() {
  hwnd = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&tabFrame);
  setDefaultFont();
  for(auto& text : tabFrame.state.text) append(text, {});
  buildImageList();
  setSelection(tabFrame.state.selection);
  synchronize();
}

void pTabFrame::destructor() {
  if(imageList) { ImageList_Destroy(imageList); imageList = 0; }
  DestroyWindow(hwnd);
}

void pTabFrame::orphan() {
  destructor();
  constructor();
}

void pTabFrame::buildImageList() {
  if(imageList) ImageList_Destroy(imageList);
  unsigned size = pFont::size(hfont, " ").height;
  imageList = ImageList_Create(size, size, ILC_COLOR32, 1, 0);
  for(auto& image : tabFrame.state.image) {
    ImageList_Append(imageList, image);
  }
  TabCtrl_SetImageList(hwnd, imageList);
  for(unsigned n = 0; n < tabFrame.state.image.size(); n++) {
    TCITEM item;
    item.mask = TCIF_IMAGE;
    item.iImage = (tabFrame.state.image(n).empty() ? -1 : n);
    TabCtrl_SetItem(hwnd, n, &item);
  }
}

void pTabFrame::synchronizeLayout() {
  unsigned selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(selection == tabFrame.state.selection);
    selection++;
  }
}

}
