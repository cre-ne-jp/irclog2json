#pragma once

#include <doctest/doctest.h>

#include <picojson.h>

#define CHECK_OBJ_STR_EQ(obj, key, expected) \
  do { \
    const auto& v = (obj).at((key)).get<std::string>(); \
    CHECK_MESSAGE(v.compare((expected)) == 0, "actual: ", v); \
  } while (false)
