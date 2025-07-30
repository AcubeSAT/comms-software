#pragma once

#include "Parameter.hpp"
#include "NotifyParameter.hpp"
// TODO ensure this matches the mission database excel sheet. the same file needs to be updated in cross-platform-software
namespace AcubeSATParameters {
    /**
     * ID enumeration of OBDH subsystem's specific parameters.
     */
    enum ParameterID : uint16_t {
        OBCUseRTT = 8,
        OBCUseUART = 9,
        OBCUseCAN = 10,

        /* OBDH Parameters */
        OBCPCBTemperature1 = 5000,
        OBCPCBTemperature2 = 5001,
        OBCMCUTemperature = 5002,
        OBCMCUInputVoltage = 5003,
        OBCMCUBootCounter = 5004,
        OBCFlashInt = 5005,
        OBCSRAMInt = 5006,
        OBCAvailableMRAM = 5007,
        OBCAvailableNAND = 5008,
        OBCSpacecraftTimeRef = 5009, ///< which subsystem holds the correct time
        OBCOnBoardTime = 5010,
        OBCOperationalMode = 5011,
        OBCMemoryPartition = 5012,
        OBCReconfigurationTimer = 5013, ///< timer responsible to reset the spacecraft to a known working state unless it has been
        ///< reset by a ground station pass.
        OBCLastFailedEvent = 5014,
        OBCMCUSystick = 5015,
        OBCCANBUSLoad1 = 5016,  ///< the CAN bus load is based on the used capacity (bandwidth) divided by maximum capacity
        OBCCANBUSLoad2 = 5017,
        OBCCANBUSActive = 5018,
        OBCMCUFDIR = 5019,
        OBCMCURestartSafeModeThreshold = 5020,
        OBCNANDFLASHLCLThreshold = 5021,
        OBCMRAMLCLThreshold = 5022,
        OBCNANDFLASHON = 5023,
        OBCMRAMON = 5024,
        OBCNANDFLASHScrubbingFrequency = 5025,
        OBCRAMScrubbingFrequency = 5026,
        OBCProgramFlashScrubbingFrequency = 5027,

        /* SU */
        SUUseRTT = 11,
        SUSUseUART = 12,
        SUSUseCAN = 13,

        SUPCBTemperature1 = 4000,
        SUPCBTemperature2 = 4001,
        SUPCBTemperatureControlValves = 4002,
        SUPCBTemperatureFlowValves = 4003,
        SUPDMSTemperature = 4004,
        SUGrowthMediumTemperature = 4005,
        SULEDIntensity1 = 4006,
        SULEDIntensity2 = 4007,
        SULEDIntensity3 = 4008,
        SULEDIntensity4 = 4009,
        SUPressureLevelSensor1 = 4010,
        SUPressureLevelSensor2 = 4011,
        SUHumidityLevelSensor1 = 4012,
        SUHumidityLevelSensor2 = 4013,
        SUNumberOfPhotosTaken = 4014,
        SUPumpFlowRate1 = 4015,
        SUPumpFlowRate2 = 4016,
        SUControlChamberValve1 = 4017,
        SUControlChamberValve2 = 4018,
        SUControlChamberValve3 = 4019,
        SUControlSandwitchValve1 = 4020,
        SUControlSandwitchValve2 = 4021,
        SUControlSandwitchValve3 = 4022,
        SUFlowValveInlet1 = 4023,
        SUFlowValveInlet2 = 4024,
        SUFlowValveInlet3 = 4025,
        SUFlowValveOutlet1 = 4026,
        SUFlowValveOutlet2 = 4027,
        SUFlowValveOutlet3 = 4028,
        SUFlowValveTest1 = 4029,
        SUFlowValveTest2 = 4030,
        SUPhotoExposureTime = 4031,
        SUPhotoInterval = 4032,
        SUPhotoCroppedResolution = 4033,
        SUPhotoCompressionRate = 4034,
        SUExperimentNumber = 4035,
        SURADFET = 4036,
        SUCameraTemperature = 4037,
        SUMCUTemperature = 4038,
        SUMCUInputVoltage = 4039,
        SUMCUBootCounter = 4040,
        SUFlashIntUsedPercentage = 4041,
        SUFlashUsedPercentage = 4042,
        SUSRAMUsedPercentage = 4043,
        SUOnBoardTime = 4044,
        SUNANDCurrentlyUsedPartition = 4045,
        SULastFailedEvent = 4046,
        SUMCUSysTick = 4047,
        SUNANDFlashLCLThreshold = 4048,
        SUFlowPumpFlashLCLThreshold = 4049,
        SUControlPumpFlashLCLThreshold = 4050,
        SUCameraLCLThreshold = 4051,
        SUNANDFlashLCLOn = 4052,
        SUFlowPumpFlashOn = 4053,
        SUControlPumpFlashLCLOn = 4054,
        SUCameraON = 4055,
        SUNANDFlashScrubbingFrequency = 4056,
        SURAMScrubbingFrequency = 4057,
        SUProgramFlashScrubbingFrequency = 4058,
        SUTemperatureFromHumiditySensor1 = 4059,
        SUTemperatureFromHumiditySensor2 = 4060,


        /* ADCS */
        ADCSUseRTT = 11,
        ADCSUseUART = 12,
        ADCSUseCAN = 13,

        ADCSXBodyFrameRegardingOrbitFrame = 1000,
        ADCSYBodyFrameRegardingOrbitFrame = 1001,
        ADCSZBodyFrameRegardingOrbitFrame = 1002,
        ADCSAngularVelocityOnX = 1003,
        ADCSAngularVelocityOnY = 1004,
        ADCSAngularVelocityOnZ = 1005,
        ADCSBDotXAxis = 1006,
        ADCSBDotYAxis = 1007,
        ADCSBDotZAxis = 1008,
        ADCSQuaternionOrbitFrameToBodyScalar = 1009,
        ADCSQuaternionOrbitFrameToBodyVectorElem1 = 1010,
        ADCSQuaternionOrbitFrameToBodyVectorElem2 = 1011,
        ADCSQuaternionOrbitFrameToBodyVectorElem3 = 1012,

        ADCSMagnetometerRawX = 1013,
        ADCSMagnetometerRawY = 1014,
        ADCSMagnetometerRawZ = 1015,
        ADCSMagnetometerFrequency = 1016,
        ADCSMagnetometerCycleCountX = 1017,
        ADCSMagnetometerCycleCountY = 1018,
        ADCSMagnetometerCycleCountZ = 1019,
        ADCSMagnetometerMeasurementUnsuccessful = 1020,
        ADCSMagnetomerSPIInvalidArg = 1021,
        ADCSMagnetometerXAxisValueCorrect = 1022,
        ADCSMagnetometerYAxisValueCorrect = 1023,
        ADCSMagnetometerZAxisValueCorrect = 1024,
        ADCSMagnetometerXAxisOK = 1025,
        ADCSMagnetometerYAxisOK = 1026,
        ADCSMagnetometerZAxisOK = 1027,
        ADCSMagnetometerSPICommunicationSuccessful = 1028,
        ADCSMagnetometerDeltaCheck= 1029,

