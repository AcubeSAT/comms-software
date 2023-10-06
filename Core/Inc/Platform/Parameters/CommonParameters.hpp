#pragma once

#include "Helpers/Parameter.hpp"
#include "Platform/COMMS_Definitions.hpp"
#include "PeakSatParameters.hpp"

namespace CommonParameters {

    inline auto &time = PeakSatParameters::commsMCUTime;
    inline auto &useUART = PeakSatParameters::commsUseUART;


}