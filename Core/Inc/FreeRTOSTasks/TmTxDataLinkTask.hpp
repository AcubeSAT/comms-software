#include "CCSDSChannel.hpp"
#include "CCSDSServiceChannel.hpp"
#include "Task.hpp"
#include "queue.h"
#include <etl/optional.h>

class TmTxDataLinkTask final : public Task {
private:
    static constexpr uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    TmTxDataLinkTask() : Task("TmTxDataLink") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<TmTxDataLinkTask>, this->TaskName,
                          TmTxDataLinkTask::TaskStackDepth, this, tskIDLE_PRIORITY + 2,
                          this->taskStack, &(this->taskBuffer));
    }
};
extern UART_HandleTypeDef huart3;
extern ServiceChannel *serviceChannelptr;
extern QueueHandle_t transmitPacketsQueue;
extern QueueHandle_t transmitPacketLengthsQueue;
inline etl::optional<TmTxDataLinkTask> tmTxDataLinkTask;