#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *NODE = (node_t *)calloc(1, sizeof(node_t));
  NODE->color = RBTREE_BLACK;
  p->root = NODE;
  p->nil = NODE;

  return p;
}


void delete_node(node_t *p, rbtree *t){ 
  if (p == t->nil) {
    return;
  }
  delete_node(p->left, t);
  delete_node(p->right, t);
  free(p);
  p = NULL;
}


void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t->root, t);
  free(t->nil);
  t->nil = NULL;
  free(t);
  t = NULL;
}


void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;             // y 설정
  x->right = y->left;               // y의 왼쪽 자식트리(b)를 x의 오른쪽 자식트리로 옮김
  if (y->left != t->nil)            // y의 왼쪽 자식트리(b)가 nil이 아니라면
    y->left->parent = x;            // y의 왼쪽 자식트리(b)의 부모를 x로 설정한다.
  y->parent = x->parent;            // x의 부모를 y의 부모로 연결

  if (x->parent == t->nil)          // x의 부모가 없다 == 즉, x가 루트라면,
    t->root = y;                    // y는 rbtree의 root가 된다.
  else if (x == x->parent->left)    // x가 부모의 왼쪽자식이라면,
    x->parent->left = y;            // y를 x의 부모의 왼쪽자식으로 설정
  else                              // x가 부모의 오른쪽자식이라면, 
    x->parent->right = y;           // y를 x의 부모의 오른쪽자식으로 설정
  y->left = x;                      // y의 왼쪽자식에 x를 설정
  x->parent = y;                    // x의 부모를 y로 설정
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left; 
  x->left = y->right;
  if (y->right != t->nil)  
    y->right->parent = x;  
  y->parent = x->parent; 

  if (x->parent == t->nil)  
    t->root = y;            
  else if (x == x->parent->right)  
    x->parent->right = y;          
  else
    x->parent->left = y; 
  y->right = x;
  x->parent = y;
}



void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;                                    // z의 삼촌 노드

  while (z->parent->color == RBTREE_RED) {      // z의 부모 색이 빨강
    if (z->parent == z->parent->parent->left) { // 부모가 왼쪽 자식인 경우
      y = z->parent->parent->right;             // 부모의 오른쪽 자식을 삼촌으로 지정
                                                // case 1
      if (y->color == RBTREE_RED)               // 삼촌이 빨강인 경우(z, z의 부모는 빨강)
      {
        z->parent->color = RBTREE_BLACK;        // 부모를 검정으로
        y->color = RBTREE_BLACK;                // 삼촌을 검정으로
        z->parent->parent->color = RBTREE_RED;  // 할아버지를 빨강으로
        z = z->parent->parent;                  // 시점을 할아버지로 변경. (할아버지(빨강)인 상황에서 할아버지 부모가 빨강이라면 문제상황이 되므로)
      }
      else {                                    // 삼촌이 검정인 경우(z, z의 부모는 빨강)
                                                // case 2 : case 3 형태를 만들기 위한 작업
        if (z == z->parent->right) {            // z가 오른쪽 자식인 경우
          z = z->parent;                        // left_rotate를 위한 사전 작업 
          left_rotate(t, z);                    // -> z를 왼쪽 자식으로 만들어 줌
        }
                                                // case 3
        z->parent->color = RBTREE_BLACK;        // right_rotate를 위한 사전 작업 1
        z->parent->parent->color = RBTREE_RED;  // right_rotate를 위한 사전 작업 2
        right_rotate(t, z->parent->parent);     // -> z의 할아버지가 같은 부모 밑의 형제가 됨
      }
    }

    else {                                      // 부모가 오른쪽 자식인 경우(대칭)
      y = z->parent->parent->left;              // 부모의 왼쪽 자식을 삼촌으로 지정

      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;  // 루트만 있는 경우, RBTREE 2번 규칙을 위반하게 되어 color만 Black으로 변경
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = calloc(1, sizeof(node_t));
  node_t *y = t->nil;       // 삽입할 위치의 부모노드, x의 부모포인터
  node_t *x = t->root; 

  while (x != t->nil) {     // 삽입할 위치의 부모노드를 찾기 위한 반복문, x가 트리의 leaf까지 오기 전까지 반복
    y = x;                  // y를 루트로 업데이트, 반복할 때마다 x의 부모 위치 기록
    if (key < x->key) {     // z 값이 x 값보다 작으면,
        x = x->left;        // x의 왼쪽자식으로 내려간다. x를 x의 자식으로 업데이트
    } 
    else {                
      x = x->right;         // x의 오른쪽자식으로 내려간다.
    }
  }
  z->parent = y;            // z의 부모 연결
                  
  if (y == t->nil) {        // 빈 트리인 경우, 즉 while문을 돌지 못해 y값이 업데이트되지 않았음
    t->root = z;            // 루트를 z로 설정
  }                            
  else if (key < y->key) {  // 부모노드 y와 z의 키 값 비교 -> 키 삽입할 위치 찾기
    y->left = z;
  }
  else {
    y->right = z;
  }  

  z->key = key;             // z 노드에 기본값 대입
  z->left = t->nil;         
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);

  return t->root;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *a = t->root;
  node_t *nil = t->nil;
  
  while (a != nil) {            // a 노드가 nil이면 중단한다.
    if (key == a->key) {        // 찾으려는 값이 a 값과 같으면
      return a;                 // a를 반환한다     
    } else if (key < a->key) {  // 찾으려는 값이 a 값보다 작으면
      a = a->left;              // a에 a의 왼쪽 자식 저장
    } else {                    // 찾으려는 값이 a 값보다 크면
      a = a->right;             // a에 a의 오른쪽 자식 저장
    }
  }

  if (a == nil) {               // a 노드가 nil이면
    return NULL;                // NULL값 반환
  }
  
  return a;               
}

