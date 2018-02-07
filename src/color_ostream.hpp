#ifndef COLOR_OSTREAM
#define COLOR_OSTREAM

#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#include <iostream>

namespace ccs {
namespace color_state {
// single colors
const auto FRED_BIT = FOREGROUND_RED;
const auto FBLUE_BIT = FOREGROUND_BLUE;
const auto FGREEN_BIT = FOREGROUND_GREEN;
const auto FITS_BIT = FOREGROUND_INTENSITY;
const auto SFRED = FOREGROUND_RED | FITS_BIT;
const auto SFBLUE = FOREGROUND_BLUE | FITS_BIT;
const auto SFGREEN = FOREGROUND_GREEN | FITS_BIT;

const auto BRED_BIT = BACKGROUND_RED;
const auto BBLUE_BIT = BACKGROUND_BLUE;
const auto BGREEN_BIT = BACKGROUND_GREEN;
const auto BITS_BIT = BACKGROUND_INTENSITY;
const auto SBRED = BACKGROUND_RED | BITS_BIT;
const auto SBBLUE = BACKGROUND_BLUE | BITS_BIT;
const auto SBGREEN = BACKGROUND_GREEN | BITS_BIT;

// compound colors
const auto FDARK = 0, BDARK = 0;
const auto SFDARK = FITS_BIT;
const auto SBDARK = BITS_BIT;

// font colors
const auto FCYAN = FBLUE_BIT | FGREEN_BIT;
const auto SFCYAN = FCYAN | FITS_BIT;
const auto FPURPLE = FBLUE_BIT | FRED_BIT;
const auto SFPURPLE = FPURPLE | FITS_BIT;
const auto FYELLOW = FRED_BIT | FGREEN_BIT;
const auto SFYELLOW = FYELLOW | FITS_BIT;
const auto FWHITE = FRED_BIT | FGREEN_BIT | FBLUE_BIT;
const auto SFWHITE = FWHITE | FITS_BIT;

// background colors
const auto BCYAN = BBLUE_BIT | BGREEN_BIT;
const auto SBCYAN = BCYAN | BITS_BIT;
const auto BPURPLE = BBLUE_BIT | BRED_BIT;
const auto SBPURPLE = BPURPLE | BITS_BIT;
const auto BYELLOW = BRED_BIT | BGREEN_BIT;
const auto SBYELLOW = BYELLOW | BITS_BIT;
const auto BWHITE = BRED_BIT | BGREEN_BIT | BBLUE_BIT;
const auto SBWHITE = BWHITE | BITS_BIT;
}  // namespace color_state

class Flu {};
class End {};

class Color_ostream {
 public:
  using color_type = decltype(color_state::SFWHITE | color_state::SBWHITE);
  friend void swap(Color_ostream &, Color_ostream &) noexcept;
  void swap(Color_ostream &obj_) noexcept;

  // constructor, may throw get_hand_failed exception
  Color_ostream(color_type pram_ = color_state::FWHITE);

  // copy control
  Color_ostream(const Color_ostream &) = delete;
  Color_ostream(Color_ostream &&) = default;
  Color_ostream &operator=(Color_ostream) noexcept;
  ~Color_ostream() noexcept;

  // interface
  Color_ostream &set_color_bits(color_type) noexcept;
  color_type current_color() noexcept;
  template <typename T>
  Color_ostream &operator<<(const T &things_) noexcept {
    do_out(things_);
    return *this;
  }
  template <typename T>
  const Color_ostream &operator<<(const T &things_) const noexcept {
    do_out(things_);
    return *this;
  }

 protected:
  template <typename T>
  void do_out(const T &things_) noexcept {
    std::cout << things_;
  }
  void do_out(const Flu &t) { out << std::flush; }
  void do_out(const End &t) { out << std::endl; }

  std::ostream &out = std::cout;
  color_type color_pram;
  HANDLE std_handle;
};
void swap(Color_ostream &, Color_ostream &) noexcept;

static Color_ostream color_cout;
static const Flu flush;
static const End endl;
}  // namespace ccs

#endif

#endif