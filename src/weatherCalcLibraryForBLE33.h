#ifndef WEATHERCALCLIBRARYFORBLE33_H
#define WEATHERCALCLIBRARYFORBLE33_H

#include <stdint.h>

#define WEATHERCALCLIBRARYFORBLE33_H_VERSION         0x01


float celsiusToKelvins(float celcius);
//float celsiusToFahrenheits(float celcius);
float pascalsTommHg(float pascals);
float pascalsToAltitude(float pascals, float celsius = 20);


// working with meteorological parameters t, h, p (temperature, humidity, pressure)
class weatherCalcForBLE33 {

public:
    // Setting initial data
    void setTemperature(float celsius);   // temperature in degrees Celsius
    void setHumidity(uint8_t percents);    // relative humidity in %
    void setPressure(float pascals);       // pressure in pascals
    void setUserAltitude(float meters);    // barometer height above sea level in meters

    // Retrieving initial data
    float getTemperature();                // temperature in degrees Celsius
    float getHumidity();                   // relative humidity in %
    float getPressure();                   // pressure in pascals
    float getUserAltitude();               // barometer height above sea level in meters

    // Calculation of values
    void calculate();

    // Retrieving calculated values; original data dependencies are indicated after the description
    float getSteamPressureSaturated();     // saturated water vapor pressure in pascals (t, p)
    float getSteamPressurePartial();       // partial pressure of water vapor in pascals (t, p, h)
    float getHumidityAbsolute();           // absolute humidity in grams per cubic meter (t, p, h)
    float getTemperatureEffective();       // effective temperature in degrees Celsius (t, p, h)
    float getKelvins();                    // temperature in Kelvins (t)
    float getFahrenheits();                // temperature in degrees Fahrenheit (t)
    float getPressuremmHg();               // pressure in millimeters of mercury column (p)
    float getAltitude();                   // altitude above sea level from pressure, meters (t, p)
    float getNormalPressure();             // normal pressure at the barometer installation altitude in pascals (t, a)
    float getNormalPressuremmHg();         // normal pressure at the barometer installation altitude in mm Hg (t, a)

private:

    float t;
    float h;
    float p;
    float a;
    float td;
    float te;
    float ps;
    float ppw;
    float pn;
    float ha;
    float hp;
};

#endif /* WEATHERCALCLIBRARYFORBLE33_H */
