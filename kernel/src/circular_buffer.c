#include <stdint.h>
#include <assert.h>

#include "circular_buffer.h"

#define CBUFF_TO_TYPED_ARRAY(sized_type,buff) ((sized_type*)((buff).data))

//*****************************************************************************
//
//! Add a single item to the buffer without performing any parameter checks.
//! This function encapsulates the code that can be shared between
//! circularBufferAddItem() and circularBufferAddMultiple().
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \param item is the pointer where the removed item should be copied to
//!
//! \return none
//
//*****************************************************************************
void addSingleItemUnsafe(circularBuffer_t *buff, void *item) {
	unsigned int index;
	int bytesLeft;
	uint8_t *dataPtr8;
	uint64_t *dataPtr64;
	
	// if the item size is a standard number of bytes, use a simpler copy
	index = buff->wrCnt % buff->numItems;
	switch (buff->itemSize) {
		case 1 :
			CBUFF_TO_TYPED_ARRAY(uint8_t, *buff)[index] = *((uint8_t *)item);
			break;
		case 2 :
			CBUFF_TO_TYPED_ARRAY(uint16_t, *buff)[index] = *((uint16_t *)item);
			break;
		case 4 :
			CBUFF_TO_TYPED_ARRAY(uint32_t, *buff)[index] = *((uint32_t *)item);
			break;
		case 8 :
			CBUFF_TO_TYPED_ARRAY(uint64_t, *buff)[index] = *((uint64_t *)item);
			break;
		default :
			// if the item size is a non-standard number of bytes, first copy
			// 64-bit chunks, then copy the remainder byte by byte
			bytesLeft = buff->itemSize;
			dataPtr64 = item;
			while (bytesLeft >= 8) {
				CBUFF_TO_TYPED_ARRAY(uint64_t, *buff)[index] = *dataPtr64;
				index += 8;
				dataPtr64++;
				bytesLeft -= 8;
			}
			dataPtr8 = (uint8_t *)dataPtr64;
			while (bytesLeft > 0) {
				CBUFF_TO_TYPED_ARRAY(uint8_t, *buff)[index] = *dataPtr8;
				index++;
				bytesLeft--;
			}
			break;
	}
	
	return;
}

bool circularBufferAddItem(circularBuffer_t *buff, void *item) {
	assert((buff != NULL) && (item != NULL));

	concurr_mutex_lock(buff->lock);

	// Buffer is full, do not add.
	if (buff->wrCnt - buff->rdCnt >= buff->numItems) {
		concurr_mutex_unlock(buff->lock);
		return false;
	}
	
	// use the unsafe internal function to add a single item
	addSingleItemUnsafe(buff, item);
	
	// increment the write counter
	buff->wrCnt++;
	
	concurr_mutex_unlock(buff->lock);
	return true;
}

unsigned int circularBufferAddMultiple(circularBuffer_t *buff, void *item, unsigned int numItems) {
	assert((buff != NULL) && (item != NULL));

	unsigned int itemsRemaining;
	
	concurr_mutex_lock(buff->lock);
	
	// if the buffer doesn't have enough room, only add enough elements to fill the buffer
	if (buff->numItems - (buff->wrCnt - buff->rdCnt) < numItems) {
		itemsRemaining = buff->numItems - (buff->wrCnt - buff->rdCnt);
	} else {
		itemsRemaining = numItems;
	}
	
	// increment the buffer's write count
	buff->wrCnt += itemsRemaining;
	
	// use the unsafe internal function to add single items until all items are added
	while (itemsRemaining > 0) {
		addSingleItemUnsafe(buff, item);
		
		// move the pointer to the next item to be added
		item = (uint8_t *)item + buff->itemSize;
		// update remaining items count
		itemsRemaining--;
	}
	
	concurr_mutex_unlock(buff->lock);
	return numItems;
}

