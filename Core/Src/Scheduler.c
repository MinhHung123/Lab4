#include "Scheduler.h"

struct Node SCH_Task_G[SCH_MAX_TASKS];
int32_t head = -1;
int32_t freehead = 0;
enum ErrorCode Error_code_G = 0;

void ResetNode(int32_t index){
	SCH_Task_G[index].data.pFunction = NULL;
	SCH_Task_G[index].data.Delay = 0;
	SCH_Task_G[index].data.Period = 0;
	SCH_Task_G[index].data.RunMe = 0;
	SCH_Task_G[index].data.Task_ID = 0;
}

void init_freelist(){
	for(int i=0; i<SCH_MAX_TASKS-1; i++){
		SCH_Task_G[i].next = i + 1;
	}
	SCH_Task_G[SCH_MAX_TASKS-1].next = -1;
}

void list_init(){
	head = -1;
	freehead = 0;
	init_freelist();
}

int32_t get_freeIndex(){
	if(freehead == -1){
		return -1;
	}
	int32_t freeIndex = freehead;
	freehead = SCH_Task_G[freehead].next;
	return freeIndex;
}

void back_freeIndex(int32_t index){
	ResetNode(index);
	SCH_Task_G[index].next = freehead;
	freehead = index;
}

int8_t is_empty(){
	return head == -1;
}

void insert(int32_t index){
	if(head == -1){
		head = index;
		SCH_Task_G[head].next = -1;
		return;
	}
	int32_t current = head;
	int32_t pre = -1;
	while(current != -1){
		if(SCH_Task_G[current].data.Delay > SCH_Task_G[index].data.Delay){
			SCH_Task_G[index].next = current;
			if(pre == -1) head = index;
			else SCH_Task_G[pre].next = index;
			SCH_Task_G[current].data.Delay -= SCH_Task_G[index].data.Delay;
			return;
		}
		SCH_Task_G[index].data.Delay -= SCH_Task_G[current].data.Delay;
		pre = current;
		current = SCH_Task_G[current].next;
	}
	SCH_Task_G[pre].next = index;
	SCH_Task_G[index].next = -1;
}

void DeleteHead(){
	if(head == -1){
		Error_code_G = ERROR_SCH_EMPTY_LIST;
		return;
	}
	head = SCH_Task_G[head].next;
}

void SCH_init(){
	list_init();
	Error_code_G = INIT;
}

void SCH_Update(void){
	if(is_empty()){
		return;
	}
	if(SCH_Task_G[head].data.Delay > 0) SCH_Task_G[head].data.Delay--;
	if(SCH_Task_G[head].data.Delay == 0) SCH_Task_G[head].data.RunMe = 1;
}

void SCH_Dispatch_Tasks(void){
	if(is_empty()) return;
	int32_t current = head;
	if(SCH_Task_G[current].data.RunMe == 1){
		SCH_Task_G[current].data.pFunction();
		SCH_Task_G[current].data.RunMe = 0;
		DeleteHead();
		if(SCH_Task_G[current].data.Period == 0){
			back_freeIndex(current);
		}
		else{
			SCH_Task_G[current].data.Delay = SCH_Task_G[current].data.Period;
			insert(current);
		}
	}
}

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	int32_t freeIndex = get_freeIndex();
	if(freeIndex == -1){
		Error_code_G = ERROR_SCH_EXCEED_MAX_TASK;
		return RETURN_ADD_ERROR;
	}
	SCH_Task_G[freeIndex].data.pFunction = pFunction;
	SCH_Task_G[freeIndex].data.Delay = DELAY;
	SCH_Task_G[freeIndex].data.Period = PERIOD;
	SCH_Task_G[freeIndex].data.RunMe = 0;
	SCH_Task_G[freeIndex].data.Task_ID = freeIndex;
	insert(freeIndex);
	return freeIndex;
}

uint8_t SCH_Delete_Task(uint32_t taskID){
	if(is_empty()){
		Error_code_G = ERROR_SCH_EMPTY_LIST_CAN_NOT_DELETE;
		return RETURN_ERROR;
	}
	int32_t current = head;
	int pre = -1;
	while(current != -1){
		if(SCH_Task_G[current].data.Task_ID == taskID){
			if(pre == -1){
				if(SCH_Task_G[current].next != -1){
					SCH_Task_G[SCH_Task_G[current].next].data.Delay += SCH_Task_G[current].data.Delay;
					if(SCH_Task_G[SCH_Task_G[current].next].data.Delay != 0){
						SCH_Task_G[SCH_Task_G[current].next].data.RunMe = 0;
					}
				}
				DeleteHead();
				back_freeIndex(current);
			}
			else{
				if(SCH_Task_G[current].next != -1){
					SCH_Task_G[SCH_Task_G[current].next].data.Delay += SCH_Task_G[current].data.Delay;
					if(SCH_Task_G[SCH_Task_G[current].next].data.Delay != 0){
						SCH_Task_G[SCH_Task_G[current].next].data.RunMe = 0;
				    }
					SCH_Task_G[pre].next = SCH_Task_G[current].next;
					ResetNode(current);
					back_freeIndex(current);
				}
			}
			return RETURN_NONE;
		}
		pre = current;
		current = SCH_Task_G[current].next;
	}
	return RETURN_NONE;
}


