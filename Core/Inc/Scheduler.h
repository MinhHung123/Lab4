#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <main.h>

#define SCH_MAX_TASKS 5
#define SCH_TICK_SCALE 100
#define RETURN_ERROR 0
#define RETURN_NONE 1
#define RETURN_ADD_ERROR 2
#define NO_TASK_ID 0

typedef struct{
	void (*pFunction)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t Task_ID;
} SCH_Task;

struct Node{
	SCH_Task data;
	int32_t next;
};

enum ErrorCode
{
	INIT,
	ERROR_SCH_EXCEED_MAX_TASK,
	ERROR_SCH_EMPTY_LIST,
	ERROR_SCH_EMPTY_LIST_CAN_NOT_DELETE
};

extern struct Node SCH_Task_G[SCH_MAX_TASKS];
extern int32_t head;
extern int32_t freehead;
extern enum ErrorCode Error_code_G;

void ResetNode(int32_t index);
void init_freelist();
void list_init();
int32_t get_freeIndex();
void back_freeIndex(int32_t index);
int8_t is_empty();
void insert(int32_t index);
void DeleteHead();
void SCH_init();
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint32_t taskID);

#endif /* INC_SCHEDULER_H_ */
