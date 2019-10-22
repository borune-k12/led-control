#include <QApplication>
#include <map>
#include "led-widget.h"
#include "communicator.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  LedWidget w;

  const std::string& pipe_file = argc > 1 ? std::string(argv[1]) : "/tmp/led.pipe";
  printf("Starting server with pipe '%s'...\n", pipe_file.c_str());
  Communicator communicator(w.get_led(), pipe_file);
  if (communicator.start()) {
    w.show();
    return a.exec();
  } else {
    printf("Error: Cannot create pipe.\n");
    return 1;
  }
}
