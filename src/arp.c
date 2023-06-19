#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "args.h"
#include "arp.h"

mm_arp_packet_t *create_arp_reply(mm_arp_packet_t *request, mm_args_t *args) {
  mm_arp_packet_t *p = malloc(sizeof(mm_arp_packet_t));

  if (p == NULL) {
    return NULL;
  }

  *p = (mm_arp_packet_t){
      .hardware_type = ARP_HARDWARE_TYPE_ETHERNET,
      .protocol_type = ARP_PROTOCOL_TYPE_IPV4,
      .hardware_address_length = ARP_HARDWARE_ADDRESS_LENGTH,
      .protocol_address_length = sizeof(p->sender_protocol_address),
      .opcode = ARP_OPCODE_REPLY,
      .sender_protocol_address = args->source_ipv4,
      .target_protocol_address = request->sender_protocol_address,
  };

  memcpy(p->sender_hardware_address, args->source_mac_address,
         ARP_HARDWARE_ADDRESS_LENGTH);
  memcpy(p->target_hardware_address, request->sender_hardware_address,
         ARP_HARDWARE_ADDRESS_LENGTH);

  return p;
}

bool send_arp_packet(int sfd, struct sockaddr_storage fromaddr,
                     socklen_t fromlen, mm_arp_packet_t *p, bool verbose) {
  uint8_t buffer[ARP_PACKET_SIZE] = {0};

  *(uint16_t *)buffer = htons(p->hardware_type);
  *(uint16_t *)(buffer + 2) = htons(p->protocol_type);
  *(buffer + 4) = p->hardware_address_length;
  *(buffer + 5) = p->protocol_address_length;
  *(uint16_t *)(buffer + 6) = htons(p->opcode);
  memcpy(buffer + 8, p->sender_hardware_address, p->hardware_address_length);
  memcpy(buffer + 14, &p->sender_protocol_address, p->protocol_address_length);
  memcpy(buffer + 18, p->target_hardware_address, p->hardware_address_length);
  memcpy(buffer + 24, &p->target_protocol_address, p->protocol_address_length);

  if (verbose) {
    print_arp_packet(p);
  }

  if (sendto(sfd, &buffer, ARP_PACKET_SIZE, 0, (struct sockaddr *)&fromaddr,
             fromlen) < 0) {
    return false;
  }

  return true;
}

bool parse_arp_packet(mm_arp_packet_t **p, const uint8_t *buffer) {
  if (buffer == NULL) {
    return false;
  }

  if (ntohs(*(uint16_t *)buffer) != ARP_HARDWARE_TYPE_ETHERNET ||
      ntohs(*(uint16_t *)(buffer + 2)) != ARP_PROTOCOL_TYPE_IPV4) {
    fprintf(stderr, "Unsupported ARP packet\n");
    return false;
  }

  if (p == NULL) {
    return false;
  }

  *p = malloc(sizeof(mm_arp_packet_t));

  if (*p == NULL) {
    return false;
  }

  **p = (mm_arp_packet_t){
      .hardware_type = ntohs(*(uint16_t *)buffer),
      .protocol_type = ntohs(*(uint16_t *)(buffer + 2)),
      .hardware_address_length = *(buffer + 4),
      .protocol_address_length = *(buffer + 5),
      .opcode = ntohs(*(uint16_t *)(buffer + 6)),
  };
  memcpy((*p)->sender_hardware_address, buffer + 8,
         ARP_HARDWARE_ADDRESS_LENGTH);
  memcpy(&(*p)->sender_protocol_address, buffer + 14,
         (*p)->protocol_address_length);
  memcpy((*p)->target_hardware_address, buffer + 18,
         ARP_HARDWARE_ADDRESS_LENGTH);
  memcpy(&(*p)->target_protocol_address, buffer + 24,
         (*p)->protocol_address_length);

  return true;
}

void free_arp_packet(mm_arp_packet_t **p) {
  if (p == NULL || *p == NULL) {
    return;
  }
  free(*p);
  *p = NULL;
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

void print_arp_packet(mm_arp_packet_t *p) {
  if (p == NULL) {
    return;
  }

  printf("ARP Packet:\n");

  char *hardware_type = arp_hardware_type_to_string(p->hardware_type);
  printf("  Hardware Type: %s (%d)\n", hardware_type, p->hardware_type);

  char *protocol_type = arp_protocol_type_to_string(p->protocol_type);
  printf("  Protocol Type: %s (%#x)\n", protocol_type, p->protocol_type);

  printf("  Hardware Address Length: %d\n", p->hardware_address_length);
  printf("  Protocol Address Length: %d\n", p->protocol_address_length);

  char *operation = arp_opcode_to_string(p->opcode);
  printf("  Operation: %s (%d)\n", operation, p->opcode);

  if (p->hardware_type == ARP_HARDWARE_TYPE_ETHERNET &&
      p->protocol_type == ARP_PROTOCOL_TYPE_IPV4) {
    printf("  Sender Hardware Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           p->sender_hardware_address[0], p->sender_hardware_address[1],
           p->sender_hardware_address[2], p->sender_hardware_address[3],
           p->sender_hardware_address[4], p->sender_hardware_address[5]);
    printf("  Sender Protocol Address: %d.%d.%d.%d\n",
           (uint8_t)(p->sender_protocol_address),
           (uint8_t)(p->sender_protocol_address >> 8),
           (uint8_t)(p->sender_protocol_address >> 16),
           (uint8_t)(p->sender_protocol_address >> 24));
    printf("  Target Hardware Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           p->target_hardware_address[0], p->target_hardware_address[1],
           p->target_hardware_address[2], p->target_hardware_address[3],
           p->target_hardware_address[4], p->target_hardware_address[5]);
    printf("  Target Protocol Address: %d.%d.%d.%d\n",
           (uint8_t)(p->target_protocol_address),
           (uint8_t)(p->target_protocol_address >> 8),
           (uint8_t)(p->target_protocol_address >> 16),
           (uint8_t)(p->target_protocol_address >> 24));
  } else {
    printf("  Sender Hardware Address: Unknown\n");
    printf("  Sender Protocol Address: Unknown\n");
    printf("  Target Hardware Address: Unknown\n");
    printf("  Target Protocol Address: Unknown\n");
  }
}
