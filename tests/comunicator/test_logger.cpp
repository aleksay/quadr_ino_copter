#include <test_logger.h>

uint8_t test_logger_init() {

  log_info("test logger info");
  log_warn("test logger warn ");

  return 0;
}
