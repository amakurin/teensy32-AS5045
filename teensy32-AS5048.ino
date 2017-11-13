#include "as5048.h"
#include "i2cbus.h"
#include "spibus.h"
#include "commands.h"
#include <memory>


//SPIBus spibus;
I2CBus i2cbus;
AS5048 as5048(&i2cbus);
byte buffer[64];
std::shared_ptr<BaseCommand> pCommand;


void setup() {
    Serial.begin(115200);
}

void loop() {
    int n = RawHID.recv(buffer, 0); // 0 timeout = do not wait
    if (n > 0) {
        USBCommand cmd_code = BaseCommand::getCommandCode(buffer);
        switch (cmd_code){
            case CMD_START_SENSORS  : pCommand.reset(new StartSensorsCommand(&as5048, buffer)); break;
            case CMD_STOP           : pCommand.reset(new GenericStopCommand (&as5048, buffer)); break;
            case CMD_OTP_ZERO       : pCommand.reset(new OTPZeroCommand		(&as5048, buffer)); break;
            default: 
                Serial.print(F("Unknown command received: "));
                Serial.println(cmd_code);
        }
        pCommand.get()->setup();
    }

    if (pCommand){
        bool continue_exec = pCommand.get()->exec();
        if (!continue_exec) pCommand.reset();
    }

}