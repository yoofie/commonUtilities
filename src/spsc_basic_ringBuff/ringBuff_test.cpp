#include "ringBuff.h"
#include <iostream>
static testStruct_t buffer[4] = {0, {0}};

static ringB_t ring;

void test_push_normal();
void test_push_wrap();
void test_pop_normal();
void test_push_wrap_pop();

/* MAIN
******************************************************** */
int main(int argc, char **) {
	test_push_normal();
	test_push_wrap();
	test_push_wrap_pop();
	return 0;
}

void test_push_normal() {
	testStruct_t temp = {1U, {1, 1, 1}};
	testStruct_t t2;
	ringb_init(&ring, 4, buffer);

	(void)ringb_push(&ring, temp);
	(void)ringb_push(&ring, temp);
	(void)ringb_push(&ring, temp);
	(void)ringb_push(&ring, temp);

	ringb_pop(&ring, &t2);
}

void test_push_wrap() {
	testStruct_t temp = {1U, {1, 1, 1}};
	testStruct_t t2;
	ringb_init(&ring, 4, buffer);

	(void)ringb_push(&ring, temp);

	temp = {2, {2, 2, 2}};
	(void)ringb_push(&ring, temp);

	temp = {3, {3, 3, 3}};
	(void)ringb_push(&ring, temp);

	temp = {4, {4, 4, 4}};
	(void)ringb_push(&ring, temp);

	temp = {5, {5, 5, 5}};
	if (!ringb_push(&ring, temp)) {
		printf("FAILED"); // Expect to fail here
	}

	(void)ringb_pop(&ring, &t2);
}

void test_push_wrap_pop() {
	testStruct_t temp = {1U, {1, 1, 1}};
	testStruct_t t2;
	ringb_init(&ring, 4, buffer);

	(void)ringb_push(&ring, temp);

	temp = {2, {2, 2, 2}};
	(void)ringb_push(&ring, temp);

	temp = {3, {3, 3, 3}};
	(void)ringb_push(&ring, temp);

	temp = {4, {4, 4, 4}};
	(void)ringb_push(&ring, temp);

	temp = {5, {5, 5, 5}};
	if (!ringb_push(&ring, temp)) {
		printf("FAILED");
	}

	ringb_pop(&ring, &t2);
	ringb_pop(&ring, &t2);
	ringb_pop(&ring, &t2);
	ringb_pop(&ring, &t2);

	if (!ringb_pop(&ring, &t2)) {
		printf("FAILED");
	}
}
