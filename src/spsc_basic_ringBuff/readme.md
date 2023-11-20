# Super basic SPSC Ring Buffer

This is a basic single producer single consumer ring buffer meant for use in embedded microcontrollers. It is backed by a fixed size static array.

## API
```C
void ringb_init(ringB_t *const input, uint8_t capacity, testStruct_t *const bufferLocation);
void ringb_reset(ringB_t *const input);

bool ringb_push(ringB_t *const input, testStruct_t item);
bool ringb_pop(ringB_t *const input, testStruct_t *const item);

bool ringb_isEmpty(ringB_t *const input);
bool ringb_isFull(ringB_t *const input);
uint8_t ringb_size(ringB_t *const input);
```
Since C does not support generics, I have used `testStruct_t` as a placeholder which is meant to be swapped with your own datastructure which is meant to be used with the ring buffer.

# Examples
```C
static testStruct_t buffer[4] = {0, {0}};

static ringB_t ring;

testStruct_t newData = {1U, {1, 1, 1}};
testStruct_t popBuffer;
ringb_init(&ring, 4, buffer);

if(!ringb_push(&ring, newData)){
	// Do something if we cant push (buffer is full)
}

if(!ringb_pop(&ring, popBuffer)){
	// Do something if we cant pop (buffer is empty)
}
```
