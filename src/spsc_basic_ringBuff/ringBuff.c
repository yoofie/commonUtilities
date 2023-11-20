/* **************************************
	File Name: RingBuff
*************************************** */
#include "ringBuff.h"

/* **************************************
	#defines
*************************************** */

/* **************************************
	Declarations
*************************************** */

/* **************************************
	Code
*************************************** */

void ringb_init(ringB_t *const input, uint8_t capacity, testStruct_t *const bufferLocation) {
	input->capacity = capacity;
	input->size = 0U;
	input->readIndex = 0;
	input->writeIndex = 0;
	input->data = bufferLocation;
}

void ringb_reset(ringB_t *const input) {
	input->size = 0U;
	input->readIndex = 0;
	input->writeIndex = 0;
}

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

bool ringb_pop(ringB_t *const input, testStruct_t *const item) {
	bool retval = false;
	if(!ringb_isEmpty(input)) {
		*item = input->data[input->readIndex];
		input->readIndex++;
		if(input->readIndex >= input->capacity) {
			input->readIndex = 0;
		}
		input->size--;
		retval = true;
	}
	return retval;
}

bool ringb_isEmpty(ringB_t *const input) {
	return (input->size == 0);
}

bool ringb_isFull(ringB_t *const input) {
	return ((input->size) >= input->capacity);
}

uint8_t ringb_size(ringB_t *const input) {
	return input->size;
}

