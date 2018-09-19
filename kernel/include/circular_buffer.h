#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#include "port_concurrent.h"

/**
 * \brief An atomic circular buffer.
 */
typedef struct circularBuffer_t_ {
	concurr_mutex lock;     //!< Mutex controlling buffer access.
	size_t        itemSize; //!< Size of items held in buffer.
	unsigned int  numItems; //!< Number of items the buffer can hold.
	unsigned int  rdCnt;    //!< Buffer read count.
	unsigned int  wrCnt;    //!< Buffer write count.
	void*         data;     //!< Pointer to buffer data array.
} circularBuffer_t;

/**
 * \brief Initialize a circular buffer.
 *
 * \hideinitializer
 *
 * \param [in] buff     Pointer to the circular buffer to initialize.
 * \param      itemType The type of items to store in the buffer.
 * \param      numItems The capacity of the buffer, in number of items.
 * \param [in] buffAddr Pointer to the user-supplied data buffer where the data will actually be
 *                      stored.
 *
 * \retval void
 */
#define initCircularBuffer(buff,itemType,numItems,buffAddr) {                         \
	ASSERT((buff != NULL) && (itemSize > 0) && (numItems > 0) && (buffAddr != NULL)); \
	concurr_mutex_init(buff->lock);                                                   \
	buff->itemSize = sizeof(itemType);                                                \
	buff->numItems = (unsigned int)numItems;                                          \
	buff->rdCnt = 0;                                                                  \
	buff->wrCnt = 0;                                                                  \
	buff->data = (void*)buffAddr;                                                     \
}

/**
 * \brief Check to see if a buffer is full.
 *
 * \hideinitializer
 *
 * \param [in] buff Pointer to the buffer to check.
 *
 * \retval true  The buffer is full.
 * \retval false The buffer is not full.
 */
#define circularBufferFull(buff) (buff->wrCnt - buff->rdCnt >= buff->numItems)

/**
 * \brief Insert a single element into a circular buffer. 
 *
 * \param [in] buff The buffer to push into.
 * \param [in] item The data item to store into the buffer.
 *
 * \retval true  Item has been added to the buffer.
 * \retval false There is not enough room in the buffer to add the item. Buffer is unmodified.
 */
bool circularBufferAddItem(circularBuffer_t *buff, void *item);

/**
 * \brief Insert an array of elements into a circular buffer.
 *
 * \param [in] buff     The buffer to push into.
 * \param [in] item     Array of elements to push into the buffer.
 * \param      numItems Number of items in the array.
 *
 * \return Returns the number of items actually added to the buffer.
 */
unsigned int circularBufferAddMultiple(circularBuffer_t *buff, void *item, unsigned int numItems);

/**
 * \brief Check to see if a buffer is empty.
 *
 * \param buff Pointer to the buffer to check.
 *
 * \retval true  The buffer is empty.
 * \retval false The buffer is not empty.
 */
#define circularBufferEmpty(buff) (buff->wrCnt - buff->rdCnt == 0)

/**
 * \brief Remove a single item from the circular buffer.
 *
 * \param [in] buff The buffer to read from.
 * \param [in] item Pointer to memory location to receive data form the buffer.
 *
 * \retval true  Item has been removed from the buffer.
 * \retval false No items in buffer.
 */
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

#endif // #ifndef CIRCULAR_BUFFER_H
