#ifndef DARRAY
#define DARRAY
#include <algorithm>
#include <array>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include "constexpr_cal.hpp"

namespace ccs {
/**
 * The abstract base class of Darray and Darray_ptr, which only provide some
 * basic methods and interfaces.
 *
 * @param T the type stored inside the array
 * @param Dims template parameter pack, the length of each dimension
 */
template <typename T, int... Dims>
class Darray_base {
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
  /** virtual destructor*/
  virtual ~Darray_base() {}
  /** an at function, may throw
   *
   * @param several size_type integers indicates location
   */
  template <typename... Args>
  reference at(Args... args_) {
    test_dimension(sizeof...(args_));
    return do_at(get_pos(args_...));
  }
  /** a const version of at function
   *
   * @see template <typename... Args> reference Darray_base::at(Args... args_)
   */
  template <typename... Args>
  const_reference at(Args... args_) const {
    test_dimension(sizeof...(args_));
    return do_at(get_pos(args_...));
  }
  /** a pure virtual overloaded operator[]
   *
   * no boundary condition test, no exception throw. but make sure that i_ is in
   * range
   * @param i_ an size_type integer indicates index
   */
  virtual reference operator[](size_type i_) noexcept = 0;
  virtual const_reference operator[](size_type i_) const noexcept = 0;
  /** a function return the size of the array, return constant expression*/
  constexpr size_type size() const noexcept {
    return get_prod<dimension_type, sizeof...(Dims), Dims...>::answer;
  }
  /** exactly the same as size(). */
  constexpr size_type max_size() const noexcept { return size(); };

  virtual iterator begin() noexcept = 0;
  virtual const_iterator cbegin() const noexcept = 0;
  virtual iterator end() noexcept = 0;
  virtual const_iterator cend() const noexcept = 0;
  virtual reverse_iterator rbegin() noexcept = 0;
  virtual const_reverse_iterator crbegin() const noexcept = 0;
  virtual reverse_iterator rend() noexcept = 0;
  virtual const_reverse_iterator crend() const noexcept = 0;

 protected:
  /** a class static const expression variable.
   * indicates the number of dimension of this type
   */
  static constexpr dimension_type dimension = sizeof...(Dims);
  /** a class static const expression std::array
   * stores the length of each dimension
   */
  static constexpr std::array<dimension_type, dimension> dims_length = {
      Dims...};
  /** a class const exception object
   * indicates there are too much initializers
   */
  const std::length_error LENGTH_ERROR =
      std::length_error("ERROR: too many initializers");
  /** a class const exception object
   * indicates there are too many arguments, which does not match the dimension
   * number of this array
   */
  const std::invalid_argument INVALIDE_ARGUMENT =
      std::invalid_argument("ERROR: too many arguments");
  /** function to end template get_pos
   *
   * @see template <typename... Args, typename... Ds> constexpr size_type
   * Darray_base::get_pos(T val_, Args... args_) const noexcept;
   */
  constexpr size_type get_pos(T val_) const noexcept {
    return val_ * get_prod<dimension_type, dimension - 1, Dims...>::answer;
  }
  /** template function to convert multi-parameter-based location to
   * single-parameter-based location
   *
   * @param val_ should be an integer
   * @param Ds function parameter pack, also should be composed of several
   * integers
   * @return single-parameter-based location with same type as the input, which
   * can be used by an std::array or built-in array to access element.
   */
  template <typename... Args, typename... Ds>
  constexpr size_type get_pos(T val_, Args... args_) const noexcept {
    return val_ * get_prod<dimension_type, dimension - 1 - sizeof...(args_),
                           Dims...>::answer +
           get_pos(args_...);
  }
  /** a pure virtual function do the actual work of at*/
  virtual reference do_at(size_type pos) noexcept = 0;
  /** a pure virtual function do the actual work of at, const version*/
  virtual const_reference do_at(size_type pos) const noexcept = 0;
  /** a function judges whether input parameters valid in dimension numbers*/
  bool is_dimension_match(dimension_type dim_) const noexcept {
    return dim_ == dimension;
  }
  /** a pure virtual function judges whether an index is out of range*/
  /** a function override the pure virtual function in Darray base*/
  bool is_out_of_range(size_type range_) const noexcept {
    return range_ > size();
  }
  /** a function tests the validness of dimension number
   *
   * @param sz_ size_type integer
   * @excepion std::invalid_argument
   */
  void test_dimension(dimension_type sz_) const {
    if (!is_dimension_match(sz_)) {
      throw INVALIDE_ARGUMENT;
    }
    return;
  }
  /** a function tests the validness of index
   *
   * @param rg_ size_type integer
   * @excepion std::length_error
   */
  void test_range(size_type rg_) const {
    if (is_out_of_range(rg_)) {
      throw LENGTH_ERROR;
    }
  }
};

template <typename T, int... Dims>
class Darray;

template <typename Farr, typename T, int... Dims>
class Darray_slice;

template <typename T, int... Dims>
void swap(Darray<T, Dims...> &arr1, Darray<T, Dims...> &arr2) noexcept;

template <typename Farr, typename T, int... Dims>
void swap(Darray_slice<Farr, T, Dims...> &arr1,
          Darray_slice<Farr, T, Dims...> &arr2) noexcept;

/** type traits class in order to get corresponding Darray_slice type*/
template <typename Farr, typename T, int sz_, int val_, int... Dims>
struct sub_itr {
  using itr_type = typename sub_itr<Farr, T, sz_ - 1, Dims..., val_>::itr_type;
};
/** end recursion*/
template <typename Farr, typename T, int val_, int... Dims>
struct sub_itr<Farr, T, 1, val_, Dims...> {
  using itr_type = Darray_slice<Farr, T, Dims...>;
};

/**
 * a class public inherited from Darray_base, which has value-like
 * behaviours.
 *
 * @see Darray_base
 */
template <typename T, int... Dims>
class Darray : public Darray_base<T, Dims...> {
 public:
  friend void swap<T, Dims...>(Darray<T, Dims...> &arr1,
                               Darray<T, Dims...> &arr2) noexcept;
  using parent_type = Darray_base<T, Dims...>;
  using slice_type =
      typename sub_itr<Darray<T, Dims...>, T, parent_type::dimension,
                       Dims...>::itr_type;
  using const_slice_type = const slice_type;
  friend slice_type;
  friend const_slice_type;
  using dimension_type = typename parent_type::dimension_type;
  using storage_type = typename parent_type::storage_type;
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

