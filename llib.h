#ifndef LLIB_H
#define LLIB_H

#include <stdlib.h>

#define _COLOR_RED 0
#define _COLOR_BLACK 1

typedef struct llib_rb_node_t {
	struct llib_rb_node_t *parent;
	struct llib_rb_node_t *left;
	struct llib_rb_node_t *right;
	void *value_ptr;
	char color;
} llib_rb_node_t;

llib_rb_node_t _rb_null = {0, 0, 0, 0, _COLOR_BLACK};

typedef llib_rb_node_t llib_rb_tree_t;

llib_rb_node_t *_node_arr[64] = {0};

#define _LLIB_RB_IS_RIGHT_CHILD(node) (node->parent->right == node)

#define LLIB_INIT_RB_TREE(root) (root = &_rb_null)

#define _LLIB_RB_TRANSPLANT(old_node, node) {\
	if (_LLIB_RB_IS_RIGHT_CHILD(old_node)) {\
		old_node->parent->right = node;\
	} else {\
		old_node->parent->left = node;\
	}\
	node->parent = old_node->parent;\
}

#define _LLIB_RB_ROTATE_LEFT(root, node) {\
	llib_rb_node_t *_node_to_rotate = node;\
	llib_rb_node_t *_right_child = _node_to_rotate->right;\
	_right_child->parent = _node_to_rotate->parent;\
	if (_right_child->parent == &_rb_null) {\
		root = _right_child;\
	} else if (_LLIB_RB_IS_RIGHT_CHILD(_node_to_rotate)) {\
		_right_child->parent->right = _right_child;\
	} else {\
		_right_child->parent->left = _right_child;\
	}\
	_node_to_rotate->right = _right_child->left;\
	if (_node_to_rotate->right != &_rb_null) {\
		_node_to_rotate->right->parent = _node_to_rotate;\
	}\
	_right_child->left = _node_to_rotate;\
	_node_to_rotate->parent = _right_child;\
}

#define _LLIB_RB_ROTATE_RIGHT(root, node) {\
	llib_rb_node_t *_node_to_rotate = node;\
	llib_rb_node_t *_left_child = _node_to_rotate->left;\
	_left_child->parent = _node_to_rotate->parent;\
	if (_left_child->parent == &_rb_null) {\
		root = _left_child;\
	} if (_LLIB_RB_IS_RIGHT_CHILD(_node_to_rotate)) {\
		_left_child->parent->right = _left_child;\
	} else {\
		_left_child->parent->left = _left_child;\
	}\
	_node_to_rotate->left = _left_child->right;\
	if (_node_to_rotate->left != &_rb_null) {\
		_node_to_rotate->left->parent = _node_to_rotate;\
	}\
	_left_child->right = _node_to_rotate;\
	_node_to_rotate->parent = _left_child;\
}

#define LLIB_RB_INSERT(root, new_value_ptr, compare) {\
	llib_rb_node_t *current_node = root;\
	llib_rb_node_t *parent_node = &_rb_null;\
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
	if (parent_node != &_rb_null) {\
		if (direction) {\
			parent_node->right = current_node;\
		} else {\
			parent_node->left = current_node;\
		}\
	} else {\
		root = current_node;\
	}\
	current_node->parent = parent_node;\
	current_node->value_ptr = new_value_ptr;\
	current_node->left = &_rb_null;\
	current_node->right = &_rb_null;\
	current_node->color = _COLOR_RED;\
	_LLIB_RB_INSERT_FIXUP(root, current_node);\
	root->color = _COLOR_BLACK;\
}

