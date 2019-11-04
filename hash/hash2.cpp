/*
 * 과  목 : 데이터구조응용
 * 설  명 : 문자 데이터가 입력될때 충돌을 이차, 이중, 체이닝을 사용하여 처리하고
			해시 테이블의 내용을 출력
 */

#include <iostream>
using namespace std;

#define MAX_COUNT_DATA 11
#define DIVISION_NUMBER 11
#define REDIVISION_NUMBER 11

int element_index=0;
bool cycle = false;

// int형태 해싱
int hash_function(int key)
{
	return key % DIVISION_NUMBER;
}
int rehash_function(int key)
{
	// 문제에서 7로 했을경우 테이블이 0~6까지 꽉차있을때에는 무한루프
	// 를 돌기때문에 나눌수 보다 더 큰수로 리해시해야한다.
	return (REDIVISION_NUMBER - (key % REDIVISION_NUMBER));
}
// 헤싱테이블
class element
{
	int key;
	int increase;
	bool fill;
public:
	element* next;
	element()
	{
		fill = false;
		increase = 0;
		key = 0;
		next = NULL;
	}
	int get_key();
	int get_increase();
	void inc_increase();
	bool get_fill();
	bool hash_lp_add(int);
	void set_next(int);
}hash_table[DIVISION_NUMBER];
// fill의 값을 가져온다.
bool element::get_fill()
{
	return fill;
}
// key의 값을 가져온다.
int element::get_key(){
	return key;
}
// increase의 값을 가져온다.
int element::get_increase(){
	return increase;
}
// increase의 값을 증가시킨다.
void element::inc_increase()
{
	increase++;
}
// 테이블이 가득 찬 경우는 false를 반환한다.
bool element::hash_lp_add(int input_key)
{
	if(fill == false)
	{
		// 비어있으므로 해당 키에 할당
		key = input_key;
		fill = true;
		return true;
	}
	else
		// 비어있지 않으므로 다른키를 갖도록함
		return false;
}
// 같은 인덱스가 나왔을시 다음번지에 할당해준다.
void element::set_next(int input_key)
{
	element *a = new element();
	a->hash_lp_add(input_key);
	this->next = a;	
}

// 해시테이블을입력 linear_search
void input_hash_table_linear_search(int key){

	element_index=hash_function(key);

	// 해시테이블이 입력결과가 꽉차있으면 다음 인덱스를 가리키도록한다.
	while(hash_table[element_index].get_fill() != false){
		element_index++;
		// 인댁스의 위치가 끝일경우 처음으로 다시돌린다.
		if(element_index>=DIVISION_NUMBER)
		{
			element_index %= DIVISION_NUMBER;
			cycle = true;
		}
		if(cycle && element_index==10)
		{
			cout << "hash table is full"<< endl;
			break;
		}
	}
	hash_table[element_index].hash_lp_add(key);
	cycle = false;
}

// 해시테이블을입력 quadratic_hash
void input_hash_table_quadratic_hash(int key){
	int count = 0;

	element_index=hash_function(key);
	count++;

	while(hash_table[element_index].get_fill() != false)
	{
		element_index += pow((double)count,2);
		element_index = hash_function(element_index);
		count++;
	}
	hash_table[element_index].hash_lp_add(key);
}

// 해시테이블을입력 Double_hash
void input_hash_table_double_hash(int key)
{
	element_index=hash_function(key);

	// 해시테이블이 입력결과가 꽉차있으면 해당결과를 다시 해쉬한다.
	while(hash_table[element_index].get_fill() != false)
	{
		hash_table[element_index].inc_increase();
		// 무한루프에서 벗어나게 한다.
		element_index += hash_table[element_index].get_increase();
		element_index = rehash_function(element_index);
	}
	hash_table[element_index].hash_lp_add(key);
}

// 해시테이블을입력 chaining
void input_hash_table_chaining(int key)
{
	element *move;
	element_index=hash_function(key);
	move = &hash_table[element_index];

	// 해시테이블이 입력결과가 꽉차있으면 해당번지 next로 간다.
	while(move->next != NULL)
	{
		move = move->next;
	}
	move->set_next(key);
}

void main()
{
	int data[MAX_COUNT_DATA] = {12, 44, 13, 88, 23, 94, 11, 39, 20, 16, 5};

	
	// quadratic_hash로 충돌해결 해시테이블 입력
	for(int temp=0; temp<MAX_COUNT_DATA; temp++)
	{
		input_hash_table_quadratic_hash(data[temp]);
	}
	cout << "Index\tquadratic_hash"<<endl;
	for(int temp=0; temp<MAX_COUNT_DATA; temp++)
	{
		cout << "  "<< temp << "\t"<< "    " << hash_table[temp].get_key() << endl;
	}

	// Double_hash로 충돌해결 해시테이블 입력
	memset(hash_table, 0 , sizeof(hash_table) * 10);
	for(int temp=0; temp<MAX_COUNT_DATA; temp++)
	{
		input_hash_table_double_hash(data[temp]);
	}
	cout << endl << "Index\tdouble_hash"<<endl;
	for(int temp=0; temp<MAX_COUNT_DATA; temp++)
	{
		cout << "  "<< temp << "\t"<< "    " << hash_table[temp].get_key() << endl;
	}

	// chaining으로 충돌해결 해시테이블 입력
	element *print;
	bool first;

	memset(hash_table, 0 , sizeof(hash_table) * 10);
	for(int temp=0; temp<MAX_COUNT_DATA; temp++)
	{
		input_hash_table_chaining(data[temp]);
	}
	cout << endl << "Index\tchaining"<<endl;
	for(int temp=0; temp<MAX_COUNT_DATA; temp++)
	{
		print = &hash_table[temp];
		first = true;
		while(print->next != NULL)
		{
			if(!first)
				cout << "  "<< temp << "\t"<< "    " << print->get_key() << endl;
			print = print->next;
			first = false;
		}
		if(print->get_key() != 0)
			cout << "  "<< temp << "\t"<< "    " << print->get_key() << endl;
	}
}