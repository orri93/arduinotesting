#include <gatldisplay.h>
#include <gatlformat.h>
#include <arduinotick.h>

#include "fds-logo.h"

#define INTERVAL 5000

#define TEXT_ID_A "A: "
#define TEXT_ID_B "B: "
#define TEXT_UNIT " C"

Tick timer(INTERVAL);

::gos::atl::format::option::Number option;
::gos::atl::buffer::Holder<> buffer1;
::gos::atl::buffer::Holder<> buffer2;
::gos::atl::buffer::Holder<> ida(TEXT_ID_A, sizeof(TEXT_ID_A));
::gos::atl::buffer::Holder<> idb(TEXT_ID_B, sizeof(TEXT_ID_B));
::gos::atl::buffer::Holder<> unit(TEXT_UNIT, sizeof(TEXT_UNIT));
::gos::atl::display::Oled<> oled;
::gos::atl::display::line::One<> oneline(oled);
::gos::atl::display::line::Two<> twoline(oled);
::gos::atl::display::Render<>* render = &oneline;

void setup() {
  oled.U8g2->begin();
  oled.logo(fds_logo_width, fds_logo_height, fds_logo_bits);
}

void loop() {
}
