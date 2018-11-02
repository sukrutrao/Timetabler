#include <gtest/gtest.h>
#include "global.h"
#include "timetabler.h"

Timetabler *timetabler;

int main(int argc, char *argv[]) {
  timetabler = new Timetabler();
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
