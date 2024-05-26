#pragma once

#include <gtkmm.h>

class SelectionFinishedHandlerInterface {
public:
  virtual ~SelectionFinishedHandlerInterface() = default;
  virtual void onSelectionFinished(int x0, int y0, int x1, int y1) = 0;
};

class ImageDrawingArea : public Gtk::DrawingArea {
public:
  ImageDrawingArea(const Glib::RefPtr<Gdk::Pixbuf> img, SelectionFinishedHandlerInterface* handler);
  void resetSelection();

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  bool on_motion_notify_event(GdkEventMotion* event) override;
  bool on_button_press_event(GdkEventButton* event) override;

private:
  using Point = struct {
    int x;
    int y;
  };
  const Glib::RefPtr<Gdk::Pixbuf> mImg;
  SelectionFinishedHandlerInterface* finishedHandler;
  Glib::RefPtr<Gdk::Pixbuf> scaledImg;
  double x1 = 0;
  double y1 = 0;
  std::vector<Point> clickedPoints;
  void resizeImageIfRequired();
  Point mapClickToPoint(GdkEventButton* event);
  double determineScalingFactor();
};
