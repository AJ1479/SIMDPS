#include <dht.h> // Include library
#include <SFE_BMP180.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define ALTITUDE 1655.0
#define DHT11_PIN 5 // Defines pin number to which the sensor is connected
#define PIR_PIN 3
#define BUZZER_PIN 9
#define MQ5_PIN 7
#define MQ2_PIN 1
#define MQ7_PIN 2

int RXPin = 6;
int TXPin = 10;

SFE_BMP180 pressure;
dht DHT; // Creates a DHT object
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

long unsigned int lowIn; // the amount of milliseconds the sensor has to be low //before we assume all motion has stopp
long unsigned int pause = 5000;
float sensorValue;
boolean lockLow = true;
boolean takeLowTime;
int mq5_gas;
float mq2_gas;

void buzz(int time_delay)
{
    tone(BUZZER_PIN, 1000);
    delay(time_delay);
    noTone(BUZZER_PIN);
    delay(time_delay);
}

void setup_pir()
{
    pinMode(PIR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    const int calibrationTime = 30;
    digitalWrite(PIR_PIN, LOW);

    Serial.print("calibrating sensor ");
    for (int i = 0; i < calibrationTime; i++)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
}

void setup_bmp180()
{
    Serial.println("BMP180 REBOOT");

    // Initialize the sensor (it is important to get calibration values stored on the device).
    if (pressure.begin())
        Serial.println("BMP180 init success");
    else
    {
        // Oops, something went wrong, this is usually a connection problem,
        // see the comments at the top of this sketch for the proper connections.
        Serial.println("BMP180 init fail\n\n");
        while (1)
            ; // Pause forever.
    }
}

void setup_mq3()
{
    Serial.println("MQ3 warming up!");
    delay(20000); // allow the MQ3 to warm up
}

void setup_mq2()
{
    delay(20000);
}

void setup_gps()
{
    gpsSerial.begin(9600);
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    setup_pir();
    setup_bmp180();
    setup_mq3();
    setup_mq2();
    setup_gps();
}

void loop_dht11()
{
    int readData = DHT.read11(DHT11_PIN);

    float t = DHT.temperature; // Read temperature
    float h = DHT.humidity;    // Read humidity

    Serial.print("Temperature = ");
    Serial.print(DHT.temperature);
    Serial.print("C | ");
    Serial.print((t * 9.0) / 5.0 + 32.0); // Convert celsius to fahrenheit
    Serial.println("F ");
    Serial.print("Humidity = ");
    Serial.print(h);
    Serial.println("% ");
    Serial.println("");

    delay(3000); // wait two seconds
}

void loop_pir()
{
    if (digitalRead(PIR_PIN) == HIGH)
    {

        if (lockLow)
        { // makes sure we wait for a transition to LOW before any further output is made:
            lockLow = false;
            Serial.println("---");
            Serial.print("motion detected at ");
            buzz(500);
            Serial.print(millis() / 1000);
            Serial.println(" sec");
            delay(50);
        }
        takeLowTime = true;
    }
    if (digitalRead(PIR_PIN) == LOW)
    {

        if (takeLowTime)
        {
            lowIn = millis();    // save the time of the transition from high to LOW
            takeLowTime = false; // make sure this is only done at the start of a LOW phase
        }                        // if the sensor is low for more than the given pause, //we assume that no more motion is going to happen
        if (!lockLow && millis() - lowIn > pause)
        { // makes sure this block of code is only executed again after //a new motion sequence has been detected
            lockLow = true;
            Serial.print("motion ended at "); // output
            buzz(500);
            Serial.print((millis() - pause) / 1000);
            Serial.println(" sec");
            delay(50);
        }
    }
}

void loop_bmp180()
{
    char status;
    double T, P, p0, a;

    Serial.println();
    Serial.print("provided altitude: ");
    Serial.print(ALTITUDE, 0);
    Serial.print(" meters, ");
    Serial.print(ALTITUDE * 3.28084, 0);
    Serial.println(" feet");

    status = pressure.startTemperature();
    if (status != 0)
    {
        // Wait for the measurement to complete:
        delay(status);

        status = pressure.getTemperature(T);
        if (status != 0)
        {
            // Print out the measurement:
            Serial.print("temperature: ");
            Serial.print(T, 2);
            Serial.print(" deg C, ");
            Serial.print((9.0 / 5.0) * T + 32.0, 2);
            Serial.println(" deg F");

            status = pressure.startPressure(3);
            if (status != 0)
            {
                // Wait for the measurement to complete:
                delay(status);

                status = pressure.getPressure(P, T);
                if (status != 0)
                {
                    // Print out the measurement:
                    Serial.print("absolute pressure: ");
                    Serial.print(P, 2);
                    Serial.print(" mb, ");
                    Serial.print(P * 0.0295333727, 2);
                    Serial.println(" inHg");

                    p0 = pressure.sealevel(P, ALTITUDE); // we're at 1655 meters (Boulder, CO)
                    Serial.print("relative (sea-level) pressure: ");
                    Serial.print(p0, 2);
                    Serial.print(" mb, ");
                    Serial.print(p0 * 0.0295333727, 2);
                    Serial.println(" inHg");

                    a = pressure.altitude(P, p0);
                    Serial.print("computed altitude: ");
                    Serial.print(a, 0);
                    Serial.print(" meters, ");
                    Serial.print(a * 3.28084, 0);
                    Serial.println(" feet");
                }
                else
                    Serial.println("error retrieving pressure measurement\n");
            }
            else
                Serial.println("error starting pressure measurement\n");
        }
        else
            Serial.println("error retrieving temperature measurement\n");
    }
    else
        Serial.println("error starting temperature measurement\n");

    delay(5000); // Pause for 5 seconds.
}

void loop_mq5()
{
    sensorValue = analogRead(MQ5_PIN); // read analog input pin 0

    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);

    delay(2000); // wait 2s for next reading
}

void loop_mq2()
{
    mq2_gas = analogRead(MQ2_PIN);
    Serial.print("Sensor Value: ");
    Serial.println(mq2_gas);

    delay(2000); // wait 2s for next reading
}

void loop_mq7()
{
    float sensorValue = analogRead(MQ7_PIN);
    Serial.println(sensorValue);
    delay(1000);
}

void displayInfo()
{
    if (gps.location.isValid())
    {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        Serial.print("Altitude: ");
        Serial.println(gps.altitude.meters());
    }
    else
    {
        Serial.println("Location: Not Available");
    }

    Serial.print("Date: ");
    if (gps.date.isValid())
    {
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.print(gps.date.day());
        Serial.print("/");
        Serial.println(gps.date.year());
    }
    else
    {
        Serial.println("Not Available");
    }

    Serial.print("Time: ");
    if (gps.time.isValid())
    {
        if (gps.time.hour() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(":");
        if (gps.time.minute() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(":");
        if (gps.time.second() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(".");
        if (gps.time.centisecond() < 10)
            Serial.print(F("0"));
        Serial.println(gps.time.centisecond());
    }
    else
    {
        Serial.println("Not Available");
    }

    Serial.println();
    Serial.println();
    delay(1000);
}

void loop_gps()
{
    while (gpsSerial.available() > 0)
        if (gps.encode(gpsSerial.read()))
            displayInfo();

    // If 5000 milliseconds pass and there are no characters coming in
    // over the software serial port, show a "No GPS detected" error
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println("No GPS detected");
        while (true)
            ;
    }
}

void loop()
{
    loop_dht11();
    loop_pir();
    loop_bmp180();
    loop_mq5();
    loop_mq2();
    loop_mq7();
    loop_gps();
}