#include <Arduino.h>
#include <SuperSoco485.h>


void setup()
{
    Serial.begin(9600);
    while(!Serial.available())
    {
        delay(100);
    }

    SuperSoco.begin();
    Serial.println("Reading data from Super Soco");
}

void loop()
{
    SuperSoco.update();
}

// global callback function that will be called by the library to signal
// changed data
void superSocoDataUpdated()
{
    // Data has been received and Super Soco container has been updated
    // Use all the wanted information here.
    Serial.print("State of Charge: ");
    Serial.println(SuperSoco._status.Soc);
}