#define _LLIB_RB_INSERT_FIXUP(root, node_ptr) {\
	while(node_ptr->parent->color != _COLOR_BLACK) {\
		if(_LLIB_RB_IS_RIGHT_CHILD(node_ptr->parent)) {\
			if (node_ptr->parent->parent->left->color == _COLOR_RED) {\
				node_ptr->parent->parent->left->color = _COLOR_BLACK;\
				node_ptr->parent->parent->color = _COLOR_RED;\
				node_ptr->parent->color = _COLOR_BLACK;\
				node_ptr = node_ptr->parent->parent;\
			} else {\
				if(!_LLIB_RB_IS_RIGHT_CHILD(node_ptr)) {\
					_LLIB_RB_ROTATE_RIGHT(root, node_ptr->parent);\
					node_ptr = node_ptr->right;\
				}\
				_LLIB_RB_ROTATE_LEFT(root, node_ptr->parent->parent);\
				node_ptr->parent->color = _COLOR_BLACK;\
				node_ptr->parent->left->color = _COLOR_RED;\
			}\
		} else {\
			if (node_ptr->parent->parent->right->color == _COLOR_RED) {\
				node_ptr->parent->parent->right->color = _COLOR_BLACK;\
				node_ptr->parent->parent->color = _COLOR_RED;\
				node_ptr->parent->color = _COLOR_BLACK;\
				node_ptr = node_ptr->parent->parent;\
			} else {\
				if(_LLIB_RB_IS_RIGHT_CHILD(node_ptr)) {\
					_LLIB_RB_ROTATE_LEFT(root, node_ptr->parent);\
					node_ptr = node_ptr->left;\
				}\
				_LLIB_RB_ROTATE_RIGHT(root, node_ptr->parent->parent);\
				node_ptr->parent->color = _COLOR_BLACK;\
				node_ptr->parent->right->color = _COLOR_RED;\
			}\
		}\
	}\
}

#define _LLIB_RB_TRAVERSE_INORDER(root, action) {\
	int _stack_size = 0;\
	llib_rb_node_t *_current_node = root;\
	while(_stack_size > 0 || _current_node != &_rb_null) {\
		if(_current_node != &_rb_null) {\
			_node_arr[_stack_size++] = _current_node;\
			_current_node = _current_node->left;\
		} else {\
			_current_node = _node_arr[--_stack_size];\
			action;\
			_current_node = _current_node->right;\
		}\
	}\
}

#define LLIB_RB_TRAVERSE_INORDER(root, variable_name, action) {\
	_LLIB_RB_TRAVERSE_INORDER(root, {\
		void *variable_name = _current_node->value_ptr;\
		action;\
	});\
}

#define _LLIB_RB_MOST_NODE(root, result_node_ptr, direction) {\
    llib_rb_node_t *current_node = root;\
	llib_rb_node_t *previous_node = 0;\
    while(current_node != &_rb_null){\
		previous_node = current_node;\
		current_node = current_node->direction;\
    }\
    *result_node_ptr = previous_node;\
}

#define _LLIB_RB_MINIMUM_NODE(root, minimum_node) {\
	_LLIB_RB_MOST_NODE(root, minimum_node, left);\
}

#define _LLIB_RB_MAXIMUX_NODE(root, maximum_node) {\
	_LLIB_RB_MOST_NODE(root, minimum_node, right);\
}

#define _LLIB_RB_SUCCESSOR_NODE(root, result_node_ptr) {\
	if (root == 0) {\
		*result_node_ptr = 0;\
	} else if (root->right != &_rb_null) {\
		_LLIB_RB_MINIMUM_NODE(root->right, result_node_ptr);\
	} else if (root->parent != 0) {\
		*result_node_ptr = root;\
		while(*result_node_ptr->parent != 0 && _LLIB_RB_IS_RIGHT_CHILD(*result_node_ptr)) {\
			*result_node_ptr = *result_node_ptr->parent;\
		}\
		*result_node_ptr = *result_node_ptr->parent;\
	} else {\
		*result_node_ptr = 0;\
	}\
}

#define _LLIB_RB_HEIGHT(root, height_ptr) {\
	*height_ptr = -1;\
	_LLIB_RB_TRAVERSE_INORDER(root, {\
		if (_stack_size > *height_ptr) *height_ptr = _stack_size;\
	});\
	*height_ptr += 1;\
}

#endif /* LLIB_H */
