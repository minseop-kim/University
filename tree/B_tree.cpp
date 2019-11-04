#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#define	TOTAL_UNIT 5

typedef struct Node
{
	Node *present;
	Node *children[TOTAL_UNIT+1];
	int unit_counter;
	int unit[TOTAL_UNIT+1];
}B_tree_root;

Node* find_leaf_node(Node *leaf, int key)
{
	Node *r;
	int i;

	if(leaf->unit_counter == TOTAL_UNIT)
	{
		for(i=0; i<TOTAL_UNIT && key>leaf->unit[i]; i++);
		r = leaf->children[i];
		r->present = leaf;
		return find_leaf_node(r, key);
	}
	else
	{
		return leaf;
	}
}

void clearOverflow(Node *pnode, Node *root, int key)
{
	Node *parent, *ne, *new_root;
	int left, right, position_key, i;
	bool bleft=false, bright=false;

	parent = pnode->present;
	if(parent != NULL)	// 부모노드가 있을경우
	{
		for(position_key=0; position_key<parent->unit_counter && key<parent->unit[position_key]; position_key++);

		switch(position_key)
		{
		case 0:
			// 오른쪽만 비교한다
			if(parent->children[position_key+1]->unit_counter < TOTAL_UNIT)
			{
				// 민다.
				for(i=(parent->children[position_key+1]->unit_counter); i>0; i--)
				{
					pnode->present->children[position_key+1]->unit[i] = pnode->present->children[position_key+1]->unit[i-1];
				}
				// 키를 넘기는 작업
				pnode->present->children[position_key+1]->unit[i] = pnode->present->unit[position_key];
				pnode->present->unit[position_key] = pnode->unit[pnode->unit_counter-1];
				pnode->present->unit_counter++;

				pnode->unit[pnode->unit_counter-1] = 0;
				pnode->unit_counter--;
				bright = true;
			}
			break;
		case TOTAL_UNIT:
			// 왼쪽만 비교한다.
			if(parent->children[position_key-1]->unit_counter < TOTAL_UNIT)
			{
				// 키를 넘기는 작업
				pnode->present->children[position_key-1]->unit[pnode->present->children[position_key-1]->unit_counter++]\
					= pnode->present->unit[position_key];
				pnode->present->unit[position_key] = pnode->unit[0];

				pnode->unit_counter--;
				// 땡긴다.
				for(i=0; i<pnode->unit_counter; i++)
				{
					pnode->unit[i] = pnode->unit[i+1];
				}
				pnode->unit[pnode->unit_counter] = 0;
				bleft = true;
			}
			break;
		default:
			if(parent->children[position_key+1]->unit_counter < TOTAL_UNIT)
				bleft = true;
			if(parent->children[position_key-1]->unit_counter < TOTAL_UNIT)
				bright = true;
			if(bleft && bright)
			{
				// 민다.
				for(i=(parent->children[position_key+1]->unit_counter); i>0; i--)
				{
					pnode->present->children[position_key+1]->unit[i] = pnode->present->children[position_key+1]->unit[i-1];
				}
				// 키를 넘기는 작업
				pnode->present->children[position_key+1]->unit[i] = pnode->present->unit[position_key];
				pnode->present->unit[position_key] = pnode->unit[pnode->unit_counter-1];
				pnode->present->unit_counter++;

				pnode->unit[pnode->unit_counter-1] = 0;
				pnode->unit_counter--;
			}
			else if(bleft && !bright)
			{
				// 민다.
				for(i=(parent->children[position_key+1]->unit_counter); i>0; i--)
				{
					pnode->present->children[position_key+1]->unit[i] = pnode->present->children[position_key+1]->unit[i-1];
				}
				// 키를 넘기는 작업
				pnode->present->children[position_key+1]->unit[i] = pnode->present->unit[position_key];
				pnode->present->unit[position_key] = pnode->unit[pnode->unit_counter-1];
				pnode->present->unit_counter++;

				pnode->unit[pnode->unit_counter-1] = 0;
				pnode->unit_counter--;
			}
			else if(!bleft && bright)
			{
				// 키를 넘기는 작업
				pnode->present->children[position_key-1]->unit[pnode->present->children[position_key-1]->unit_counter++]\
					= pnode->present->unit[position_key];
				pnode->present->unit[position_key] = pnode->unit[0];

				pnode->unit_counter--;
				// 땡긴다.
				for(i=0; i<pnode->unit_counter; i++)
				{
					pnode->unit[i] = pnode->unit[i+1];
				}
				pnode->unit[pnode->unit_counter] = 0;
			}
		}

		if(!bleft && !bright)
		{
			ne = (Node*)calloc(0,sizeof(Node));
			left = pnode->unit_counter/2;
			right = pnode->unit_counter - left;

			// 새로 생성한 형제노드에 옮기기
			for(i=1; i<right; i++)
			{
				ne->unit[i-1] = pnode->unit[left+i];
			}

			// 부모노드에 끼워 넣기
			for(i=pnode->present->unit_counter; i>position_key; i--)
			{
				pnode->present->unit[i] = pnode->present->unit[i-1];
			}
			pnode->present->unit[i] = pnode->unit[left];

			// 옮겨진 유닛 노드에서 지우기
			for(i=pnode->unit_counter-1; i>=left; i--)
			{
				pnode->unit[i] = 0;
			}

			// 한쪽씩 자식의 위치를 한칸씩 옮긴다.
			for(i=pnode->present->unit_counter; i>left; i--)
			{
				pnode->present->children[i] = pnode->present->children[i-1];
			}
			pnode->present->children[i] = ne;
			
			// 옮긴후 정리
			pnode->present->unit_counter++;
			pnode->unit_counter=left-1;
			ne->unit_counter=right;
			ne->present = pnode->present;

			// 부모노드에서 오버플로가 발생했을때
			if(pnode->present->unit_counter == TOTAL_UNIT+1)
				clearOverflow(pnode->present, root, key);
		}
	}
	else				// 루트노드인 경우
	{
		ne = (Node*)calloc(0,sizeof(Node));
		new_root = (Node*)calloc(0,sizeof(Node));

		left = pnode->unit_counter/2;
		right = pnode->unit_counter - left;

		// 새로 생성한 형제노드에 옮기기
		for(i=1; i<right; i++)
		{
			ne->unit[i-1] = pnode->unit[left+i];
		}
		// 새 루트노드에 값넣기
		new_root->unit[0] = pnode->unit[left];
		// 옮겨진 유닛 노드에서 지우기
		for(i=pnode->unit_counter-1; i>=left; i--)
		{
			pnode->unit[i] = 0;
		}

		pnode->unit_counter=left;
		ne->unit_counter=right;
		ne->present = new_root;

		new_root->children[0] = pnode;
		new_root->children[1] = ne;

		root = new_root;
	}
}

