#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>
#include <stdint.h>

typedef struct mm_args {
  uint32_t source_ipv4;
  uint8_t source_mac_address[6];
  uint32_t target_ipv4;
  uint8_t target_mac_address[6];

  bool verbose;
  bool announce;
} mm_args_t;

mm_args_t *init_args();
void cleanup_args(mm_args_t *a);

bool parse_args(mm_args_t *args, int argc, char **argv);
void print_args(mm_args_t *a);

#endif /* ARGS_H */
