/* **************************************
	File Name: RingBuff
*************************************** */
#include "ringBuffExt.h"

/* **************************************
	#defines
*************************************** */

/* **************************************
	API
*************************************** */

/* **************************************
	Init & Clearing
*************************************** */

void ringb_init(ringB_t *const input, uint8_t capacity, testStruct_t *const bufferLocation) {
	input->capacity = capacity;
	input->size = 0U;
	input->readIndex = 0;
	input->writeIndex = 0;
	input->data = bufferLocation;
	input->clearData = NULL;
}

void ringb_initExtended(ringB_t *const input, uint8_t capacity, testStruct_t *const bufferLocation, void (*clearData)(testStruct_t *)) {
	input->capacity = capacity;
	input->size = 0U;
	input->readIndex = 0;
	input->writeIndex = 0;
	input->data = bufferLocation;
	input->clearData = clearData;
}

void ringb_reset(ringB_t *const input) {
	input->size = 0U;
	input->readIndex = 0;
	input->writeIndex = 0;

	ringb_clear_buffer(input);
}

void ringb_deregister_clear_function(ringB_t *const input) {
	input->clearData = NULL;
}
void ringb_register_clear_function(ringB_t *const input, void (*clearData)(testStruct_t *)) {
	input->clearData = clearData;
}

/* ********************************************************
	Add to the buffer
******************************************************** */
// Push data by copy
bool ringb_push(ringB_t *const input, testStruct_t item) {
	bool retval = false;
	if (!ringb_isFull(input)) {
		input->data[input->writeIndex] = item;
		input->writeIndex++;
		if (input->writeIndex >= input->capacity) {
			input->writeIndex = 0;
		}
		input->size++;
		retval = true;
	}
	return retval;
}

// Push by reference
bool ringb_push_by_ref(ringB_t *const input, testStruct_t *const item) {
	bool retval = false;
	if (!ringb_isFull(input)) {
		input->data[input->writeIndex] = *item;
		input->writeIndex++;
		if (input->writeIndex >= input->capacity) {
			input->writeIndex = 0;
		}
		input->size++;
		retval = true;
	}
	return retval;
}
/* ********************************************************
	Pop
	Pop the element from the rear end (older values).
	The data remains in place (without being cleared)
******************************************************** */
bool ringb_pop(ringB_t *const input, testStruct_t *const item) {
	bool retval = false;
	if (!ringb_isEmpty(input)) {
		*item = input->data[input->readIndex];
		input->readIndex++;
		if (input->readIndex >= input->capacity) {
			input->readIndex = 0;
		}
		input->size--;
		retval = true;
	}
	return retval;
}

/* ********************************************************
	Clear pop
	Clear the data as you pop it from the ring buffer via
	a user provided function
******************************************************** */
bool ringb_clear_pop(ringB_t *const input, testStruct_t *const item) {
	bool retval = false;
	if (!ringb_isEmpty(input)) {
		*item = input->data[input->readIndex];

		if (input->clearData != NULL) {
			// User function wipes the data in this slot
			(*input->clearData)(&input->data[input->readIndex]); // User provided function
		}

		input->readIndex++;
		if (input->readIndex >= input->capacity) {
			input->readIndex = 0;
		}
		input->size--;
		retval = true;
	}
	return retval;
}

/* ********************************************************
	Pop First
	Pops the latest (newest element in the ring buffer) instead
	of the rear
******************************************************** */
bool ringb_pop_first(ringB_t *const input, testStruct_t *const item) {
	bool retval = false;

	if (!ringb_isEmpty(input)) {
		if (input->writeIndex == 0) {
			// Rewind the writeIndex back to the array max value
			input->writeIndex = (input->capacity - 1);
			*item = input->data[input->writeIndex];
			input->size--;
		} else {
			*item = input->data[input->writeIndex--];
			input->size--;
		}

		retval = true;
	}
	return retval;
}

/* ********************************************************
	INSPECTION
******************************************************** */

