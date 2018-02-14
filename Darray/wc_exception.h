#ifndef WC_EXCEPTION
#define WC_EXCEPTION

#if (defined(_WIN32) || defined(_WIN64))
#include <stdexcept>
#include <windows.h>
namespace ccs
{

class get_handle_failed : public std::runtime_error
{
  public:
    get_handle_failed() : runtime_error("failed to get handle\n") {}
};

};

#endif
#endif