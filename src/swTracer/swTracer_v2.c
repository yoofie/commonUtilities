/* **************************************
	File Name: SW Tracer
*************************************** */
#include "swTracer.h"
#include <string.h>
/* **************************************
	#defines
*************************************** */


/* **************************************
	Declarations
*************************************** */
static bool isDuplicateEvent(const char* msg1, const char* msg2);
swTracer_t sw;
/* **************************************
	Code
*************************************** */
void reset_swTraceBuffer(swTracer_t* const tracer) {
	if (tracer->resetSignal) {
		tracer->bufferIndex = 0;
		tracer->latestError = &tracer->dataBuffer[0];
		tracer->resetSignal = false;

		for (uint16_t i = 0; i < tracer->bufferSize; i++) {
			tracer->dataBuffer[i] = (event_t){
			.timeStamp = 0,
			.timeDelta = 0,
			.dupeInfo = {0, 0},
			.msg = "",
			.eventData = 0
			};
		}

	}
}

void swTracerInit(swTracer_t* const tracer) {
	reset_swTraceBuffer(tracer);
	tracer->bufferSize = TRACE_BUFFER_SIZE;
	tracer->sysTick = 0;
}



static uint32_t getPrevTimeDelta(swTracer_t* const tracer) {
	uint32_t retval = 0;
	if (tracer->bufferIndex > 0 && tracer->bufferIndex < tracer->bufferSize) {
		uint8_t prevIndex = tracer->bufferIndex - 1;
		uint32_t prevTime = tracer->dataBuffer[prevIndex].timeStamp;
		retval = (tracer->sysTick - prevTime);
	}
	return retval;
}

/* uint32_t calculateDelta(uint32_t start, uint32_t end) {
	return (uint32_t)(end - start);
} */

void sysTick(swTracer_t* const tracer) {
	tracer->sysTick++;
}

void addEvent(swTracer_t* const tracer, const char* msg) {

	if (tracer->bufferIndex == tracer->bufferSize) {
		tracer->bufferIndex = 0;
		tracer->cycles++;
	}
	uint8_t prevIndex = tracer->bufferIndex - 1;

	if (isDuplicateEvent(msg, tracer->dataBuffer[prevIndex].msg)) {
		// We have a duplicate event compared to last time, so lets take note of that
		tracer->dataBuffer[prevIndex].dupeInfo = (dupeInfo_t){
			.duplicateCounter = tracer->dataBuffer[prevIndex].dupeInfo.duplicateCounter + 1,
			.lastTimeStamp = tracer->sysTick // Last task cycle this event occured in
		};

	} else {
		tracer->dataBuffer[tracer->bufferIndex] = (event_t){
			.timeStamp = tracer->sysTick,
			.timeDelta = getPrevTimeDelta(tracer),
			.msg = msg,
			.eventData = 0,
			.eventData1 = 0,
			.eventData2 = 0
		};
		tracer->latestError = &tracer->dataBuffer[tracer->bufferIndex];
		tracer->bufferIndex++;
	}
}

void addExtEvent(swTracer_t* const tracer, const char* msg, uint8_t extData) {

	if (tracer->bufferIndex == tracer->bufferSize) {
		tracer->bufferIndex = 0;
		tracer->cycles++;
	}

	uint8_t prevIndex = tracer->bufferIndex - 1;

	if (isDuplicateEvent(msg, tracer->dataBuffer[prevIndex].msg)) {
		// We have a duplicate event compared to last time, so lets take note of that
		tracer->dataBuffer[prevIndex].dupeInfo = (dupeInfo_t){
			.duplicateCounter = tracer->dataBuffer[prevIndex].dupeInfo.duplicateCounter + 1,
			.lastTimeStamp = tracer->sysTick // Last task cycle this event occured in
		};

	} else {
		// This a distinct event that is different from the last entry
		tracer->dataBuffer[tracer->bufferIndex] = (event_t){
		.timeStamp = tracer->sysTick,
		.timeDelta = getPrevTimeDelta(tracer),
		.msg = msg,
		.eventData = extData,
		.eventData1 = 0,
		.eventData2 = 0
		};
		tracer->latestError = &tracer->dataBuffer[tracer->bufferIndex];
		tracer->bufferIndex++;
	}

}

void addExt2Event(swTracer_t* const tracer, const char* msg, uint8_t extData, uint8_t extData1) {

	if (tracer->bufferIndex == tracer->bufferSize) {
		tracer->bufferIndex = 0;
		tracer->cycles++;
	}

	uint8_t prevIndex = tracer->bufferIndex - 1;

	if (isDuplicateEvent(msg, tracer->dataBuffer[prevIndex].msg)) {
		// We have a duplicate event compared to last time, so lets take note of that
		tracer->dataBuffer[prevIndex].dupeInfo = (dupeInfo_t){
			.duplicateCounter = tracer->dataBuffer[prevIndex].dupeInfo.duplicateCounter + 1,
			.lastTimeStamp = tracer->sysTick // Last task cycle this event occured in
		};

	} else {
		tracer->dataBuffer[tracer->bufferIndex] = (event_t){
			.timeStamp = tracer->sysTick,
			.timeDelta = getPrevTimeDelta(tracer),
			.msg = msg,
			.eventData = extData,
			.eventData1 = extData1,
			.eventData2 = 0
		};
		tracer->latestError = &tracer->dataBuffer[tracer->bufferIndex];
		tracer->bufferIndex++;
	}
}

void addExt3dEvent(swTracer_t* const tracer, const char* msg, uint8_t extData, uint8_t extData1, uint8_t extData2) {

	if (tracer->bufferIndex == tracer->bufferSize) {
		tracer->bufferIndex = 0;
		tracer->cycles++;
	}

	uint8_t prevIndex = tracer->bufferIndex - 1;

	if (isDuplicateEvent(msg, tracer->dataBuffer[prevIndex].msg)) {
		// We have a duplicate event compared to last time, so lets take note of that
		tracer->dataBuffer[prevIndex].dupeInfo = (dupeInfo_t){
			.duplicateCounter = tracer->dataBuffer[prevIndex].dupeInfo.duplicateCounter + 1,
			.lastTimeStamp = tracer->sysTick // Last task cycle this event occured in
		};

	} else {
		tracer->dataBuffer[tracer->bufferIndex] = (event_t){
			.timeStamp = tracer->sysTick,
			.timeDelta = getPrevTimeDelta(tracer),
			.msg = msg,
			.eventData = extData,
			.eventData1 = extData1,
			.eventData2 = extData2
		};
		tracer->latestError = &tracer->dataBuffer[tracer->bufferIndex];
		tracer->bufferIndex++;
	}
}

static bool isDuplicateEvent(const char* msg1, const char* msg2) {
	// Simply check if the message pointers are the same
	bool retval = (msg1 == msg2) ? true : false;
	return retval;
}