 protected:
  using count_type = unsigned int;
  /** reference count provided for Darray_slice type*/
  count_type *cptr;

 public:
  /** conversion constructor
   * provide the ability to be list-initialized
   *
   * @param list_ a std::initializer_list<T> object
   * @excepion std::bad_alloc
   */
  Darray(std::initializer_list<T> list_)
      : arr_ptr(new storage_type), cptr(new count_type(1)) {
    this->test_range(list_.size());
    std::copy(list_.begin(), list_.end(), arr_ptr->begin());
  }
  /** defualt constructor
   *
   * the elements in the array is defualt constructed
   * @excepion std::bad_alloc
   */
  Darray() : arr_ptr(new storage_type), cptr(new count_type(1)) {}
  /** copy constructor
   *
   * @exception std::bad_alloc
   */
  Darray(const Darray &arr)
      : arr_ptr(new storage_type), cptr(new count_type(1)) {
    std::copy(arr.cbegin(), arr.cend(), arr_ptr->begin());
  }
  /** move constructor*/
  Darray(Darray &&arr) noexcept {
    arr_ptr = arr.arr_ptr;
    cptr = arr.cptr;
    arr.arr_ptr = nullptr;
    arr.cptr = nullptr;
  }
  /** copy&&move- assignment operator*/
  Darray &operator=(Darray arr) { ccs::swap(*this, arr); }
  virtual ~Darray() {
    delete arr_ptr;
    if (--*cptr == 0) delete cptr;
  };
  void swap(Darray &arr) noexcept { ccs::swap(*this, arr); }
  /** override Darray_base::operator[]*/
  virtual reference operator[](size_type i_) noexcept override {
    return (*arr_ptr)[i_];
  };
  /** override Darray_base::operator[]*/
  virtual const_reference operator[](size_type i_) const noexcept override {
    return (*arr_ptr)[i_];
  };

  virtual iterator begin() noexcept override { return arr_ptr->begin(); }
  virtual const_iterator cbegin() const noexcept override {
    return arr_ptr->cbegin();
  }
  virtual iterator end() noexcept override { return arr_ptr->end(); }
  virtual const_iterator cend() const noexcept override {
    return arr_ptr->cend();
  }
  virtual reverse_iterator rbegin() noexcept override {
    return arr_ptr->rbegin();
  }
  virtual const_reverse_iterator crbegin() const noexcept override {
    return arr_ptr->crbegin();
  }
  virtual reverse_iterator rend() noexcept override { return arr_ptr->rend(); }
  virtual const_reverse_iterator crend() const noexcept override {
    return arr_ptr->crend();
  }

  slice_type sbegin() { return slice_type(arr_ptr->begin(), cptr); }
  const_slice_type csbegin() const {
    return slice_type(arr_ptr->begin(), cptr);
  }
  slice_type send() { return slice_type(arr_ptr->end(), cptr); }
  const_slice_type csend() const { return slice_type(arr_ptr->end(), cptr); }

