#include <errno.h>
#include <fcntl.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "args.h"
#include "arp.h"

bool run(int sfd, mm_args_t *args) {
  int size;
  uint8_t buffer[ARP_PACKET_SIZE];

  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    fprintf(stderr, "Error setting socket option: %s\n", strerror(errno));
  }

  time_t last_announce = time(NULL);

  while (true) {
    size = recvfrom(sfd, buffer, ARP_PACKET_SIZE, 0, NULL, NULL);

    if (size < 0) {
      if (errno != EAGAIN && errno != EWOULDBLOCK) {
        fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
        return false;
      }
    } else {
      mm_arp_packet_t *p = NULL;
      if (parse_arp_packet(&p, buffer) == false) {
        free_arp_packet(p);
        return false;
      }

      printf("New ARP packet received\n");

      if (args->verbose) {
        print_arp_packet(p);
      }

      if (p->sender_hardware_address == args->target_mac_address &&
          *(uint32_t *)p->sender_protocol_address == args->target_ipv4) {
        printf("ARP sender match target\n");
        if (send_arp_packet(&(mm_arp_packet_t){}) == false) {
          fprintf(stderr, "Error sending ARP reply");
        } else {
          printf("Spoofed ARP reply sent");
        }
      } else {
        printf("ARP sender doest not match target\n");
      }

      free_arp_packet(p);
    }

    if (time(NULL) - last_announce > 1000 && args->announce) {
      printf("Sending ARP announce");
      if (send_arp_packet(&(mm_arp_packet_t){}) == false) {
        fprintf(stderr, "Error sending ARP announce");
      } else {
        printf("ARP annouce sent");
      }
      last_announce = time(NULL);
    }
  }

  return true;
}

int main(int argc, char **argv) {
  mm_args_t *args = init_args();

  if (parse_args(args, argc, argv) == false) {
    cleanup_args(args);
    return EXIT_FAILURE;
  }

  if (args->verbose) {
    print_args(args);
  }

  int sfd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
  if (sfd < 0) {
    fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
    cleanup_args(args);
    return EXIT_FAILURE;
  }

  bool error = run(sfd, args);

  close(sfd);

  cleanup_args(args);

  return error ? EXIT_FAILURE : EXIT_SUCCESS;
}
