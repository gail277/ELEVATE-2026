#include "packet.h"

void packet_create(packet_t *packet, uint8_t *data, uint8_t length)
{
     if (length > PAYLOAD_MAX_LENGTH)
    {
        length = PAYLOAD_MAX_LENGTH;
    }

    packet->length = length;

    for (uint8_t i = 0; i < length; i++)    
    {
        packet->data[i] = data[i];
    }
}
static
void packet_get_message(packet_t *packet, uint8_t *data, uint8_t length)
{
    for (uint8_t i = 1; i < length; i++)
    {
        data[i] = packet->data[i];
    }
}
