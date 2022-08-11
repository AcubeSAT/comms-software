#ifndef ECSS_SERVICES_PLATFORMPARAMETERS_HPP
#define ECSS_SERVICES_PLATFORMPARAMETERS_HPP

#include "Helpers/Parameter.hpp"

namespace PlatformParameters {
    /**
     * Definition of enumeration class that will be used to describe the status of the Antenna Deployment
     */
    enum ADMStatus {
        notDeployed,
        Deployed,
        element1NotDeployed,
        elements12NotDeployed,
        elements13NotDeployed,
        elements14NotDeployed,
        elements123NotDeployed,
        elements124NotDeployed,
        elements134NotDeployed,
        element2NotDeployed,
        elements23NotDeployed,
        elements24NotDeployed,
        elements234NotDeployed,
        element3NotDeployed,
        elements34NotDeployed,
        element4NotDeployed
    };

    /**
     * Power Amplifier modes by Transceiver in dB
     */
    enum PAGain {
        noReduction = 0,
        reduction1dB = -1,
        reduction2dB = -2,
        reduction3dB = -3,
    };

    /**
     * Memory Partition, as defined in DDJF_OBSW
     */
    enum memoryPartition {
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
        PCBTemperature1 = 2026,
        PCBTemperature2 = 2027,
        MCUTemperature = 2028,
        MCUInputVoltage = 2029,
        MCUBootCounter = 2030,
        OnBoardTime = 2031,
        NANDUsedMemoryPartition = 2032,
        LastFailedEvent = 2033,
        MCUSystick = 2034,

    };

    // Parameters used for testing
    inline Parameter <uint16_t> onBoardYear(2022);
    inline Parameter <uint8_t> onBoardMonth(1);
    inline Parameter <uint8_t> onBoardDay(1);
    inline Parameter <uint8_t> onBoardHour(0);
    inline Parameter <uint8_t> onBoardMinute(0);
    inline Parameter <uint8_t> onBoardSecond(0);
    inline Parameter <uint16_t> reportParametersUnusedStack(0);
    inline Parameter <uint16_t> availableHeap(0);

    // COMMS parameters
    inline Parameter <ADMStatus> AntennaDeploymentStatus(notDeployed);
    inline Parameter <uint64_t> SymbolRateSBandTX(0);
    inline Parameter <uint16_t> CWInterval(0);
    inline Parameter <uint16_t> GMSKBeaconInterval(0);
    inline Parameter <uint32_t> ChannelNumberUFH(0);
    inline Parameter <uint32_t> ChannelNumberSBand(0);
    inline Parameter <PAGain> PowerAmplifierUHF(0);
    inline Parameter <PAGain> PowerAmplifierS(0);
    inline Parameter <uint8_t> VariableGainAmplifier(0);
    inline Parameter <uint32_t> ReceivedSignalStrengthIndicator(0);
    inline Parameter<bool> UHFBandTX(false);
    inline Parameter<bool> SBandTX(false);
    inline Parameter <uint16_t> NumberOfPackagesRejected(0);

    inline Parameter <uint16_t> InvalidHMAC(0);
    inline Parameter <uint16_t> InvalidPacketStructure(0);
    inline Parameter <uint16_t> InvalidSpacecraftID(0);
    inline Parameter <uint16_t> TCFrameSequenceCounter(0);
    inline Parameter<float> PCBTemperature1(0);
    inline Parameter<float> PCBTemperature2(0);
    inline Parameter<float> MCUTemperature(0);
    inline Parameter<float> MCUInputVoltage(0);
    inline Parameter <uint32_t> MCUBootCounter(0);
    inline Parameter <uint32_t> OnBoardTime(0);
    inline Parameter <memoryPartition> NANDUsedMemoryPartition(firstPartition);
    inline Parameter <uint16_t> LastFailedEvent(0);
    inline Parameter <uint32_t> MCUSystick(0);
}

#endif
