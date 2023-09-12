#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "ezgargs/ezgargs.hpp"

namespace ezgargs {

TEST_CASE("test doctest") { CHECK(1 == 1); }

TEST_CASE("test helloworld") { hellowolrd(); }

}  // namespace ezgargs
