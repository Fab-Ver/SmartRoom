#ifndef __MOTION_SENSOR__
#define __MOTION_SENSOR__

class MotionSensor{
    public:
        /*
            Return true if an object is detected, false otherwise. 
        */
        virtual bool isDetected() = 0;
};

#endif