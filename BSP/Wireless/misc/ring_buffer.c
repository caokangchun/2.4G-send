
#include "ring_buffer.h"

/******************************************************************************
 * @name        RingBuffer_Init
 *
 * @brief       This routine is used for init ring buffer
 *
 * @param       None
 *
 * @return      None
 *
 *****************************************************************************/
void RingBuffer_Init(RING_CTRL *ring, uint8_t *pbuffer, uint16_t buffer_size)
{
	ring->pbuffer = pbuffer;
	ring->buffer_size = buffer_size;
	
	ring->data_len = 0;
	ring->rd_index = 0;
	ring->wr_index = 0;
}


/******************************************************************************
 * @name        RingBuffer_Read
 *
 * @brief       This routine is used for ring buffer read
 *
 * @param       None
 *
 * @return      None
 *
 *****************************************************************************/
int8_t RingBuffer_Read(RING_CTRL *ring, uint8_t *out_buffer, uint16_t data_size)
{
	/* validate the function's parameter --------------------*/
	if((0 == data_size) || (NULL == out_buffer) || (NULL == ring))
	{
		return -1;
	}

	/* buffer is empty */
	if(0 == ring->data_len)
	{
		return 0;
	}

	/* read out valid length data */
	if(data_size > ring->data_len)
	{
		data_size = ring->data_len;
	}
	
	if((ring->rd_index + data_size) > ring->buffer_size)
	{
		uint16_t tmp_len;
		
		/* copy ring's tail data to out buffer */
		tmp_len = ring->buffer_size - ring->rd_index;
		memcpy(out_buffer, &ring->pbuffer[ring->rd_index], tmp_len);

		/* fix for next copy */
		out_buffer += tmp_len;
		tmp_len = data_size - tmp_len;
		
		/* copy ring's head data to out buffer */
		memcpy(out_buffer, ring->pbuffer, tmp_len);
		
		/* fix read pointer */
		ring->rd_index = tmp_len;
	}
	else
	{
		/* copy all data to the out buffer */
		memcpy(out_buffer, &ring->pbuffer[ring->rd_index], data_size);
		
		/* fix read pointer */
		ring->rd_index += data_size;
		
		/* index achieve the end of the buff */
		if(ring->buffer_size == ring->rd_index)
		{
			ring->rd_index = 0;
		}
	}
	
	ring->data_len -= data_size;
	
	return data_size;
}

/******************************************************************************
 * @name        RingBuffer_Write
 *
 * @brief       This routine is used for ring buffer write
 *
 * @param       None
 *
 * @return      None
 *
 *****************************************************************************/
int8_t RingBuffer_Write(RING_CTRL *ring, uint8_t *data_ptr, uint16_t data_size)
{
	/* validate the function's parameter --------------------*/
	if((0 == data_size) || (NULL == data_ptr) || (NULL == ring))
	{
		return -1;
	}

	/* if buffer is not enough, fill full with data, discard others */
	if((ring->data_len + data_size) > ring->buffer_size)
	{
	#if 0
		/* buffer overflow */
		ring->buffer_ov = 1;
		
		/* buffer is full, cann't write any data */
		if(ring->data_len == ring->buffer_size)
		{
			return 0;
		}
		else
		{
			data_size = ring->buffer_size - ring->data_len;
		}
	#else
		return -1;
	#endif /* 0 */
	}
	
	ring->data_len += data_size;
	
	if((ring->wr_index + data_size) > ring->buffer_size)
	{
		uint16_t tmp_len;
		
		/* copy tail part to ring buffer first */
		tmp_len = ring->buffer_size - ring->wr_index;
		memcpy(&ring->pbuffer[ring->wr_index], data_ptr, tmp_len);

		/* fix for next copy */
		data_ptr += tmp_len;
		tmp_len = data_size - tmp_len;

		/* copy head part to ring buffer second */
		memcpy(ring->pbuffer, data_ptr, tmp_len);
		
		/* fix write pointer */
		ring->wr_index = tmp_len;
	}
	else
	{
		/* copy all data to the ring buffer */
		memcpy(&ring->pbuffer[ring->wr_index], data_ptr, data_size);
		
		/* fix write pointer */
		ring->wr_index += data_size;

		/* index achieve the end of the buff */
		if(ring->buffer_size == ring->wr_index)
		{
			/*index goto head */
			ring->wr_index = 0;
		}
	}

	return data_size;
}

/******************************************************************************
 * @name        RingBuffer_Ov_Flag
 *
 * @brief       This routine is used for ring buffer write
 *
 * @param       None
 *
 * @return      None
 *
 *****************************************************************************/
#if 0
uint8_t RingBuffer_Ov_Flag(RING_CTRL ring)
{
	return ring.buffer_ov;
}
#else
void RingBuffer_Clear(RING_CTRL *ring)
{
	ring->rd_index = 0;
	ring->wr_index = 0;
	ring->data_len = 0;
}
#endif /* 0 */

/******************************************************************************
 * @name        RingBuffer_Data_Len
 *
 * @brief       This routine is used for ring buffer write
 *
 * @param       None
 *
 * @return      None
 *
 *****************************************************************************/
uint16_t RingBuffer_Data_Len(RING_CTRL *ring)
{
	return ring->data_len;
}

