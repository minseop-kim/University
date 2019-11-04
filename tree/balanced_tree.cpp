/* 
 * ��  �� : ��ǻ�Ͱ��а�
 * ��  �� : ����Ʈ�� �����
 */
#include<iostream>
#define COUNTINPUT 10
using namespace std;

// ������� ������ �����鼭 ���� ū ���� ã���� �������� ���� ���������ΰ��� ������
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
	// ���� ũ��
	if(value > this -> info){

		if(right == NULL){
			right = new binary_tree();
			right -> input_info(value);		// �����ʿ� �Ҵ�
		}else
			right -> build_tree(value);		// ������Ʈ���� ��
	}
	// ���� ������
	else if(value < this -> info){

		if(left == NULL){
			left = new binary_tree();
			left -> input_info(value);		// ���ʿ� �Ҵ�
		}else
			left -> build_tree(value);		// ����Ʈ���� ��
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
	// ������ȸ
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
	// ������ȸ
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
	// ���������� ������ ������ ã��
	while(present->balanced_value == 2 || present->balanced_value == -2)
	{

		// ���� �������� 2�� -2�� �ƴҶ����� ã��
		last_not_balanced = present;
		if(value < present->info)
			present = present->left;
		else
			present = present->right;
	}
	// ���� ���������� �������� ����Ʈ���� �Է³�尡 �޸� ����� �ֻ�����
	if(last_not_balanced != NULL){

		last_not_balanced_child = present;
		// ��쿡 ���� ȸ���ϱ�
		switch(last_not_balanced->balanced_value + (last_not_balanced_child->balanced_value * 3)){
		case -1:		// �θ��� �������� 2, �ڽĳ�� �������� -1�� ��
			last_not_balanced_child = &left_rotation(last_not_balanced_child);
			last_not_balanced = &right_rotation(last_not_balanced);
			break;
		case 5:		// �θ��� �������� 2, �ڽĳ�� �������� 1�� ��
			last_not_balanced = &right_rotation(last_not_balanced);
			break;

		case -5:		// �θ��� �������� -2, �ڽĳ�� �������� -1�� ��
			last_not_balanced = &left_rotation(last_not_balanced);
			break;

		case 1:		// �θ��� �������� -2, �ڽĳ�� �������� 1�� ��
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
	
	// ���� Ʈ�������
	tree->input_info(info[0]);
	for(int index_info=0; index_info<COUNTINPUT; index_info++){
		tree->build_tree(info[index_info]);
		real = NULL;
		tree->get_deep(); // ������ ���� : �ϱ����� ������
		real = &tree->balance(tree, info[index_info]);
		tree = NULL;
		tree = real;
		tree->get_deep(); // ������ �ٽ� ���� : �ϰ��� ������ ������
	}
	tree->free();
}