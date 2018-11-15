#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity/unity.h"
#include "llib.h"

#define COMPARE_INT(a, b) ((*(int*)a) - (*(int*)b))
#define VALUE(node) (node ? *(int*)(node->value_ptr) : 0)

int compare_int(const void *a, const void *b) {
	return *(int*)a - *(int*)b;
}

void test_LLIB_RB() {
	llib_rb_tree_t * tree;
	LLIB_INIT_RB_TREE(tree);

	int values[] = {30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
	int length = sizeof(values) / sizeof(values[0]);
	for(int i = 0; i < length; i++) {
		LLIB_RB_INSERT(tree, &values[i], COMPARE_INT);
	}

	int *sorted_values = malloc(sizeof(values) * sizeof(int));
	memcpy(sorted_values, values, sizeof(values) * sizeof(int));
	qsort(sorted_values, length, sizeof(int), compare_int);
	int result_values[length], i = 0;
	LLIB_RB_TRAVERSE_INORDER(tree, value_ptr, result_values[i++] = *(int*)value_ptr);
	TEST_ASSERT_EQUAL_INT_ARRAY(sorted_values, result_values, length);

	llib_rb_node_t *minimum;
	_LLIB_RB_MINIMUM_NODE(tree, &minimum);
	TEST_ASSERT_EQUAL_INT(sorted_values[0], VALUE(minimum));

	llib_rb_node_t *current = minimum;
	for(int i = 0; i < length; i++) {
		llib_rb_node_t *successor;
		result_values[i] = VALUE(current);
		_LLIB_RB_SUCCESSOR_NODE(current, &successor);
		current = successor;
	}
	TEST_ASSERT_EQUAL_INT_ARRAY(result_values, sorted_values, length);

	int height;
	_LLIB_RB_HEIGHT(tree, &height);
	printf("Height of the tree is %d\n", height);
}

int main() {
	UNITY_BEGIN();
	RUN_TEST(test_LLIB_RB);

	test_LLIB_RB();
	return UNITY_END();
}