void b_tree_insert(Node *root, int key)
{
	int insert_index, i, left, right;
	Node *r;

	// key를 삽입할 노드 r을 찾는다.
	r = find_leaf_node(root, key);

	// key를 x에 삽입한다.
	for(insert_index=0; insert_index<r->unit_counter && key<r->unit[insert_index]; insert_index++);

	// 배열의 뒤에있는것들을 한칸씩 미뤄준다.
	for(i=r->unit_counter-1; i>insert_index; i--)
	{
		r->unit[i] = r->unit[i-1];
	}

	// 키값을 넣어주고 총 유닛의 갯수를 늘려준다.
	r->unit[insert_index] = key;
	r->unit_counter++;

	// 오버플로 발생했는지 확인한다.
	if(r->unit_counter == TOTAL_UNIT+1)
		clearOverflow(r, root, key);
}

Node* find_key_node(Node *root, int key)
{
	int child_index;

	for(child_index=0; child_index<root->unit_counter && root->unit[child_index]>key; child_index++);
	if(root->unit[child_index] == key)
		return root;
	else 
		return find_key_node(root->children[child_index], key);
}

Node* find_delet_leaf_nod(Node *leaf)
{
	if(leaf->children[0] == NULL)
		return leaf;
	else
		return find_delet_leaf_nod(leaf->children[0]);
}

