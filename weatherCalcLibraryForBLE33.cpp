#include "weatherCalcLibraryForBLE33.h"
#include <cmath>

// Coefficients for calculations
#define PSAT_FA         1.0016
#define PSAT_FB         3.15e-8
#define PSAT_FC         7.4
#define PSAT_EA         17.62
#define PSAT_EB         243.12
#define PSAT_EM         611.2
#define PRESS_NORMAL    101325.0
#define PRESS_P2MMHG(p) (p * (760.0 / PRESS_NORMAL))
#define ALT_COEFF       29.256
#define HUM_RV          461.5e-3
#define TEMP_C2K(c)     (273.15 + c)
#define TEMP_C2F(c)     (c * (9.0 / 5.0) + 32)
#define TEFF_D          4.25
#define TEFF_M          3.78e-3

void weatherCalcForBLE33::setTemperature(float celsius) {
    t = celsius;
}

void weatherCalcForBLE33::setHumidity(uint8_t percents) {
    h = (float)percents / 100.0;
}

void weatherCalcForBLE33::setPressure(float pascals) {
    p = pascals;
}

void weatherCalcForBLE33::setUserAltitude(float meters) {
    a = meters;
}

float weatherCalcForBLE33::getTemperature() {
    return t;
}

float weatherCalcForBLE33::getHumidity() {
    return 100.0 * h;
}

float weatherCalcForBLE33::getPressure() {
    return p;
}

float weatherCalcForBLE33::getUserAltitude() {
    return a;
}

void weatherCalcForBLE33::calculate() {
    float tmp;

    /* Saturated water vapor pressure
    * 
    *  //////////// using Magnus-Tetens equation  \\\\\\\\\\\\\\\\\\
                                          17.62 T                                7.4
       Ps = e(t) f(p); e(t) = 611.2 exp ----------; f(p) = 1.0016 + 3.15e-8  P - ---; P = [Pa]; T = [°C]
                                        243.12 + T                                P

    */
    tmp = PSAT_EA * t;
    tmp /= (PSAT_EB + t);
    tmp = exp(tmp);
    tmp *= PSAT_EM;
    ps = PSAT_FA;
    ps += PSAT_FB * p;
    ps -= PSAT_FC / p;
    ps *= tmp;

    /* Partial water vapor pressure
       Pw = Ps RH; 0 < RH < 1   ;RH= Relative Humidity
    */
    ppw = ps;
    ppw *= h;

    /* Absolute humidity in g/m^3
                P(H2O)               
       AH = -----------; Rv = 461.5e-3 ; P = [Pa]; T = [°C]
            Rv.(273.15 + t)
    */
    ha = ppw;
    ha /= HUM_RV;
    ha /= TEMP_C2K(t);

    /* Effective temperature (perceived)
                        
       Teff = t + 3.78e-3.Pw.RH - 4.25; Teff, t = [°C]; Pw = [Pa]; 0 < RH < 1
    */
    te = ppw;
    te *= TEFF_M;
    te -= TEFF_D;
    te += t;

    /* Altitude from pressure, meters
                   Po
       h = C T ln ----; C = 29.256; T = [K] = 273.15 + t[°C]
                   Px
    */
    hp = PRESS_NORMAL;
    hp /= p;
    hp = log(hp);
    hp *= TEMP_C2K(t);
    hp *= ALT_COEFF;

    // Normalized pressure
    pn = a;
    pn /= ALT_COEFF;
    pn /= TEMP_C2K(t);
    pn = exp(pn);
    pn = PRESS_NORMAL / pn;
}

//float weatherCalcForBLE33::getDewPoint() {
//    return td;
//}

float weatherCalcForBLE33::getSteamPressureSaturated() {
    return ps;
}

float weatherCalcForBLE33::getSteamPressurePartial() {
    return ppw;
}

float weatherCalcForBLE33::getHumidityAbsolute() {
    return ha;
}

float weatherCalcForBLE33::getTemperatureEffective() {
    return te;
}

float weatherCalcForBLE33::getKelvins() {
    return TEMP_C2K(t);
}

float weatherCalcForBLE33::getFahrenheits() {
    return TEMP_C2F(t);
}

float weatherCalcForBLE33::getPressuremmHg() {
    return PRESS_P2MMHG(p);
}

float weatherCalcForBLE33::getAltitude() {
    return hp;
}

float weatherCalcForBLE33::getNormalPressure() {
    return pn;
}

float weatherCalcForBLE33::getNormalPressuremmHg() {
    return PRESS_P2MMHG(pn);
}

// Separate conversion functions
float celsiusToKelvins(float celcius) {
    return TEMP_C2K(celcius);
}

float celsiusToFahrenheits(float celcius) {
    return TEMP_C2F(celcius);
}

float pascalsTommHg(float pascals) {
    return PRESS_P2MMHG(pascals);
}

float pascalsToAltitude(float pascals, float celsius) {
    float hp = PRESS_NORMAL;
    hp /= pascals;
    hp = log(hp);
    hp *= TEMP_C2K(celsius);
    hp *= ALT_COEFF;
    return hp;
}
