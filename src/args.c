#include <arpa/inet.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "args.h"
#include "utils.h"

mm_args_t *init_args() {
  mm_args_t *a = malloc(sizeof(mm_args_t));

  if (a == NULL) {
    return NULL;
  }

  *a = (mm_args_t){
      .source_ipv4 = (in_addr_t)(-1),
      .source_mac_address = {-1},
      .target_ipv4 = 0,
      .target_mac_address = {-1},

      .verbose = false,
      .announce = false,
  };

  return a;
}

void cleanup_args(mm_args_t *a) {
  if (a == NULL) {
    return;
  }
  free(a);
}

void _print_help(char *name) {
  printf(
      "Usage: %s [options] <source_ip> <source_mac> <target_ip> <target_mac>\n",
      name);
  printf("Options:\n");
  printf("  -v, --verbose                 Verbose output\n");
  printf("  -a, --announce                Send gratuitous ARP messages\n");
  printf("  -h, --help                    Print this help message\n");
}

bool _parse_mac_address(char *mac_address, uint8_t *dest) {
  unsigned long i = 0, len = strlen(mac_address);

  while (i < len) {
    if (i % 3 == 2) {
      if (mac_address[i] != ':') {
        return false;
      }
      i++;
    } else {
      long val = 0;
      if (hextodec(mac_address + i, &val, 2) == false) {
        return false;
      }
      dest[i / 3] = val;
      i += 2;
    }
  }

  return true;
}

bool parse_args(mm_args_t *args, int argc, char **argv) {
  if (args == NULL || argc < 1 || argv == NULL || *argv == NULL) {
    return false;
  }

  int i = 1;

  for (; i < argc; i++) {
    if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
      args->verbose = true;
    } else if (strcmp(argv[i], "-a") == 0 ||
               strcmp(argv[i], "--announce") == 0) {
      args->announce = true;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
#ifndef TESTING
      _print_help(argv[0]);
#endif
      cleanup_args(args);
      exit(0);
    } else if (isdigit(argv[i][0])) {
      break;
    } else {
#ifndef TESTING
      fprintf(stderr, "Unknown option: %s\n", argv[i]);
      _print_help(argv[0]);
#endif
      return false;
    }
  }

  if (i < argc) {
    args->source_ipv4 = inet_addr(argv[i]);
    if (args->source_ipv4 == (in_addr_t)(-1)) {
#ifndef TESTING
      fprintf(stderr, "Invalid source IP address: %s\n", argv[i]);
      _print_help(argv[0]);
#endif
      return false;
    }
    i++;
  } else {
#ifndef TESTING
    fprintf(stderr, "Missing source IP address\n");
    _print_help(argv[0]);
#endif
    return false;
  }

  if (i < argc) {
    if (_parse_mac_address(argv[i], args->source_mac_address) == false) {
#ifndef TESTING
      fprintf(stderr, "Invalid source MAC address: %s\n", argv[i]);
      _print_help(argv[0]);
#endif
      return false;
    }
    i++;
  } else {
#ifndef TESTING
    fprintf(stderr, "Missing source MAC address\n");
    _print_help(argv[0]);
#endif
    return false;
  }

  if (i < argc) {
    args->target_ipv4 = inet_addr(argv[i]);
    if (args->target_ipv4 == (in_addr_t)(-1)) {
#ifndef TESTING
      fprintf(stderr, "Invalid target IP address: %s\n", argv[i]);
      _print_help(argv[0]);
#endif
      return false;
    }
    i++;
  } else {
#ifndef TESTING
    fprintf(stderr, "Missing target IP address\n");
    _print_help(argv[0]);
#endif
    return false;
  }

  if (i < argc) {
    if (_parse_mac_address(argv[i], args->target_mac_address) == false) {
#ifndef TESTING
      fprintf(stderr, "Invalid target MAC address: %s\n", argv[i]);
      _print_help(argv[0]);
#endif
      return false;
    }
    i++;
  } else {
#ifndef TESTING
    fprintf(stderr, "Missing target MAC address\n");
    _print_help(argv[0]);
#endif
    return false;
  }

  for (; i < argc; i++) {
#ifndef TESTING
    fprintf(stderr, "Unknown argument: %s\n", argv[i]);
    _print_help(argv[0]);
#endif
    return false;
  }

  return true;
}

void _mac_address_to_string(uint8_t *address, char *dest) {
  sprintf(dest, "%02x:%02x:%02x:%02x:%02x:%02x", address[0], address[1],
          address[2], address[3], address[4], address[5]);
}

void _ipv4_to_string(in_addr_t ip, char *dest) {
  uint8_t *address = (uint8_t *)&ip;
  sprintf(dest, "%d.%d.%d.%d", address[0], address[1], address[2], address[3]);
}

void print_args(mm_args_t *a) {
  printf("[DEBUG] Arguments:\n");

  char source_ip[INET_ADDRSTRLEN];
  _ipv4_to_string(a->source_ipv4, source_ip);
  printf("  Source IP: %s\n", source_ip);

  char source_mac_address[18];
  _mac_address_to_string(a->source_mac_address, source_mac_address);
  printf("  Source MAC Address: %s\n", source_mac_address);

  char target_ip[INET_ADDRSTRLEN];
  _ipv4_to_string(a->target_ipv4, target_ip);
  printf("  Target IP: %s\n", target_ip);

  char target_mac_address[18];
  _mac_address_to_string(a->source_mac_address, target_mac_address);
  printf("  Target MAC Address: %s\n", target_mac_address);

  printf("  Verbose: %s\n", a->verbose ? "true" : "false");
  printf("  Announce: %s\n", a->announce ? "true" : "false");
}