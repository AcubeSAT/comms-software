#pragma once

#include "Helpers/Parameter.hpp"
#include "Platform/COMMS_Definitions.hpp"
#include "AcubeSATParameters.hpp"

namespace CommonParameters {

    inline auto &time = AcubeSATParameters::commsMCUTime;
    inline auto &useUART = AcubeSATParameters::commsUseUART;


}