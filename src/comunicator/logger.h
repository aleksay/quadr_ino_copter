#ifndef LOGGER_H
#define LOGGER_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifdef __AVR__

#include <avr/pgmspace.h>

#endif

// Remove text preceding / and \ from FILE preprocessor directive
#define __FILE_NO_PATH__                                                       \
  (strrchr(__FILE__, '/')    ? strrchr(__FILE__, '/') + 1                      \
   : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1                     \
                             : __FILE__)

#define log_err(M, ...)                                                        \
  fprintf_P(stdout, PSTR("[ERROR] (%s:%d) " M "\n"), __FILE_NO_PATH__,         \
            __LINE__, ##__VA_ARGS__)
#define log_warn(M, ...)                                                       \
  fprintf_P(stdout, PSTR("[WARN] (%s:%d) " M "\n"), __FILE_NO_PATH__,          \
            __LINE__, ##__VA_ARGS__)
#define log_info(M, ...)                                                       \
  fprintf_P(stdout, PSTR("[INFO] (%s:%d) " M "\n"), __FILE_NO_PATH__,          \
            __LINE__, ##__VA_ARGS__)

#endif