/* ********************************************************
	Random access a slot
	===
	Given a slot index within bounds, this will return the
	data at the given slot.

	Note that the data is not gauranteed to be valid. Its
	up to the programmer to ensure the slot contains valid
	data
******************************************************** */
bool ringb_unsafe_random_access(ringB_t *const input, uint32_t slotIndex, testStruct_t *const item) {
	bool retval = false;
	// Ensure the slot index is valid
	if (slotIndex < input->capacity) {
		*item = input->data[slotIndex];
		retval = true;
	}
	return retval;
}

void ringb_scan_items(ringB_t *const input, void (*userFn)(uint8_t, void *const), void *userData) {
	uint8_t slotIndex = input->readIndex;
	uint8_t sizeCounter = 0;
	// sizeLimit must be atomic/protected by mutex, otherwise it will break
	// mutex start here
	uint8_t sizeLimit = input->size;
	// mutex end goes here
	if (userFn != NULL && !ringb_isEmpty(input)) {
		while (sizeCounter != sizeLimit) {
			(*userFn)(slotIndex, userData);
			slotIndex = (slotIndex >= input->capacity - 1) ? 0 : (slotIndex + 1);
			sizeCounter++;
		}
	}
}

void ringb_scan_item_element(ringB_t *const input, void (*userFn)(uint8_t, testStruct_t * data, void *const), void *userData) {
	uint8_t slotIndex = input->readIndex;
	uint8_t sizeCounter = 0;
	// sizeLimit must be atomic/protected by mutex, otherwise it will break
	// mutex start here
	uint8_t sizeLimit = input->size;
	// mutex end goes here
	if (userFn != NULL && !ringb_isEmpty(input)) {
		while (sizeCounter != sizeLimit) {
			(*userFn)(slotIndex, &input->data[slotIndex], userData);
			slotIndex = (slotIndex >= input->capacity - 1) ? 0 : (slotIndex + 1);
			sizeCounter++;
		}
	}
}
/* ********************************************************
	Peek()
	Read the element without removing it from thr ring buffer
******************************************************** */
bool ringb_peek(ringB_t *const input, testStruct_t *const item) {
	bool retval = false;
	if (!ringb_isEmpty(input)) {
		*item = input->data[input->readIndex];
		retval = true;
	}
	return retval;
}

/* ********************************************************
	Peek First
******************************************************** */
bool ringb_peek_first(ringB_t *const input, testStruct_t *const item) {
	bool retval = false;
	if (!ringb_isEmpty(input)) {
		if (input->writeIndex == 0) {
			// Rewind the writeIndex back to the array max value
			uint8_t prevWriteIndex = (input->capacity - 1);
			*item = input->data[prevWriteIndex];
		} else {
			uint8_t prevWriteIndex = (input->writeIndex - 1);
			*item = input->data[prevWriteIndex];
		}
		retval = true;
	}
	return retval;
}

/* ********************************************************
	Support functions
	Self Explanatory
******************************************************** */
bool ringb_isEmpty(ringB_t *const input) {
	return (input->size == 0);
}

bool ringb_isFull(ringB_t *const input) {
	return ((input->size) >= input->capacity);
}

uint8_t ringb_size(ringB_t *const input) {
	return input->size;
}

// Number of "slots" available
uint8_t ringb_free_space(ringB_t *const input) {
	return (input->capacity - input->size);
}

// Number of "slots" available
uint8_t ringb_space_available(ringB_t *const input) {
	return ringb_free_space(input);
}

// Total memory consumption of just the data (in bytes)
uint32_t ringb_total_data_size_bytes(ringB_t *const input) {
	// Note that this does not factor in alignment, but its a good ballpark figure
	uint32_t memoryConsumption = (sizeof(testStruct_t) * input->capacity);
	return memoryConsumption;
}

// Go through entire array and wipe data
void ringb_clear_buffer(ringB_t *const input) {
	for (uint32_t i = 0; i < input->capacity; i++) {
		if (input->clearData != NULL) {
			// User function wipes the data in this slot
			(*input->clearData)(&input->data[i]); // User provided function
		}
	}
}

testStruct_t * ringb_get_data_pointer(ringB_t *const input, uint8_t * const size, uint8_t * const capacity) {
	*size = input->size;
	*capacity = input->capacity;
	return input->data;
}
