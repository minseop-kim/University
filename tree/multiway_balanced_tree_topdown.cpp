/*
 * 과  목 : 데이터구조응용
 * 개  요 : B-5차 트리에 1~10000사이의 숫자중 
			200개를 입력하기(미완성)
 */
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define MULTIWAY 5
#define MAXDATA 200
#define MAXNUMBER 10000

int number = 0;

// 트리처럼 보이기 위한 함수
void print_space(){
	for (int temp = 0; temp < number; temp++)
		cout << "\t";
	for (int temp = 0; temp < number; temp++){
		if(temp == 0)
			cout << "└";
		else
			cout << "─";
	}
}
// 다중 B트리
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
	// 이전의 트리를 반환하는 함수
	multiway_bree* change();
	// 키값을 입력하는 함수
	void insert_key(int);
	// 꽉차있을경우 노드를 쪼개는 함수
	void divide_sub_tree();
	// 트리 출력하는 함수
	void display_tree();
};
void multiway_bree::display_tree(){
	// 공백출력
	print_space();
	// 현재 노드의 값을 전부 출력
	for (int temp = 0; temp < count; temp++)
		cout << key[temp]<<"   ";
	cout << endl;
	// 하위 자식 트리로 이동
	for (int temp = 0; temp<=count; temp++){
		number++;
		if (next[temp] != NULL)
			next[temp]->display_tree();
		number--;
	}
}
multiway_bree* multiway_bree::change(){
	// 이전 자식을 반환
	if (pre == NULL)
		return this;
	else if (pre != NULL)
		return pre;
}
void multiway_bree::insert_key(int k){
	int c = count + 1;

	for (int temp = 0; temp < c; temp++){ 
		// 현재 노드가 키보다 클때
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

	// 난수 생성
	srand((unsigned int)time(NULL));
	// 트리만들기
	for (int i = 0; i < MAXDATA; i++){
		int k = rand() % MAXNUMBER +1;
		root->insert_key(k);
		root = root->change();
	}
	root->display_tree();
}