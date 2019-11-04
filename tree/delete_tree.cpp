/* 
 * 학  과 : 컴퓨터공학과
 * 개  요 : 트리에서 삭제할 노드를 찾아 삭제한후  트리에서의 중위순회와
			기존트리에서의 중위순회를 비교한다.
 */
#include<iostream>
#include<cstdlib>		// exit 함수사용을 위해 사용
#define COUNTINPUT 14
using namespace std;

// 지우려는 수보다 작으면서 가장 큰 수를 찾을때 왼쪽으로 갈지 오른쪽으로갈지 선택함
bool right_way_flage;

class binary_tree
{
	int info;
	binary_tree *parent;
	binary_tree *left;
	binary_tree *right;
public :
	binary_tree()
	{
		info = 0;
		this ->parent = NULL;
		this ->left = NULL;
		this ->right = NULL;
		right_way_flage = false;
	}
	void input_info(int);
	void build_tree(int);
	void preorder_traverse(binary_tree*);
	void inorder_traverse(binary_tree*);
	void postorder_traverse(binary_tree*);
	// 지울 수를 차는 함수
	binary_tree& find_tree_info(int);
	// 지우려는 수보다 작은수 중에 가장 큰 수를 찾는함수
	binary_tree& less_but_big();
	// 지우려는 수보다 작은수 중에 가장 큰 수를 찾는함수트리에서 가장작은 수를 찾는함수
	binary_tree& smallest_lbb();
	// 숫자를 찾아서 지우는함수
	void delete_tree_info(int);
	// 메모리할당한 것을 해제해줌
	void free();
};
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
			right -> parent = this;		// 부모클래스를 할당
			right -> input_info(value);		// 오른쪽에 할당
		}else
			right -> build_tree(value);		// 오른쪽트리 로감
	}
	// 값이 작으면
	else if(value < this -> info){
		if(left == NULL){
			left = new binary_tree();
			left -> parent = this;		// 부모클래스 할당
			left -> input_info(value);		// 왼쪽에 할당
		}else
			left -> build_tree(value);		// 왼쪽트리로감
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
	cout << info << "   ";
	if(left != NULL){
		present = left;
		left -> preorder_traverse(present);
	}
	if(right != NULL){
		present = right;
		right -> preorder_traverse(present);
	}
}
void binary_tree::inorder_traverse(binary_tree *present)
{
	// 중위순회
	if(left != NULL){
		present = left;
		left -> inorder_traverse(present);
	}
	cout << info << "   ";
	if(right != NULL){
		present = right;
		right -> inorder_traverse(present);
	}
}
void binary_tree::postorder_traverse(binary_tree *present){
	// 후위순회
	if(left != NULL){
		present = left;
		left -> postorder_traverse(present);
	}
	if(right != NULL){
		present = right;
		right -> postorder_traverse(present);
	}
	cout << info << "   ";
}
binary_tree& binary_tree::find_tree_info(int value)
{
	// 트리 이진탐색을 이용해 지우려는 수의 주소를 찾음.
	if(value == info)
		return *this;
	else if(value < info)
	{
		if(left == NULL)
		{
			cout << "트리에 수가 없습니다."<< endl;
			exit(1);
		}
		left -> find_tree_info(value);
	}
	else{
		if(right == NULL)
		{
			cout << "트리에 수가 없습니다."<< endl;
			exit(1);
		}
		right -> find_tree_info(value);
	}
}
binary_tree& binary_tree::less_but_big(){
	// 지우는 수보다 작은수중 가장큰 수를 찾음
	// 왼쪽으로 갔다가 오른쪽이 비어있을때 까지 오른쪽으로 감
	if(right_way_flage)
	{
		if(right == NULL)
			return *this;
		right ->less_but_big();
	}
	else{
		right_way_flage = true;
		left ->less_but_big();
	}
}
binary_tree& binary_tree::smallest_lbb(){
	// 가장 왼쪽으로 가서 맨 왼쪽에 것을 반환
	if(left==NULL)
		return *this;
	else
		left -> smallest_lbb();
}
void binary_tree::delete_tree_info(int value)
{
	binary_tree *delete_tree, *less_but_biggest, *smallest_lbb;

	// 지우는 노드 찾아서 할당
	delete_tree = &find_tree_info(value);
	// 지우는 수 기준으로 기준보다 작으면서 가장 큰 수 찾는다.
	less_but_biggest = &(delete_tree -> less_but_big());
	// 작으면서 가장 큰 수 기준으로 가장 작은 수 찾는다.
	if(less_but_biggest ->left != NULL)
		smallest_lbb = &(less_but_biggest ->left -> smallest_lbb());
	else smallest_lbb = less_but_biggest;

	// 지우는 수 기준으로 기준보다 작으면서 가장 큰 수의 부모노드의 오른쪽을 널로 할당
	((less_but_biggest -> parent) -> right) = NULL;
	// 지우는 수 기준으로 기준보다 작으면서 가장 큰 수의 부모노드를 지우는 수의 부모로 할당
	less_but_biggest->parent = delete_tree->parent;
	// 지우는 수 기준으로 기준보다 작으면서 가장 큰 수의 오른쪽을 지우는 수 오른쪽으로 할당
	(less_but_biggest -> right) = delete_tree->right;

		if(value < delete_tree->parent->info)
			delete_tree->parent->left = less_but_biggest;
		else
			delete_tree->parent->right = less_but_biggest;
	
	(smallest_lbb -> left) = (delete_tree ->left);


}

void main(){
	binary_tree tree;
	int delete_number;
	int info[COUNTINPUT]={16, 6, 22, 2, 10, 18, 28, 12, 19, 24, 14, 21, 26, 20};

	tree.input_info(info[0]);
	// 트리만들기
	for(int index_info=0; index_info<COUNTINPUT; index_info++)
		tree.build_tree(info[index_info]);

	cout << "지울 수를 입력 : ";
	cin >> delete_number;

	cout << endl << "Inorder Traverse (before delete)"<< endl;
	tree.inorder_traverse(&tree);
	cout << endl<<endl;

	cout << "Inorder Traverse (after delete)"<< endl;
	tree.delete_tree_info(delete_number);
	tree.inorder_traverse(&tree);
	cout << endl;

	cout << "Postorder Traverse (after delete)"<< endl;
	tree.postorder_traverse(&tree);
	cout << endl<<endl;

	tree.free();
}