/* 
 * 학  과 : 컴퓨터공학과
 * 개  요 : 균형트리 만들기
 */
#include<iostream>
#define COUNTINPUT 10
using namespace std;

// 지우려는 수보다 작으면서 가장 큰 수를 찾을때 왼쪽으로 갈지 오른쪽으로갈지 선택함
bool right_way_flage;

class binary_tree
{
	int balanced_value;
	int info;
	binary_tree *left;
	binary_tree *right;
public :
	binary_tree()
	{
		info = -1;
		balanced_value = 0;
		this ->left = NULL;
		this ->right = NULL;
		right_way_flage = false;
	}
	int get_deep();
	void input_info(int);
	void build_tree(int);
	void preorder_traverse(binary_tree*);
	void inorder_traverse(binary_tree*);
	void postorder_traverse(binary_tree*);
	binary_tree& left_rotation(binary_tree *);
	binary_tree& right_rotation(binary_tree *);
	binary_tree& balance(binary_tree*,int);
	void find_last_not_balanced(binary_tree*);
	void free();
};

int binary_tree::get_deep(){
	int left_Maxlevel=0, right_Maxlevel=0;

	if(this == NULL)
		return 0;
	else
	{
		left_Maxlevel = left -> get_deep() + 1;
		right_Maxlevel = right -> get_deep() + 1;

		if(left_Maxlevel < right_Maxlevel){
			balanced_value = left_Maxlevel - right_Maxlevel;
			return right_Maxlevel;
		}
		else
		{
			balanced_value = left_Maxlevel - right_Maxlevel;
			return left_Maxlevel;
		}
	}
}
void binary_tree::input_info(int value)
{
	this -> info = value;
}
void binary_tree::build_tree(int value)
{
	// 값이 크면
	if(value > this -> info){

		if(right == NULL){
			right = new binary_tree();
			right -> input_info(value);		// 오른쪽에 할당
		}else
			right -> build_tree(value);		// 오른쪽트리로 감
	}
	// 값이 작으면
	else if(value < this -> info){

		if(left == NULL){
			left = new binary_tree();
			left -> input_info(value);		// 왼쪽에 할당
		}else
			left -> build_tree(value);		// 왼쪽트리로 감
	}
}
void binary_tree::free()
{
	// 맨왼쪽부터 메모리 해제
	info = 0;
	if(left!=NULL){
		left->free();
		delete left;
		left = NULL;
	}
	if(right!=NULL){
		right->free();
		delete right;
		right = NULL;
	}
}
void binary_tree::preorder_traverse(binary_tree *present)
{
	// 전위순회
	cout << present->info << "   ";
	if(present->left != NULL){
		preorder_traverse(present->left);
	}
	if(present->right != NULL){
		preorder_traverse(present->right);
	}
}
void binary_tree::inorder_traverse(binary_tree *present)
{
	// 중위순회
	if(present->left != NULL){
		present = present->left;
		present->left -> inorder_traverse(present);
	}
	cout << info << "   ";
	if(present->right != NULL){
		present = present -> right;
		present -> right -> inorder_traverse(present);
	}
}
void binary_tree::postorder_traverse(binary_tree *present){
	// 후위순회
	if(present->left != NULL){
		present = left;
		left -> postorder_traverse(present);
	}
	if(present->right != NULL){
		present = right;
		right -> postorder_traverse(present);
	}
	cout << info << "   ";
}
binary_tree& binary_tree::left_rotation(binary_tree *present){
	binary_tree *hold, *q;

	q = present->right;
	hold = q->left;
	q->left = present;
	present->right = hold;
	present = q;
	return *present;
}
binary_tree& binary_tree::right_rotation(binary_tree *present){
	binary_tree *hold, *q;

	q = present->left;
	hold = q->right;
	q->right = present;
	present->left = hold;
	present = q;
	return *present;
}
binary_tree& binary_tree::balance(binary_tree *present, int value){
	binary_tree *last_not_balanced=NULL, *last_not_balanced_child, **back, *temp;

	back = &present;
	// 마지막으로 균형이 깨진곳 찾기
	while(present->balanced_value == 2 || present->balanced_value == -2)
	{

		// 현재 균형값이 2나 -2가 아닐때까지 찾음
		last_not_balanced = present;
		if(value < present->info)
			present = present->left;
		else
			present = present->right;
	}
	// 균형 마지막으로 깨진곳의 서브트리중 입력노드가 달린 노드의 최상위층
	if(last_not_balanced != NULL){

		last_not_balanced_child = present;
		// 경우에 따라 회전하기
		switch(last_not_balanced->balanced_value + (last_not_balanced_child->balanced_value * 3)){
		case -1:		// 부모노드 균형값이 2, 자식노드 균형값이 -1일 때
			last_not_balanced_child = &left_rotation(last_not_balanced_child);
			last_not_balanced = &right_rotation(last_not_balanced);
			break;
		case 5:		// 부모노드 균형값이 2, 자식노드 균형값이 1일 때
			last_not_balanced = &right_rotation(last_not_balanced);
			break;

		case -5:		// 부모노드 균형값이 -2, 자식노드 균형값이 -1일 때
			last_not_balanced = &left_rotation(last_not_balanced);
			break;

		case 1:		// 부모노드 균형값이 -2, 자식노드 균형값이 1일 때
			last_not_balanced_child = &right_rotation(last_not_balanced_child);
			last_not_balanced = &left_rotation(last_not_balanced);
			break;
		}
		present = last_not_balanced;
	}
	preorder_traverse(*back);
	cout<<endl;
	return **back;
}
void main(){
	binary_tree *tree = new binary_tree, *real;
	int info[COUNTINPUT]={49, 25, 16, 54, 76, 61, 32, 40, 36, 45};

	cout << endl << "Preorder Traverse "<< endl;
	
	// 균형 트리만들기
	tree->input_info(info[0]);
	for(int index_info=0; index_info<COUNTINPUT; index_info++){
		tree->build_tree(info[index_info]);
		real = NULL;
		tree->get_deep(); // 균형값 측정 : 하기전에 맞춰줌
		real = &tree->balance(tree, info[index_info]);
		tree = NULL;
		tree = real;
		tree->get_deep(); // 균형값 다시 측정 : 하고나서 균형값 맞춰줌
	}
	tree->free();
}