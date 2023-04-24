#ifndef __LIGHT_SENSOR__
#define __LIGHT_SENSOR__

#include "PhotoResistor.h"
#define THRESHOLD 1000

class LightSensor : public PhotoResistor{
    private:
        int pin;
    public:
        LightSensor(int pin);
        bool isDark();
};

#endif