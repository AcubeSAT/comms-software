#include "TmTxDataLinkTask.hpp"
#include "Task.hpp"
#include "etl/string.h"

void TmTxDataLinkTask::execute(){
    //uint8_t packet[] = {0,34,13,1,3};
    //uint8_t packetDestination[] = {0,0,0,0,0};
    //serviceChannelptr->storePacketTm(packet, 5, 0);
    //serviceChannelptr->vcGenerationService(60, 0);
    //serviceChannelptr->mcGenerationTMRequest();
    //serviceChannelptr->allFramesGenerationTMRequest(packetDestination, TmTransferFrameSize);
    HAL_Delay(100);
    //UART_HandleTypeDef huart3;
    //std::string frame ((char *)serviceChannelptr->frameAfterALlFramesGenerationService()->packetData(), TmTransferFrameSize);
    //char string[40];
    //snprintf(string, sizeof("[%d]TM TX Frame\n\r"),"[%d]TM TX Frane\n\r", sizeof((char *)serviceChannelptr->frameAfterALlFramesGenerationService()->packetData()), (char *)serviceChannelptr->frameAfterALlFramesGenerationService()->packetData());

    
}
