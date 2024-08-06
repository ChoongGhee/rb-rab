#include "rbtree.h"

#include <stdlib.h>
// initialaize
rbtree *new_rbtree(void);
// 트리 삭제 부분
void recursive_delete_rbtree(rbtree *t, node_t *node);
void delete_rbtree(rbtree *t);
// 삽입 부분
void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
node_t *rbtree_insert(rbtree *t, const key_t key);
// 찾기, 최소, 최대 등등
node_t *rbtree_find(const rbtree *t, const key_t key);
node_t *rbtree_min(const rbtree *t);
node_t *rbtree_max(const rbtree *t);
// 삭제 부분
void erase_fixup(rbtree *t, node_t *node, node_t *parent);
void transplant(rbtree *t, node_t *node, node_t *change);
int rbtree_erase(rbtree *t, node_t *p);

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n);

// tree 생성 부분@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t *sentinel = (node_t *)malloc(sizeof(node_t));
  sentinel->color = RBTREE_BLACK;
  sentinel->key = 0;
  sentinel->left = NULL;
  sentinel->right = NULL;
  sentinel->parent = NULL;

  p->nil = sentinel;
  p->root = sentinel;

  return p;
}

// tree delete 부분@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void recursive_delete_rbtree(rbtree *t, node_t *node)
{
  if (node == t->nil)
    return;
  recursive_delete_rbtree(t, node->right);
  recursive_delete_rbtree(t, node->left);

  free(node);
}
void delete_rbtree(rbtree *t)
{
  recursive_delete_rbtree(t, t->root);
  free(t->nil);
  free(t);
}

// insert 부분@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y;

  y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y;

  y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
  {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{

  node_t *new1 = (node_t *)malloc(sizeof(node_t));
  new1->key = key;
  new1->left = t->nil;
  new1->right = t->nil;
  new1->color = RBTREE_RED;

  // 새 노드를 트리에 연결
  node_t *parent = t->nil;
  node_t *position_find = t->root;

  while (position_find != t->nil)
  {
    parent = position_find;
    if (key < position_find->key)
    {
      position_find = position_find->left;
    }
    else
    {
      position_find = position_find->right;
    }
  }

  new1->parent = parent;
  if (parent == t->nil)
  {
    t->root = new1;
  }
  else if (key < parent->key)
  {
    parent->left = new1;
  }
  else
  {
    parent->right = new1;
  }
  // 여기 까지 insert
  // fixup

  node_t *temp = new1;
  while (temp != t->root && temp->parent->color == RBTREE_RED)
  {

    node_t *uncle;
    // 할아버지 기준 부모의 위치에 따라 어떤 작업을 해야할지 달라짐. (데칼코마니)
    if (temp->parent->parent->left == temp->parent)
    {
      uncle = temp->parent->parent->right;
      // 경우1 엉클샘
      if (uncle->color == RBTREE_RED)
      {
        temp->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        temp->parent->parent->color = RBTREE_RED;
        temp = temp->parent->parent;
        // continue;
        // while문 할아버지 기준 다시 시작
      }
      // 경우 2,3 척추펴 -> 관성드리프트
      // 이 부분의 경우 1회만 실시하면 모든 조건이 만족됨. So 마지막에 break
      else
      {
        // 척추 여부
        if ((temp == temp->parent->right))
        {
          // 드리프트(rotate)를 위해 노드 변경
          temp = temp->parent;
          left_rotate(t, temp);
        }
        // 척추 여부 검사 후 관성드리프트
        temp->parent->color = RBTREE_BLACK;
        temp->parent->parent->color = RBTREE_RED;
        right_rotate(t, temp->parent->parent);
        // while문으로 넘어가지 않고 싶기 때문에 break
        break;
      }
    }

    else
    {
      uncle = temp->parent->parent->left;

      if (uncle->color == RBTREE_RED)
      {
        temp->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        temp->parent->parent->color = RBTREE_RED;
        temp = temp->parent->parent;
        continue;
      }
      else
      {
        if (temp == temp->parent->left)
        {
          temp = temp->parent;
          right_rotate(t, temp);
        }

        temp->parent->color = RBTREE_BLACK;
        temp->parent->parent->color = RBTREE_RED;
        left_rotate(t, temp->parent->parent);
        break;
      }
    }
  }
  t->root->color = RBTREE_BLACK;

  return new1;
}

// find 부분 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  if (t->root == t->nil)
  {
    return NULL;
  }

  node_t *temp = t->root;
  while (temp != t->nil)
  {
    if (temp->key == key)
    {
      break;
    }
    else if (temp->key < key)
    {
      temp = temp->right;
    }
    else
    {
      temp = temp->left;
    }
  }

  if (temp == t->nil)
  {
    return NULL;
  }
  else
  {
    return temp;
  }
}

