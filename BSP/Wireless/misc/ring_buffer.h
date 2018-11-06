
#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <stdint.h>
#include <string.h>

typedef struct
{
	uint8_t *pbuffer;
	uint16_t buffer_size;
	uint16_t data_len;
	uint16_t wr_index;
	uint16_t rd_index;
}RING_CTRL;

void RingBuffer_Init(RING_CTRL *ring, uint8_t *pbuffer, uint16_t buffer_size);
int8_t RingBuffer_Read(RING_CTRL *ring, uint8_t *out_buffer, uint16_t data_size);
int8_t RingBuffer_Write(RING_CTRL *ring, uint8_t *data_ptr, uint16_t data_size);
uint8_t RingBuffer_Ov_Flag(RING_CTRL ring);
uint16_t RingBuffer_Data_Len(RING_CTRL *ring);
void RingBuffer_Clear(RING_CTRL *ring);

#endif /* __RING_BUFFER_H__ */

