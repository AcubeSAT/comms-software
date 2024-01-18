#include "TemperatureSensorsTask.hpp"
#include "TMP117.hpp"

etl::string<TemperatureSensorsTask::maxErrorStringSize> TemperatureSensorsTask::errorString(TMP117::Error error){
    switch(error) {
        case TMP117::NoErrors:
            return "NoErrors";
        case TMP117::Timeout:
            return "Timeout";
        case TMP117::InvalidEEPROM:
            return "InvalidEEPROM";
        case TMP117::TemperatureHigh:
            return "TemperatureHigh";
        case TMP117::TemperatureLow:
            return "TemperatureLow";
        case TMP117::NoDataReady:
            return "NoDataReady";
        case TMP117::InvalidCalibrationOffset:
            return "InvalidCalibrationOffset";
        default:
            break;
    }

    return "UnknownError";
}

etl::string<TemperatureSensorsTask::maxSensorNameSize> TemperatureSensorsTask::sensorName(TMP117::I2CAddress slaveAddress){
    switch(slaveAddress) {
        case TMP117::I2CAddress::Address3:
            return "PCB MCU";
        case TMP117::I2CAddress::Address1:
            return "PCB UHF PA";
        case TMP117::I2CAddress::Address4:
            return "PCB SBAND PA";
        default:
            break;
    }

    return "UNKNOWN ADDRESS";
}


/**
 * Actual execute method
 */
 /*
void TemperatureSensorsTask::execute() {

    etl::array<sensor, 3> sensors = {
        sensor {TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address3, config),
                         PlatformParameters::commsPCBTemperatureMCU,
                         sensorName(TMP117::Address3)},
        sensor {TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address1, config),
                         PlatformParameters::commsPCBTemperatureUHF,
                         sensorName(TMP117::I2CAddress::Address1)},
        sensor {TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address4, config),
                         PlatformParameters::commsPCBTemperatureSBAND,
                         sensorName(TMP117::I2CAddress::Address4)},
    };

    // for temporarily holding of a temperature and error
    etl::pair<TMP117::Error, float> temperature;

    while (true) {
        Logger::format.precision(LoggerPrecision);
        for (auto s : sensors) {
            temperature = s.sensorObject.getTemperature(true);
            if (temperature.first == TMP117::Error::NoErrors) {
                LOG_DEBUG << "Temperature at " << s.sensorName->data() << ": " << temperature.second;
                s.platformParameterReference.setValue(temperature.second);
            } else {
                LOG_ERROR << "Could not get temperature at " << s.sensorName->data() << ". Error: "
                          << errorString(temperature.first).data();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
*/

/** Tests the methods: readRegister(),writeRegister()
 *  Writes a specified value to a specified register.It is expected that the read value is the same as the specified value
**/
/*
void TemperatureSensorsTask::execute() {
    TMP117::TMP117 sensor = TMP117::TMP117(hi2c2,TMP117::I2CAddress::Address1,config);

    uint16_t writeValue = 100;
    TMP117::RegisterAddress targetRegister = TMP117::RegisterAddress::TemperatureHighLimit;

    TMP117::Error writeError;
    etl::pair<TMP117::Error, std::optional<uint16_t>> readPair;
    while (true) {
        Logger::format.precision(LoggerPrecision);

        writeError = sensor.writeRegister(targetRegister,writeValue);
        readPair = sensor.readRegister(targetRegister);
        LOG_DEBUG << "Write Error: " << writeError << "\r\n";
        LOG_DEBUG << "Read Error: " << readPair.first << "\r\n";
        LOG_DEBUG << "Expected: " << writeValue << " got: " << (readPair.second.has_value() ? readPair.second.value() : -9999) << "\r\n";
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
 */