        ADCSMagnetometerISISCalibratedXAxis = 1030,
        ADCSMagnetometerISISCalibratedYAxis = 1031,
        ADCSMagnetometerISISCalibratedZAxis = 1032,
        ADCSMagnetometerISISFilteredXAxis = 1033,
        ADCSMagnetometerISISFilteredYAxis = 1034,
        ADCSMagnetometerISISFilteredZAxis = 1035,
        ADCSMagnetometerISISRawXAxis = 1036,
        ADCSMagnetometerISISRawYAxis = 1037,
        ADCSMagnetometerISISRawZAxis = 1038,

        ADCSGyroscopeRateX = 1039,
        ADCSGyroscopeRateY = 1040,
        ADCSGyroscopeRateZ = 1041,

        ADCSGyroXLOCST1Register = 1042,
        ADCSGyroXHICST1Register = 1043,
        ADCSGyroXQUAD1Register = 1044,
        ADCSGyroXFAULTRegisterFailBit = 1045,
        ADCSGyroXFAULTRegisterOVBit = 1046,
        ADCSGyroXFAULTRegisterUVBit = 1047,
        ADCSGyroXFAULTRegisterPPLBit = 1048,
        ADCSGyroXFAULTRegisterQbit = 1049,
        ADCSGyroXFAULTRegisterNVMBit = 1050,
        ADCSGyroXFAULTRegisterPORBit = 1051,
        ADCSGyroXFAULTRegisterCSTBit = 1052,
        ADCSGyroXSPIInvalidArg = 1053,
        ADCSGyroXMeasurementUnsuccessful = 1054,
        ADCSGyroXValueCorrect = 1055,
        ADCSGyroXSPICommunicationSuccessful = 1056,

        ADCSGyroYLOCS1TRegister = 1057,
        ADCSGyroYHICST1Register = 1058,
        ADCSGyroYQUAD1Register = 1059,
        ADCSGyroYFAULTRegisterFailBit = 1060,
        ADCSGyroYFAULTRegisterOVBit = 1061,
        ADCSGyroYFAULTRegisterUVBit = 1062,
        ADCSGyroYFAULTRegisterPPLBit = 1063,
        ADCSGyroYFAULTRegisterQbit = 1064,
        ADCSGyroYFAULTRegisterNVMBit = 1065,
        ADCSGyroYFAULTRegisterPORBit = 1066,
        ADCSGyroYFAULTRegisterCSTBit = 1067,
        ADCSGyroYSPIInvalidArg = 1068,
        ADCSGyroYMeasurementUnsuccessful = 1069,
        ADCSGyroYValueCorrect = 1070,
        ADCSGyroYSPICommunicationSuccessful = 1071,

        ADCSGyroZLOCS1TRegister = 1072,
        ADCSGyroZHICST1Register = 1073,
        ADCSGyroZQUAD1Register = 1074,
        ADCSGyroZFAULTRegisterFailBit = 1075,
        ADCSGyroZFAULTRegisterOVBit = 1076,
        ADCSGyroZFAULTRegisterUVBit = 1077,
        ADCSGyroZFAULTRegisterPPLBit = 1078,
        ADCSGyroZFAULTRegisterQbit = 1079,
        ADCSGyroZFAULTRegisterNVMBit = 1080,
        ADCSGyroZFAULTRegisterPORBit = 1081,
        ADCSGyroZFAULTRegisterCSTBit = 1082,
        ADCSGyroZSPIInvalidArg = 1083,
        ADCSGyroZMeasurementUnsuccessful = 1084,
        ADCSGyroZValueCorrect = 1085,
        ADCSGyroZSPICommunicationSuccessful = 1086,

        ADCSMagnetorquerOnXAxisHBridgePolarity = 1087,
        ADCSMagnetorquerOnYAxisHBridgePolarity = 1088,
        ADCSMagnetorquerOnZAxisHBridgePolarity = 1089,
        ADCSMagnetorquerOnXAxisOnOff = 1090,
        ADCSMagnetorquerOnYAxisOnOff = 1091,
        ADCSMagnetorquerOnZAxisOnOff = 1092,

        ADCSGainBdotXAxis = 1093,
        ADCSGainBdotYAxis = 1094,
        ADCSGainBdotZAxis = 1095,
        ADCSGainProportionalPDXAxis = 1096,
        ADCSGainProportionalPDYAxis = 1097,
        ADCSGainProportionalPDZAxis = 1098,
        ADCSGainDerivativePDXAxis = 1099,
        ADCSGainDerivativePDYAxis = 1100,
        ADCSGainDerivativePDZAxis = 1101,
        ADCSiMQTBoardCoilXTemperatureSensor = 1102,
        ADCSiMQTBoardCoilYTemperatureSensor = 1103,
        ADCSiMQTBoardCoilZTemperatureSensor = 1104,
        ADCSiMQTBoardExtraTemperatureSensor = 1105,

        ADCSGyroscopeXTemperature = 1106,
        ADCSGyroscopeYTemperature = 1107,
        ADCSGyroscopeZTemperature = 1108,
        ADCSBoardTemperature1 = 1109,
        ADCSBoardTemperature2 = 1110,

        ADCSMode = 1111,
        ADCSTLE = 1112,
        ADCSInitialKalmanQuaternionECIFrameToBodyScalar = 1113,
        ADCSInitialKalmanQuaternionECIFrameToBodyVectorElem1 = 1114,
        ADCSInitialKalmanQuaternionECIFrameToBodyVectorElem2 = 1115,
        ADCSInitialKalmanQuaternionECIFrameToBodyVectorElem3 = 1116,
        ADCSTLEOrbitalElementsArgumentPerigee = 1117,
        ADCSTLEOrbitalElementsRAAN = 1118,
        ADCSTLEOrbitalElementsInclination = 1119,
        ADCSTLEOrbitalElementsEccentricity = 1120,
        ADCSTLEOrbitalElementsMeanAnomaly = 1121,
        ADCSSGP4OrbitalElementsArgumentPerigee = 1122,
        ADCSSGP4OrbitalElementsRAAN = 1123,
        ADCSSGP4OrbitalElementsInclination = 1124,
        ADCSSGP4OrbitalElementsMeanAnomaly = 1125,
        ADCSTimeGST_JD = 1126,

        ADCSDeterminationInitialMatricesQ1 = 1127,
        ADCSDeterminationInitialMatricesQ2 = 1128,
        ADCSDeterminationInitialMatricesQ3 = 1129,
        ADCSDeterminationInitialMatricesQ4 = 1130,
        ADCSDeterminationInitialMatricesQ5 = 1131,
        ADCSDeterminationInitialMatricesQ6 = 1132,
        ADCSDeterminationInitialMatricesQ7 = 1133,
        ADCSDeterminationInitialMatricesQ8 = 1134,
        ADCSDeterminationInitialMatricesQ9 = 1135,
        ADCSDeterminationInitialMatricesQ10 = 1136,
        ADCSDeterminationInitialMatricesR1 = 1137,
        ADCSDeterminationInitialMatricesR2 = 1138,
        ADCSDeterminationInitialMatricesR3 = 1139,
        ADCSDeterminationInitialMatricesR4 = 1140,
        ADCSDeterminationInitialMatricesR5 = 1141,
        ADCSDeterminationInitialMatricesR6 = 1142,
        ADCSDeterminationInitialMatricesR7 = 1143,
        ADCSDeterminationInitialMatricesR8 = 1144,
        ADCSDeterminationInitialMatricesR9 = 1145,