node_t *rbtree_min(const rbtree *t) { // 트리의 최솟값 : 트리의 왼쪽 최하단 노드
  // TODO: implement find
  node_t *tmp = t->root;              
  while (tmp->left != t->nil)         // tmp의 왼쪽 자식이 nil이면 중단한다.
    tmp = tmp->left;                  // tmp에 tmp의 왼쪽 자식 저장

  return tmp;                         // tmp의 왼쪽 자식이 nil이므로, tmp이 왼쪽 최하단 노드
  // return t->root;
}

node_t *rbtree_max(const rbtree *t) { // 트리의 최댓값 : 트리의 오른쪽 최하단 노드
  // TODO: implement find
  node_t *tmp = t->root;             
  while (tmp->right != t->nil)        // tmp의 오른쪽 자식이 nil이면 중단한다.
    tmp = tmp->right;                 // tmp에 tmp의 왼쪽 자식 저장

  return tmp;                         // tmp의 오른쪽 자식이 nil이므로, tmp이 오른쪽 최하단 노드
  // return t->root;
}




void transplant(rbtree *t, node_t *u, node_t *v) {
                              // 부모가 v를 가리키게 함
  if (u->parent == t->nil) {  // u가 루트인 경우
    t->root = v;              // root가 v를 가리키게 함
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
                              // v가 부모를 가리킬 때
  v->parent = u->parent;      // 부모 연결, v의 부모 주소값에 u의 부모 주소값 전달
}


void rbtree_erase_fixup(rbtree *t, node_t *x) {
    node_t *w;  // 형제
  
  while (x != t->root && x->color == RBTREE_BLACK) {  // x가 루트가 되거나 x가 검정이면 종료
    if (x == x->parent->left){                        // x가 왼쪽 자식일때
      w = x->parent->right;
      
      if (w->color == RBTREE_RED){      // case 1 : w 빨강
        w->color = RBTREE_BLACK;        // 형제노드와 부모노드를 색상변환한다. red 삭제시 속성위반 x
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);      // 부모 노드를 기준으로 left 회전을 한다. 
        w = x->parent->right;           // 형제노드가 블랙인 상황이 되어, case 2,3,4로 해결한다.
      }
                                        // case 2 : w 검정, w 자식들 검정
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {  
        w->color = RBTREE_RED;          // x와 w의 extra black을 x의 부모에 전달, x는 검정 w는 빨강으로 변한다
        x = x->parent;                  // x->parent는 1)red&black 이면 black으로 변환하고 종료 되거나 2)doubly black이 되어 다시 while문을 돈다
      }
      else {                                             
        if (w->right->color == RBTREE_BLACK) {  // case 3 : w 검정, w 왼쪽자식 빨강, w 오른쪽자식 검정
          w->left->color = RBTREE_BLACK;        // 형제노드와 형제노드의 왼쪽자식을 색상변환한다.
          w->color = RBTREE_RED;
          right_rotate(t, w);                   // 형제 노드를 중심으로 right 회전을 한다. 
          w = x->parent->right;                 // 형제 노드를 다시 설정한다 -> case 4와 같은 상황이 된다.
        }
                                          // case 4 : w 검정, w 왼쪽자식 (), w 오른쪽자식 빨강
        w->color = x->parent->color;      // 형제노드와 부모노드를 색상변환한다. 
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);        // 부모노드 중심으로 left 회전을 한다. 이 과정에서 x의 extra black이 삭제되고 문제노드가 사라진다.
        x = t->root;                      // 루트로 시점을 바꾼다 == while문 종료
      }
    }
    else {  // x가 오른쪽 자식일때(대칭)
      w = x->parent->left;
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;  // black
}



int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y = z;                  // 삭제된 노드 or 이동한 노드
  node_t *x;                      // 삭제할 노드의 남은 트리 저장
  color_t y_ori_color = y->color;

  if (z->left == t->nil) {        // z가 오른쪽 자식만 있는 경우
    x = z->right;                 // x에 z의 오른쪽 값 저장
    transplant(t, z, z->right);   // z와 z의 오른쪽 자식의 교체
  }
  else if (z->right == t->nil) {  // z가 왼쪽 자식만 있는 경우(대칭)
    x = z->left;                
    transplant(t, z, z->left);
  }
  else {                          // z가 자식이 둘 있는 경우                  
    y = z->right;                 // z를 대체할 다음 값(z와 가장 가까운 값) 찾기 
    while (y->left != t->nil) {   // -> z의 오른쪽 자식에서 왼쪽 트리의 끝
      y = y->left;
    }

    y_ori_color = y->color;       // y의 색 저장
    x = y->right;                 // x에 y의 오른쪽 값 저장
    if (y->parent == z) {         // y가 오른쪽 자식일 유일한 경우, y의 부모가 z라면,
      x->parent = y;              // x의 부모자리에 y의 주소값을 넣음(y 위치로 x가 갈 수 있도록)
    } 
    else {                        // y가 왼쪽 자식일 경우
      transplant(t, y, y->right); // y의 자리를 y->right(x)가 대체함
      y->right = z->right;        // z의 오른쪽자식 관계를 y에 전달(z 대체하는 중)
      y->right->parent = y;       // x의 부모가 y가 되게 한다
    }
    transplant(t, z, y);          // z의 부모관계를 y에 전달
    y->left = z->left;            // z의 왼쪽자식 관계를 y에 전달
    y->left->parent = y;          // y의 왼쪽자식들에게 부모를 y로 설정
    y->color = z->color;          // z의 색을 y에 전달
  }
  
  free(z);  // 트리에서 떨어진 노드는 할당부분을 free해줘야 함, delete는 트리에 붙어있는 노드만 할당 반환함
  z = NULL; // free된 부분이 여전히 *z로 접근가능하므로, z의 메모리에 있는 주소값을 초기화한다. 

  if (y_ori_color == RBTREE_BLACK) {  // 삭제된 노드가 검정색이면 함수실행
    rbtree_erase_fixup(t, x);
  }

  return 0;
}



int inorder_array(node_t *p, const rbtree *t, key_t *arr, const size_t n, int index) {
  // 중위 순회
  if (p == t->nil) {  // p가 leaf_node이면 
    return index;     // index를 업데이트하지 않고 그대로 반환
  }
  index = inorder_array(p->left, t, arr, n, index);  // 재귀를 돌며, 1 증가되어 반환된 index로 업데이트
  arr[index++] = p->key;                          // arr에 p 값 대입하고, arr의 index 증가
  // index++;
  index = inorder_array(p->right, t, arr, n, index);
  return index;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  inorder_array(t->root, t, arr, n, 0);              // 재귀를 사용하기 위해 make array 함수를 만듦
  return 0;
}