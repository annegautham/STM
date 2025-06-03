#ifndef TASKS_HPP
#define TASKS_HPP

#include "config.hpp"

// Sets up and pins FreeRTOS tasks
extern QueueHandle_t teensyQueue;
void initTasks();
void teensySenderTask(void *param);
#endif