        ADCSDeterminationInitialMatricesPInitial1 = 1146,
        ADCSDeterminationInitialMatricesPInitial2 = 1147,
        ADCSDeterminationInitialMatricesPInitial3 = 1148,
        ADCSDeterminationInitialMatricesPInitial4 = 1149,
        ADCSDeterminationInitialMatricesPInitial5 = 1150,
        ADCSDeterminationInitialMatricesPInitial6 = 1151,
        ADCSDeterminationInitialMatricesPInitial7 = 1152,
        ADCSDeterminationInitialMatricesPInitial8 = 1153,
        ADCSDeterminationInitialMatricesPInitial9 = 1154,
        ADCSDeterminationInitialMatricesPInitial10 = 1155,

        ADCSReactionWheelOn = 1156,
        ADCSReactionWheelAngularVelocity = 1157,
        ADCSMagnetorquerAxisAssignment = 1158,
        ADCSControlProfileSwitching = 1159,
        ADCSTransformationAxisXAssignment = 1160,
        ADCSTransformationAxisYAssignment = 1161,
        ADCSTransformationAxisZAssignment = 1162,
        ADCSMagnetometerAxisAssignment = 1163,
        ADCSSunSensorAxisAssignment = 1164,
        ADCSGyroscopeAxisAssignment = 1165,
        ADCSFrameAxisSignECIToECEF_X = 1166,
        ADCSFrameAxisSignECIToECEF_Y = 1167,
        ADCSFrameAxisSignECIToECEF_Z = 1168,
        ADCSFrameAxisSignECIToOrbit_X = 1169,
        ADCSFrameAxisSignECIToOrbit_Y = 1170,
        ADCSFrameAxisSignECIToOrbit_Z = 1171,
        ADCSFrameAxisSignNEDToECEF_X = 1172,
        ADCSFrameAxisSignNEDToECEF_Y = 1173,
        ADCSFrameAxisSignNEDToECEF_Z = 1174,
        ADCSMagnetometerSignX = 1175,
        ADCSMagnetometerSignY = 1176,
        ADCSMagnetometerSignZ = 1177,
        ADCSSunSensorXAxisSign = 1178,
        ADCSSunSensorYAxisSign = 1179,
        ADCSSunSensorZAxisSign = 1180,
        ADCSGyroSignX = 1181,
        ADCSGyroSignY = 1182,
        ADCSGyroSignZ = 1183,
        ADCSDetumblingNominalThresholds = 1184,
        ADCSNominalDetumblingThresholds = 1185,
        ADCSStandByDetumblingThresholds = 1186,
        ADCSDetumblingStandByThresholds = 1187,
        ADCSNominalStandByThresholds = 1188,
        ADCSMagnetorquerMagneticDipoleX = 1189,
        ADCSMagnetorquerMagneticDipoleY = 1190,
        ADCSMagnetorquerMagneticDipoleZ = 1191,
        ADCSMagnetorquerDutyCycle = 1192,
        ADCSMagnetorquerCurrentX = 1193,
        ADCSMagnetorquerCurrentY = 1194,
        ADCSMagnetorquerCurrentZ = 1195,
        ADCSMagnetorquerBDot = 1196,
        ADCSReactionWheelTorque = 1197,
        ADCSReactionWheelAngularAcceleration = 1198,
        ADCSDesiredQuaternionOrbitToBodyScalar = 1199,
        ADCSDesiredQuaternionVectorElem1 = 1200,
        ADCSDesiredQuaternionVectorElem2 = 1201,
        ADCSDesiredQuaternionVectorElem3 = 1202,
        ADCSAngularVelocityDesiredX = 1203,
        ADCSAngularVelocityDesiredY = 1204,
        ADCSAngularVelocityDesiredZ = 1205,


        ADCSGyroBiasX = 1206,
        ADCSGyroBiasY = 1207,
        ADCSGyroBiasZ = 1208,

        ADCSSatellitePositionLatitude = 1209,
        ADCSSatellitePositionLongitude = 1210,
        ADCSSatellitePositionHeight = 1211,
        ADCSSatellitePositionECIX = 1212,
        ADCSSatellitePositionECIY = 1213,
        ADCSSatellitePositionECIZ = 1214,
        ADCSSatelliteLinearVelocityECIX = 1215,
        ADCSSatelliteLinearVelocityECIY = 1216,
        ADCSSatelliteLinearVelocityECIZ = 1217,
        ADCSMagneticFieldNEDX = 1218,
        ADCSMagneticFieldNEDY = 1219,
        ADCSMagneticFieldNEDZ = 1220,
        ADCSMagneticFieldECIX = 1221,
        ADCSMagneticFieldECIY = 1222,
        ADCSMagneticFieldECIZ = 1223,
        ADCSSunPositionECIX = 1224,
        ADCSSunPositionECIY = 1225,
        ADCSSunPositionECIZ = 1226,
        ADCSEclipse = 1227,
        ADCSDesaturationPecentageParameter = 1228,
        ADCSISISMTQModes = 1229,
        ADCSMagnetorquerActuationMethod = 1230,
        ADCSDutyCycleDetermination = 1231,
        ADCSDutyCycleControl = 1232,
        ADCSDutyCycleOther = 1233,
        ADCSDesatInitialAngularVelocityParam = 1234,

        ADCSMCUTemperature = 1235,

        ADCSMCUInputVoltage = 1236,

        ADCSBootCounter = 1237,
        ADCSOnBoardTime = 1238,

        ADCSMemoryPartition = 1239,
        ADCSLastFailedEvent = 1240,

        ADCSSystick = 1241,

        ADCSRAMScrubbingFrequency = 1242,
        ADCSProgramFlashScrubbingFrequency = 1243,
        ADCSFLASHInt = 1244,
        ADCSSRAMInt = 1245,

        ADCSSCHA63TRateX = 1246,
        ADCSSCHA63TRateY = 1247,
        ADCSSCHA63TRateZ = 1248,
        ADCSSCHA63TTemperature = 1249,

