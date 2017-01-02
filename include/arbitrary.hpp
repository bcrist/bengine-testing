#pragma once
#ifndef BE_TESTING_ARBITRARY_HPP_
#define BE_TESTING_ARBITRARY_HPP_

#include <be/util/xorshift_128_plus.hpp>
#include <vector>

namespace be {
namespace testing {

template <typename T, typename Enable = void>
class Arbitrary;

///////////////////////////////////////////////////////////////////////////////
/// \brief  Base class for specializing Arbitrary.
template <typename T>
class ArbitraryBase {
public:
   using value_type = std::decay_t<T>;

   ArbitraryBase(U64 seed = 0)
      : seed_(seed),
        generation_(0),
        rnd_(seed)
   { }
   
   value_type operator()() {
      ++generation_;
      auto self = static_cast<Arbitrary<T>*>(this);
      return self->get_(self->dimension_());
   }

   value_type operator()(std::size_t dimension) {
      ++generation_;
      auto self = static_cast<Arbitrary<T>*>(this);
      return self->get_(dimension);
   }

   std::vector<value_type> split(value_type value) {
      return std::vector<value_type>();
   }

   std::size_t generations() const {
      return generation_;
   }

   void reset() {
      generation_ = 0;
      rnd_.seed(seed_);
   }

protected:
   std::size_t dimension_k_() { return 1; }
   std::size_t dimension_d_() { return 10; }

   std::size_t dimension_() {
      auto self = static_cast<Arbitrary<T>*>(this);
      return self->dimension_k_() * (1 + generation_ / self->dimension_d_());
   }

   T get_(std::size_t dimension) {
      BE_IGNORE(dimension);
      return T{};
   }

   U64 seed_;
   std::size_t generation_;
   rnd::xs128p rnd_;
};


///////////////////////////////////////////////////////////////////////////////
/// \brief  Generates arbitrary objects of a particular type.
/// \details Arbitrary satisfies the Generator concept for use with
///         std::generate.
///
///         Inspired by Ben Deane's CppCon2015 talk:
///         https://www.youtube.com/watch?v=OPoZWnYIcP4
template <typename T, typename Enable>
class Arbitrary : public ArbitraryBase<T> {
public:
   Arbitrary(U64 seed = 0) : ArbitraryBase(seed) { }
};

} // be::perf
} // be

#endif
