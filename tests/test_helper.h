#pragma once

#include <doctest/doctest.h>

#include <picojson.h>

#define CHECK_OBJ_STR_EQ(obj, key, expected)                                   \
  do {                                                                         \
    const auto& v = (obj).at((key)).get<std::string>();                        \
    CHECK_EQ(v, (expected));                                                   \
  } while (false)
