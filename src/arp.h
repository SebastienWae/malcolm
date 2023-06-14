#ifndef ARP_H
#define ARP_H

#include <stdint.h>

#define ARP_PACKET_SIZE 28

typedef struct mm_arp_packet {
  uint16_t hardware_type;
  uint16_t protocol_type;
  uint8_t hardware_address_length;
  uint8_t protocol_address_length;
  uint16_t opcode;
  uint8_t *sender_hardware_address;
  uint8_t *sender_protocol_address;
  uint8_t *target_hardware_address;
  uint8_t *target_protocol_address;
} mm_arp_packet_t;

mm_arp_packet_t *parse_arp_packet(uint8_t *buffer);
void free_arp_packet(mm_arp_packet_t *p);
void print_arp_packet(mm_arp_packet_t *p);

#endif /* ARP_H */
