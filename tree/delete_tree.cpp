/* 
 * ��  �� : ��ǻ�Ͱ��а�
 * ��  �� : Ʈ������ ������ ��带 ã�� ��������  Ʈ�������� ������ȸ��
			����Ʈ�������� ������ȸ�� ���Ѵ�.
 */
#include<iostream>
#include<cstdlib>		// exit �Լ������ ���� ���
#define COUNTINPUT 14
using namespace std;

// ������� ������ �����鼭 ���� ū ���� ã���� �������� ���� ���������ΰ��� ������
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
	// ���� ���� ���� �Լ�
	binary_tree& find_tree_info(int);
	// ������� ������ ������ �߿� ���� ū ���� ã���Լ�
	binary_tree& less_but_big();
	// ������� ������ ������ �߿� ���� ū ���� ã���Լ�Ʈ������ �������� ���� ã���Լ�
	binary_tree& smallest_lbb();
	// ���ڸ� ã�Ƽ� ������Լ�
	void delete_tree_info(int);
	// �޸��Ҵ��� ���� ��������
	void free();
};
void binary_tree::input_info(int value)
{
	this -> info = value;
}
void binary_tree::build_tree(int value)
{
	// ���� ũ��
	if(value > this -> info){
		if(right == NULL){
			right = new binary_tree();
			right -> parent = this;		// �θ�Ŭ������ �Ҵ�
			right -> input_info(value);		// �����ʿ� �Ҵ�
		}else
			right -> build_tree(value);		// ������Ʈ�� �ΰ�
	}
	// ���� ������
	else if(value < this -> info){
		if(left == NULL){
			left = new binary_tree();
			left -> parent = this;		// �θ�Ŭ���� �Ҵ�
			left -> input_info(value);		// ���ʿ� �Ҵ�
		}else
			left -> build_tree(value);		// ����Ʈ���ΰ�
	}
}
void binary_tree::free()
{
	// �ǿ��ʺ��� �޸� ����
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
	// ������ȸ
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
	// ������ȸ
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
	// ������ȸ
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
	// Ʈ�� ����Ž���� �̿��� ������� ���� �ּҸ� ã��.
	if(value == info)
		return *this;
	else if(value < info)
	{
		if(left == NULL)
		{
			cout << "Ʈ���� ���� �����ϴ�."<< endl;
			exit(1);
		}
		left -> find_tree_info(value);
	}
	else{
		if(right == NULL)
		{
			cout << "Ʈ���� ���� �����ϴ�."<< endl;
			exit(1);
		}
		right -> find_tree_info(value);
	}
}
binary_tree& binary_tree::less_but_big(){
	// ����� ������ �������� ����ū ���� ã��
	// �������� ���ٰ� �������� ��������� ���� ���������� ��
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
	// ���� �������� ���� �� ���ʿ� ���� ��ȯ
	if(left==NULL)
		return *this;
	else
		left -> smallest_lbb();
}
void binary_tree::delete_tree_info(int value)
{
	binary_tree *delete_tree, *less_but_biggest, *smallest_lbb;

	// ����� ��� ã�Ƽ� �Ҵ�
	delete_tree = &find_tree_info(value);
	// ����� �� �������� ���غ��� �����鼭 ���� ū �� ã�´�.
	less_but_biggest = &(delete_tree -> less_but_big());
	// �����鼭 ���� ū �� �������� ���� ���� �� ã�´�.
	if(less_but_biggest ->left != NULL)
		smallest_lbb = &(less_but_biggest ->left -> smallest_lbb());
	else smallest_lbb = less_but_biggest;

	// ����� �� �������� ���غ��� �����鼭 ���� ū ���� �θ����� �������� �η� �Ҵ�
	((less_but_biggest -> parent) -> right) = NULL;
	// ����� �� �������� ���غ��� �����鼭 ���� ū ���� �θ��带 ����� ���� �θ�� �Ҵ�
	less_but_biggest->parent = delete_tree->parent;
	// ����� �� �������� ���غ��� �����鼭 ���� ū ���� �������� ����� �� ���������� �Ҵ�
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
	// Ʈ�������
	for(int index_info=0; index_info<COUNTINPUT; index_info++)
		tree.build_tree(info[index_info]);

	cout << "���� ���� �Է� : ";
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