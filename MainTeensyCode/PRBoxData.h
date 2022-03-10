#ifndef _PRBOX_DATA_H_
#define _PRBOX_DATA_H_

#include "PRBoxController.h"

PRBoxController yellowBox(17);
PRBoxController greenBox(20);
PRBoxController grayBox(21);
PRBoxController blueBox(22);
PRBoxController orangeBox(23);

void loopPRBoxes() {
  yellowBox.tick();
  greenBox.tick();
  grayBox.tick();
  blueBox.tick();
  orangeBox.tick();
}

#endif
