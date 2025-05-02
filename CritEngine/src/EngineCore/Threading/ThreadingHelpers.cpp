#include "ThreadingHelpers.h"

const unsigned int GetThreadCountEstimate()
{
    return std::thread::hardware_concurrency();
}