#ifdef BE_TEST

#include <catch/catch.hpp>
#include "version.hpp"
#include <be/core/version.hpp>
#include <be/util/version.hpp>

TEST_CASE("Version", "[version]") {
   REQUIRE(BE_TESTING_VERSION == BE_TESTING_VERSION);
   REQUIRE(BE_CORE_VERSION == BE_CORE_VERSION);
   REQUIRE(BE_UTIL_VERSION == BE_UTIL_VERSION);
}

#endif
