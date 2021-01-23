#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <cxxabi.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace sylar
{
    /**
    * @brief 返回当前线程的ID
    */
    pid_t GetThreadId();

    uint32_t GetFiberId()
    {
        return 0;
    }

} // namespace name


#endif