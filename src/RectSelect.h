#pragma once

#include "ImageDrawingArea.h"

#include <gtkmm.h>

class RectSelect : public SelectionFinishedHandlerInterface {
public:
  int run(int argc, char** argv);
  virtual void onSelectionFinished(int x0, int y0, int x1, int y1);

private:
  Gtk::Window* mWindow = nullptr;
  int remainingSelectionCount = 1;
};