        /* EPS Parameters */
        EPSSwitch3V1 = 3000,
        EPSSwitch3V2 = 3001,
        EPSSwitch3V3 = 3002,
        EPSSwitch3V4 = 3003,
        EPSSwitch5V1 = 3004,
        EPSSwitch5V2 = 3005,
        EPSSwitch5V3 = 3006,
        EPSSwitch5V4 = 3007,
        EPSSwitch1 = 3008,
        EPSSwitch2 = 3009,
        EPSSwitch3 = 3010,
        EPSSwitch4 = 3011,
        EPSSwitch5 = 3012,
        EPSCurrentForRail3V1 = 3013,
        EPSCurrentForRail3V2 = 3014,
        EPSCurrentForRail3V3 = 3015,
        EPSCurrentForRail3V4 = 3016,
        EPSCurrentForRail5V1 = 3017,
        EPSCurrentForRail5V2 = 3018,
        EPSCurrentForRail5V3 = 3019,
        EPSCurrentForRail5V4 = 3020,
        EPSCurrentForRail1 = 3021,
        EPSCurrentForRail2 = 3022,
        EPSCurrentForRail3 = 3023,
        EPSCurrentForRail4 = 3024,
        EPSCurrentForRail5 = 3025,
        EPSVoltageForEachRail3V1 = 3026,
        EPSVoltageForEachRail3V2 = 3027,
        EPSVoltageForEachRail3V3 = 3028,
        EPSVoltageForEachRail3V4 = 3029,
        EPSVoltageForEachRail5V1 = 3030,
        EPSVoltageForEachRail5V2 = 3031,
        EPSVoltageForEachRail5V3 = 3032,
        EPSVoltageForEachRail5V4 = 3033,
        EPSVoltageForEachRail12V1 = 3034,
        EPSVoltageForEachRail12V2 = 3035,
        EPSVoltageForEachRail12V3 = 3036,
        EPSVoltageForEachRail12V4 = 3037,
        EPSPanelVoltageXPlus = 3038,
        EPSPanelVoltageXMinus = 3039,
        EPSPanelVoltageYPlus = 3040,
        EPSPanelVoltageYMinus = 3041,
        EPSPanelVoltageZ = 3042,
        EPSPanelCurrentXPlus = 3043,
        EPSPanelCurrentXMinus = 3044,
        EPSPanelCurrentYPlus = 3045,
        EPSPanelCurrentYMinus = 3046,
        EPSPanelCurrentZ = 3047,
        EPSBatteryPackVoltage = 3048,
        EPSPanelTemperatureXPlus = 3049,
        EPSPanelTemperatureXMinus = 3050,
        EPSPanelTemperatureYPlus = 3051,
        EPSPanelTemperatureYMinus = 3052,
        EPSPanelTemperatureZ = 3053,
        EPSPCBTemperatureSensor1 = 3054,
        EPSPCBTemperatureSensor2 = 3055,
        EPSBatteryTemperatureSensor1 = 3056,
        EPSBatteryTemperatureSensor2 = 3057,

        /* COMMS Parameters */
        COMMSUHFBandPATemperature = 2000,
        COMMSSBandPATemperature = 2001,
        COMMSPCBTemperature = 2002,
        COMMSAntennaDeploymentStatus = 2003,
        COMMSDataRateUHFTX = 2004,
        COMMSDataRateUHFRX = 2005,
        COMMSSymbolRateSBand = 2006,
        COMMSCWInterval = 2007,
        COMMSGMSKBeaconInterval = 2008,
        COMMSUHFBandTXPower = 2009,
        COMMSSBandTXPower = 2010,
        COMMSChannelNumberUHFBand = 2011,
        COMMSChannelNumberSBand = 2012,
        COMMSLNAGain = 2013,
        COMMSPAGainUHFBand = 2014,
        COMMSPAGainSBand = 2015,
        COMMSVGAGain = 2016,
        COMMSRSSI = 2017,
        COMMSUHFBandTXOnOff = 2018,
        COMMSUHFBandRXOnOff = 2019,
        COMMSSBandTXOnOff = 2020,
        COMMSPacketsRejectedCOMMS = 2021,
        COMMSInvalidHMAC = 2022,
        COMMSInvalidPacketStructure = 2023,
        COMMSInvalidSpacecraftID = 2024,
        COMMSFrameSequenceCounter = 2025,
        COMMSPCBTemperature1 = 2026,
        COMMSPCBTemperature2 = 2027,
        COMMSMCUTemperature = 2028,
        COMMSMCUInputVoltage = 2029,
        COMMSMCUBootCounter = 2030,
        COMMSOnBoardTime = 2031,
        COMMSNANDCurrentlyUsedMemoryPartition = 2032,
        COMMSLastFailedEvent = 2033,
        COMMSMCUSystick = 2034,
        COMMSFlashInt = 2035,
        COMMSSRAMInt = 2036,
        COMMSUseUart = 2037,
        COMMSUseCAN = 2038,
    };

    /******************* OBDH ENUMS *******************/
    enum SpacecraftTimeRef : uint8_t {
        Spacecraft = 0,
        GroundStation = 1
    };

    enum OperationalMode : uint8_t {
        CommissioningMode = 0,
        NominalMode = 1,
        ScienceMode = 2,
        SafeMode = 3
    };

    enum MemoryPartition : uint8_t {
        First = 0,
        Second = 1
    };

    enum CANBUSActive : uint8_t {
        Main = 0,
        Reductant = 1
    };

    enum MCUFDIR : uint8_t {
        OBC = 0,
        ADCS = 1
    };

    /******************* ADCS ENUMS *******************/
    enum ADCSModeOfOperation : uint8_t {
        StandByModeADCS = 0,
        DetumbingModeADCS = 1,
        NominalModeADCS = 2
    };

    enum AxisAssignment : uint8_t {
        Xaxis = 0,
        Yaxis = 1,
        Zaxis = 2
    };

    enum MangetorquerDutyCycle : uint8_t {
        VeryHigh = 0,
        High = 1,
        Medium = 2,
        Low = 3
    };

    enum MagnetorquerModes : uint8_t {
        Idle = 0,
        SelfTest = 1,
        Detumbling =2
    };

    enum MagnetorquerActuationMethod : uint8_t {
        Current = 0,
        Dipole = 1,
        PWM =2
    };

    /******************* SU ENUMS *******************/
    enum CroppingResolution : uint64_t {
        Default = 0
    };

    enum CompressionRate : uint32_t {
        None = 0
    };

    enum ExprerimentNumber : uint8_t {
        FirstExperiment = 0,
        SecondExperiment = 1,
        ThirdExperiment = 2
    };

    /******************* COMMS ENUMS *******************/
    enum AntennaDeploymentStatus : uint8_t {
        Closed = 0,
        OneDoorOpen = 1,
        TwoDoorOpen = 2,
        ThreeDoorOpen = 3,
        FullyDeployed = 4
    };

    enum SampleRateUHFTX : uint8_t {
        Rate = 0
    };

    enum AntennaGains : uint8_t {
        Gain = 0
    };

    /******************* OBDH PARAMETERS *******************/
    inline Parameter<bool> obcUseRTT(true);
    inline Parameter<bool> obcUseUART(true);
    inline Parameter<bool> obcUseCAN(false);

    inline Parameter<float> obcPCBTemperature1(0);
    inline Parameter<float> obcPCBTemperature2(0);
    inline Parameter<float> obcMCUTemperature(0);
    inline Parameter<float> obcMCUInputVoltage(0);

    inline Parameter<uint16_t> obcMCUBootCounter(0);
    inline Parameter<uint32_t> obcFlashInt(0);
    inline Parameter<uint32_t> obcSRAMInt(0);
    inline Parameter<uint32_t> obcAvailableMRAM(0);
    inline Parameter<uint32_t> obcAvailableNAND(0);

    inline Parameter<Time::DefaultCUC> obcOnBoardTime(Time::DefaultCUC(0));

    inline Parameter<SpacecraftTimeRef> obcSpacecraftTimeRef(Spacecraft); // enum
    inline Parameter<OperationalMode> obcOperationalMode(NominalMode); // enum
    inline Parameter<MemoryPartition> obcMemoryPartition(First); // enum

    inline Parameter<uint32_t> obcReconfigurationTimer(0);
    inline Parameter<uint16_t> obcLastFailedEvent(0);
    inline Parameter<uint32_t> obcMCUSystick(0);

    inline Parameter<float> obcCANBUSLoad1(0);
    inline Parameter<float> obcCANBUSLoad2(0);

    inline Parameter<CAN::Driver::ActiveBus> obcCANBUSActive(CAN::Driver::Main); // enum
    inline Parameter<MCUFDIR> obcMCUFDIR(OBC); // enum

