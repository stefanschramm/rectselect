#include "ImageDrawingArea.h"

#include <algorithm>

ImageDrawingArea::ImageDrawingArea(const Glib::RefPtr<Gdk::Pixbuf> img, SelectionFinishedHandlerInterface* handler)
    : mImg(img), finishedHandler(handler), scaledImg(img) {
  add_events(Gdk::BUTTON_PRESS_MASK);
  add_events(Gdk::POINTER_MOTION_MASK);
}

void ImageDrawingArea::resetSelection() {
  clickedPoints.clear();
  queue_draw();
}

bool ImageDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  resizeImageIfRequired();

  // draw image

  Gdk::Cairo::set_source_pixbuf(cr, scaledImg, 0, 0);
  cr->rectangle(0, 0, scaledImg->get_width(), scaledImg->get_height());
  cr->fill();

  // draw cursor position

  cr->set_source_rgba(1, 1, 1, 0.8);
  std::vector<double> dashPattern = {10, 10};
  cr->set_dash(dashPattern, 0);

  cr->set_line_width(1);

  cr->move_to(0, y1);
  cr->line_to(scaledImg->get_width(), y1);
  cr->stroke();

  cr->move_to(x1, 0);
  cr->line_to(x1, scaledImg->get_height());
  cr->stroke();

  // selection
  if (clickedPoints.size() == 1) {
    auto& p = clickedPoints[0];
    auto factor = determineScalingFactor();

    cr->move_to(factor * p.x, factor * p.y);
    cr->line_to(factor * p.x, y1);
    cr->stroke();

    cr->move_to(factor * p.x, factor * p.y);
    cr->line_to(x1, factor * p.y);
    cr->stroke();
  }

  return true;
}

bool ImageDrawingArea::on_button_press_event(GdkEventButton* event) {
  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1)) {
    if (clickedPoints.size() >= 2) {
      clickedPoints.clear(); // new selection
      queue_draw();
    }
    clickedPoints.push_back(mapClickToPoint(event));
    if (clickedPoints.size() == 2) {
      finishedHandler->onSelectionFinished(
        clickedPoints[0].x,
        clickedPoints[0].y,
        clickedPoints[1].x,
        clickedPoints[1].y
      );
    }

    return true;
  }

  return false;
}

void ImageDrawingArea::resizeImageIfRequired() {
  auto factor = determineScalingFactor();
  auto effectiveHeight = std::max(1, (int)(mImg->get_height() * factor));
  auto effectiveWidth = std::max(1, (int)(mImg->get_width() * factor));

  if (scaledImg->get_width() == effectiveWidth && scaledImg->get_height() == effectiveHeight) {
    return;
  }

  // TODO: delete previous object? https://developer-old.gnome.org/gtkmm-tutorial/stable/sec-refptr-copying.html.en
  // TODO: check if nullptr was returned
  scaledImg = mImg->scale_simple(effectiveWidth, effectiveHeight, Gdk::InterpType::INTERP_BILINEAR);
}

ImageDrawingArea::Point ImageDrawingArea::mapClickToPoint(GdkEventButton* event) {
  auto factor = determineScalingFactor();
  auto x = std::min((int)(event->x / factor), mImg->get_width());
  auto y = std::min((int)(event->y / factor), mImg->get_height());

  return Point{x, y};
}

double ImageDrawingArea::determineScalingFactor() {
  auto widthFactor = (double)get_width() / mImg->get_width();
  auto heightFactor = (double)get_height() / mImg->get_height();

  return std::min(1.0, std::min(widthFactor, heightFactor)); // don't enlarge
}

bool ImageDrawingArea::on_motion_notify_event(GdkEventMotion* event) {
  if (event->type == GDK_MOTION_NOTIFY) {
    x1 = event->x;
    y1 = event->y;

    queue_draw();

    return true;
  }

  return false;
}
