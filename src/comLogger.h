#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <avr/pgmspace.h>

// Remove text preceding / and \ from FILE preprocessor directive
#define __FILE_NO_PATH__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)


#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf_P(stdout, PSTR("DEBUG %s:%d " M "\n"), __FILE_NO_PATH__, __LINE__, ##__VA_ARGS__)
#endif

#define log_err(M, ...) fprintf_P(stderr,PSTR("[ERROR] (%s:%d) " M "\n"), __FILE_NO_PATH__, __LINE__,  ##__VA_ARGS__)
#define log_warn(M, ...) fprintf_P(stderr, PSTR("[WARN] (%s:%d) " M "\n"), __FILE_NO_PATH__, __LINE__,  ##__VA_ARGS__)
#define log_info(M, ...) fprintf_P(stderr, PSTR("[INFO] (%s:%d) " M "\n"), __FILE_NO_PATH__, __LINE__, ##__VA_ARGS__)


/*
#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE_NO_PATH__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE_NO_PATH__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE_NO_PATH__, __LINE__, ##__VA_ARGS__)
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
#define check_mem(A) check((A), "Out of memory.")
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }
*/


#endif


