/* **************************************
	File Name: RINGBUFF
	Created: Monday November 20 2023
*************************************** */
#ifndef _RING_BUFF_H_
#define _RING_BUFF_H_

#include <stdint.h>
/* **************************************
	#defines
*************************************** */

typedef struct {
	uint8_t index;
	uint8_t data[3];
} testStruct_t;

/* **************************************
	Declarations
*************************************** */
typedef struct {
	uint8_t size;
	uint8_t capacity;
	uint8_t readIndex;
	uint8_t writeIndex;
	testStruct_t *data;
} ringB_t;

/* **************************************
	Extras
*************************************** */
void ringb_init(ringB_t *const input, uint8_t capacity, testStruct_t *const bufferLocation);
void ringb_reset(ringB_t *const input);

bool ringb_push(ringB_t *const input, testStruct_t item);
bool ringb_pop(ringB_t *const input, testStruct_t *const item);

bool ringb_isEmpty(ringB_t *const input);
bool ringb_isFull(ringB_t *const input);
uint8_t ringb_size(ringB_t *const input);

#endif
