/*
 * ��  �� : �����ͱ�������
 * ��  �� : ���� �����Ͱ� �Էµɶ� �浹�� ����, ����, ü�̴��� ����Ͽ� ó���ϰ�
			�ؽ� ���̺��� ������ ���
 */

#include <iostream>
using namespace std;

#define MAX_COUNT_DATA 11
#define DIVISION_NUMBER 11
#define REDIVISION_NUMBER 11

int element_index=0;
bool cycle = false;

// int���� �ؽ�
int hash_function(int key)
{
	return key % DIVISION_NUMBER;
}
int rehash_function(int key)
{
	// �������� 7�� ������� ���̺��� 0~6���� �������������� ���ѷ���
	// �� ���⶧���� ������ ���� �� ū���� ���ؽ��ؾ��Ѵ�.
	return (REDIVISION_NUMBER - (key % REDIVISION_NUMBER));
}
// ������̺�
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
// fill�� ���� �����´�.
bool element::get_fill()
{
	return fill;
}
// key�� ���� �����´�.
int element::get_key(){
	return key;
}
// increase�� ���� �����´�.
int element::get_increase(){
	return increase;
}
// increase�� ���� ������Ų��.
void element::inc_increase()
{
	increase++;
}
// ���̺��� ���� �� ���� false�� ��ȯ�Ѵ�.
bool element::hash_lp_add(int input_key)
{
	if(fill == false)
	{
		// ��������Ƿ� �ش� Ű�� �Ҵ�
		key = input_key;
		fill = true;
		return true;
	}
	else
		// ������� �����Ƿ� �ٸ�Ű�� ��������
		return false;
}
// ���� �ε����� �������� ���������� �Ҵ����ش�.
void element::set_next(int input_key)
{
	element *a = new element();
	a->hash_lp_add(input_key);
	this->next = a;	
}

// �ؽ����̺����Է� linear_search
void input_hash_table_linear_search(int key){

	element_index=hash_function(key);

	// �ؽ����̺��� �Է°���� ���������� ���� �ε����� ����Ű�����Ѵ�.
	while(hash_table[element_index].get_fill() != false){
		element_index++;
		// �δ콺�� ��ġ�� ���ϰ�� ó������ �ٽõ�����.
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

// �ؽ����̺����Է� quadratic_hash
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

// �ؽ����̺����Է� Double_hash
void input_hash_table_double_hash(int key)
{
	element_index=hash_function(key);

	// �ؽ����̺��� �Է°���� ���������� �ش����� �ٽ� �ؽ��Ѵ�.
	while(hash_table[element_index].get_fill() != false)
	{
		hash_table[element_index].inc_increase();
		// ���ѷ������� ����� �Ѵ�.
		element_index += hash_table[element_index].get_increase();
		element_index = rehash_function(element_index);
	}
	hash_table[element_index].hash_lp_add(key);
}

// �ؽ����̺����Է� chaining
void input_hash_table_chaining(int key)
{
	element *move;
	element_index=hash_function(key);
	move = &hash_table[element_index];

	// �ؽ����̺��� �Է°���� ���������� �ش���� next�� ����.
	while(move->next != NULL)
	{
		move = move->next;
	}
	move->set_next(key);
}

void main()
{
	int data[MAX_COUNT_DATA] = {12, 44, 13, 88, 23, 94, 11, 39, 20, 16, 5};

	
	// quadratic_hash�� �浹�ذ� �ؽ����̺� �Է�
	for(int temp=0; temp<MAX_COUNT_DATA; temp++)
	{
		input_hash_table_quadratic_hash(data[temp]);
	}
	cout << "Index\tquadratic_hash"<<endl;
	for(int temp=0; temp<MAX_COUNT_DATA; temp++)
	{
		cout << "  "<< temp << "\t"<< "    " << hash_table[temp].get_key() << endl;
	}

	// Double_hash�� �浹�ذ� �ؽ����̺� �Է�
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

	// chaining���� �浹�ذ� �ؽ����̺� �Է�
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