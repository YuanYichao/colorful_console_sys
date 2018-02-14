#include "color_ostream.hpp"
#include "wc_exception.h"
#include <algorithm>
#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#include <ostream>

void ccs::swap(Color_ostream &stm1, Color_ostream &stm2) noexcept
{
    using std::swap;

    swap(stm1.color_pram, stm2.color_pram);
    swap(stm1.std_handle, stm2.std_handle);
}

void ccs::Color_ostream::swap(Color_ostream &obj_) noexcept
{
    ccs::swap(*this, obj_);
}

ccs::Color_ostream::Color_ostream(color_type pram_)
    : color_pram(pram_)
{
    std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!std_handle)
        throw get_handle_failed();
}

ccs::Color_ostream &ccs::Color_ostream::operator=(Color_ostream obj_) noexcept
{
    ccs::swap(*this, obj_);
    return *this;
}

ccs::Color_ostream::~Color_ostream() noexcept
{
    set_color_bits(color_state::FWHITE);
}

ccs::Color_ostream &ccs::Color_ostream::set_color_bits(color_type pram_) noexcept
{
    color_pram = pram_;
    SetConsoleTextAttribute(std_handle, color_pram);
    return *this;
}

typename ccs::Color_ostream::color_type ccs::Color_ostream::current_color() noexcept
{
    return color_pram;
}

#endif