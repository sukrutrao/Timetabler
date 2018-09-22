#include "global.h"
#include "time_tabler.h"
#include <gtest/gtest.h>

TimeTabler *timeTabler;

int main(int argc, char *argv[]) {
    timeTabler = new TimeTabler();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}