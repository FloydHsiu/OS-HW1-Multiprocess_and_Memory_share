#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//void dec_to_bin();

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
	int store_top = 0;
	int *SHM_dec_num;
	char *SHM_bin_num;
	int *SHM_flag;

int main(int argc, char *argv[]){

	int segment_id = atoi(argv[0]);
	int flag;
	int dec_num;//for trans dec to bin
	int i;//for loop
	int index, frequency_1, num_index, answer;//temp attribute of the answer


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
				storage[store_top].frequency_1 = frequency_1;
				storage[store_top].dec = *SHM_dec_num;
				storage[store_top].length = (*SHM_dec_num/10) + index +3;
				sprintf(storage[store_top].dec_bin, "%s", SHM_bin_num);
				shared_memory->flag = 2;

			}
			else if(*SHM_dec_num == -1){
				sprintf(shared_memory->bin_num, "GGGGG");
				shared_memory->flag = 2;
			}
			else if(*SHM_dec_num == -2){

			}
			else{

			}
		}
	}

	/*

	while(switcher){
		flag = shared_memory->flag;
		if(flag == 1){
			dec_num = shared_memory->dec_num;
			if(dec_num >= 0){
				index = -1;
				frequency_1 = 0;
				// dec to bin
				while( dec_num >= 0){
					reverse[++index] = dec_num & mask;
					if(reverse[index-1] == 1) frequency_1++;
					dec_num = dec_num >> 1;
				}
				for(i =index; i >=0; i--){
					result[index-i] = reverse[i];
				}
				result[++index] = '\0';
				//start to save data
				num_index = shared_memory->dec_num/10 + 1;
				storage[store_top].length = num_index + index + 3;
				storage[store_top].frequency_1 = frequency_1;
				sprintf(storage[store_top].dec_bin, "%d:%s;", shared_memory->dec_num, result);
				//save it to SHM
				sprintf(shared_memory->bin_num, "%s", storage[store_top].dec_bin);
				store_top++;
			}
			else if(dec_num == -1){

			}
			else if(dec_num == -2){

			}
			else if(dec_num == -3){
				//process off
				switcher = 0;
			}
			else{

			}
		}
	}
	*/
}

/*void dec_to_bin(){
	int dec_num, mask =1;//for trans dec to bin
	int i;//for loop
	char reverse[20], result[20];//temp array to save answer
	int index, frequency_1, num_index;//temp attribute of the answer

	index = -1;
	frequency_1 = 0;
	// dec to bin
	while( dec_num >0){
		index++;
		reverse[index] = dec_num & mask;
		sprintf(&reverse[index], "%d", dec_num&mask);
		if(reverse[index] == '1') frequency_1++;
		dec_num = dec_num >> 1;
	}
	for(i =index; i >=0; i--){
		result[index-i] = reverse[i];
	}
	index++;
	result[index] = '\0';
	//start to save data
	num_index = shared_memory->dec_num/10 + 1;
	storage[store_top].length = num_index + index + 3;
	storage[store_top].frequency_1 = frequency_1;
	sprintf(storage[store_top].dec_bin, "%s;", result);
	//save it to SHM
	sprintf(shared_memory->bin_num, "%s", storage[store_top].dec_bin);
	store_top++;
}*/
