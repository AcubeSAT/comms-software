#include "ECSS_Configuration.hpp"
#include "Platform//Parameters/PlatformParameters.hpp"
#include "Services/ParameterService.hpp"

#ifdef SERVICE_PARAMETER

void ParameterService::initializeParameterMap() {
    parameters = {{PlatformParameters::ReportParametersUnusedStack,     PlatformParameters::reportParametersUnusedStack},
                  {PlatformParameters::AvailableHeap,                   PlatformParameters::availableHeap},

                  {PlatformParameters::AntennaDeploymentStatus,         PlatformParameters::antennaDeploymentStatus},
                  {PlatformParameters::SampleRateUHFTX,                 PlatformParameters::sampleRateUHFTX},
                  {PlatformParameters::SymbolRateSBandTX,               PlatformParameters::symbolRateSBandTX},
                  {PlatformParameters::SampleRateSBandTX,               PlatformParameters::sampleRateSBandTX},
                  {PlatformParameters::SymbolRateUHFTX,                 PlatformParameters::symbolRateUHFBandTX},
                  {PlatformParameters::CWInterval,                      PlatformParameters::cwInterval},

                  {PlatformParameters::GMSKBeaconInterval,              PlatformParameters::gmskBeaconInterval},
                  {PlatformParameters::ChannelNumberUHF,                PlatformParameters::channelNumberUHF},
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
                  {PlatformParameters::COMMSFLASHInt,                   PlatformParameters::commsFlashInt},
                  {PlatformParameters::COMMSSRAMInt,                    PlatformParameters::commsSRAMInt},
    };
}
#endif
