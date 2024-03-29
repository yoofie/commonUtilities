

#include "version.h"
#include <filesystem>
#include <iostream>

#include "ringBuff.h"

static testStruct_t buffer[4] = {0, {0}};

static ringB_t ring;

void test_push_normal();
void test_push_wrap();
void test_pop_normal();
void test_push_wrap_pop();
void test_pop_first_normal();
void test_pop_first_wrap();
void test_clear_pop();

void test_clear_buffer();
void test_peek_first_normal();
void test_peek_first_wrap();

void test_scan_items();
void test_scan_items_limit();
void test_scan_items_limit_wrap();
void test_scan_items_element_limit_wrap();

static bool breakpoint = false;
/* MAIN
******************************************************** */
int main(int argc, char **argv) {
	test_push_normal();
	test_push_wrap();
	test_push_wrap_pop();
	test_pop_first_normal();
	test_pop_first_wrap();
	test_clear_pop();

	test_peek_first_normal();
	test_peek_first_wrap();

	test_scan_items();
	test_scan_items_limit();
	test_scan_items_limit_wrap();
	test_scan_items_element_limit_wrap();
	return 0;
}

/* ********************************************************
	TEST FUNCCTIONS TO EXERCISE FUNCTIONALITY
******************************************************** */

void test_push_normal() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	ringb_init(&ring, 4, buffer);

	(void)ringb_push(&ring, newData);
	(void)ringb_push(&ring, newData);
	(void)ringb_push(&ring, newData);
	(void)ringb_push(&ring, newData);

	ringb_pop(&ring, &temp);
}

void test_push_wrap() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	ringb_init(&ring, 4, buffer);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){5, {5, 5, 5}};
	if (!ringb_push(&ring, newData)) {
		printf("FAILED"); // Expect to fail here
	}

	(void)ringb_pop(&ring, &temp);
}

void test_push_wrap_pop() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	ringb_init(&ring, 4, buffer);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){5, {5, 5, 5}};
	if (!ringb_push(&ring, newData)) {
		printf("FAILED");
	}

	ringb_pop(&ring, &temp);
	ringb_pop(&ring, &temp);
	ringb_pop(&ring, &temp);
	ringb_pop(&ring, &temp);

	if (!ringb_pop(&ring, &temp)) {
		printf("FAILED");
	}
}

void test_pop_first_normal() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	ringb_init(&ring, 4, buffer);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){5, {5, 5, 5}};
	if (!ringb_pop_first(&ring, &temp)) {
		printf("FAILED");
	}
}
void test_pop_first_wrap() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	ringb_init(&ring, 4, buffer);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){5, {5, 5, 5}};

	if (!ringb_pop_first(&ring, &temp)) {
		printf("FAILED");
	} else {
		printf("PASSED");
		// write index should be 3
	}
}

// Clearing function needed for next test (test_clear_pop())
void clearData(testStruct_t *data) {
	data->index = 0;
	data->data[0] = 0;
	data->data[1] = 1;
	data->data[2] = 2;
}

void test_clear_pop() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	ringb_initExtended(&ring, 4, buffer, &clearData);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	// Observe the data being cleared as you pop (inside the debugger)
	(void)ringb_clear_pop(&ring, &temp);
	(void)ringb_clear_pop(&ring, &temp);

	newData = (testStruct_t){5, {5, 5, 5}};
	(void)ringb_push(&ring, newData);
}

void test_clear_buffer() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	ringb_initExtended(&ring, 4, buffer, &clearData);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	ringb_clear_buffer(&ring);
}

void test_peek_first_normal() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	ringb_init(&ring, 4, buffer);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	// Peek oldest
	if (!ringb_peek(&ring, &temp)) {
		printf("FAILED");
	}

	// Peek latest
	if (!ringb_peek_first(&ring, &temp)) {
		printf("FAILED");
	}
}