void clearUnderflow(Node *pnode, Node *root, int key)
{
	Node *parent;
	int left, right, position_key, i;
	bool bleft=false, bright=false;

	parent = pnode->present;
	for(position_key=0; position_key<parent->unit_counter && key<parent->unit[position_key]; position_key++);

	switch(position_key)
	{
	case 0:
		if(parent->children[position_key+1]->unit_counter > 2)
		{// 형제 노드에서 가져올것이 있을때 오른쪽만가능
			pnode->unit[pnode->unit_counter++] = pnode->present->unit[0];
			pnode->present->unit[position_key] = pnode->present->children[position_key+1]->unit[0];
			for(i=0; i<pnode->present->children[position_key+1]->unit_counter-1;i++)
			{
				pnode->present->children[position_key+1]->unit[i] = \
				pnode->present->children[position_key+1]->unit[i+1];
			}
			pnode->present->children[position_key+1]->unit_counter--;
			bright = true;
		}
		else
		{// 안될때
			pnode->unit[pnode->unit_counter++] = pnode->present->unit[position_key];
			for(i=0; i<2;i++)
			{
				pnode->unit[pnode->unit_counter++] = \
					pnode->present->children[position_key+1]->unit[i];
			}
			// 자식들을 한칸씩 땡긴다.
			for(i=position_key+1; i<pnode->present->unit_counter; i++)
			{
				pnode->present->children[i] = pnode->present->children[i+1];
			}
			pnode->present->children[i] = NULL;
			pnode->present->unit_counter--;
		}
		break;
	case TOTAL_UNIT:
		if(parent->children[position_key-1]->unit_counter > 2)
		{// 형제 노드에서 가져올것이 있을때 왼쪽만 가능
			for(i=pnode->unit_counter; i>0;i--)
			{
				pnode->unit[i] = pnode->unit[i-1];
			}
			pnode->unit[i] = pnode->present->unit[pnode->present->unit_counter-1];
			pnode->unit_counter++;

			pnode->present->unit[position_key] = pnode->present->children[position_key-1]->unit[pnode->present->children[position_key-1]->unit_counter - 1];
			pnode->present->children[position_key-1]->unit[pnode->present->children[position_key-1]->unit_counter - 1] = 0;
			pnode->present->children[position_key-1]->unit_counter--;
			bleft = true;
		}
		else
		{// 안될때
			//세칸을 밀어둔다.
			pnode->unit[3] = pnode->unit[0];
			pnode->unit[2] = pnode->present->unit[position_key-1];
			pnode->unit[1] = pnode->present->children[position_key-1]->unit[1];
			pnode->unit[0] = pnode->present->children[position_key-1]->unit[0];
			pnode->present->unit_counter--;
			pnode->present->children[position_key-1] = pnode->present->children[position_key];
			pnode->present->children[position_key] = NULL;
		}
		break;
	default:
		if(parent->children[position_key+1]->unit_counter > 2)
			bright = true;
		if(parent->children[position_key-1]->unit_counter > 2)
			bleft = true;
		if(bright && bleft)
		{// 둘다 줄 수 있을때
			pnode->unit[pnode->unit_counter++] = pnode->present->unit[0];
			pnode->present->unit[position_key] = pnode->present->children[position_key+1]->unit[0];
			for(i=0; i<pnode->present->children[position_key+1]->unit_counter-1;i++)
			{
				pnode->present->children[position_key+1]->unit[i] = \
				pnode->present->children[position_key+1]->unit[i+1];
			}
			pnode->present->children[position_key+1]->unit_counter--;
		}
		else if(bright && !bleft)
		{// 오른쪽만 줄 수 있을때
			pnode->unit[pnode->unit_counter++] = pnode->present->unit[0];
			pnode->present->unit[position_key] = pnode->present->children[position_key+1]->unit[0];
			for(i=0; i<pnode->present->children[position_key+1]->unit_counter-1;i++)
			{
				pnode->present->children[position_key+1]->unit[i] = \
				pnode->present->children[position_key+1]->unit[i+1];
			}
			pnode->present->children[position_key+1]->unit_counter--;
		}
		else if(!bright && bleft)
		{// 왼쪽만 줄 수 있을 때
			for(i=pnode->unit_counter; i>0;i--)
			{
				pnode->unit[i] = pnode->unit[i-1];
			}
			pnode->unit[i] = pnode->present->unit[pnode->present->unit_counter-1];
			pnode->unit_counter++;

			pnode->present->unit[position_key] = pnode->present->children[position_key-1]->unit[pnode->present->children[position_key-1]->unit_counter - 1];
			pnode->present->children[position_key-1]->unit[pnode->present->children[position_key-1]->unit_counter - 1] = 0;
			pnode->present->children[position_key-1]->unit_counter--;
		}
	}
}

void b_tree_delet(Node *root, int key)
{
	Node *v, *r;
	int swap, key_index;

	v = find_key_node(root, key);
	for(key_index=0; key_index<v->unit_counter && root->unit[key_index]!=key ;key_index++);

	if(v->children[0] != NULL)
	{// 리프노드가 아닐때 key 의 직후원소 를 가진 리프노드를 찾아 서로 key와 맞바꾼다.
		r = find_delet_leaf_nod(v->children[key_index+1]);

		swap = v->unit[key_index];
		v->unit[key_index] = r->children[key_index+1]->unit[0];
		r->children[key_index+1]->unit[0] = swap;
	}
	else
	{
		r = v;
	}

	// 리프노드에서 x를 제거한다.
	for(int i=0; i<r->unit_counter-1; i++)
	{
		r->unit[i] = r->unit[i+1];
	}
	r->unit_counter--;

	if(r->unit_counter == 1)
		clearUnderflow(r, root, key);
}

void tap(int n){
	for (int i = 0; i < n; i++)
		printf("\t");
	for (int i = 0; i < n; i++)
		printf("-");
	printf(">");
}
void print(Node *root, int num)
{
	tap(num);
	for (int i = 0; i < root->unit_counter; i++)
		printf("%6d", root->unit[i]);
	printf("\n");
	for (int i = 0; i <= root->unit_counter; i++)
		if (root->children[i] != NULL)
			print(root->children[i], num + 1);
}
int main()
{
	B_tree_root *root;
	int key, mode, rd;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 200; i++){
		rd = rand() % 1000;
		b_tree_insert(root, rd);
	}
	print(root, 0);
	while (true){
		printf("삭제 : 1\t삽입 : 2\n");
		scanf("%d", &key);
		if (mode == 1){
			printf("삭제할 숫자 : ");
			scanf("%d", &mode);
			b_tree_delet(root, key);
		}
		else if (mode == 2){
			printf("삽입할 숫자 : ");
			scanf("%d", &key);
			b_tree_insert(root, key);
		}
		print(root, 0);
	}
	return 0;
}