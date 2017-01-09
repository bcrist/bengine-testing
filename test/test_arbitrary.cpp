#ifdef BE_TEST

#include "arbitrary_builtins.hpp"
#include <catch/catch.hpp>
#include <vector>
#include <set>
#include <map>

#define BE_CATCH_TAGS "[testing][testing:Arbitrary]"

using namespace be;

TEST_CASE("testing::Arbitrary<int>", BE_CATCH_TAGS) {
   testing::Arbitrary<int> arb;

   REQUIRE(arb.generations() == 0);

   SECTION("front-loaded edge cases") {
      REQUIRE(arb() == 0);
      REQUIRE(arb() == std::numeric_limits<int>::max());
      REQUIRE(arb() == std::numeric_limits<int>::min());
      REQUIRE(arb() != arb());

      SECTION("recheck generations counter") {
         REQUIRE(arb.generations() == 5);

         SECTION("reset") {
            arb.reset();
            REQUIRE(arb.generations() == 0);
            REQUIRE(arb() == 0);
            REQUIRE(arb() == std::numeric_limits<int>::max());
            REQUIRE(arb() == std::numeric_limits<int>::min());
            REQUIRE(arb() != arb());
         }
      }
   }
}

TEST_CASE("testing::Arbitrary<I8>", BE_CATCH_TAGS) {
   testing::Arbitrary<I8> arb;
   REQUIRE(arb() == 0);
   REQUIRE(arb() == std::numeric_limits<I8>::max());
   REQUIRE(arb() == std::numeric_limits<I8>::min());
   REQUIRE(arb() != arb());
}

TEST_CASE("testing::Arbitrary<I16>", BE_CATCH_TAGS) {
   testing::Arbitrary<I16> arb;
   REQUIRE(arb() == 0);
   REQUIRE(arb() == std::numeric_limits<I16>::max());
   REQUIRE(arb() == std::numeric_limits<I16>::min());
   REQUIRE(arb() != arb());
}

TEST_CASE("testing::Arbitrary<I32>", BE_CATCH_TAGS) {
   testing::Arbitrary<I32> arb;
   REQUIRE(arb() == 0);
   REQUIRE(arb() == std::numeric_limits<I32>::max());
   REQUIRE(arb() == std::numeric_limits<I32>::min());
   REQUIRE(arb() != arb());
}

TEST_CASE("testing::Arbitrary<I64>", BE_CATCH_TAGS) {
   testing::Arbitrary<I64> arb;
   REQUIRE(arb() == 0);
   REQUIRE(arb() == std::numeric_limits<I64>::max());
   REQUIRE(arb() == std::numeric_limits<I64>::min());
   REQUIRE(arb() != arb());
}

TEST_CASE("testing::Arbitrary<U8>", BE_CATCH_TAGS) {
   testing::Arbitrary<U8> arb;
   REQUIRE(arb() == 0);
   REQUIRE(arb() == std::numeric_limits<U8>::max());
   REQUIRE(arb() != arb());
}

TEST_CASE("testing::Arbitrary<U16>", BE_CATCH_TAGS) {
   testing::Arbitrary<U16> arb;
   REQUIRE(arb() == 0);
   REQUIRE(arb() == std::numeric_limits<U16>::max());
   REQUIRE(arb() != arb());
}

TEST_CASE("testing::Arbitrary<U32>", BE_CATCH_TAGS) {
   testing::Arbitrary<U32> arb;
   REQUIRE(arb() == 0);
   REQUIRE(arb() == std::numeric_limits<U32>::max());
   REQUIRE(arb() != arb());
}

TEST_CASE("testing::Arbitrary<U64>", BE_CATCH_TAGS) {
   testing::Arbitrary<U64> arb;
   REQUIRE(arb() == 0);
   REQUIRE(arb() == std::numeric_limits<U64>::max());
   REQUIRE(arb() != arb());
}

TEST_CASE("testing::Arbitrary<F32>", BE_CATCH_TAGS) {
   testing::Arbitrary<F32> arb;
   REQUIRE(arb() == 0.f);
   REQUIRE(arb() == -0.f);
   REQUIRE(arb() == std::numeric_limits<F32>::max());
   REQUIRE(arb() == std::numeric_limits<F32>::min());
   REQUIRE(arb() == std::numeric_limits<F32>::lowest());
   REQUIRE(arb() == std::numeric_limits<F32>::denorm_min());
   REQUIRE(arb() == std::numeric_limits<F32>::infinity());
   REQUIRE(arb() == -std::numeric_limits<F32>::infinity());
   REQUIRE(std::isnan(arb()));
}

TEST_CASE("testing::Arbitrary<F64>", BE_CATCH_TAGS) {
   testing::Arbitrary<F64> arb;
   REQUIRE(arb() == 0.f);
   REQUIRE(arb() == -0.f);
   REQUIRE(arb() == std::numeric_limits<F64>::max());
   REQUIRE(arb() == std::numeric_limits<F64>::min());
   REQUIRE(arb() == std::numeric_limits<F64>::lowest());
   REQUIRE(arb() == std::numeric_limits<F64>::denorm_min());
   REQUIRE(arb() == std::numeric_limits<F64>::infinity());
   REQUIRE(arb() == -std::numeric_limits<F64>::infinity());
   REQUIRE(std::isnan(arb()));
}

TEST_CASE("testing::Arbitrary<std::vector<int>>", BE_CATCH_TAGS) {
   testing::Arbitrary<std::vector<int>> arb;
   for (int i = 0; i < 100; ++i) {
      auto vec = arb();
   }
}

TEST_CASE("testing::Arbitrary<std::set<int>>", BE_CATCH_TAGS) {
   testing::Arbitrary<std::set<int>> arb;
   for (int i = 0; i < 100; ++i) {
      arb();
   }
}

TEST_CASE("testing::Arbitrary<std::map<int, int>>", BE_CATCH_TAGS) {
   testing::Arbitrary<std::map<U16, U16>> arb;
   for (int i = 0; i < 100; ++i) {
      arb();
   }
}

#endif

