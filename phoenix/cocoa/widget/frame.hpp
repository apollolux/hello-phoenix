@interface CocoaFrame : NSBox {
@public
  phoenix::Frame* frame;
}
-(id) initWith:(phoenix::Frame&)frame;
@end

namespace phoenix {

struct pFrame : public pWidget {
  Frame& frame;
  CocoaFrame* cocoaFrame = nullptr;

  void setFont(string font);
  void setGeometry(Geometry geometry);
  void setText(string text);

  pFrame(Frame& frame) : pWidget(frame), frame(frame) {}
  void constructor();
  void destructor();
  void orphan();
};

}
