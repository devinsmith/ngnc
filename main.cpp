#include <fx.h>
#include "ngnc.h"

int
main(int argc, char *argv[])
{
  FXApp app("ngnc", "drsmicro");
  app.init(argc, argv);

  new ngnc(&app); // Deleted by FXTopWindow::Close
  app.create();

  app.run();
}
