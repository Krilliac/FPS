#pragma once
#include "../Core/framework.h"

#ifdef _DEBUG
    #define SPARK_ASSERT(cond) assert(cond)
    #define SPARK_ASSERT_MSG(cond,msg) assert((cond)&&(msg))
#else
    #define SPARK_ASSERT(cond) ((void)0)
    #define SPARK_ASSERT_MSG(cond,msg) ((void)0)
#endif

#ifndef ASSERT
    #define ASSERT(cond) SPARK_ASSERT(cond)
#endif
#ifndef ASSERT_MSG
    #define ASSERT_MSG(cond,msg) SPARK_ASSERT_MSG(cond,msg)
#endif
