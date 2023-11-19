#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <atomic>

#include "Utility.hpp"

extern Utility util;

extern std::atomic<uint64_t> numRays;
extern std::atomic<uint64_t> numBVIntersections;
extern std::atomic<uint64_t> numObjectIntersections;

#endif