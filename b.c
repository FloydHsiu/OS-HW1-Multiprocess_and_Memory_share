#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

struct datas{
	int dec;
	char dec_bin[25];
	int frequency_1;
	int length;
};

struct mem_shared{
	int dec_num;
	char bin_num[200];
	int flag; //checked who is using the memory
};

typedef struct mem_shared m_sh;
typedef struct datas data;

m_sh *shared_memory;
	//B storage
	data storage[30];
	int store_top = -1;
	int *SHM_dec_num;
	char *SHM_bin_num;
	int *SHM_flag;

int main(int argc, char *argv[]){

	int segment_id = atoi(argv[0]);
	int flag;
	int dec_num;//for trans dec to bin
	int i;//for loop
	int index, frequency_1, answer;//temp attribute of the answer
	int max, min;


	shared_memory = (m_sh *) shmat(segment_id, NULL, 0);
	SHM_dec_num = &shared_memory->dec_num;
	SHM_bin_num = shared_memory->bin_num;
	SHM_flag = &shared_memory->flag;

	int switcher = 1;
	while(switcher){
		if( *SHM_flag == 1){
			if(*SHM_dec_num >= 0){
				frequency_1 = 0;
				for(dec_num = *SHM_dec_num, index = 0, answer = 0; dec_num > 0; dec_num = dec_num >> 1,index++){
					answer += (int)((dec_num & 1) * pow(10, index));
					if((dec_num & 1) == 1) frequency_1++;
				}
				sprintf(SHM_bin_num, "%d:%d;", *SHM_dec_num, answer);
				store_top++;
				storage[store_top].frequency_1 = frequency_1;
				storage[store_top].dec = *SHM_dec_num;
				storage[store_top].length = (*SHM_dec_num/10) + index +3;
				sprintf(storage[store_top].dec_bin, "%s", SHM_bin_num);
				*SHM_flag = 2;
			}
			else if(*SHM_dec_num == -1){
				if(store_top < 0){
					sprintf(SHM_bin_num, "Error!");
				}
				else{
					max = storage[0].frequency_1;
					//find max
					for(i=1; i<=store_top ; i++){
						if(storage[i].frequency_1 > max){
							max = storage[i].frequency_1;
						}
					}
					//output search result
					for(i=0,index=0; i<=store_top ; i++){
						if(storage[i].frequency_1 == max){
							sprintf(&SHM_bin_num[index], "%s", storage[i].dec_bin);
							index += storage[i].length;
						}
					}
				}
				*SHM_flag = 2;
			}
			else if(*SHM_dec_num == -2){
				if(store_top < 0){
					sprintf(SHM_bin_num, "Error!");
				}
				else{
					min = storage[0].frequency_1;
					//find max
					for(i=1; i<=store_top ; i++){
						if(storage[i].frequency_1 < min){
							min = storage[i].frequency_1;
						}
					}
					//output search result
					for(i=0,index=0; i<=store_top ; i++){
						if(storage[i].frequency_1 == min){
							sprintf(&SHM_bin_num[index], "%s", storage[i].dec_bin);
							index += storage[i].length;
						}
					}
				}
				*SHM_flag = 2;
			}
			else{
				*SHM_flag = 3;
				switcher = 0;
			}
		}
	}
}