/** Tests the methods: configure()
 * Reads the configuration register (only the 11 least significant bits concern settings) and compares it with
 * the value written when configure was called.
**/
/*
void TemperatureSensorsTask::execute() {
    TMP117::TMP117 sensor = TMP117::TMP117(hi2c2,TMP117::I2CAddress::Address1,config);

    etl::pair<TMP117::Error, std::optional<uint16_t>> readPair;
    while (true){
        Logger::format.precision(LoggerPrecision);

        readPair = sensor.readRegister(TMP117::RegisterAddress::ConfigurationRegister);
        uint16_t configVal =
                (static_cast<uint16_t>(sensor.configuration.conversionMode) << 10) |
                ((sensor.configuration.cycleTime & 0x7) << 7) |
                (static_cast<uint16_t>(sensor.configuration.averaging) << 5) |
                (sensor.configuration.thermalAlert << 4) |
                (sensor.configuration.polarityAlert << 3) |
                (sensor.configuration.drAlert << 2);
        LOG_DEBUG << "Read Error: " << readPair.first << "\r\n";
        LOG_DEBUG << "Expected: " << configVal << " got: " << (readPair.second.has_value() ? (readPair.second.value() & 0x7FF): -9999) << "\r\n";
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
 */

/** Tests the methods: readEEPROM(),writeEEPROM()
 * To test if a value is indeed written to EEPROM (and not just the volatile register),the following steps are performed:
 * 1  Read preexisting values of EEPROM1 and EEPROM2 volatile registers.These are the values that
 *    were stored in the corresponding EEPROM addresses.
 * 2. Write new values to EEPROM1 via writeEEPROMM() (this is the proper way,since this method unlocks the EEPROM),and to
 *    EEPROM2 via writeRegister().Also ensure that after a writeEEPROM() the EEPROM is locked.
 * 3. Issue a software reset.This should clean the registers,so they will be reloaded with whatever value is stored in EEPROM.
 * 4. Read the registers again.It is expected that only EEPROM1 will show the new value.
 *
**/
/*
void TemperatureSensorsTask::execute() {
    TMP117::TMP117 sensor = TMP117::TMP117(hi2c2,TMP117::I2CAddress::Address1,config);
    TMP117::RegisterAddress targetedEEPROM1 = TMP117::RegisterAddress::EEPROM1;
    TMP117::RegisterAddress targetedEEPROM2 = TMP117::RegisterAddress::EEPROM2;

    while (true) {
        Logger::format.precision(LoggerPrecision);
        //First,read the preexisting values.
        auto preRead1 = sensor.readEEPROM(1);
        auto preRead2 = sensor.readEEPROM(2);
        LOG_DEBUG << "pre-read errors: " << preRead1.first << " and " << preRead1.first;
        LOG_DEBUG << "Preexisting values: " << preRead1.second.value() << " and " << preRead1.second.value();

        //Write new values (with writeEEPROM in the first register and writeRegister in the second)
        uint16_t val1 = 89;
        uint16_t val2 = 139;
        auto write1 = sensor.writeEEPROM(1,val1);
        // Ensure that EEPROM is locked
        LOG_DEBUG << "EUN bit is:" << (sensor.readRegister(TMP117::EEPROMUnlock).second.value() & 0x8000);
        auto write2 = sensor.writeRegister(targetedEEPROM2,val2);
        LOG_DEBUG << "write errors: " << write1 << " and " << write2;

        //Issue a soft reset (that should clean the current values of the registers,making them to load whatever they
        // have in the actual EEPROM)
        etl::pair<TMP117::Error,std::optional<uint16_t>> currentVal = sensor.readRegister(TMP117::RegisterAddress::ConfigurationRegister);
        sensor.writeRegister(TMP117::RegisterAddress::ConfigurationRegister, currentVal.second.value() | 0x2);
        sensor.configure();

        //Read the registers again.Only the one that was read written with writeEEPROM should hold it's new value.
        auto afterRead1 = sensor.readEEPROM(1);
        auto afterRead2 = sensor.readEEPROM(2);
        LOG_DEBUG << "after-read errors: " << afterRead1.first << " and " << afterRead1.first;
        LOG_DEBUG << "Final values: " << preRead1.second.value() << " and " << preRead1.second.value();
        LOG_DEBUG << "Expected" << val1 << " and " << val2;

        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
*/

/** Tests the methods: getRevNumber()
 *  Just ensuring that something is returned.
 **/
 /*
void TemperatureSensorsTask::execute() {
    TMP117::TMP117 sensor = TMP117::TMP117(hi2c2,TMP117::I2CAddress::Address1,config);

    etl::pair<TMP117::Error, uint16_t> readPair;
    while (true) {
        Logger::format.precision(LoggerPrecision);
        readPair = sensor.getRevNumber();
        LOG_DEBUG << "Read Error: " << readPair.first << "\r\n";
        LOG_DEBUG <<  "Got: " << ((readPair.first== TMP117::NoErrors) ? readPair.second: -9999) << "\r\n";
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
*/

