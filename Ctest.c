#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>



bool table[] = {false, false, false};

bool stop_programm = false; //���� ����������� ������ ���������

pthread_mutex_t table_lock;

char *ingredient_name(int i){
	switch(i){
				case 0:
					return "paper";
				case 1:
					return "tobacco";
				case 2:
					return "matches";
				default:
					return "Wrong input: i is not in [0;3)";
				}
}

void smoke_weed(int i)
//��������� table, ������� �������
{
	char in;
	for(int i = 0; i<3;i+=1){
		table[i] = false;
	}
	printf("smoker with %s smokes a cigarette\n Would you like to continue? (input 'e' to stop)",ingredient_name(i));
	in = getchar();
	if(in=='e'){
		stop_programm = true;
	}
}

bool table_not_empty()
//�������� ������� ���� �� 1 ����������� �� �����
{
	for(int j = 0;j<3;j+=1){
		if(table[j]==true){
			return true;
		}
	}
	return false;
}

void look_on_table(int arg)
//����� ��� ����������: ��������� ������� �� ����, ���� ���� �� ������ � �� ����� �� ������� ��� �����������, �� �����
{
	int i = arg;
	//printf("called look_on table with id %d \n",i);
	while(!stop_programm){
		pthread_mutex_lock(&table_lock);
		if(table_not_empty() > 0 && table[i]==false){
			smoke_weed(i);
		}
		pthread_mutex_unlock(&table_lock);
	}
	pthread_exit(arg);
}

void place_on_table(void *arg)
//����� �������: ������ ��������� ����, ���� �� ������, �� �� ����� 2 ��������� �����������
{
	int i,i2,i3;
	while(!stop_programm){
		pthread_mutex_lock(&table_lock);
		if(!table_not_empty()){
			for(int i = 0;i<3;i++){
				printf("%d",table[i]);
			}
			i = rand()%3;
			i2= (i+1)%3;
			table[i]=true;
			table[i2]=true;
			printf("Bartender puts ingredients on table: %s, %s\n",ingredient_name(i),ingredient_name(i2));
		}
		pthread_mutex_unlock(&table_lock); //����� �� ����������� ����
	}
}



int main(void){
	pthread_mutex_init(&table_lock,NULL);
	pthread_t thread_id[4]; //������ 4 �����: 0,1,2 -- ����������, 3 -- ������
	for(int i=0;i<3;i+=1){
		pthread_create(&thread_id[i],NULL,look_on_table,i); //��������� ����� �����������
	}
	pthread_create(&thread_id[3],NULL,place_on_table,NULL); //��������� ����� �������
	for(int i = 0;i<4;i++){
		pthread_join(thread_id[i],NULL); //��� ��������� ������ �����
	}
	return  EXIT_SUCCESS;
}
