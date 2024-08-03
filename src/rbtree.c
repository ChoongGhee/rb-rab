#include "rbtree.h"

#include <stdlib.h>

void left_rotate(rbtree *t, node_t *x){
    node_t * y;
  
    y =x->right;
    x->right = y->left;
    if(y->left != t->nil){y->left->parent = x;}
    y->parent = x->parent;
    if(x->parent == t->nil){t->root = y;}
    else if ( x == x->parent->left){x->parent->left = y;}
    else{x->parent->right = y;}
    y->left = x;
    x->parent = y;
  }

void right_rotate(rbtree *t, node_t *x){
    node_t * y;
    
    y =x->left;
    x->left = y->right;
    if(y->right != t->nil){y->right->parent = x;}
    y->parent = x->parent;
    if(x->parent == t->nil){t->root = y;}
    else if ( x == x->parent->left){x->parent->left = y;}
    else{x->parent->right = y;}
    y->right = x;
    x->parent = y;
  }


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  node_t *sentinel = (node_t*)malloc(sizeof(node_t));
  sentinel->color = RBTREE_BLACK;
  sentinel->key = 0;
  sentinel->left = NULL;
  sentinel->right = NULL;
  sentinel->parent = NULL;

  p->nil = sentinel;
  p->root = sentinel;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {

  node_t *new1= (node_t*)malloc(sizeof(node_t));
  new1->key = key;
  new1->left = t->nil;
  new1->right = t->nil;
  new1->color = RBTREE_RED;

  node_t *position_find = t->root;
  node_t *check = t->nil;
  while(position_find != check){
    check = position_find;
    if(position_find->key == key){
      free(new1);
      return NULL;
    }
    else if(position_find->key < key){
      position_find =position_find->right;
    }
    else{
      position_find = position_find->left;
    }
  }

  new1->parent = check;
  if (check == t->nil)
    t->root = new1;
  else if (key < check->key)
    check->left = new1;
  else
    check->right = new1;
  // 여기 까지 insert

    node_t * temp = new1;
    while(temp->parent->color == RBTREE_RED && temp !=t->root){

    node_t *uncle;

    if(temp->parent->parent->left == temp->parent){
      uncle = temp->parent->parent->right;

      if(uncle->color == RBTREE_RED){
        temp->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        temp->parent->parent->color = RBTREE_RED;
        temp=temp->parent->parent;
      }
      else if(temp == temp->parent->right){
        temp = temp->parent;
        left_rotate(t,temp);
      }
      
      temp->parent->color = RBTREE_BLACK;
      temp->parent->parent->color = RBTREE_RED;
      right_rotate(t,temp->parent->parent);
    }

    else{ 
      uncle = temp->parent->parent->left;

      if(uncle->color == RBTREE_RED){
        temp->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        temp->parent->parent->color = RBTREE_RED;
        temp=temp->parent->parent;
      }
      else if(temp == temp->parent->left){
        temp = temp->parent;
        right_rotate(t,temp);
      }
      
      temp->parent->color = RBTREE_BLACK;
      temp->parent->parent->color = RBTREE_RED;
      left_rotate(t,temp->parent->parent);
    }
    t->root->color = RBTREE_BLACK;
    }

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
