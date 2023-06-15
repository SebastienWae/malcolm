#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "arp.h"

mm_arp_packet_t *parse_arp_packet(uint8_t *buffer) {
  mm_arp_packet_t *p = malloc(sizeof(mm_arp_packet_t));

  if (p == NULL) {
    return NULL;
  }

  *p = (mm_arp_packet_t){
      .hardware_type = ntohs(((uint16_t *)buffer)[0]),
      .protocol_type = ntohs(((uint16_t *)buffer)[2]),
      .hardware_address_length = buffer[4],
      .protocol_address_length = buffer[5],
      .opcode = ntohs(((uint16_t *)buffer)[6]),
      .sender_hardware_address = buffer + 8,
      .sender_protocol_address = buffer + 14,
      .target_hardware_address = buffer + 18,
      .target_protocol_address = buffer + 24,
  };

  return p;
}

void free_arp_packet(mm_arp_packet_t *p) {
  if (p == NULL) {
    return;
  }
  free(p);
}

char *arp_hardware_type_to_string(uint16_t hardware_type) {
  switch (hardware_type) {
  case 1:
    return "Ethernet";
  default:
    return "Unknown";
  }
}

char *arp_protocol_type_to_string(uint16_t protocol_type) {
  switch (protocol_type) {
  case 0x0800:
    return "IPv4";
  default:
    return "Unknown";
  }
}

char *arp_opcode_to_string(uint16_t opcode) {
  switch (opcode) {
  case 1:
    return "Request";
  case 2:
    return "Response";
  default:
    return "Unknown";
  }
}

char *arp_hardware_address_to_string(uint16_t hardware_type, uint8_t *address,
                                     uint8_t length) {
  if (hardware_type == 1) { // ethernet
    char *string = malloc(length * 3);

    if (string == NULL) {
      return NULL;
    }

    int c = 0;
    for (int i = 0; i < length; i++) {
      c += sprintf(string + c, "%02x", address[i]);
      if (i < length - 1) {
        c += sprintf(string + c, ":");
      }
    }

    string[c] = '\0';

    return string;
  } else {
    return NULL;
  }
}

char *arp_protocol_address_to_string(uint16_t protocol_type, uint8_t *address,
                                     uint8_t length) {
  if (protocol_type == 0x0800) { // ipv4
    char *string = malloc(length * 4);

    if (string == NULL) {
      return NULL;
    }

    int c = 0;
    for (int i = 0; i < length; i++) {
      c += sprintf(string + c, "%d", address[i]);
      if (i < length - 1) {
        c += sprintf(string + c, ".");
      }
    }

    string[c] = '\0';

    return string;
  } else {
    return NULL;
  }
}

void print_arp_packet(mm_arp_packet_t *p) {
  if (p == NULL) {
    return;
  }

  printf("ARP Packet Received:\n");

  char *hardware_type = arp_hardware_type_to_string(p->hardware_type);
  printf("  Hardware Type: %s (%d)\n", hardware_type, p->hardware_type);

  char *protocol_type = arp_protocol_type_to_string(p->protocol_type);
  printf("  Protocol Type: %s (%#x)\n", protocol_type, p->protocol_type);

  printf("  Hardware Address Length: %d\n", p->hardware_address_length);
  printf("  Protocol Address Length: %d\n", p->protocol_address_length);

  char *operation = arp_opcode_to_string(p->opcode);
  printf("  Operation: %s (%d)\n", operation, p->opcode);

  char *sender_hardware_address = arp_hardware_address_to_string(
      p->hardware_type, p->sender_hardware_address, p->hardware_address_length);
  if (sender_hardware_address == NULL) {
    printf("  Sender Hardware Address: Unknown\n");
  } else {
    printf("  Sender Hardware Address: %s\n", sender_hardware_address);
    free(sender_hardware_address);
  }

  char *sender_protocol_address = arp_protocol_address_to_string(
      p->protocol_type, p->sender_protocol_address, p->protocol_address_length);
  if (sender_protocol_address == NULL) {
    printf("  Sender Protocol Address: Unknown\n");
  } else {
    printf("  Sender Protocol Address: %s\n", sender_protocol_address);
    free(sender_protocol_address);
  }

  char *target_hardware_address = arp_hardware_address_to_string(
      p->hardware_type, p->target_hardware_address, p->hardware_address_length);
  if (target_hardware_address == NULL) {
    printf("  Target Hardware Address: Unknown\n");
  } else {
    printf("  Target Hardware Address: %s\n", target_hardware_address);
    free(target_hardware_address);
  }

  char *target_protocol_address = arp_protocol_address_to_string(
      p->protocol_type, p->target_protocol_address, p->protocol_address_length);
  if (target_protocol_address == NULL) {
    printf("  Target Protocol Address: Unknown\n");
    return;
  } else {
    printf("  Target Protocol Address: %s\n", target_protocol_address);
    free(target_protocol_address);
  }
}