    inline Parameter<uint8_t> obcMCURestartSafeModeThreshold(0);
    inline Parameter<float> obcNANDFLASHLCLThreshold(0);
    inline Parameter<float> obcMRAMLCLThreshold(0);
    inline Parameter<float> obcNANDFLASHON(0);
    inline Parameter<float> obcNANDFlashScrubbingFrequency(0);
    inline Parameter<float> obcMRAMON(0);
    inline Parameter<float> obcRAMScrubbingFrequency(0);
    inline Parameter<float> obcProgramFlashScrubbingFrequency(0);

    /******************* SU PARAMETERS *******************/
    inline Parameter<bool> suUseRTT(true);
    inline Parameter<bool> suUseUART(true);
    inline Parameter<bool> suUseCAN(false);

    // MCU
    inline Parameter<uint32_t> suMCUBootCounter(0);
    inline Parameter<uint32_t> suMCUSysTick(0);

    inline Parameter<float> suMCUTemperature(0);
    inline Parameter<float> suMCUInputVoltage(0);

    // Memory
    inline Parameter<uint32_t> suFlashIntUsedPercentage(0);
    inline Parameter<uint32_t> suFlashUsedPercentage(0);
    inline Parameter<uint32_t> suSRAMUsedPercentage(0);

    inline Parameter<MemoryPartition> suNANDCurrentlyUsedPartition(First); // enum
    inline Parameter<float> suNANDFlashLclOn(0);
    inline Parameter<float> suNANDFlashLclThreshold(0);

    inline Parameter<float> suNANDFlashScrubbingFrequency(0);
    inline Parameter<float> suRAMScrubbingFrequency(0);
    inline Parameter<float> suProgramFlashScrubbingFrequency(0);

    // Components
    inline Parameter<float> suControlPumpFlashLclOn(0);
    inline Parameter<float> suControlPumpFlashLclThreshold(0);

    inline Parameter<float> suFlowPumpFlashOn(0);
    inline Parameter<float> suFlowPumpFlashLclThreshold(0);

    inline Parameter<float> suCameraOn(0);
    inline Parameter<float> suCameraLCLThreshold(0);

    // Temperatures
    inline Parameter<float> suPCBTemperature1(0);
    inline Parameter<float> suPCBTemperature2(0);

    inline Parameter<float> suPCBTemperatureControlValves(0);
    inline Parameter<float> suPCBTemperatureFlowValves(0);

    inline Parameter<float> suCameraTemperature(0);
    inline Parameter<float> suPDMSTemperature(0);
    inline Parameter<float> suGrowthMediumTemperature(0);

    // Sensors
    inline Parameter<float> suPressureLevelSensor1(0);
    inline Parameter<float> suPressureLevelSensor2(0);

    inline Parameter<float> suHumidityLevelSensor1(0);
    inline Parameter<float> suHumidityLevelSensor2(0);

    inline Parameter<double> suRadfet(0);

    // Components
    inline Parameter<float> suPumpFlowRate1(0);
    inline Parameter<float> suPumpFlowRate2(0);

    inline Parameter<bool> suControlChamberValve1(0);
    inline Parameter<bool> suControlChamberValve2(0);
    inline Parameter<bool> suControlChamberValve3(0);

    inline Parameter<bool> suControlSandwitchValve1(0);
    inline Parameter<bool> suControlSandwitchValve2(0);
    inline Parameter<bool> suControlSandwitchValve3(0);

    inline Parameter<bool> suFlowValveInlet1(0);
    inline Parameter<bool> suFlowValveInlet2(0);
    inline Parameter<bool> suFlowValveInlet3(0);

    inline Parameter<bool> suFlowValveOutlet1(0);
    inline Parameter<bool> suFlowValveOutlet2(0);
    inline Parameter<bool> suFlowValveOutlet3(0);

    inline Parameter<bool> suFlowValveTest1(0);
    inline Parameter<bool> suFlowValveTest2(0);

    inline Parameter<float> suLEDIntensity1(0);
    inline Parameter<float> suLEDIntensity2(0);
    inline Parameter<float> suLEDIntensity3(0);
    inline Parameter<float> suLEDIntensity4(0);

    // Photos
    inline Parameter<uint16_t> suNumberOfPhotosTaken(0);
    inline Parameter<float> suPhotoExposureTime(0);
    inline Parameter<uint16_t> suPhotoInterval(0);
    inline Parameter<CroppingResolution> suPhotoCroppedResolution(Default); // enum
    inline Parameter<CompressionRate> suPhotoCompressionRate(None); // enum

    // Misc
    inline Parameter<ExprerimentNumber> suExperimentNumber(FirstExperiment); // enum
    inline Parameter<Time::DefaultCUC> suOnBoardTime(Time::DefaultCUC(0));
    inline Parameter<uint16_t> suLastFailedEvent(0);

    inline Parameter<float> suTemperatureFromHumiditySensor1(0);
    inline Parameter<float> suTemperatureFromHumiditySensor2(0);

    /******************* ADCS PARAMETERS *******************/
    inline Parameter<bool> adcsUseRTT(false);
    inline Parameter<bool> adcsUseUART(true);
    inline Parameter<bool> adcsUseCAN(false);

    inline Parameter<float> adcsXBodyFrameRegardingOrbitFrame(0);
    inline Parameter<float> adcsYBodyFrameRegardingOrbitFrame(0);
    inline Parameter<float> adcsZBodyFrameRegardingOrbitFrame(0);
    inline Parameter<float> adcsAngularVelocityOnX(0);
    inline Parameter<float> adcsAngularVelocityOnY(0);
    inline Parameter<float> adcsAngularVelocityOnZ(0);
    inline Parameter<float> adcsBDotXAxis(0);
    inline Parameter<float> adcsBDotYAxis(0);
    inline Parameter<float> adcsBDotZAxis(0);
    inline Parameter<float> adcsQuaternionOrbitFrameToBodyScalar(0);
    inline Parameter<float> adcsQuaternionOrbitFrameToBodyVectorElem1(0);
    inline Parameter<float> adcsQuaternionOrbitFrameToBodyVectorElem2(0);
    inline Parameter<float> adcsQuaternionOrbitFrameToBodyVectorElem3(0);

    inline Parameter<double> adcsMagnetometerRawX(0);
    inline Parameter<double> adcsMagnetometerRawY(0);
    inline Parameter<double> adcsMagnetometerRawZ(0);
    inline Parameter<uint8_t> adcsMagnetometerFrequency(0);
    inline Parameter<int8_t> adcsMagnetometerCycleCountX(0);
    inline Parameter<int8_t> adcsMagnetometerCycleCountY(0);
    inline Parameter<int8_t> adcsMagnetometerCycleCountZ(0);
    inline Parameter<bool> adcsMagnetometerMeasurementUnsuccessful(0);
    inline Parameter<bool> adcsMagnetometerSPIInvalidArg(0);
    inline Parameter<bool> adcsMagnetometerXAxisValueCorrect(0);
    inline Parameter<bool> adcsMagnetometerYAxisValueCorrect(0);
    inline Parameter<bool> adcsMagnetometerZAxisValueCorrect(0);
    inline Parameter<bool> adcsMagnetometerXAxisOK(0);
    inline Parameter<bool> adcsMagnetometerYAxisOK(0);
    inline Parameter<bool> adcsMagnetometerZAxisOK(0);
    inline Parameter<bool> adcsMagnetometerSPICommunicationSuccessful(0);
    inline Parameter<float> adcsMagnetometerDeltaCheck(0);

