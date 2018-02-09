#ifndef DARRAY
#define DARRAY
#include <algorithm>
#include <array>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include "constexpr_cal.hpp"

namespace ccs {

template <typename T, int... Dims>
class Darray {
 public:
  using dimension_type = size_t;
  using storage_type =
      std::array<T, get_prod<dimension_type, sizeof...(Dims), Dims...>::answer>;
  using value_type = typename storage_type::value_type;
  using size_type = typename storage_type::size_type;
  using reference = typename storage_type::reference;
  using const_reference = typename storage_type::const_reference;
  using pointer = typename storage_type::pointer;
  using const_pointer = typename storage_type::const_pointer;
  using difference_type = typename storage_type::difference_type;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;
  using reverse_iterator = typename storage_type::reverse_iterator;
  using const_reverse_iterator = typename storage_type::const_reverse_iterator;

 private:
  storage_type arr;

 protected:
  static constexpr dimension_type dimension = sizeof...(Dims);
  static constexpr std::array<dimension_type, dimension> dims_length = {
      Dims...};
  const std::length_error LENGTH_ERROR =
      std::length_error("ERROR: too many initializers");
  const std::invalid_argument INVALIDE_ARGUMENT =
      std::invalid_argument("ERROR: too many initializers");

  constexpr size_type get_pos(T val_) const noexcept {
    return val_ * get_prod<dimension_type, dimension - 1, Dims...>::answer;
  }
  template <typename... Args, typename... Ds>
  constexpr size_type get_pos(T val_, Args... args_) const noexcept {
    return val_ * get_prod<dimension_type, dimension - 1 - sizeof...(args_),
                           Dims...>::answer +
           get_pos(args_...);
  }
  bool is_dimension_match(dimension_type dim_) const noexcept {
    return dim_ == dimension;
  }
  bool is_out_of_range(size_type range_) const noexcept {
    return range_ > arr.size();
  }

  void test_dimension(dimension_type sz_) const {
    if (!is_dimension_match(sz_)) {
      throw INVALIDE_ARGUMENT;
    }
    return;
  }
  void test_range(size_type rg_) const {
    if (is_out_of_range(rg_)) {
      throw LENGTH_ERROR;
    }
  }

 public:
  Darray(std::initializer_list<T> list_) {
    test_range(list_.size());
    std::copy(list_.begin(), list_.end(), arr.begin());
  }
  Darray() = default;
  virtual ~Darray() = default;

  template <typename... Args>
  reference at(Args... args_) {
    test_dimension(sizeof...(args_));
    return arr.at(get_pos(args_...));
  }
  template <typename... Args>
  const_reference at(Args... args_) const {
    test_dimension(sizeof...(args_));
    return arr.at(get_pos(args_...));
  }

  reference operator[](size_type i_) noexcept { return arr[i_]; };
  const_reference operator[](size_type i_) const noexcept { return arr[i_]; };

  iterator begin() noexcept { return arr.begin(); }
  const_iterator cbegin() const noexcept { return arr.cbegin(); }
  iterator end() noexcept { return arr.end(); }
  const_iterator cend() const noexcept { return arr.cend(); }
  reverse_iterator rbegin() noexcept { return arr.rbegin(); }
  const_reverse_iterator crbegin() const noexcept { return arr.crbegin(); }
  reverse_iterator rend() noexcept { return arr.rend(); }
  const_reverse_iterator crend() const noexcept { return arr.crend(); }
  constexpr size_type size() const noexcept { return arr.size(); }
  constexpr size_type max_size() const noexcept { return arr.max_size(); }
};
}  // namespace ccs

#endif