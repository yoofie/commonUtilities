# swTracer

A primitive in-memory trace buffer.

## How to use
STEP 1:
In some file where you want to trace events, add the header:
#include <stdbool.h>
#include "swTracer.h"

### STEP 2: Declare and Init module:
```
swTracer_t sw;
```
and you in your init function: inititialize everything
```
swTracerInit(&sw);
```
### STEP 3 (OPTIONAL BUT RECCOMENDED): Add system tick timer
It will increment by one everytime its hit. It gives you a sense of time.
```
sysTick(&sw);
```
### STEP 4: Add events to Trace
```
addEvent(&sw, "Line XYZ | DESCRIPTIVE MSG HERE");
```
You can add events with upto 3 extra parameters:
```
addExt2Event(&sw, "Line XYZ | DESCRIPTIVE MSG HERE", var1);
addExt2Event(&sw, "Line XYZ | DESCRIPTIVE MSG HERE", var1, var2);
addExt3Event(&sw, "Line XYZ | DESCRIPTIVE MSG HERE", var1, var2, var3);
```
Where var 1, 2 & 3 are one byte (that means something specific for you.)

NEW in v2: You can now use macros:
```
LOGX(FILE_LINE " | YOUR MSG HERE");
LOGX1(FILE_LINE " | YOUR MSG HERE", param1);
LOGX2(FILE_LINE " | YOUR MSG HERE", param1, param2);
LOGX2(" | YOUR MSG HERE", param1, param2, param3);
```
Note the macro name as you add params. 
Also: Note that FILE_LINE is optional

## TO VIEW DATA

Set breakpoint and look at sw->dataBuffer structure.
sw->cycle counts how many times the buffer overflowed