    inline Parameter<int32_t> adcsMagnetometerISISCalibratedXAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISCalibratedYAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISCalibratedZAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISFilteredXAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISFilteredYAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISFilteredZAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISRawXAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISRawYAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISRawZAxis(0);

    inline Parameter<float> adcsGyroscopeRateX(0);
    inline Parameter<float> adcsGyroscopeRateY(0);
    inline Parameter<float> adcsGyroscopeRateZ(0);

    inline Parameter<uint16_t> adcsGyroXLOCSTRegister(0);
    inline Parameter<uint16_t> adcsGyroXHICSTRegister(0);
    inline Parameter<uint16_t> adcsGyroXQUADRegister(0);
    inline Parameter<bool> adcsGyroXFAULTRegisterFailBit(0);
    inline Parameter<bool> adcsGyroXFAULTRegisterOVBit(0);
    inline Parameter<bool> adcsGyroXFAULTRegisterUVBit(0);
    inline Parameter<bool> adcsGyroXFAULTRegisterPPLBit(0);
    inline Parameter<bool> adcsGyroXFAULTRegisterQbit(0);
    inline Parameter<bool> adcsGyroXFAULTRegisterNVMBit(0);
    inline Parameter<bool> adcsGyroXFAULTRegisterPORBit(0);
    inline Parameter<bool> adcsGyroXFAULTRegisterCSTBit(0);
    inline Parameter<bool> adcsGyroXSPIInvalidArg(0);
    inline Parameter<bool> adcsGyroXMeasurementUnsuccessful(0);
    inline Parameter<bool> adcsGyroXValueCorrect(0);
    inline Parameter<bool> adcsGyroXSPICommunicationSuccesful(0);

    inline Parameter<uint16_t> adcsGyroYLOCSTRegister(0);
    inline Parameter<uint16_t> adcsGyroYHICSTRegister(0);
    inline Parameter<uint16_t> adcsGyroYQUADRegister(0);
    inline Parameter<bool> adcsGyroYFAULTRegisterFailBit(0);
    inline Parameter<bool> adcsGyroYFAULTRegisterOVBit(0);
    inline Parameter<bool> adcsGyroYFAULTRegisterUVBit(0);
    inline Parameter<bool> adcsGyroYFAULTRegisterPPLBit(0);
    inline Parameter<bool> adcsGyroYFAULTRegisterQbit(0);
    inline Parameter<bool> adcsGyroYFAULTRegisterNVMBit(0);
    inline Parameter<bool> adcsGyroYFAULTRegisterPORBit(0);
    inline Parameter<bool> adcsGyroYFAULTRegisterCSTBit(0);
    inline Parameter<bool> adcsGyroYSPIInvalidArg(0);
    inline Parameter<bool> adcsGyroYMeasurementUnsuccessful(0);
    inline Parameter<bool> adcsGyroYValueCorrect(0);
    inline Parameter<bool> adcsGyroYSPICommunicationSuccesful(0);

    inline Parameter<uint16_t> adcsGyroZLOCSTRegister(0);
    inline Parameter<uint16_t> adcsGyroZHICSTRegister(0);
    inline Parameter<uint16_t> adcsGyroZQUADRegister(0);
    inline Parameter<bool> adcsGyroZFAULTRegisterFailBit(0);
    inline Parameter<bool> adcsGyroZFAULTRegisterOVBit(0);
    inline Parameter<bool> adcsGyroZFAULTRegisterUVBit(0);
    inline Parameter<bool> adcsGyroZFAULTRegisterPPLBit(0);
    inline Parameter<bool> adcsGyroZFAULTRegisterQbit(0);
    inline Parameter<bool> adcsGyroZFAULTRegisterNVMBit(0);
    inline Parameter<bool> adcsGyroZFAULTRegisterPORBit(0);
    inline Parameter<bool> adcsGyroZFAULTRegisterCSTBit(0);
    inline Parameter<bool> adcsGyroZSPIInvalidArg(0);
    inline Parameter<bool> adcsGyroZMeasurementUnsuccessful(0);
    inline Parameter<bool> adcsGyroZValueCorrect(0);
    inline Parameter<bool> adcsGyroZSPICommunicationSuccesful(0);

    inline Parameter<bool> adcsMagnetorquerOnXAxisHBridgePolarity(0);
    inline Parameter<bool> adcsMagnetorquerOnYAxisHBridgePolarity(0);
    inline Parameter<bool> adcsMagnetorquerOnZAxisHBridgePolarity(0);
    inline Parameter<bool> adcsMagnetorquerOnXAxisOnOff(0);
    inline Parameter<bool> adcsMagnetorquerOnYAxisOnOff(0);
    inline Parameter<bool> adcsMagnetorquerOnZAxisOnOff(0);

    inline Parameter<float> adcsGainBdotXAxis(0);
    inline Parameter<float> adcsGainBdotYAxis(0);
    inline Parameter<float> adcsGainBdotZAxis(0);
    inline Parameter<float> adcsGainProportionalPDXAxis(0);
    inline Parameter<float> adcsGainProportionalPDYAxis(0);
    inline Parameter<float> adcsGainProportionalPDZAxis(0);
    inline Parameter<float> adcsGainDerivativePDXAxis(0);
    inline Parameter<float> adcsGainDerivativePDYAxis(0);
    inline Parameter<float> adcsGainDerivativePDZAxis(0);

    inline Parameter<int16_t> adcsiMQTBoardCoilXTemperatureSensor(0);
    inline Parameter<int16_t> adcsiMQTBoardCoilYTemperatureSensor(0);
    inline Parameter<int16_t> adcsiMQTBoardCoilZTemperatureSensor(0);
    inline Parameter<int16_t> adcsiMQTBoardExtraTemperatureSensor(0);

    inline Parameter<float> adcsGyroscopeXTemperature(0);
    inline Parameter<float> adcsGyroscopeYTemperature(0);
    inline Parameter<float> adcsGyroscopeZTemperature(0);

    inline Parameter<float> adcsBoardTemperature1(0);
    inline Parameter<float> adcsBoardTemperature2(0);

    inline Parameter<ADCSModeOfOperation> adcsMode(StandByModeADCS); // enum
    inline Parameter<uint8_t> adcsTLE(0);

    inline Parameter<float> adcsInitialKalmanQuaternionECIFrameToBodyScalar(0);
    inline Parameter<float> adcsInitialKalmanQuaternionECIFrameToBodyVectorElem1(0);
    inline Parameter<float> adcsInitialKalmanQuaternionECIFrameToBodyVectorElem2(0);
    inline Parameter<float> adcsInitialKalmanQuaternionECIFrameToBodyVectorElem3(0);
    inline Parameter<float> adcsTLEOrbitalElementsArgumentPerigee(0);
    inline Parameter<float> adcsTLEOrbitalElementsRAAN(0);
    inline Parameter<float> adcsTLEOrbitalElementsInclination(0);
    inline Parameter<float> adcsTLEOrbitalElementsEccentricity(0);
    inline Parameter<float> adcsTLEOrbitalElementsMeanAnomaly(0);

