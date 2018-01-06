#pragma once
#ifndef BE_TESTING_ARBITRARY_BUILTINS_HPP_
#define BE_TESTING_ARBITRARY_BUILTINS_HPP_

#include "arbitrary.hpp"
#include <be/core/t_select.hpp>
#include <be/core/t_is_container.hpp>
#include <limits>
#include <random>

namespace be::testing {

template <typename T>
class Arbitrary<T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T> && sizeof(T) != 1>> final : public ArbitraryBase<T> {
   friend class ArbitraryBase<T>;
   using value_type = typename ArbitraryBase<T>::value_type;
public:
   Arbitrary(U64 seed = 0) : ArbitraryBase<T>(seed) { }
private:
   value_type get_(std::size_t dimension) {
      if (dimension == 1) {
         switch (this->generation_) {
            case 1: return static_cast<value_type>(0);
            case 2: return std::numeric_limits<value_type>::max();
            case 3: return std::numeric_limits<value_type>::min();
         }
      }
      using dist_type = std::uniform_int_distribution<value_type>;
      dist_type dist(std::numeric_limits<value_type>::min(), std::numeric_limits<value_type>::max());
      return dist(this->rnd_);
   }
};

template <typename T>
class Arbitrary<T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T> && sizeof(T) != 1>> final : public ArbitraryBase<T> {
   friend class ArbitraryBase<T>;
   using value_type = typename ArbitraryBase<T>::value_type;
public:
   Arbitrary(U64 seed = 0) : ArbitraryBase<T>(seed) { }
private:
   value_type get_(std::size_t dimension) {
      if (dimension == 1) {
         switch (this->generation_) {
            case 1: return static_cast<value_type>(0);
            case 2: return std::numeric_limits<value_type>::max();
         }
      }
      using dist_type = std::uniform_int_distribution<value_type>;
      dist_type dist(std::numeric_limits<value_type>::min(), std::numeric_limits<value_type>::max());
      return dist(this->rnd_);
   }
};

template <typename T>
class Arbitrary<T, std::enable_if_t<std::is_integral_v<T> && sizeof(T) == 1>> final : public ArbitraryBase<T> {
   friend class ArbitraryBase<T>;
   using value_type = typename ArbitraryBase<T>::value_type;
public:
   Arbitrary(U64 seed = 0) : ArbitraryBase<T>(seed) { }
private:
   value_type get_(std::size_t dimension) {
      if (dimension == 1) {
         switch (this->generation_) {
            case 1: return static_cast<value_type>(0);
            case 2: return std::numeric_limits<value_type>::max();
            case 3: if (std::numeric_limits<value_type>::min() != static_cast<value_type>(0)) return std::numeric_limits<value_type>::min();
         }
      }
      using dist_type = std::uniform_int_distribution<int>; // uniform_int_distribution cant be specialized with char variants
      dist_type dist(std::numeric_limits<value_type>::min(), std::numeric_limits<value_type>::max());
      return static_cast<value_type>(dist(this->rnd_));
   }
};

template <typename T>
class Arbitrary<T, std::enable_if_t<std::is_floating_point_v<T>>> final : public ArbitraryBase<T> {
   friend class ArbitraryBase<T>;
   using value_type = typename ArbitraryBase<T>::value_type;
public:
   Arbitrary(U64 seed = 0) : ArbitraryBase<T>(seed) { }
private:
   value_type get_(std::size_t dimension) {
      if (dimension == 1) {
         switch (this->generation_) {
            case 1: return static_cast<value_type>(0.f);
            case 2: return static_cast<value_type>(-0.f);
            case 3: return std::numeric_limits<value_type>::max();
            case 4: return std::numeric_limits<value_type>::min();
            case 5: return std::numeric_limits<value_type>::lowest();
            case 6: return std::numeric_limits<value_type>::denorm_min();
            case 7: return std::numeric_limits<value_type>::infinity();
            case 8: return -std::numeric_limits<value_type>::infinity();
            case 9: return std::numeric_limits<value_type>::quiet_NaN();
         }
      }

      if (dimension <= 2) {
         std::uniform_int_distribution<int> dist(0, 100);
         return static_cast<value_type>(dist(this->rnd_));
      }

      if (dimension == 3) {
         std::uniform_real_distribution<value_type> dist(static_cast<value_type>(0), static_cast<value_type>(1));
         return dist(this->rnd_);
      }

      if (dimension <= 9) {
         std::normal_distribution<value_type> dist(static_cast<value_type>(0), static_cast<value_type>(exp2(std::numeric_limits<value_type>::digits*0.5f)));
         return dist(this->rnd_);
      }

      // uniform_real_distribution doesn't allow intervals wider than numeric_limits<T>::max()
      std::uniform_real_distribution<value_type> dist(static_cast<value_type>(0), std::numeric_limits<value_type>::max() - 1);
      return dist(this->rnd_);
   }
};

template <typename T>
class Arbitrary<T, std::enable_if_t<t::IsPushBackContainer<T>::value>> final : public ArbitraryBase<T> {
   friend class ArbitraryBase<T>;
   using value_type = typename ArbitraryBase<T>::value_type;
public:
   Arbitrary(U64 seed = 0) : ArbitraryBase<T>(seed) { }

