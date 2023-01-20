#pragma once

#include "Task.hpp"
#include "GMSKTranscoder.hpp"
#include "ConvolutionalEncoder.hpp"
#include "etl/bitset.h"

constexpr uint16_t CodewordLength = 2048;
constexpr uint8_t Rate = 2;
constexpr uint8_t SamplesPerSymbol = 6;

/**
 * Packet fragmentation in order to temporarily save memory (currently 77.72% with no fragmentation)
 * Possible values: 1, 2 and 4
 */
constexpr uint8_t NumberOfSubpackets = 1;

class TxUHFTask : public Task {
private:
    etl::bitset<CodewordLength> tmPacket = etl::bitset<CodewordLength>();
    etl::bitset<CodewordLength * Rate> tmEncodedPacket = etl::bitset<CodewordLength * Rate>();
    // For samples per symbol = 10
    // TODO: Store these buffers to an external memory or define 2-3 general buffers for every task
    double inPhaseBuffer[(CodewordLength * Rate * SamplesPerSymbol) / NumberOfSubpackets] = {0};
    double quadraturePhaseBuffer[(CodewordLength * Rate * SamplesPerSymbol) / NumberOfSubpackets] = {0};

    GMSKTranscoder<SamplesPerSymbol, (CodewordLength * Rate) / NumberOfSubpackets> gmskTranscoder;

    ConvolutionalEncoder convolutionalEncoder;

    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    TxUHFTask(uint16_t sampleFrequency, uint16_t symbolRate, bool equalize) : Task("Tx UHF Task"),
                gmskTranscoder(sampleFrequency, symbolRate, equalize), convolutionalEncoder() {
        for (int i = 0; i < CodewordLength; ++i) {
            tmPacket.set(i, std::rand() % 2);
        }
    }

    void createTask(){
        xTaskCreateStatic(vClassTask<TxUHFTask>, this->TaskName,
                          TxUHFTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<TxUHFTask> txUHFTask;