#include <Arduino.h>
#include <SuperSoco485.h>

// cycle in milli seconds
#define CHANGE_CYCLE 10000

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
    unsigned long now = millis();
    
    // timestamp when to change wakeup / sleep
    static unsigned long changeTS = now + CHANGE_CYCLE;
    static bool sleeping = false;

    SuperSoco.update();

    if (now >= changeTS)
    {
        // either set the module to standby or wake it up again
        if (sleeping)
        {
            // library is sleeping, wake it up. Data from the vehicle is received
            // again.
            SuperSoco.wakeup();
            sleeping = false;
        }
        else
        {
            // Send library to standby which means that the receiving driver is 
            // deactivated. This will lead to not receiving any data from the
            // vehicle anymore, but the power consumption of the hardware module
            // is reduced
            SuperSoco.standby();
            sleeping = true;
        }
        Serial.print("SuperSoco library is in standby: ");
        Serial.println(sleeping ? "Yes" : "No");

        changeTS = millis() + CHANGE_CYCLE;
    }
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