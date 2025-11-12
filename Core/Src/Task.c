#include "Task.h"
#include "main.h"

void Task1(){
	HAL_GPIO_TogglePin(LED_A_SGN_GPIO_Port, LED_A_SGN_Pin);

}

void Task2(){
	HAL_GPIO_TogglePin(LED_B_SGN_GPIO_Port, LED_B_SGN_Pin);

}

void Task3(){
	HAL_GPIO_TogglePin(LED_C_SGN_GPIO_Port, LED_C_SGN_Pin);

}

void Task4(){
	HAL_GPIO_TogglePin(LED_D_SGN_GPIO_Port, LED_D_SGN_Pin);

}

void Task5(){
	HAL_GPIO_TogglePin(LED_E_SGN_GPIO_Port, LED_E_SGN_Pin);
}

