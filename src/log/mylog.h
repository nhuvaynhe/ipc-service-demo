#include <stdio.h>

// Extracts only the file name from __FILE__
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

// Log level control (0 = ERROR only, 3 = FULL DEBUG)
#define LOG_LEVEL   3

#if LOG_LEVEL >= 0
#define LOG_ERROR(fmt, ...)   printf("[ERROR] [%s] " fmt "\n", __func__, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)
#endif

#if LOG_LEVEL >= 1
#define LOG_WARN(fmt, ...)    printf("[WARN ] [%s] " fmt "\n", __func__, ##__VA_ARGS__)
#else
#define LOG_WARN(fmt, ...)
#endif

#if LOG_LEVEL >= 2
#define LOG_INFO(fmt, ...)    printf("[INFO ] [%s] " fmt "\n", __func__, ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)
#endif

#if LOG_LEVEL >= 3
#define LOG_DEBUG(fmt, ...)   printf("[DEBUG] [%s] " fmt "\n", __func__, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#endif
