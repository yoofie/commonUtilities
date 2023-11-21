# Super basic SPSC Ring Buffer Extended

This is a basic single producer single consumer ring buffer meant for use in embedded microcontrollers. It is up to you, the programmer, to make sure you use it safely in interrupt context or in multithreaded applications.

## Features
- It is backed by a fixed size static array.
- This version has an extended API for potentially useful operations you may want to do. 

# Notes

- Since C does not support generics, this datastructure needs to have its data types, such as `uint8_t` and `testStruct_t` replaced for your specific use case. This should be a simple search and replace in your editor of choice.
- Some functions return `bool`. All functions return true if intended operation was successful. Otherwise, false if intended operation failed.
- By default, this data structure will not overwrite data. 

## API
```C
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

/* Meta data & support functions
******************************************************** */
bool ringb_isEmpty(ringB_t *const input);
bool ringb_isFull(ringB_t *const input);
uint8_t ringb_size(ringB_t *const input);
uint8_t ringb_free_space(ringB_t *const input);
uint8_t ringb_space_available(ringB_t *const input);
uint32_t ringb_total_data_size_bytes(ringB_t *const input);
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
