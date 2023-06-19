#ifndef ARP_H
#define ARP_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>

#include "args.h"

#define ARP_PACKET_SIZE 28
#define ARP_HARDWARE_TYPE_ETHERNET 1
#define ARP_PROTOCOL_TYPE_IPV4 0x0800
#define ARP_OPCODE_REQUEST 1
#define ARP_OPCODE_REPLY 2
#define ARP_HARDWARE_ADDRESS_LENGTH 6

typedef struct mm_arp_packet {
  uint16_t hardware_type;
  uint16_t protocol_type;
  uint8_t hardware_address_length;
  uint8_t protocol_address_length;
  uint16_t opcode;
  uint8_t sender_hardware_address[ARP_HARDWARE_ADDRESS_LENGTH];
  uint32_t sender_protocol_address;
  uint8_t target_hardware_address[ARP_HARDWARE_ADDRESS_LENGTH];
  uint32_t target_protocol_address;
} mm_arp_packet_t;

mm_arp_packet_t *create_arp_reply(mm_arp_packet_t *request, mm_args_t *args);
bool parse_arp_packet(mm_arp_packet_t **packet, const uint8_t *buffer);
void free_arp_packet(mm_arp_packet_t **p);
bool send_arp_packet(int sfd, struct sockaddr_storage fromaddr,
                     socklen_t fromlen, mm_arp_packet_t *p, bool verbose);
void print_arp_packet(mm_arp_packet_t *p);

#endif /* ARP_H */
