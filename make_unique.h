#pragma once

#ifndef __cpp_lib_make_unique

namespace std {
  template<typename T, typename... Ts>
  unique_ptr<T> make_unique(Ts&&... params)
  {
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
  }
}

#endif
