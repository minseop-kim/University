/*
 * ��  �� : �����ͱ�������
 * ��  �� : B-5�� Ʈ���� 1~10000������ ������ 
			200���� �Է��ϱ�(�̿ϼ�)
 */
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define MULTIWAY 5
#define MAXDATA 200
#define MAXNUMBER 10000

int number = 0;

// Ʈ��ó�� ���̱� ���� �Լ�
void print_space(){
	for (int temp = 0; temp < number; temp++)
		cout << "\t";
	for (int temp = 0; temp < number; temp++){
		if(temp == 0)
			cout << "��";
		else
			cout << "��";
	}
}
// ���� BƮ��
class multiway_bree{
	multiway_bree *next[MULTIWAY+1];
	multiway_bree *pre;
	int key[MULTIWAY];
	int count;
public:
	multiway_bree(){
		this->pre = NULL;
		for (int temp = 0; temp < MULTIWAY; temp++){
			next[temp] = NULL;
			key[temp] = NULL;
		}
		next[MULTIWAY] = NULL;
		count = 0;
	}
	// ������ Ʈ���� ��ȯ�ϴ� �Լ�
	multiway_bree* change();
	// Ű���� �Է��ϴ� �Լ�
	void insert_key(int);
	// ����������� ��带 �ɰ��� �Լ�
	void divide_sub_tree();
	// Ʈ�� ����ϴ� �Լ�
	void display_tree();
};
void multiway_bree::display_tree(){
	// �������
	print_space();
	// ���� ����� ���� ���� ���
	for (int temp = 0; temp < count; temp++)
		cout << key[temp]<<"   ";
	cout << endl;
	// ���� �ڽ� Ʈ���� �̵�
	for (int temp = 0; temp<=count; temp++){
		number++;
		if (next[temp] != NULL)
			next[temp]->display_tree();
		number--;
	}
}
multiway_bree* multiway_bree::change(){
	// ���� �ڽ��� ��ȯ
	if (pre == NULL)
		return this;
	else if (pre != NULL)
		return pre;
}
void multiway_bree::insert_key(int k){
	int c = count + 1;

	for (int temp = 0; temp < c; temp++){ 
		// ���� ��尡 Ű���� Ŭ��
		if (key[temp] > k){
			if (next[temp] != NULL){
				next[temp]->insert_key(k);
			}
			else {
				for (int temp2 = c; temp2 > temp; temp2--)
					key[temp2] = key[temp2 - 1];
				count = c;
				key[temp] = k;
			}
			break;
		}
		else if (temp == c-1){
			if (next[temp] != NULL){
				next[temp]->insert_key(k);
			}
			else{
				count++;
				key[temp] = k;
			}
			break;
		}
	}
	if (count == MULTIWAY){
		divide_sub_tree();
	}
}
void multiway_bree::divide_sub_tree(){
	int c = count;
	int temp = c / 2 + 1;
	multiway_bree* sub = new multiway_bree();

	for (int temp2 = temp; temp2 < c; temp2++){
		sub->key[temp2 - temp] = key[temp2];
		key[temp2] = NULL;
		if (next[temp2] != NULL){
			sub->next[temp2 - temp] = next[temp2];
			next[temp2] = NULL;
			sub->next[temp2 - temp]->pre = sub;
		}
		count--;
		sub->count++;
	}
	if (next[count] != NULL){
		sub->next[c - temp] = next[count];
		next[c] = NULL;
		sub->next[c - temp]->pre = sub;
	}
	if (pre==NULL){
		multiway_bree* pr = new multiway_bree();
		pre = pr;
		sub->pre = pr;
		pre->next[0] = this;
		pre->next[1] = sub;
		pre->key[0] = key[temp-1];
		key[temp - 1] = NULL;
		count--;
		pr->count++;
	}else{
		sub->pre = pre;
		for (int temp2 = 0; temp2 <= pre->count; temp2++){
			if (key[temp-1] < pre->key[temp2]){
				for (int temp3 = pre->count; temp3 > temp2; temp3--){
					pre->key[temp3] = pre->key[temp3 - 1];
					pre->next[temp3 + 1] = pre->next[temp3];
				}
				pre->key[temp2] = key[temp-1];
				key[temp-1] = NULL;
				count--;
				pre->next[temp2+1] = sub;
				pre->count++;
				if (pre->count == MULTIWAY)
					pre->divide_sub_tree();
				break;
			}else if(temp2 == pre->count && key[temp-1] > pre->key[temp2]){
				pre->key[temp2] = key[temp-1];
				key[temp-1] = NULL;
				count--;
				pre->next[temp2+1] = sub;
				pre->count++;
				if (pre->count == MULTIWAY)
					pre->divide_sub_tree();
			}
		}
	}
}
void main(){
	multiway_bree alpha;
	multiway_bree *root=&alpha;

	// ���� ����
	srand((unsigned int)time(NULL));
	// Ʈ�������
	for (int i = 0; i < MAXDATA; i++){
		int k = rand() % MAXNUMBER +1;
		root->insert_key(k);
		root = root->change();
	}
	root->display_tree();
}