 protected:
  /** a pointer point to storage memory*/
  storage_type *arr_ptr;

  /** override pure virtual function in Darray_base*/
  virtual reference do_at(size_type pos_) noexcept override {
    return arr_ptr->at(pos_);
  }
  /** override pure virtual function in Darray_base*/
  virtual const_reference do_at(size_type pos_) const noexcept override {
    return arr_ptr->at(pos_);
  }
};

template <typename T, int... Dims>
void swap(Darray<T, Dims...> &arr1, Darray<T, Dims...> &arr2) noexcept {
  using std::swap;
  swap(arr1.arr_ptr, arr2.arr_ptr);
  swap(arr1.cptr, arr2.cptr);
}

// /** overloaded operator== for Darray_slice type*/
// template <typename Farr, typename T, int... Dims>
// bool operator==(const Darray_slice<Farr, T, Dims...> &s1,
//                 const Darray_slice<Farr, T, Dims...> &s2) noexcept;

/** an adaptor-like objct.
 *
 * can only be constructed by Darray as a return value.
 * all its content is all based on the cotent of the Darray which generates it.
 * provide all common methods that can be used in Darray.
 * @param Farr the type which generates it
 * @param typename the type "stored" inside the slice
 * @param Dims dimensions' length of the slice, which is always identical to the
 * Farr except the last dimension
 */
template <typename Farr, typename T, int... Dims>
class Darray_slice : public Darray_base<T, Dims...> {
 public:
  // friend operator==
  //     <Farr, T, Dims...>(const Darray_slice<Farr, T, Dims...> &,
  //                        const Darray_slice<Farr, T, Dims...> &) noexcept;
  using based_on_type = Farr;
  friend based_on_type;
  using parent_type = Darray_base<T, Dims...>;
  using dimension_type = typename parent_type::dimension_type;
  using storage_type = typename parent_type::storage_type;
  using value_type = typename based_on_type::value_type;
  using size_type = typename based_on_type::size_type;
  using reference = typename based_on_type::reference;
  using const_reference = typename based_on_type::const_reference;
  using pointer = typename based_on_type::pointer;
  using const_pointer = typename based_on_type::const_pointer;
  using difference_type = typename based_on_type::difference_type;
  using iterator = typename based_on_type::iterator;
  using const_iterator = typename based_on_type::const_iterator;
  using reverse_iterator = typename based_on_type::reverse_iterator;
  using const_reverse_iterator = typename based_on_type::const_reverse_iterator;

  Darray_slice &operator++() noexcept {
    start_ptr = start_ptr + this->size();
    return *this;
  }
  Darray_slice operator++(int i) noexcept {
    auto t = *this;
    start_ptr = start_ptr + this->size();
    return t;
  }
  Darray_slice &operator--() noexcept {
    start_ptr = start_ptr - this->size();
    return *this;
  }
  Darray_slice &operator--(int)noexcept {
    auto t = *this;
    start_ptr = start_ptr - this->size();
    return t;
  }

  virtual reference operator[](size_type i_) noexcept override {
    return *(start_ptr + i_);
  }
  virtual const_reference operator[](size_type i_) const noexcept override {
    return *(start_ptr + i_);
  }

  virtual iterator begin() noexcept override { return start_ptr; }
  virtual const_iterator cbegin() const noexcept override { return start_ptr; }
  virtual iterator end() noexcept override { return start_ptr + this->size(); }
  virtual const_iterator cend() const noexcept override {
    return start_ptr + this->size();
  }
  virtual reverse_iterator rbegin() noexcept override {
    return std::make_reverse_iterator(end());
  }
  virtual const_reverse_iterator crbegin() const noexcept override {
    return std::make_reverse_iterator(cend());
  }
  virtual reverse_iterator rend() noexcept override {
    return std::make_reverse_iterator(begin());
  }
  virtual const_reverse_iterator crend() const noexcept override {
    return std::make_reverse_iterator(cbegin());
  }

 protected:
  using count_type = typename Farr::count_type;
  Darray_slice(iterator it_, count_type *cptr_) noexcept
      : start_ptr(it_), cptr(cptr_) {}
  iterator start_ptr;
  count_type *cptr;
  /** override pure virtual function in Darray_base*/
  virtual reference do_at(size_type pos_) noexcept override {
    return start_ptr[pos_];
  }
  /** override pure virtual function in Darray_base*/
  virtual const_reference do_at(size_type pos_) const noexcept override {
    return start_ptr[pos_];
  }
};

}  // namespace ccs

#endif