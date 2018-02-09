#ifndef CONSTEXPR_CAL
#define CONSTEXPR_CAL
#include <numeric>
namespace ccs {
template <typename T, unsigned int N, T val_, T... args_>
struct get_sum;
template <typename T, T val_, T... args_>
struct get_sum<T, 1, val_, args_...>;
template <typename T, T val_, T... args_>
struct get_sum<T, 0, val_, args_...>;
template <typename T, unsigned int N, T val_, T... args_>
struct get_prod;
template <typename T, T val_, T... args_>
struct get_prod<T, 1, val_, args_...>;
template <typename T, T val_, T... args_>
struct get_prod<T, 0, val_, args_...>;

template <typename T, unsigned int N, T val_, T... args_>
struct get_sum {
  static constexpr T answer = val_ + get_sum<T, N - 1, args_...>::answer;
};

template <typename T, T val_, T... args_>
struct get_sum<T, 1, val_, args_...> {
  static constexpr T answer = val_;
};

template <typename T, T val_, T... args_>
struct get_sum<T, 0, val_, args_...> {
  static constexpr T answer = 0;
};

template <typename T, unsigned int N, T val_, T... args_>
struct get_prod {
  static constexpr T answer = val_ * get_prod<T, N - 1, args_...>::answer;
};

template <typename T, T val_, T... args_>
struct get_prod<T, 1, val_, args_...> {
  static constexpr T answer = val_;
};

template <typename T, T val_, T... args_>
struct get_prod<T, 0, val_, args_...> {
  static constexpr T answer = 1;
};
}  // namespace ccs
#endif