#include <stdint.h>
#include <stdbool.h>

struct circularBuffer {
	unsigned int itemSize;
	unsigned int numItems;
	uint8_t *data;
};

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
//! \return false if any parameters are out of bounds, true otherwise
//
//*****************************************************************************
bool initCircularBuffer(struct circularBuffer *buff, unsigned int itemSize, unsigned int numItems);

//*****************************************************************************
//
//! These functions insert elements into a circular buffer. 
//!
//! \note The circular buffer pointed to by buffAddr need not have elements of
//! size matched to the circularBufferAddXX() function. In other words, all of
//! these functions will work properly on any circular buffer initialized by
//! initCircularBuffer() regardless of the itemSize passed into
//! initCircularBuffer(). When the buffer's item size is smaller than the chunk
//! of data passed into circularBufferAddXX(), the data will be zero-padded in
//! its most significant bits. When the buffer's item size is larger than the
//! chunk of data passed into circularBufferAddXX(), it will be split into
//! chunks matching the buffer's item size. For example, using
//! circularBufferAdd32() to add to a buffer with item size of 2 bytes will add
//! two items to the buffer. Using circularBufferAdd32() to add to a buffer
//! with item size of 3 bytes with add the least significant three bytes as one
//! item to the buffer, and the most significant byte will be zero-padded in
//! its most significant bits to form a second 3-byte item. 
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \param item is the data to store into teh buffer or, in the case of
//! circularBufferAddLarge(), a pointer to the chunk of data to add.
//!
//! \return false if any parameters are invalid or the data cannot fit in the
//! buffer, true otherwise. The buffer is not modified at all if the function
//! returns false. 
//
//*****************************************************************************
bool circularBufferAdd8(struct circularBuffer *buff, uint8_t item);
bool circularBufferAdd16(struct circularBuffer *buff, uint16_t item);
bool circularBufferAdd32(struct circularBuffer *buff, uint32_t item);
bool circularBufferAddLarge(struct circularBuffer *buff, void *item);

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
bool circularBufferRemoveItem(struct circularBuffer *buff, void *data);

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
//! \return the number of items written
//
//*****************************************************************************
unsigned int circularBufferRemoveMultiple(struct circularBuffer *buff, void *data, unsigned int numItems);