   std::enable_if_t<t::IsSizedContainer<T>::value, std::vector<value_type>> split(value_type container) {
      std::vector<value_type> vec;
      vec.reserve(2);

      std::size_t size = container.size();
      if (size > 1) {
         std::size_t first_capacity = size >> 1;

         vec.push_back();
         vec.push_back();

         value_type& first = vec.front();
         value_type& second = vec.back();

         for (auto& v : container) {
            if (first.size() < first_capacity) {
               first.push_back(v);
            } else {
               second.push_back(v);
            }
         }
      }
      return vec;
   }

private:
   value_type get_(std::size_t dimension) {
      if (dimension == 10 && this->generation_ == 1) {
         return value_type();
      }

      Arbitrary<typename value_type::value_type> value_arb(this->rnd_());
      value_type container;
      for (std::size_t i = 0; i < dimension; ++i) {
         container.push_back(value_arb(dimension));
      }

      return container;
   }

   std::size_t dimension_k_() { return 10; }
   std::size_t dimension_d_() { return 100; }
};

template <typename T>
class Arbitrary<T, std::enable_if_t<t::IsInsertContainer<T>::value>> final : public ArbitraryBase<T> {
   friend class ArbitraryBase<T>;
   using value_type = typename ArbitraryBase<T>::value_type;
public:
   Arbitrary(U64 seed = 0) : ArbitraryBase<T>(seed) { }

   std::enable_if_t<t::IsSizedContainer<T>::value, std::vector<value_type>> split(value_type container) {
      std::vector<value_type> vec;
      vec.reserve(2);

      std::size_t size = container.size();
      if (size > 1) {
         std::size_t first_capacity = size >> 1;

         vec.push_back();
         vec.push_back();

         value_type& first = vec.front();
         value_type& second = vec.back();

         for (auto& v : container) {
            if (first.size() < first_capacity) {
               first.insert(v);
            } else {
               second.insert(v);
            }
         }
      }
      return vec;
   }

private:
   value_type get_(std::size_t dimension) {
      if (dimension == 10 && this->generation_ == 1) {
         return value_type();
      }

      Arbitrary<typename value_type::value_type> value_arb(this->rnd_());
      value_type container;
      for (std::size_t i = 0; i < dimension; ++i) {
         container.insert(value_arb(dimension));
      }

      return container;
   }

   std::size_t dimension_k_() { return 10; }
   std::size_t dimension_d_() { return 100; }
};

template <typename T, typename U>
class Arbitrary<std::pair<T, U>> final : public ArbitraryBase<std::pair<T, U>> {
   friend class ArbitraryBase<std::pair<T, U>>;
   using value_type = typename ArbitraryBase<std::pair<T, U>>::value_type;
public:
   Arbitrary(U64 seed = 0) : ArbitraryBase<std::pair<T, U>>(seed) { }

   std::vector<std::pair<T, U>> split(std::pair<T, U> container) {
      std::vector<T> vec;
      if (container.first != T() && container.second != U()) {
         vec.reserve(2);
         vec.push_back(std::make_pair(container.first, U()));
         vec.push_back(std::make_pair(T(), container.second));
      }
      return vec;
   }

private:
   std::pair<T, U> get_(std::size_t dimension) {
      Arbitrary<T> t_arb { this->rnd_() };
      Arbitrary<U> u_arb { this->rnd_() };
      return std::make_pair(t_arb(dimension), u_arb(dimension));
   }
};

// TODO std::array, T[N], tuple

} // be::testing

#endif
