#ifndef LLIB_H
#define LLIB_H

#include <stdlib.h>

typedef struct llib_rb_node_t {
	struct llib_rb_node_t *parent;
	struct llib_rb_node_t *left;
	struct llib_rb_node_t *right;
	void *value_ptr;
	char color;
} llib_rb_node_t;

llib_rb_node_t _rb_null = {0, 0, 0, 0, 1};

typedef llib_rb_node_t llib_rb_tree_t;

llib_rb_node_t *_node_arr[64] = {0};

#define LLIB_INIT_RB_TREE(tree) (*tree = &_rb_null)

#define LLIB_RB_INSERT(tree, new_value_ptr, compare) {\
	llib_rb_node_t *current_node = *tree;\
	llib_rb_node_t *parent_node = 0;\
	char direction;\
	while(current_node != &_rb_null) {\
		parent_node = current_node;\
		if (compare(new_value_ptr, current_node->value_ptr) < 0) {\
			current_node = current_node->left;\
			direction = 0;\
		} else {\
			current_node = current_node->right;\
			direction = 1;\
		}\
	}\
	current_node = malloc(sizeof(llib_rb_node_t));\
	if (parent_node != 0) {\
		if (direction) {\
			parent_node->right = current_node;\
		} else {\
			parent_node->left = current_node;\
		}\
	} else {\
		*tree = current_node;\
	}\
	current_node->parent = parent_node;\
	current_node->value_ptr = new_value_ptr;\
	current_node->left = &_rb_null;\
	current_node->right = &_rb_null;\
	current_node->color = 0;\
}

#define LLIB_RB_TRAVERSE_INORDER(tree, variable_name, action) {\
	int stack_size = 0;\
	llib_rb_node_t *current_node = tree;\
	while(stack_size > 0 || current_node != &_rb_null) {\
		if(current_node != &_rb_null) {\
			_node_arr[stack_size++] = current_node;\
			current_node = current_node->left;\
		} else {\
			current_node = _node_arr[--stack_size];\
			void * variable_name = current_node->value_ptr;\
			action;\
			current_node = current_node->right;\
		}\
	}\
}

#define _LLIB_RB_MOST_NODE(tree, result_node_ptr, direction) {\
    llib_rb_node_t *current_node = tree;\
	llib_rb_node_t *previous_node = 0;\
    while(current_node != &_rb_null){\
		previous_node = current_node;\
		current_node = current_node->direction;\
    }\
    *result_node_ptr = previous_node;\
}

#define _LLIB_RB_MINIMUM_NODE(tree, minimum_node) {\
	_LLIB_RB_MOST_NODE(tree, minimum_node, left);\
}

#define _LLIB_RB_MAXIMUX_NODE(tree, maximum_node) {\
	_LLIB_RB_MOST_NODE(tree, minimum_node, right);\
}

#define _LLIB_RB_IS_RIGHT(node) (node->parent->right == node)

#define _LLIB_RB_SUCCESSOR_NODE(tree, result_node_ptr) {\
	if (tree == 0) {\
		*result_node_ptr = 0;\
	} else if (tree->right != &_rb_null) {\
		_LLIB_RB_MINIMUM_NODE(tree->right, result_node_ptr);\
	} else if (tree->parent != 0) {\
		*result_node_ptr = tree;\
		while(*result_node_ptr->parent != 0 && _LLIB_RB_IS_RIGHT(*result_node_ptr)) {\
			*result_node_ptr = *result_node_ptr->parent;\
		}\
		*result_node_ptr = *result_node_ptr->parent;\
	} else {\
		*result_node_ptr = 0;\
	}\
}

#define _LLIB_RB_HEIGHT(tree, height_ptr) {\
	int head = 0, tail = 0, height = 0;\
}

#endif /* LLIB_H */


