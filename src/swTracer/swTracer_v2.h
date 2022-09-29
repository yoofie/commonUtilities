/* **************************************
	File Name: Software Tracer
	Created: Monday June 06 2022
	Version: 2.0
*************************************** */
#ifndef _SW_TRACER_H_
#define _SW_TRACER_H_
#include "Common.h"
#include <stdbool.h>


/* **************************************
	#defines
*************************************** */
#define TRACE_ENABLED
#define TRACE_BUFFER_SIZE 64

#define STRINGIZING(x) #x
#define STR(x) STRINGIZING(x)
#define FILE_LINE2 __FILE__ ":" STR(__LINE__)
#define FILE_LINE __BASE__ ":" STR(__LINE__)

#define JUST_LINE STR(__LINE__)
/* **************************************
	Declarations
*************************************** */
typedef struct {
	uint32_t duplicateCounter;
	uint32_t lastTimeStamp;
} dupeInfo_t;

typedef struct {
	uint32_t timeStamp;
	uint32_t timeDelta;
	dupeInfo_t dupeInfo;
	const char* msg;
	uint8_t eventData;
	uint8_t eventData1;
	uint8_t eventData2;
} event_t;

typedef struct {
	uint16_t bufferIndex;
	uint16_t bufferSize;
	uint32_t sysTick;
	bool resetSignal;
	uint8_t cycles;
	event_t* latestError;
	event_t dataBuffer[TRACE_BUFFER_SIZE];
} swTracer_t;


/* **************************************
	Extras
*************************************** */
extern swTracer_t sw;
/* Init & Reset */
extern void swTracerInit(swTracer_t* const tracer);
extern void reset_swTraceBuffer(swTracer_t* const tracer);

/* Timer Functions */
extern void sysTick(swTracer_t* const tracer);

//extern uint32_t calculateDelta(uint32_t start, uint32_t end);

/* Events */
extern void addEvent(swTracer_t* const tracer, const char* msg);
extern void addExtEvent(swTracer_t* const tracer, const char* msg, uint8_t extData);
extern void addExt2Event(swTracer_t* const tracer, const char* msg, uint8_t extData, uint8_t extData1);
extern void addExt3Event(swTracer_t* const tracer, const char* msg, uint8_t extData, uint8_t extData1, uint8_t extData2);

/* Easy to use macros */
#ifdef TRACE_ENABLED
#define LOGX(MSG) addEvent(&sw, MSG);
#define LOGX1(MSG, X1) addExtEvent(&sw, MSG, X1)
#define LOGX2(MSG, X1, X2) addExt2Event(&sw, MSG, X1, X2)
#define LOGX3(MSG, X1, X2, X3) addExt3Event(&sw, MSG, X1, X2, X3)
#else
#define LOGX(MSG)
#define LOGX1(MSG, X1)
#define LOGX2(MSG, X1, X2)
#define LOGX3(MSG, X1, X2, X3)
#endif
/* ********************************************************
	Software Tracer usage guide:
******************************************************** */
/*
STEP 1:
In some file where you want to trace events, add the header:
#include <stdbool.h>
#include "swTracer.h"

STEP 2: Declare and Init module:

swTracer_t sw;

and you in your init function: inititialize everything

swTracerInit(&sw);

STEP 3 (OPTIONAL BUT RECCOMENDED): Add system tick timer
It will increment by one everytime its hit. It gives you a sense of time.

sysTick(&sw);

STEP 4: Add events to Trace

addEvent(&sw, "Line XYZ | DESCRIPTIVE MSG HERE");

You can add events with upto 3 extra parameters:

addExt2Event(&sw, "Line XYZ | DESCRIPTIVE MSG HERE", var1);
addExt2Event(&sw, "Line XYZ | DESCRIPTIVE MSG HERE", var1, var2);
addExt3Event(&sw, "Line XYZ | DESCRIPTIVE MSG HERE", var1, var2, var3);

Where var 1, 2 & 3 are one byte (that means something specific for you.)

NEW in v2: You can now use macros:
LOGX(FILE_LINE " | YOUR MSG HERE");
LOGX1(FILE_LINE " | YOUR MSG HERE", param1);
LOGX2(FILE_LINE " | YOUR MSG HERE", param1, param2);
LOGX2(" | YOUR MSG HERE", param1, param2, param3);

Note the macro name as you add params. 
Also: Note that FILE_LINE is optional
 ********************************************************
	TO VIEW DATA
********************************************************
Set breakpoint and look at sw->dataBuffer structure.
sw->cycle counts how many times the buffer overflowed


 */

 /* ********************************************************
	 CHANGELOG
 ******************************************************** */
 /* V1: Initial implementation
 V2: Added dupeInfo data. This allows events to be duplicated without trashing
 the event data buffer with the same entries over and over again. Instead, we simply
 keep track of the first and last time the event happened and a count of how many
 times the event happened in a row.
  */
#endif
