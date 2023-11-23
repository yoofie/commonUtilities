/* **************************************
	File Name: RINGBUFF
	Created: Monday November 20 2023
*************************************** */
#ifndef _RING_BUFF_H_
#define _RING_BUFF_H_

#include <stdint.h>
#include <stdbool.h>
/* **************************************
	#defines
*************************************** */
// Dummy struct used as test element. Search and replace this and substitue with your own structure
// since C does not support generics
#define NUM_OF_ELEM_IN_DATA_ARRAY 3U

typedef struct {
	uint8_t index;
	uint8_t data[NUM_OF_ELEM_IN_DATA_ARRAY];
} testStruct_t;

/* **************************************
	Declarations
*************************************** */
// Notes: Everything uint8_t by default. Simple search and replace if you need to use bigger sizes
typedef struct {
	uint8_t size;
	uint8_t capacity;
	uint8_t readIndex;
	uint8_t writeIndex;
	testStruct_t *data;
	void (*clearData)(testStruct_t *); // Function Pointer to clear data as you pop data
} ringB_t;

/* **************************************
	API
*************************************** */

/* Init & clearing
******************************************************** */
void ringb_init(ringB_t *const input, uint8_t capacity, testStruct_t *const bufferLocation);

// Same as init but you can provide clearing data function
void ringb_initExtended(ringB_t *const input, uint8_t capacity, testStruct_t *const bufferLocation, void (*clearData)(testStruct_t *));

// Reset the ring buffer metadata
void ringb_reset(ringB_t *const input);

// Clears the data in each slot using the user defined function
void ringb_clear_buffer(ringB_t *const input);

// Register the clear Function
void ringb_register_clear_function(ringB_t *const input, void (*clearData)(testStruct_t *));

// Deregister the clearing data function
void ringb_deregister_clear_function(ringB_t *const input);

/* Adding to the buffer
******************************************************** */
// Push data by copy
bool ringb_push(ringB_t *const input, testStruct_t item);

// Push data by reference
bool ringb_push_by_ref(ringB_t *const input, testStruct_t *const item);

/* Removing from the buffer
******************************************************** */
// Pop the oldest item
bool ringb_pop(ringB_t *const input, testStruct_t *const item);

// Pop the newest item
bool ringb_pop_first(ringB_t *const input, testStruct_t *const item);

// Pop the oldest item, but clear the data after its popped via UDF
bool ringb_clear_pop(ringB_t *const input, testStruct_t *const item);

/* Inspecting the buffer (without removing anything)
******************************************************** */
// Peek at the last item without removing it from the buffer
bool ringb_peek(ringB_t *const input, testStruct_t *const item);

// Peek at the first (latest written element) from the buffer
bool ringb_peek_first(ringB_t *const input, testStruct_t *const item);

// Given a slot index, get the data at that position. Data is not gauranteed to be valid
bool ringb_unsafe_random_access(ringB_t *const input, uint32_t slotIndex, testStruct_t *const item);

// Scan items without removing items and perform action via UDF
void ringb_scan_items(ringB_t *const input, void (*userFn)(uint8_t, void *const), void * userData);

// Scan items without removing items and perform action via UDF, but with access to the current element
void ringb_scan_item_element(ringB_t *const input, void (*userFn)(uint8_t, testStruct_t *, void *const), void *userData);

/* Meta data & support functions
******************************************************** */
bool ringb_isEmpty(ringB_t *const input);
bool ringb_isFull(ringB_t *const input);
uint8_t ringb_size(ringB_t *const input);
uint8_t ringb_free_space(ringB_t *const input);
uint8_t ringb_space_available(ringB_t *const input);
uint32_t ringb_total_data_size_bytes(ringB_t *const input);

// Pointer to the data array and total current size &  capacity
testStruct_t * ringb_get_data_pointer(ringB_t *const input, uint8_t * const size, uint8_t * const capacity);
#endif