//*****************************************************************************
//
//! Remove a single item from the buffer without performing any parameter
//! checks. This function encapsulates the code that can be shared between
//! circularBufferRemoveItem() and circularBufferRemoveMultiple().
//!
//! \param buff is a pointer to a circularBuffer struct
//!
//! \param item is the pointer where the removed item should be copied to
//!
//! \return none
//
//*****************************************************************************
void removeSingleItemUnsafe(circularBuffer_t *buff, void *data) {
	unsigned int index;
	unsigned int bytesLeft;
	uint64_t *dataPtr64;
	uint8_t *dataPtr8;
	
	// if the item size is a standard number of bytes, use a simpler copy
	index = buff->rdCnt % buff->numItems;
	switch (buff->itemSize) {
		case 1 :
			*((uint8_t *)data) = CBUFF_TO_TYPED_ARRAY(uint8_t, *buff)[index];
			break;
		case 2 :
			*((uint16_t *)data) = CBUFF_TO_TYPED_ARRAY(uint16_t, *buff)[index];
			break;
		case 4 :
			*((uint32_t *)data) = CBUFF_TO_TYPED_ARRAY(uint32_t, *buff)[index];
			break;
		case 8 :
			*((uint64_t *)data) = CBUFF_TO_TYPED_ARRAY(uint64_t, *buff)[index];
			break;
		default :
			// if the item size is a non-standard number of bytes, first copy
			// 64-bit chunks, then copy the remainder byte by byte
			bytesLeft = buff->itemSize;
			dataPtr64 = data;
			while (bytesLeft >= 8) {
				*dataPtr64 = CBUFF_TO_TYPED_ARRAY(uint8_t, *buff)[index];
				dataPtr64++;
				index += 8;
				bytesLeft -= 8;
			}
			dataPtr8 = (uint8_t *)dataPtr64;
			while (bytesLeft > 0) {
				*dataPtr8 = CBUFF_TO_TYPED_ARRAY(uint8_t, *buff)[index];
				dataPtr8++;
				index++;
				bytesLeft--;
			}
			break;
	}
	
	return;
}

bool circularBufferRemoveItem(circularBuffer_t *buff, void *data) {
	assert((buff != NULL) && (data != NULL));

	concurr_mutex_lock(buff->lock);
	
	// if the buffer is empty, return false
	if (buff->rdCnt >= buff->wrCnt) {
		concurr_mutex_unlock(buff->lock);
		return false;
	}
	
	// use the unsafe internal function to remove a single item
	removeSingleItemUnsafe(buff, data);
	
	// increment the read counter
	buff->rdCnt++;
	
	// Do a rollover check to avoid eventual overflows in the counters. Presumably the write count
	// is always larger than the read count, so it doesn't need to be checked.
	if (buff->rdCnt >= buff->numItems) {
		buff->rdCnt -= buff->numItems;
		buff->wrCnt -= buff->numItems;
	}
	
	concurr_mutex_unlock(buff->lock);
	return true;
}

unsigned int circularBufferRemoveMultiple(circularBuffer_t *buff, void *data, unsigned int numItems) {
	assert((buff != NULL) && (data != NULL));
	
	unsigned int itemsRemaining;
	
	concurr_mutex_lock(buff->lock);
	
	if (numItems == 0) {
		concurr_mutex_unlock(buff->lock);
		return 0;
	}
	
	// check if there are actually numItems items in the buffer. If not, only
	// remove as many items as there are in the buffer.
	if (buff->wrCnt - buff->rdCnt < numItems) {
		numItems = buff->wrCnt - buff->rdCnt;
	}
	itemsRemaining = numItems;
	
	// increment the buffer's read count
	buff->rdCnt += itemsRemaining;
	
	// use the unsafe internal function to remove single items until all items
	// are removed
	while (itemsRemaining > 0) {
		removeSingleItemUnsafe(buff, data);
		
		// move the pointer to the next empty space in the data array
		data = (uint8_t *)data + buff->itemSize;
		// update remaining items count
		itemsRemaining--;
	}
	
	// do a rollover check to avoid eventual overflows in the counters.
	// presumably the write count is always larger than the read count, so it
	// doesn't need to be checked.
	if (buff->rdCnt >= buff->numItems) {
		buff->rdCnt -= buff->numItems;
		buff->wrCnt -= buff->numItems;
	}
	
	concurr_mutex_unlock(buff->lock);
	return numItems;
}