void test_peek_first_wrap() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	ringb_init(&ring, 4, buffer);
	ringb_register_clear_function(&ring, clearData);
	ringb_clear_buffer(&ring);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);
	uint8_t ringb__size = ringb_size(&ring);
	uint8_t total = ringb_space_available(&ring);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	(void)ringb_clear_pop(&ring, &temp);

	newData = (testStruct_t){5, {5, 5, 5}};
	(void)ringb_push(&ring, newData);

	// Peek oldest
	if (ringb_peek(&ring, &temp)) {
		printf("PASS\n");
	}

	// Peek latest
	if (ringb_peek_first(&ring, &temp)) {
		printf("PASS\n");
	}

	total = ringb_space_available(&ring);
	uint8_t totalBytes = ringb_total_data_size_bytes(&ring);
	printf("TOTAL SLOTS: %u\nTOTAL BYTES: %u\nTOTAL SIZE: %u", total, totalBytes, ringb__size);
	breakpoint = true;
}

/* ********************************************************
	Scan items
******************************************************** */
typedef struct {
	uint8_t counter;
} dummyTestStruct_t;

void user_defined_function(uint8_t index, void *userData) {
	dummyTestStruct_t *tst = (dummyTestStruct_t *)userData;
	tst->counter = index;
	printf("\nIndex[%u]", index);
}
void user_defined_function2(uint8_t index, testStruct_t *currentElement, void *userData) {
	dummyTestStruct_t *tst = (dummyTestStruct_t *)userData;
	tst->counter = index;
	testStruct_t *currentItem = (testStruct_t *)currentElement;

	printf("\nIndex[%u] | %u\t", index, currentItem->index);
	printf("{");
	for (uint8_t i = 0; i < NUM_OF_ELEM_IN_DATA_ARRAY; i++) {
		printf("%u ", currentItem->data[i]);
	}
	printf("}");
}

/* Tests
******************************************************** */
// Test a normal scan through all items
void test_scan_items() {
	testStruct_t newData = {1U, {1, 1, 1}};
	// testStruct_t temp;

	ringb_init(&ring, 4, buffer);
	ringb_register_clear_function(&ring, clearData);
	ringb_clear_buffer(&ring);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	// newData = (testStruct_t){4, {4, 4, 4}};
	//(void)ringb_push(&ring, newData);

	dummyTestStruct_t dummy;
	ringb_scan_items(&ring, user_defined_function, &dummy);
	breakpoint = true;
}

// Test scan when we are at array limit
void test_scan_items_limit() {
	testStruct_t newData = {1U, {1, 1, 1}};
	// testStruct_t temp;
	printf("\n");
	ringb_init(&ring, 4, buffer);
	ringb_register_clear_function(&ring, clearData);
	ringb_clear_buffer(&ring);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	dummyTestStruct_t dummy;
	ringb_scan_items(&ring, user_defined_function, &dummy);
	breakpoint = true;
}

void test_scan_items_limit_wrap() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	printf("\n");
	ringb_init(&ring, 4, buffer);
	ringb_register_clear_function(&ring, clearData);
	ringb_clear_buffer(&ring);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	(void)ringb_clear_pop(&ring, &temp);

	newData = (testStruct_t){5, {5, 5, 5}};
	(void)ringb_push(&ring, newData);

	dummyTestStruct_t dummy;
	ringb_scan_items(&ring, user_defined_function, &dummy);
	breakpoint = true;
}

void test_scan_items_element_limit_wrap() {
	testStruct_t newData = {1U, {1, 1, 1}};
	testStruct_t temp;
	printf("\n");
	ringb_init(&ring, 4, buffer);
	ringb_register_clear_function(&ring, clearData);
	ringb_clear_buffer(&ring);

	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){2, {2, 2, 2}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){3, {3, 3, 3}};
	(void)ringb_push(&ring, newData);

	newData = (testStruct_t){4, {4, 4, 4}};
	(void)ringb_push(&ring, newData);

	(void)ringb_clear_pop(&ring, &temp);

	newData = (testStruct_t){5, {5, 5, 5}};
	(void)ringb_push(&ring, newData);

	dummyTestStruct_t dummy;
	ringb_scan_item_element(&ring, user_defined_function2, &dummy);
	breakpoint = true;
	printf("\n");
}
