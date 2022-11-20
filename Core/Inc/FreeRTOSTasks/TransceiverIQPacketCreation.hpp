#include "Task.hpp"
#include "optional"
#include "GMSKTranscoder.hpp"
#include "filter.hpp"

class TransceiverIQPacketCreation : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];

    static constexpr uint16_t Length = 16;
    static constexpr uint8_t SamplesPerSymbol = 6;

    double inPhaseBuffer[Length * SamplesPerSymbol] = {0};
    double quadraturePhaseBuffer[Length * SamplesPerSymbol] = {0};
    uint32_t transceiverData[Length * SamplesPerSymbol] = {0};

public:
    void execute();

    TransceiverIQPacketCreation() : Task("TransceiverIQPacketCreation") {
        uint16_t kikasBytes = 0xF1FA;
        int sampleFrequency = 48000;
        int symbolRate = sampleFrequency / SamplesPerSymbol;
        etl::bitset<Length> gmskIn = etl::bitset<Length>(kikasBytes);
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