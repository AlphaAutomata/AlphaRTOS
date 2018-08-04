#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	unsigned int itemSize;
	unsigned int numItems;
	unsigned int rdCnt;
	unsigned int wrCnt;
	uint8_t *data;
} circularBuffer_t;

//*****************************************************************************
//
//! Initialize a circular buffer located at buffAddr. The buffer has a capacity
//! of numItems number of items, each of which is itemSize number of bytes.
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \param itemSize is the size of a single item in bytes
//!
//! \param numItems is the capacity of the buffer in number of items
//!
//! \param buffAddr is the address of the data buffer where the data will
//! actually be stored
//!
//! \return false if any parameters are out of bounds, true otherwise
//
//*****************************************************************************
bool initCircularBuffer(circularBuffer_t *buff, unsigned int itemSize, unsigned int numItems, void *buffAddr);

//*****************************************************************************
//
//! Check to see if a buffer is full
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \return true if the buffer is full, false otherwise
//
//*****************************************************************************
bool circularBufferFull(circularBuffer_t *buff);

//*****************************************************************************
//
//! This function inserts a single element into a circular buffer. 
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \param item is the data to store into the buffer. It is treated as a packed
//! array where each item is buff.itemSize bytes. For circularBufferAddItem(),
//! this array is assumed to be a single element long.
//!
//! \return false if any parameters are invalid or the data cannot fit in the
//! buffer, true otherwise. The buffer is not modified at all if the function
//! returns false. 
//
//*****************************************************************************
bool circularBufferAddItem(circularBuffer_t *buff, void *item);

//*****************************************************************************
//
//! This insert multiple elements into a circular buffer. 
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \param item is the data to store into the buffer. It is treated as a packed
//! array where each item is buff.itemSize bytes. For circularBufferAddItem(),
//! this array is assumed to be a single element long.
//!
//! \param numItems is the number of items to add to the buffer
//!
//! \return the number of items actually added to the buffer
//
//*****************************************************************************
unsigned int circularBufferAddMultiple(circularBuffer_t *buff, void *item, unsigned int numItems);

//*****************************************************************************
//
//! Check to see if a buffer is empty
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \return true if the buffer is empty, false otherwise
//
//*****************************************************************************
bool circularBufferEmpty(circularBuffer_t *buff);

//*****************************************************************************
//
//! Remove a single item from the circular buffer
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \param data is a pointer to the memory location to store the data
//!
//! \return false if the buffer is empty, true otherwise
//
//*****************************************************************************
bool circularBufferRemoveItem(circularBuffer_t *buff, void *data);

//*****************************************************************************
//
//! Remove numItems number of items from the circular buffer. Writes into the
//! location pointed to by *data as if it were a packed array of items of size
//! buff.itemSize. For example, if buff.itemSize == 3, the call
//! circularBufferRemoveMultiple(buff, data, 3) will write 9 consecutive bytes.
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \param data is a pointer to the memory location to store the data
//!
//! \return the number of items removed
//
//*****************************************************************************
unsigned int circularBufferRemoveMultiple(circularBuffer_t *buff, void *data, unsigned int numItems);

#endif