/** Tests the methods: getDeviceID()
 *  Same as test method for getRevNumber()
 **/
 /*
void TemperatureSensorsTask::execute() {
    TMP117::TMP117 sensor = TMP117::TMP117(hi2c2,TMP117::I2CAddress::Address1,config);

    etl::pair<TMP117::Error, uint16_t> readPair;
    while (true) {
        Logger::format.precision(LoggerPrecision);
        readPair = sensor.getDeviceID();
        LOG_DEBUG << "Read Error: " << readPair.first << "\r\n";
        LOG_DEBUG <<  "Got: " << ((readPair.first== TMP117::NoErrors) ? readPair.second: -9999) << "\r\n";
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
*/

/** Tests the methods: getDeviceID()
 *  Same as test method for getRevNumber()
 **/
 /*
void TemperatureSensorsTask::execute() {
    TMP117::TMP117 sensor = TMP117::TMP117(hi2c2,TMP117::I2CAddress::Address1,config);

    etl::pair<TMP117::Error, uint16_t> readPair;
    while (true) {
        Logger::format.precision(LoggerPrecision);
        readPair = sensor.getDeviceID();
        LOG_DEBUG << "Read Error: " << readPair.first << "\r\n";
        LOG_DEBUG <<  "Got: " << ((readPair.first== TMP117::NoErrors) ? readPair.second: -9999) << "\r\n";
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
*/

/** Tests the methods: setCalibrationOffset(),getCalibrationOffset()
 *  Test different calibration offsets,positive and negative.If |offset|>256, setCalibrationOffset should
 *  return an error.
 **/
/*
void TemperatureSensorsTask::execute() {
    TMP117::TMP117 sensor = TMP117::TMP117(hi2c2,TMP117::I2CAddress::Address1,config);

    float writeValue = 0;
    etl::pair<TMP117::Error, std::optional<float>> CalibPair;
    etl::pair<TMP117::Error, float> readPair;
    while (true) {
        Logger::format.precision(LoggerPrecision);
        CalibPair = sensor.setCalibrationOffset(writeValue);
        readPair = sensor.getCalibrationOffset();
        LOG_DEBUG << "Set Calibration Error: " << CalibPair.first << "\r\n";
        LOG_DEBUG << "Read Calibration Error: " << readPair.first << "\r\n";
        LOG_DEBUG << "Expected: " << writeValue <<" Got: " << ((readPair.first== TMP117::NoErrors) ? readPair.second: -9999) << "\r\n";
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
*/

/** Tests the methods: getTemperature
 *  Extract a temperature with both getTemperature and readRegister. They should give the same results.
 *  Tested in both modes (continuous and one-shot)
 **/
/*
void TemperatureSensorsTask::execute() {

    TMP117::TMP117 sensor = TMP117::TMP117(hi2c2,TMP117::I2CAddress::Address1,config);

    // for temporarily holding a temperature and an error
    etl::pair<TMP117::Error, float> temperature;

    // these are for the manual extraction of the temperature (via readRegister())
    etl::pair<TMP117::Error,float> offset;
    etl::pair<TMP117::Error, std::optional<uint16_t>> temp;

    while (true) {
        Logger::format.precision(LoggerPrecision);
        temperature = sensor.getTemperature(true);
        LOG_DEBUG << "getTemperatureValues:\r\n";
        LOG_DEBUG << "Error: " << temperature.first << " Temperature: " << temperature.second << "\r\n";


        offset = sensor.getCalibrationOffset();
        temp = sensor.readRegister(TMP117::RegisterAddress::TemperatureRegister);
        LOG_DEBUG << "Manually read values:\r\n";
        LOG_DEBUG << "Calibration Offset reading errors: " << offset.first << "\r\n";
        LOG_DEBUG << "Temp reading errors: " << temp.first << "\r\n";
        LOG_DEBUG << "Temp obtained: " << sensor.convertTemperature(temp.second.value()) + offset.second << "\r\n";


        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
*/
