#pragma once

#include "Helpers/Parameter.hpp"
#include "Platform/COMMS_Definitions.hpp"
#include "PlatformParameters.hpp"

namespace CommonParameters {

    inline auto &time = PlatformParameters::commsMCUTime;
    inline auto &useUART = PlatformParameters::commsUseUART;


}