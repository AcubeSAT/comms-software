#ifndef ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP
#define ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP

#include "PlatformParameters.hpp"
#include "Services/HousekeepingService.hpp"

namespace HousekeepingStructures {
    using namespace PlatformParameters;

    static etl::array<HousekeepingStructure, ECSSMaxHousekeepingStructures> structures = {
            {
                    // COMMS Parameters that will be monitored in the campaign
                    {0, 1000, true,
                     {COMMSPCBTemperature1,
                      COMMSPCBTemperature2,
                      COMMSMCUTemperature,
                      COMMSMCUBootCounter}},
            }
    };
}

#endif
