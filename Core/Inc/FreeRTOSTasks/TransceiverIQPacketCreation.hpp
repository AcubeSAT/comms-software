#include "Task.hpp"
#include "optional"
#include "GMSKTranscoder.hpp"
#include "filter.hpp"


inline constexpr uint8_t idleData[] = {
        0x53, 0x45, 0x24, 0x3, 0xce, 0xf0, 0xd2, 0x75, 0x50, 0xb9, 0x57, 0x24, 0x70, 0x83, 0xa8, 0x4e, 0x44, 0xd4, 0xa6,
        0x90, 0xc2, 0x9d, 0x1a, 0xb, 0x42, 0xe9, 0x42, 0x66, 0x28, 0x80, 0x5e, 0x30, 0x1f, 0x75, 0x3b, 0x5, 0xcb, 0x93,
        0xe3,
};

class TransceiverIQPacketCreation : public Task {
private:
    static constexpr uint16_t TaskStackDepth = 2000;
    uint16_t delayTime = 100;
    StackType_t taskStack[TaskStackDepth];
    uint16_t transmitCounter = 0;

    static constexpr uint16_t Length = sizeof(idleData)*8;
    static constexpr uint8_t SamplesPerSymbol = 6;

    double inPhaseBuffer[Length * SamplesPerSymbol] = {0};
    double quadraturePhaseBuffer[Length * SamplesPerSymbol] = {0};
    uint32_t transceiverData[Length * SamplesPerSymbol] = {0};

public:
    void execute();

    TransceiverIQPacketCreation(uint16_t delayTime) : Task("TransceiverIQPacketCreation") {
        this->delayTime = delayTime;
        int sampleFrequency = 48000;
        int symbolRate = sampleFrequency / SamplesPerSymbol;
        etl::bitset<Length> gmskIn = etl::bitset<Length>();
        for(uint16_t i = 0 ; i < sizeof(idleData) ; i++){
            uint8_t byte = idleData[i];
            for(uint8_t j = 0 ; j < 8 ; j++){
                gmskIn.set(i*8 + j, (byte & 1));
                byte = (byte >> 1) & 0xff;
            }
        }
        GMSKTranscoder<SamplesPerSymbol, Length> gmskTranscoder(sampleFrequency,symbolRate,false);
        gmskTranscoder.modulate(gmskIn, inPhaseBuffer,quadraturePhaseBuffer);
        packetizeIQ(inPhaseBuffer, quadraturePhaseBuffer, Length, transceiverData);
    }

    void createTask(){
        xTaskCreateStatic(vClassTask<TransceiverIQPacketCreation>, this->TaskName,
                          TransceiverIQPacketCreation::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<TransceiverIQPacketCreation> transceiverIQPacketCreation;