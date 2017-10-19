#include <gtest/gtest.h>
#include "time_tabler.h"
#include "global.h"


TimeTabler *Global::timeTabler = nullptr;

int main(int argc, char *argv[]) {
	Global::timeTabler = new TimeTabler();
	testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}