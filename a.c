#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <unistd.h>

struct mem_shared{
	int dec_num;
	char bin_num[200];
	int flag; //紀錄執行步驟
};

typedef struct mem_shared m_sh;

int main(){
	const int SIZE = sizeof(m_sh);
	//宣告共享記憶體
	m_sh *shared_memory;
	int segment_id;
	segment_id = shmget(IPC_PRIVATE, SIZE, S_IRUSR | S_IWUSR);
	shared_memory = (m_sh *) shmat(segment_id, NULL, 0);
	//初始化
	shared_memory->flag = 0;
	shared_memory->dec_num = 0;
	char s[10];
	/* trigger to turn off the program */
	int switcher = 1;
	sprintf(s, "%d", segment_id);
	pid_t pid;

	pid = fork();

	if(pid < 0){
		printf("Fork fail!");
	}
	else if (pid == 0) { 
		execlp("./B10232017_b.out", s, (char *)0);
    	}
    	else{
		while(switcher){
			if(shared_memory->flag == 0){
				printf("Please input a number:");
				scanf("%d",&shared_memory->dec_num);
				shared_memory->flag = 1;
			}
			else if(shared_memory->flag == 2){
				printf("%s\n",shared_memory->bin_num);
				shared_memory->flag = 0;
			}
			else if(shared_memory->flag == 3){
				//結束程式
				switcher = 0;
			}
		}
    	}

    /* now detach the shared memory segment */
    shmdt(shared_memory);
    /* now remove the shared memory segment */
    shmctl(segment_id, IPC_RMID, NULL);
    return 0;
}
