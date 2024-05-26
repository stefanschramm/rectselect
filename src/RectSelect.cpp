#include "RectSelect.h"

#include <iostream>

void RectSelect::onSelectionFinished(int x0, int y0, int x1, int y1) {
  // Output raw points:
  // std::cout << x0 << ";" << y0 << ";" << x1 << ";" << y1 << std::endl;
  // Output for ImageMagick -crop argument
  // Example: convert original.jpg -crop 182x120+3161+1354 cropped.jpg
  std::cout << (x1 - x0) << "x" << (y1 - y0) << "+" << x0 << "+" << y0 << std::endl;
  remainingSelectionCount--;
  if (remainingSelectionCount <= 0 && mWindow) {
    mWindow->close();
  }
}

int RectSelect::run(int argc, char** argv) {
  if (argc < 2 || argc > 3) {
    std::cerr << "Usage: " << argv[0] << " <imagefile> <selection count>" << std::endl;
    return EXIT_FAILURE;
  }
  if (argc == 3) {
    remainingSelectionCount = atoi(argv[2]);
  }

  auto gtkApp = Gtk::Application::create("net.stefanschramm.rectselect");
  auto img = Gdk::Pixbuf::create_from_file(argv[1]);
  auto area = ImageDrawingArea(img, this);
  area.show();

  Gtk::Window window;
  mWindow = &window;
  window.set_default_size(800, 600);
  window.fullscreen();
  window.add(area);
  window.signal_key_press_event().connect(
    [&window, &area](GdkEventKey* event) -> bool {
      switch (event->keyval) {
        case GDK_KEY_Return:
          std::cout << "Return" << std::endl;
          break;
        case GDK_KEY_Escape:
          area.resetSelection();
          break;
        case 'q':
          window.close();
          break;
        default:
          break;
      }
      return true;
    },
    false
  );

  return gtkApp->run(window);
}