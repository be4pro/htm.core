/* ---------------------------------------------------------------------
 * HTM Community Edition of NuPIC
 * Copyright (C) 2013, Numenta, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 *
 * You should have received a copy of the GNU Affero Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 * --------------------------------------------------------------------- */

/** @file
 *  Random Number Generator interface
 */

#ifndef NTA_RANDOM_HPP
#define NTA_RANDOM_HPP

#include <algorithm>
#include <iterator>
#include <random>
#include <string>
#include <vector>

#include <htm/types/Types.hpp>
#include <htm/types/Serializable.hpp>
#include <htm/utils/Log.hpp>

namespace htm {

#define DEBUG_RANDOM_SEED std::mt19937::default_seed

/**
 * Random class
 *
 * ### Description
 * This class provides a standardized random number generator for the NuPIC
 * Runtime Engine.  It is deterministic and cross platform.  The random seed is
 * logged and can be set manually.  This makes it possible to reproduce tests
 * that are driven by random number generation.
 *
 * The functionality is similar to the standard random function that is provided
 * by C++ Uniform Random Distribution.
 *
 * Each Random object is a random number generator. There are two ways of
 * creating one:
 * 1) explicit seed
 *       Random rng(seed);
 * 2) self-seeded
 *       Random rng;
 *
 * The self-seed is logged to NTA_INFO if used.
 *
 * In Release mode: good self-seeds are generated by an internal global random
 * number generator, which is seeded from the system time.
 *
 * In Debug mode: good self-seeds are generated by an internal global random
 * number generator, which is seeded by the hardcoded constant
 * DEBUG_RANDOM_SEED, which is defined above.  To debug a specific random seed,
 * such as the ones used in release mode, simply change this definition and
 * recompile.
 *
 * @TODO Make extend random engine type.
 * @TODO Add ability to specify different rng algorithms.
 */
class Random : public Serializable  {
public:
  Random(UInt64 seed = 0);


  CerealAdapter;
  template<class Archive>
  void save_ar(Archive & ar) const {
    ar( cereal::make_nvp("seed", seed_), cereal::make_nvp("steps", steps_));  
  }
  template<class Archive>
  void load_ar(Archive & ar) {
    ar( seed_, steps_);  
    gen.seed(static_cast<unsigned int>(seed_)); //reseed
    gen.discard(steps_); //advance n steps
  }

  bool operator==(const Random &other) const;
  inline bool operator!=(const Random &other) const {
    return !operator==(other);
  }

  //main API methods:
  /** return a value (uniformly) distributed between [0,max)
   */
  inline UInt32 getUInt32(const UInt32 max = MAX32) {
    NTA_ASSERT(max > 0);
    steps_++;
    return gen() % max; //uniform_int_distribution(gen) replaced, as is not same on all platforms! 
  }

  /** return a double uniformly distributed on [0,1.0)
   * May not be cross-platform (but currently is to our experience)
   */
  inline Real64 getReal64() {
    steps_++;
    return gen() / static_cast<Real64>(max());
  }

  // populate choices with a random selection of nChoices elements from
  // population. throws exception when nPopulation < nChoices
  // templated functions must be defined in header
  //TODO replace with std::sample in c++17 : https://en.cppreference.com/w/cpp/algorithm/sample 
  template <class T>
  std::vector<T> sample(const std::vector<T>& population, UInt nChoices) {
    if (nChoices == 0) {
      return std::vector<T>{};
    }
    NTA_CHECK(nChoices <= static_cast<UInt>(population.size())) << "population size must be greater than number of choices";
    std::vector<T> pop(population); //deep copy
    this->shuffle(std::begin(pop), std::end(pop));
    pop.resize(nChoices); //keep only first nChoices, drop rest
    return pop;
  }


  /**
   * return random from range [from, to)
   */
  Real realRange(Real from, Real to) {
    NTA_ASSERT(from <= to);
    const Real split = to - from;
    return from + static_cast<Real>(split * getReal64());
  }


  // randomly shuffle the elements
  template <class RandomAccessIterator>
  void shuffle(RandomAccessIterator first, RandomAccessIterator last) {
    //std::shuffle(first, last, gen); //not platform independent results :(
    platform_independent_shuffle(first, last);
  }

  // for STL compatibility
  UInt32 operator()(UInt32 n = MAX32) { 
    NTA_ASSERT(n > 0);
    return getUInt32(n);
  }

  // normally used for debugging only
  UInt64 getSeed() const { return seed_; }

  // for STL
  typedef unsigned long argument_type;
  typedef unsigned long result_type;
  result_type max() const { return gen.max(); }
  result_type min() const { return gen.min(); }
  static const UInt32 MAX32 = std::numeric_limits<UInt32>::max();

protected:
  friend class RandomTest;
  friend std::ostream &operator<<(std::ostream &, const Random &);
  friend std::istream &operator>>(std::istream &, Random &);
  friend UInt32 GetRandomSeed();
private:
  UInt64 seed_;
  UInt64 steps_ = 0;  //step counter, used in serialization. It is important that steps_ is in sync with number of 
  // calls to RNG
  std::mt19937 gen; //Standard mersenne_twister_engine 64bit seeded with seed_
//  std::random_device rd; //HW random for random seed cases, undeterministic -> problems with op= and copy-constructor, therefore disabled

  // our reimpementation of std::shuffle, 
  // as the implementation is not dictated by the standard, 
  // resuting in differences between impementations (OS, stdlib,...) :(
  // https://en.cppreference.com/w/cpp/algorithm/random_shuffle 
  template<class RandomIt>
  void platform_independent_shuffle(RandomIt first, RandomIt last)
  {
    typename std::iterator_traits<RandomIt>::difference_type i, n;
    n = last - first;
    for (i = n-1; i > 0; --i) {
        using std::swap;
        swap(first[i], first[this->getUInt32(static_cast<UInt32>(i+1))]);
    }
  }
};

// serialization/deserialization
std::ostream &operator<<(std::ostream &, const Random &);
std::istream &operator>>(std::istream &, Random &);

// This function returns seeds from the Random singleton in our
// "universe" (application, plugin, python module). If, when the
// Random constructor is called, seeder_ is NULL, then seeder_ is
// set to this function. The plugin framework can override this
// behavior by explicitly setting the seeder to the RandomSeeder
// function provided by the application.
UInt32 GetRandomSeed();

} // namespace htm

#endif // NTA_RANDOM_HPP
