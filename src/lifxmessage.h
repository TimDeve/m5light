#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
  /* frame */
  uint16_t size;
  uint16_t protocol : 12;
  uint8_t addressable : 1;
  uint8_t tagged : 1;
  uint8_t origin : 2;
  uint32_t source;
  /* frame address */
  uint8_t target[8];
  uint8_t reserved[6];
  uint8_t res_required : 1;
  uint8_t ack_required : 1;
  uint8_t : 6;
  uint8_t sequence;
  /* protocol header */
  uint64_t : 64;
  uint16_t type;
  uint16_t : 16;
  /* variable length payload follows */
} Header;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
  uint16_t level;
  uint32_t duration;
} SetPowerPayload;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
  Header header;
  SetPowerPayload payload;
} SetPowerMessage;
#pragma pack(pop)