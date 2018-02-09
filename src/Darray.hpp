#include <algorithm>
#include <array>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include "constexpr_cal.hpp"
namespace ccs {
template <typename T, int... Dims>
class Darray
    : public std::array<
          T, get_prod<unsigned long int, sizeof...(Dims), Dims...>::answer> {
 private:
  using parent_type =
      std::array<T,
                 get_prod<unsigned long int, sizeof...(Dims), Dims...>::answer>;
  using dimension_type = unsigned long int;
  static constexpr dimension_type dimension = sizeof...(Dims);
  static constexpr std::array<T, dimension> dims_length = {Dims...};

 protected:
  typename parent_type::size_type get_pos(T val_) {
    return val_ * get_prod<dimension_type, dimension - 1, Dims...>::answer;
  }
  template <typename... Args, typename... Ds>
  typename parent_type::size_type get_pos(T val_, Args... args_) {
    return val_ * get_prod<dimension_type, dimension - 1 - sizeof...(args_),
                           Dims...>::answer +
           get_pos(args_...);
  }

 public:
  Darray(std::initializer_list<T> list_) {
    if (this->size() < list_.size())
      throw std::length_error("ERROR: too many initializers");
    std::copy(list_.begin(), list_.end(), this->begin());
  }
  Darray() = default;

  template <typename... Args>
  typename parent_type::value_type &at(Args... args_) {
    if (sizeof...(args_) != dimension) {
      throw std::invalid_argument(std::string("ERROR: dimension mismatches"));
    } else {
      return parent_type::at(get_pos(args_...));
    }
  }

  typename parent_type::value_type &operator[](typename parent_type::size_type i_) {
    return parent_type::operator[](i_);
  };
};
}  // namespace ccs
