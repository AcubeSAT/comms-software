#ifndef ECSS_SERVICES_PLATFORMPARAMETERS_HPP
#define ECSS_SERVICES_PLATFORMPARAMETERS_HPP

#include "Helpers/Parameter.hpp"

namespace PlatformParameters {
    /**
     * Definition of enumeration class that will be used to describe the status of the Antenna Deployment
     */
    enum ADMStatus : uint16_t {
        notDeployed = 0b000,
        Deployed = 0b1111,
        element1NotDeployed = 0b0001,
        elements12NotDeployed = 0b0011,
        elements13NotDeployed = 0b0101,
        elements14NotDeployed = 0b1001,
        elements123NotDeployed = 0b0111,
        elements124NotDeployed = 0b1011,
        elements134NotDeployed = 0b1101,
        element2NotDeployed = 0b0010,
        elements23NotDeployed = 0b0110,
        elements24NotDeployed = 0b1010,
        elements234NotDeployed = 0b1110,
        element3NotDeployed = 0b0100,
        elements34NotDeployed = 0b1100,
        element4NotDeployed = 0b1000
    };

    /**
     * Power Amplifier modes by Transceiver in dB
     */
    enum PAGain : uint8_t {
        noReduction = 0,
        reduction1dB = 1,
        reduction2dB = 2,
        reduction3dB = 3,
    };

    /**
     * Sample Rate of the sub-register in TX
     */
    enum TXSampleRate : uint8_t {
        case1 = 0x1,
        case2 = 0x2,
        case3 = 0x3,
        case4 = 0x4,
        case5 = 0x5,
        case6 = 0x6,
        case7 = 0x8,
        case8 = 0xA,
    };

    /**
     * Memory Partition, as defined in DDJF_OBSW
     */
    enum memoryPartition : uint8_t {
        firstPartition = 0,
        secondPartition = 1,
    };
    /**
     * ID enumeration of COMMS subsystem's specific parameters.
     */
    enum ParameterID : uint16_t {
        OnBoardYear = 0,
        OnBoardMonth = 1,
        OnBoardDay = 2,
        OnBoardHour = 3,
        OnBoardMinute = 4,
        OnBoardSecond = 5,
        ReportParametersUnusedStack = 6,
        AvailableHeap = 7,

        AntennaDeploymentStatus = 2003,
        SampleRateUHFTX = 2004,
        SymbolRateSBandTX = 2006,
        CWInterval = 2007,
        GMSKBeaconInterval = 2008,
        ChannelNumberUHF = 2011,
        ChannelNumberSBand = 2012,
        PowerAmplifierUHF = 2014,
        PowerAmplifierS = 2015,
        VariableGainAmplifier = 2016,
        ReceivedSignalStrengthIndicator = 2017,
        UHFBandTX = 2018,
        SBandTX = 2020,
        NumberOfPackagesRejected = 2021,
        InvalidHMAC = 2022,
        InvalidPacketStructure = 2023,
        InvalidSpacecraftID = 2024,
        TCFrameSequenceCounter = 2025,
        COMMSPCBTemperature1 = 2026,
        COMMSPCBTemperature2 = 2027,
        COMMSMCUTemperature = 2028,
        COMMSMCUInputVoltage = 2029,
        COMMSMCUBootCounter = 2030,
        OnBoardTime = 2031,
        NANDUsedMemoryPartition = 2032,
        LastFailedEvent = 2033,
        COMMSMCUSystick = 2034,
        COMMSFLASHInt = 2035,
        COMMSSRAMInt = 2036
    };

    // Parameters used for testing
    inline Parameter<uint16_t> onBoardYear(2022);
    inline Parameter<uint8_t> onBoardMonth(1);
    inline Parameter<uint8_t> onBoardDay(1);
    inline Parameter<uint8_t> onBoardHour(0);
    inline Parameter<uint8_t> onBoardMinute(0);
    inline Parameter<uint8_t> onBoardSecond(0);
    inline Parameter<uint16_t> reportParametersUnusedStack(0);
    inline Parameter<uint16_t> availableHeap(0);

    // COMMS parameters
    inline Parameter<ADMStatus> antennaDeploymentStatus(notDeployed);
    inline Parameter<TXSampleRate> sampleRateUHFTX(case1);
    inline Parameter<uint64_t> symbolRateSBandTX(0);
    inline Parameter<uint16_t> cwInterval(300);
    inline Parameter<uint16_t> gmskBeaconInterval(0);
    inline Parameter<uint32_t> channelNumberUFH(0);
    inline Parameter<uint32_t> channelNumberSBand(0);
    inline Parameter<PAGain> powerAmplifierUHF(noReduction);
    inline Parameter<PAGain> powerAmplifierS(noReduction);
    inline Parameter<uint8_t> variableGainAmplifier(0);
    inline Parameter<float> receivedSignalStrengthIndicator(0);
    inline Parameter<bool> uhfBandTX(false);
    inline Parameter<bool> sBandTX(false);
    inline Parameter<uint16_t> numberOfPackagesRejected(0);

    inline Parameter<uint16_t> invalidHMAC(0);
    inline Parameter<uint16_t> invalidPacketStructure(0);
    inline Parameter<uint16_t> invalidSpacecraftID(0);
    inline Parameter<uint16_t> tcFrameSequenceCounter(0);

    /**
     * The measurements of the Temperature and the Input Voltage from the sensors are floats. However, the sensors
     * measure a specific range. Hence, to cut 2 bytes, the parameter variables are now uint16_t. The max value of the
     * variable (2^16) now represents the upper value of the measurement range, and 0 the lower value of the range.
     */
    inline Parameter<uint16_t> commsPCBTemperature1(0);
    inline Parameter<uint16_t> commsPCBTemperature2(0);
    inline Parameter<uint16_t> commsMCUTemperature(0);
    inline Parameter<uint16_t> commsMCUInputVoltage(0);

    inline Parameter<uint32_t> commsMCUBootCounter(0);
    inline Parameter<uint32_t> onBoardTime(0);
    inline Parameter<memoryPartition> nandUsedMemoryPartition(firstPartition);
    inline Parameter<uint16_t> lastFailedEvent(0);
    inline Parameter<uint32_t> commsMCUSystick(0);
    inline Parameter<uint32_t> commsFlashInt(0);
    inline Parameter<uint32_t> commsSRAMInt(0);
}

#endif
