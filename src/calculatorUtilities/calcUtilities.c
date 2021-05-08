/* **************************************
	File Name: Calculator Utilities
*************************************** */
#include <stdint.h>
/* **************************************
	#defines
*************************************** */


/* **************************************
	Declarations
*************************************** */


/* **************************************
	Code
*************************************** */

/* ********************************************************
	Find Byte Index
	===============
	Given a decimal number, this function will find how many 
	byte index it falls into.

	Value: input number
	MaxLimit: UINT8_MAX
******************************************************** */
uint32_t findByteIndex(uint32_t value) {
	uint32_t byteIndex = 0;
	for (uint8_t i = 1; i < UINT8_MAX; i++) {
		if ((value < (8u * i)))
		{
			byteIndex = i;
			break;
		}
	}
	return byteIndex;
}