    inline Parameter<double> adcsSGP4OrbitalElementsArgumentPerigee(0);
    inline Parameter<double> adcsSGP4OrbitalElementsRAAN(0);
    inline Parameter<double> adcsSGP4OrbitalElementsInclination(0);
    inline Parameter<double> adcsSGP4OrbitalElementsMeanAnomaly(0);
    inline Parameter<double> adcsTimeGST_JD(0);  ///< Greenwich Sidereal Time

    inline Parameter<float> adcsDeterminationInitialMatricesQ1(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ2(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ3(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ4(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ5(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ6(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ7(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ8(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ9(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ10(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR1(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR2(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR3(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR4(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR5(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR6(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR7(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR8(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR9(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial1(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial2(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial3(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial4(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial5(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial6(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial7(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial8(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial9(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial10(0);

    inline Parameter<bool> adcsReactionWheelOn(0);
    inline Parameter<float> adcsReactionWheelAngularVelocity(0);
    inline Parameter<bool> adcsControlProfileSwitching(0);

    inline Parameter<AxisAssignment> adcsMagnetorquerAxisAssignment(Xaxis); // enum
    inline Parameter<AxisAssignment> adcsTransformationAxisXAssignment(Xaxis); // enum
    inline Parameter<AxisAssignment> adcsTransformationAxisYAssignment(Xaxis); // enum
    inline Parameter<AxisAssignment> adcsTransformationAxisZAssignment(Xaxis); // enum
    inline Parameter<AxisAssignment> adcsMagnetometerAxisAssignment(Xaxis); // enum
    inline Parameter<AxisAssignment> adcsSunSensorAxisAssignment(Xaxis); // enum
    inline Parameter<AxisAssignment> adcsGyroscopeAxisAssignment(Xaxis); // enum

    inline Parameter<bool> adcsFrameAxisSignECIToECEF_X(0);
    inline Parameter<bool> adcsFrameAxisSignECIToECEF_Y(0);
    inline Parameter<bool> adcsFrameAxisSignECIToECEF_Z(0);
    inline Parameter<bool> adcsFrameAxisSignECIToOrbit_X(0);
    inline Parameter<bool> adcsFrameAxisSignECIToOrbit_Y(0);
    inline Parameter<bool> adcsFrameAxisSignECIToOrbit_Z(0);
    inline Parameter<bool> adcsFrameAxisSignNEDToECEF_X(0);
    inline Parameter<bool> adcsFrameAxisSignNEDToECEF_Y(0);
    inline Parameter<bool> adcsFrameAxisSignNEDToECEF_Z(0);

    inline Parameter<bool> adcsMagnetometerSignX(0);
    inline Parameter<bool> adcsMagnetometerSignY(0);
    inline Parameter<bool> adcsMagnetometerSignZ(0);

    inline Parameter<bool> adcsSunSensorXAxisSign(0);
    inline Parameter<bool> adcsSunSensorYAxisSign(0);
    inline Parameter<bool> adcsSunSensorZAxisSign(0);

    inline Parameter<bool> adcsGyroSignX(0);
    inline Parameter<bool> adcsGyroSignY(0);
    inline Parameter<bool> adcsGyroSignZ(0);

    inline Parameter<float> adcsDetumblingNominalThresholds(0);
    inline Parameter<float> adcsNominalDetumblingThresholds(0);
    inline Parameter<float> adcsStandByDetumblingThresholds(0);
    inline Parameter<float> adcsDetumblingStandByThresholds(0);
    inline Parameter<float> adcsNominalStandByThresholds(0);
    inline Parameter<float> adcsMagnetorquerMagneticDipoleX(0);
    inline Parameter<float> adcsMagnetorquerMagneticDipoleY(0);
    inline Parameter<float> adcsMagnetorquerMagneticDipoleZ(0);

    inline Parameter<MangetorquerDutyCycle> adcsMagnetorquerDutyCycle(Low); // enum

    inline Parameter<int16_t> adcsMagnetorquerCurrentX(0);
    inline Parameter<int16_t> adcsMagnetorquerCurrentY(0);
    inline Parameter<int16_t> adcsMagnetorquerCurrentZ(0);

    inline Parameter<float> adcsMagnetorquerBDot(0);
    inline Parameter<float> adcsReactionWheelTorque(0);
    inline Parameter<float> adcsReactionWheelAngularAcceleration(0);
    inline Parameter<float> adcsDesiredQuaternionOrbitToBodyScalar(0);
    inline Parameter<float> adcsDesiredQuaternionVectorElem1(0);
    inline Parameter<float> adcsDesiredQuaternionVectorElem2(0);
    inline Parameter<float> adcsDesiredQuaternionVectorElem3(0);
    inline Parameter<float> adcsAngularVelocityDesiredX(0);
    inline Parameter<float> adcsAngularVelocityDesiredY(0);
    inline Parameter<float> adcsAngularVelocityDesiredZ(0);

    inline Parameter<float> adcsGyroBiasX(0);
    inline Parameter<float> adcsGyroBiasY(0);
    inline Parameter<float> adcsGyroBiasZ(0);

    inline Parameter<double> adcsSatellitePositionLatitude(0);
    inline Parameter<double> adcsSatellitePositionLongitude(0);
    inline Parameter<double> adcsSatellitePositionHeight(0);

    inline Parameter<float> adcsSatellitePositionECIX(0);
    inline Parameter<float> adcsSatellitePositionECIY(0);
    inline Parameter<float> adcsSatellitePositionECIZ(0);
    inline Parameter<float> adcsSatelliteLinearVelocityECIX(0);
    inline Parameter<float> adcsSatelliteLinearVelocityECIY(0);
    inline Parameter<float> adcsSatelliteLinearVelocityECIZ(0);
    inline Parameter<float> adcsMagneticFieldNEDX(0);
    inline Parameter<float> adcsMagneticFieldNEDY(0);
    inline Parameter<float> adcsMagneticFieldNEDZ(0);
    inline Parameter<float> adcsMagneticFieldECIX(0);
    inline Parameter<float> adcsMagneticFieldECIY(0);
    inline Parameter<float> adcsMagneticFieldECIZ(0);
    inline Parameter<float> adcsSunPositionECIX(0);
    inline Parameter<float> adcsSunPositionECIY(0);
    inline Parameter<float> adcsSunPositionECIZ(0);

    inline Parameter<bool> adcsEclipse(0);

    inline Parameter<float> adcsDesaturationPecentageParameter(0);

    inline Parameter<MagnetorquerModes> adcsISISMTQModes(Idle); // enum
    inline Parameter<MagnetorquerActuationMethod> adcsMagnetorquerActuationMethod(Current); // enum

    inline Parameter<float> adcsDutyCycleDetermination(0);
    inline Parameter<float> adcsDutyCycleControl(0);
    inline Parameter<float> adcsDutyCycleOther(0);
    inline Parameter<float> adcsDesatInitialAngularVelocityParam(0);

    inline Parameter<float> adcsMCUTemperature(0);
    inline Parameter<float> adcsMCUInputVoltage(0);
    inline Parameter<uint16_t> adcsBootCounter(0);
    inline NotifyParameter<Time::DefaultCUC> adcsOnBoardTime(Time::DefaultCUC(0));
    inline Parameter<MemoryPartition> adcsMemoryPartition(First); // enum
    inline Parameter<int32_t> adcsLastFailedEvent(0);
    inline Parameter<uint64_t> adcsSystick(0);
    inline Parameter<float> adcsRAMScrubbingFrequency(0);
    inline Parameter<float> adcsProgramFlashScrubbingFrequency(0);
    inline Parameter<uint32_t> adcsSRAMInt(0);
    inline Parameter<uint32_t> adcsFlashInt(0);

    inline Parameter<float> adcsSCHA63TRateX(0);
    inline Parameter<float> adcsSCHA63TRateY(0);
    inline Parameter<float> adcsSCHA63TRateZ(0);
    inline Parameter<float> adcsSCHA63TTemperature(0);

    /****************** EPS PARAMETERS *******************/
    inline Parameter<bool> epsSwitch3V1(0);
    inline Parameter<bool> epsSwitch3V2(0);
    inline Parameter<bool> epsSwitch3V3(0);
    inline Parameter<bool> epsSwitch3V4(0);
    inline Parameter<bool> epsSwitch5V1(0);
    inline Parameter<bool> epsSwitch5V2(0);
    inline Parameter<bool> epsSwitch5V3(0);
    inline Parameter<bool> epsSwitch5V4(0);
    inline Parameter<bool> epsSwitch1(0);
    inline Parameter<bool> epsSwitch2(0);
    inline Parameter<bool> epsSwitch3(0);
    inline Parameter<bool> epsSwitch4(0);
    inline Parameter<bool> epsSwitch5(0);

    inline Parameter<float> epsCurrentForRail3V1(0);
    inline Parameter<float> epsCurrentForRail3V2(0);
    inline Parameter<float> epsCurrentForRail3V3(0);
    inline Parameter<float> epsCurrentForRail3V4(0);
    inline Parameter<float> epsCurrentForRail5V1(0);
    inline Parameter<float> epsCurrentForRail5V2(0);
    inline Parameter<float> epsCurrentForRail5V3(0);
    inline Parameter<float> epsCurrentForRail5V4(0);
    inline Parameter<float> epsCurrentForRail1(0);
    inline Parameter<float> epsCurrentForRail2(0);
    inline Parameter<float> epsCurrentForRail3(0);
    inline Parameter<float> epsCurrentForRail4(0);
    inline Parameter<float> epsCurrentForRail5(0);

    inline Parameter<float> epsVoltageForEachRail3V1(0);
    inline Parameter<float> epsVoltageForEachRail3V2(0);
    inline Parameter<float> epsVoltageForEachRail3V3(0);
    inline Parameter<float> epsVoltageForEachRail3V4(0);
    inline Parameter<float> epsVoltageForEachRail5V1(0);
    inline Parameter<float> epsVoltageForEachRail5V2(0);
    inline Parameter<float> epsVoltageForEachRail5V3(0);
    inline Parameter<float> epsVoltageForEachRail5V4(0);
    inline Parameter<float> epsVoltageForEachRail12V1(0);
    inline Parameter<float> epsVoltageForEachRail12V2(0);
    inline Parameter<float> epsVoltageForEachRail12V3(0);
    inline Parameter<float> epsVoltageForEachRail12V4(0);

    inline Parameter<float> epsPanelVoltageXPlus(0);
    inline Parameter<float> epsPanelVoltageXMinus(0);
    inline Parameter<float> epsPanelVoltageYPlus(0);
    inline Parameter<float> epsPanelVoltageYMinus(0);
    inline Parameter<float> epsPanelVoltageZ(0);

    inline Parameter<float> epsPanelCurrentXPlus(0);
    inline Parameter<float> epsPanelCurrentXMinus(0);
    inline Parameter<float> epsPanelCurrentYPlus(0);
    inline Parameter<float> epsPanelCurrentYMinus(0);
    inline Parameter<float> epsPanelCurrentZ(0);

    inline Parameter<float> epsBatteryPackVoltage(0);

    inline Parameter<float> epsPanelTemperatureXPlus(0);
    inline Parameter<float> epsPanelTemperatureXMinus(0);
    inline Parameter<float> epsPanelTemperatureYPlus(0);
    inline Parameter<float> epsPanelTemperatureYMinus(0);
    inline Parameter<float> epsPanelTemperatureZ(0);

    inline Parameter<float> epsPCBTemperatureSensor1(0);
    inline Parameter<float> epsPCBTemperatureSensor2(0);

    inline Parameter<float> epsBatteryTemperatureSensor1(0);
    inline Parameter<float> epsBatteryTemperatureSensor2(0);

    /****************** COMMS PARAMETERS *******************/
    inline Parameter<uint16_t> commsUHFBandPATemperature(0);
    inline Parameter<uint16_t> commsSBandPATemperature(0);
    inline Parameter<uint16_t> commsPCBTemperature(0);

    inline Parameter<AntennaDeploymentStatus> commsAntennaDeploymentStatus(Closed); // enum

    inline Parameter<SampleRateUHFTX> commsDataRateUHFTX(Rate); // enum
    inline Parameter<uint32_t> commsDataRateUHFRX(0);
    inline Parameter<uint32_t> commsSymbolRateSBand(0);
    inline Parameter<uint16_t> commsCWInterval(0);
    inline Parameter<uint16_t> commsGMSKBeaconInterval(0);
    inline Parameter<uint32_t> commsUHFBandTXPower(0);
    inline Parameter<uint32_t> commsSBandTXPower(0);
    inline Parameter<uint32_t> commsChannelNumberUHFBand(0);
    inline Parameter<uint32_t> commsChannelNumberSBand(0);

    inline Parameter<AntennaGains> commsLNAGain(Gain); // enum
    inline Parameter<AntennaGains> commsPAGainUHFBand(Gain); // enum
    inline Parameter<AntennaGains> commsPAGainSBand(Gain); // enum

    inline Parameter<uint8_t> commsVGAGain(0);
    inline Parameter<float> commsRSSI(0);

    inline Parameter<bool> commsUHFBandTXOnOff(0);
    inline Parameter<bool> commsUHFBandRXOnOff(0);
    inline Parameter<bool> commsSBandTXOnOff(0);

    inline Parameter<uint16_t> commsPacketsRejectedCOMMS(0);
    inline Parameter<uint16_t> commsInvalidHMAC(0);
    inline Parameter<uint16_t> commsInvalidPacketStructure(0);
    inline Parameter<uint16_t> commsInvalidSpacecraftID(0);
    inline Parameter<uint16_t> commsFrameSequenceCounter(0);

    inline Parameter<float> commsPCBTemperature1(0);
    inline Parameter<float> commsPCBTemperature2(0);
    inline Parameter<float> commsMCUTemperature(0);
    inline Parameter<float> commsMCUInputVoltage(0);

    inline Parameter<uint32_t> commsMCUBootCounter(0);
    inline Parameter<Time::DefaultCUC> commsOnBoardTime(Time::DefaultCUC(0));

    inline Parameter<MemoryPartition> commsNANDCurrentlyUsedMemoryPartition(First); // enum

    inline Parameter<uint16_t> commsLastFailedEvent(0);
    inline Parameter<uint32_t> commsMCUSystick(0);
    inline Parameter<uint32_t> commsFlashInt(0);
    inline Parameter<uint32_t> commsSRAMInt(0);

    inline Parameter<bool> commsUseUART(true);
    inline Parameter<bool> commsUseCAN(true);
}