#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "EasygoingArgs/argparse.hpp"
#include "doctest/doctest.h"

namespace EasygoingArgs {

TEST_CASE("test doctest") { CHECK(1 == 1); }

TEST_CASE("test helloworld") { hellowolrd(); }

}  // namespace EasygoingArgs
