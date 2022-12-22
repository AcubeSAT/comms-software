#include "ECSS_Configuration.hpp"


#include "PlatformParameters.hpp"
#include "Services/ParameterService.hpp"

#ifdef SERVICE_PARAMETER

void ParameterService::initializeParameterMap() {
    parameters = {{PlatformParameters::OnBoardYear,                     PlatformParameters::onBoardYear},
                  {PlatformParameters::OnBoardMonth,                    PlatformParameters::onBoardMonth},
                  {PlatformParameters::OnBoardDay,                      PlatformParameters::onBoardDay},
                  {PlatformParameters::OnBoardHour,                     PlatformParameters::onBoardHour},
                  {PlatformParameters::OnBoardMinute,                   PlatformParameters::onBoardMinute},
                  {PlatformParameters::OnBoardSecond,                   PlatformParameters::onBoardSecond},
                  {PlatformParameters::ReportParametersUnusedStack,     PlatformParameters::reportParametersUnusedStack},
                  {PlatformParameters::AvailableHeap,                   PlatformParameters::availableHeap},

                  {PlatformParameters::AntennaDeploymentStatus,         PlatformParameters::antennaDeploymentStatus},
                  {PlatformParameters::SampleRateUHFTX,                 PlatformParameters::sampleRateUHFTX},
                  {PlatformParameters::SymbolRateSBandTX,               PlatformParameters::symbolRateSBandTX},
                  {PlatformParameters::CWInterval,                      PlatformParameters::cwInterval},

                  {PlatformParameters::GMSKBeaconInterval,              PlatformParameters::gmskBeaconInterval},
                  {PlatformParameters::ChannelNumberUHF,                PlatformParameters::channelNumberUFH},
                  {PlatformParameters::ChannelNumberSBand,              PlatformParameters::channelNumberSBand},
                  {PlatformParameters::PowerAmplifierUHF,               PlatformParameters::powerAmplifierUHF},
                  {PlatformParameters::PowerAmplifierS,                 PlatformParameters::powerAmplifierS},
                  {PlatformParameters::VariableGainAmplifier,           PlatformParameters::variableGainAmplifier},
                  {PlatformParameters::ReceivedSignalStrengthIndicator, PlatformParameters::receivedSignalStrengthIndicator},
                  {PlatformParameters::UHFBandTX,                       PlatformParameters::uhfBandTX},

                  {PlatformParameters::SBandTX,                         PlatformParameters::sBandTX},
                  {PlatformParameters::NumberOfPackagesRejected,        PlatformParameters::numberOfPackagesRejected},
                  {PlatformParameters::InvalidHMAC,                     PlatformParameters::invalidHMAC},

                  {PlatformParameters::InvalidPacketStructure,          PlatformParameters::invalidPacketStructure},
                  {PlatformParameters::InvalidSpacecraftID,             PlatformParameters::invalidSpacecraftID},
                  {PlatformParameters::TCFrameSequenceCounter,          PlatformParameters::tcFrameSequenceCounter},
                  {PlatformParameters::COMMSPCBTemperature1,            PlatformParameters::commsPCBTemperature1},
                  {PlatformParameters::COMMSPCBTemperature2,            PlatformParameters::commsPCBTemperature2},
                  {PlatformParameters::COMMSMCUTemperature,             PlatformParameters::commsMCUTemperature},
                  {PlatformParameters::COMMSMCUInputVoltage,            PlatformParameters::commsMCUInputVoltage},
                  {PlatformParameters::COMMSMCUBootCounter,             PlatformParameters::commsMCUBootCounter},

                  {PlatformParameters::OnBoardTime,                     PlatformParameters::onBoardTime},
                  {PlatformParameters::NANDUsedMemoryPartition,         PlatformParameters::nandUsedMemoryPartition},
                  {PlatformParameters::LastFailedEvent,                 PlatformParameters::lastFailedEvent},
                  {PlatformParameters::COMMSMCUSystick,                 PlatformParameters::commsMCUSystick},

    };
}
#endif

