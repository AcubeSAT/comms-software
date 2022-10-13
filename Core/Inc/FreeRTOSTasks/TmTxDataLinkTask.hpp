#include "CCSDSChannel.hpp"
#include "CCSDSServiceChannel.hpp"
#include "Task.hpp"

class TmTxDataLinkTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    TmTxDataLinkTask() : Task("TmTxDataLink") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<TmTxDataLinkTask>, this->TaskName,
                          TmTxDataLinkTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};
extern etl::unique_ptr<ServiceChannel> serviceChannelptr;

inline std::optional<TmTxDataLinkTask> tmTxDataLinkTask;