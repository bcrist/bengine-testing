#pragma once
#ifndef BE_TESTING_BENCHMARK_HPP_
#define BE_TESTING_BENCHMARK_HPP_

#include "testing_autolink.hpp"
#include <be/core/time.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/kurtosis.hpp>
#include <boost/accumulators/statistics/skewness.hpp>

namespace be::testing {
namespace acc = boost::accumulators;

///////////////////////////////////////////////////////////////////////////////
class BenchmarkStats : NonCopyable {
   using results_container = std::vector<F64>;
   using stats_container = acc::accumulator_set<F64, acc::stats<
      acc::tag::count,
      acc::tag::mean,
      acc::tag::min,
      acc::tag::max,
      acc::tag::lazy_variance,
      acc::tag::kurtosis,
      acc::tag::skewness
      >>;
public:
   void run_sample() {
      run_sample_();
   }

   const results_container& results() const {
      return results_;
   }

   U64 samples() const {
      return acc::count(stats_);
   }

   F64 mean() const {
      return acc::mean(stats_);
   }

   F64 min() const {
      return acc::min(stats_);
   }

   F64 max() const {
      return acc::max(stats_);
   }

   F64 variance() const {
      return acc::variance(stats_);
   }

   F64 std_dev() const {
      U64 n = samples();
      return std::sqrt(variance() * n / (n - 1));
   }

   F64 kurtosis() const {
      return acc::kurtosis(stats_);
   }

   F64 skewness() const {
      return acc::skewness(stats_);
   }

protected:
   BenchmarkStats(BenchmarkStats&& other)
      : results_(std::move(other.results_)),
        stats_(std::move(other.stats_))
   { }
   virtual ~BenchmarkStats() { }
   virtual void run_sample_() = 0;

   void swap_stats_(BenchmarkStats& other) {
      using std::swap;
      swap(results_, other.results_);
      swap(stats_, other.stats_);
   }

   results_container results_;
   stats_container stats_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename Func>
class Benchmark final : private Func, BenchmarkStats {
   friend void swap(Benchmark<Func>& a, Benchmark<Func>& b) { a.swap_(b); }
public:
   Benchmark(Func func, bool save_all_samples = false)
      : Func(std::move(func)),
        save_all_(save_all_samples)
   { }

   Benchmark(Benchmark<Func>&& other)
      : Func(std::move(other)),
        BenchmarkStats(std::move(other)),
        save_all_(other.save_all_)
   { }

   Benchmark<Func>& operator=(Benchmark<Func> other) {
      swap_(other);
   }

   void operator()() {
      U64 start = perf_now();
      static_cast<Func&>(*this)();
      U64 end = perf_now();
      F64 result = perf_delta_to_seconds(end - start);
      if (save_all_) {
         results_.push_back(result);
      }
      stats_(result);
   }

private:
   virtual void run_sample_() {
      (*this)();
   }

   void swap_(Benchmark<Func>& other) {
      using std::swap;
      swap_stats_(other);
      swap(static_cast<Func&>(*this), static_cast<Func&>(other));
      swap(save_all_, other.save_all_);
   }

   bool save_all_;
};

} // be::testing

#endif