// erase 부분 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void erase_fixup(rbtree *t, node_t *x, node_t *parent)
{
  // 형제 노드 설정
  node_t *w;
  //  갱신후 x가 root가 되고 색이 검정이면 tree완성
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    // 부모기준 x가 좌측일 경우
    if (x == parent->left)
    {
      w = parent->right;
      // case 1
      // 형제가 red일 경우(조카들은 자연스레 무조건 black임)
      if (w->color == RBTREE_RED)
      {
        // 1. 회전 후 속성을 만족 시키기 위해 회전 전 부모와 형제의 색을 바꿔줌.
        w->color = RBTREE_BLACK;
        parent->color = RBTREE_RED;
        // 2. 부모 기준 좌회전
        left_rotate(t, parent);
        // 3. 이제 x기준 case 2,3,4가 됨. 다시 검사. (x는 그대로)
        // 3-1 회전 과정에서 형제가 바뀜. So 다시 업데이트
        w = parent->right;
      }
      // case 2
      // 형제, 사촌들 모두 블랙일 때, all black
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        // 1. extra blacc과 형제의 black을 부모로 전가함.
        // 2. So 형제의 색 red로 바꿈
        w->color = RBTREE_RED;
        // while에 부모가 검사 받을 수 있게 조정(만약 red라면 red and black, black이라면 더블리 블랙)
        x = parent;
        // 검사할 부모도 같이 조정
        parent = x->parent;
      }
      else
      {
        // case 3
        // 더블리 블랙의 형제가 black & 그 형제의 오른자녀(else는 왼자녀)가 블랙 && 왼자녀(else는 오른자녀)가 red일 때
        // +@ 부모의 색은 몰루(노 상관)
        // 왼쪽 사촌의 red를 오른 사촌으로 옮기기 위함.
        if (w->right->color == RBTREE_BLACK)
        {
          // 1. 왼쪽 사촌과 형제의 색 교환
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          // 2. 그후 형제 기준 관성드리프트
          right_rotate(t, w);
          // 관성드리프트 후 형제(w)는 rotate전 w의 왼 자녀가 됨. 그래서 형제를 그것으로 업데이트
          w = parent->right;
        }
        // case4
        // 더블리 블랙의 형제가 black & 그 형제의 오른자녀(else는 왼자녀)가 red일 때
        // +@ 부모와 그 나머지 자녀는 몰루(노 상관)
        // 먼저 형제의 black을 조카들한테 뿌림 (1. 형제 red > 조카들 블랙 하나씩 red인 조카 black)
        w->color = parent->color;     // 아래에서 rotate를 하는 데 결론적으로 형제는 부모 위치로 오게됨. (2. 부모와 형제의 color값 변경)
        parent->color = RBTREE_BLACK; // (3. 마지막으로 회전 후 좌측 조카(색을 모르는 조카)가 형제의 black을 들고오고 검사하는 x 노드의 더블 블랙과 합쳐서 부모를 black으로 해줌)
        // 조카가 nil이 아닐 때 Black으로 해줌(1. 형제의 색 전파)
        w->right->color = RBTREE_BLACK;
        // 색을 넘기고(구현에선 결론만 남음 So 색칠 X) 부모기준 좌회전
        left_rotate(t, parent);
        break;
      }
    }
    else
    {
      w = parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        parent->color = RBTREE_RED;
        right_rotate(t, parent);
        w = parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = parent;
        parent = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = parent->left;
        }
        w->color = parent->color;
        parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, parent);
        break;
      }
    }
  }

  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *node)
{
  color_t original_color = node->color;
  node_t *target = node;
  node_t *child;

  // 후임자 찾기 및 값 교체
  // 일단 후임자 찾기
  // 만약 자식이 1개 이하의 경우 바로 아래 case로 감.
  if (node->left != t->nil && node->right != t->nil)
  {
    target = node->right;
    while (target->left != t->nil)
    {
      target = target->left;
    }
    node->key = target->key;
    original_color = target->color;
  }
  // 실제로 제거될 노드(target)의 자식 결정
  // target 자식이 1일 때 if문에서 걸려져 좌측에 대체할 자식을 갖고 있음. 나머지 경우는 모두 target->right
  // target의 자식이 2개일 경우 위에서 후임자를 찾고 항상 삭제 후 대체 할 child은 right를 가짐
  child = (target->left != t->nil) ? target->left : target->right;

  // 노드 제거 및 연결 조정
  // parent는 target이 free될 때 부모의 정보를 저장하기 위함.
  node_t *parent = target->parent;
  // child 가 있다면 parent에 연결해줌.
  if (child != t->nil)
  {
    child->parent = parent;
  }

  // parent가 nil이면 현재 target이 root이므로 root에 child를 연결
  if (parent == t->nil)
  {
    t->root = child;
    // 아닌 경우 parent의 reft, right를 확인 후 연결
  }
  else if (target == parent->left)
  {
    parent->left = child;
  }
  else
  {
    parent->right = child;
  }

  // 메모리 해제
  if (target != node)
  {
    free(target);
  }
  else
  {
    free(node);
  }
  // 경우가 2가지 생김, child가 nil일 경우 아닐 경우
  //
  // 필요한 경우 균형 조정
  if (original_color == RBTREE_BLACK)
  {
    erase_fixup(t, child, parent);
  }

  return 0;
}

// min/max 부분 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
node_t *rbtree_min(const rbtree *t)
{
  node_t *search_node = t->root;
  node_t *min_node = t->root;
  while (search_node != t->nil)
  {
    min_node = search_node;
    search_node = search_node->left;
  }
  if (min_node == t->nil)
  {
    return NULL;
  }
  return min_node;
}

node_t *rbtree_max(const rbtree *t)
{
  node_t *search_node = t->root;
  node_t *max_node = t->root;
  while (search_node != t->nil)
  {
    max_node = search_node;
    search_node = search_node->right;
  }
  if (max_node == t->nil)
  {
    return NULL;
  }
  return max_node;
}

// 중위순회 부분 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
int recursive_DFS(const rbtree *t, key_t *arr, node_t *node, size_t *n)
{
  if (node == t->nil)
  {
    return 0;
  }
  recursive_DFS(t, arr, node->left, n);
  int a = (*n);
  arr[a] = node->key;
  (*n)++;
  recursive_DFS(t, arr, node->right, n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  if (t == NULL || arr == NULL)
  {
    return -1;
  }
  int num = 0;
  int count = recursive_DFS(t, arr, t->root, &num);

  return count;
}