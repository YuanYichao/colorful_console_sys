#include "../include/color_ostream.hpp"
#include <string>
#include <utility>

int main() {
  system("cls");
  for (int i = 0; i < 25; i++)
    ccs::color_cout.set_color_bits(ccs::color_state::SBBLUE)
        << std::string(100, ' ') << ccs::endl;
  return 0;
}