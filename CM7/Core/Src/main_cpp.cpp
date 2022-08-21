#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "FreeRTOSTasks/DummyTask.h"

#include <iostream>

template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}
void vTaskCode( void * pvParameters ) {
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.

*/  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    uint8_t t = 0;
    while(t<100)
    {
        t++;
        /* Task code goes here. */
    }
}

extern "C" void main_cpp(){

    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;
    xReturned = xTaskCreate(vTaskCode, "Dummy", 20000, nullptr, tskIDLE_PRIORITY + 1, &xHandle );
    if( xReturned == pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        vTaskDelete( xHandle );
    }

    vTaskStartScheduler();

    while (true) {


    }
    return;
}
