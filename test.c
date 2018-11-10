#include <stdio.h>
#include "unity/unity.h"
#include "llib.h"



#define COMPARE_INT(a, b) ((*(int*)a) - (*(int*)b))
#define VALUE(node) (node ? *(int*)(node->value_ptr) : 0)

void test_LLIB_RB() {
	llib_rb_tree_t * tree;
	LLIB_INIT_RB_TREE(&tree);

	int values[] = {2,5,3,8,4,20,7,9,12};
	int length = sizeof(values) / sizeof(values[0]);
	for(int i = 0; i < length; i++) {
		LLIB_RB_INSERT(&tree, &values[i], COMPARE_INT);
	}

	int sorted_values[] = {2,3,4,5,7,8,9,12,20};
	int result_values[length], i = 0;
	LLIB_RB_TRAVERSE_INORDER(tree, value_ptr, result_values[i++] = *(int*)value_ptr);
	TEST_ASSERT_EQUAL_INT_ARRAY(sorted_values, result_values, length);

	llib_rb_node_t *minimum;
	_LLIB_RB_MINIMUM_NODE(tree, &minimum);
	TEST_ASSERT_EQUAL_INT(2, VALUE(minimum));

	llib_rb_node_t *current = minimum;
	for(int i = 0; i < length; i++) {
		llib_rb_node_t *successor;
		result_values[i] = VALUE(current);
		_LLIB_RB_SUCCESSOR_NODE(current, &successor);
		current = successor;
	}
	TEST_ASSERT_EQUAL_INT_ARRAY(result_values, sorted_values, length);
}

int main() {
	UNITY_BEGIN();
	RUN_TEST(test_LLIB_RB);
	return UNITY_END();
}
