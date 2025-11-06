#include <Arduino.h>
#include <SuperSoco485.h>

// Example: Serial1 for Super Soco communication
#define SERIAL_INTERFACE Serial1

static void superSocoDataUpdated(void *user_data, SuperSoco485 *sender);

void setup()
{
    // Start Serial for debug output
    Serial.begin(115200);
    while (!Serial.available())
    {
        delay(100);
    }

    // Initialize Serial1 for Super Soco communication
    SERIAL_INTERFACE.begin(9600);

    // Start Super Soco library
    SuperSoco.begin(superSocoDataUpdated, NULL);

    Serial.println("Reading data from Super Soco");
}

void loop()
{
    // Read available data from Serial1
    while (SERIAL_INTERFACE.available())
    {
        uint8_t byteReceived = SERIAL_INTERFACE.read();
        SuperSoco.parseChunk(&byteReceived, 1);
    }
}

static void superSocoDataUpdated(void *user_data, SuperSoco485 *sender)
{
    // Data has been received and Super Soco container has been updated
    // Use all the wanted information here.
    Serial.print(F("State of Charge: "));
    Serial.println(sender->_status.Soc);
}
