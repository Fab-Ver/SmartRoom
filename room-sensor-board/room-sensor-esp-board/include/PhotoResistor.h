#ifndef __PHOTO_RESISTOR__
#define __PHOTO_RESISTOR__

class PhotoResistor{
    public:
        /**
         * Report the light status of the room.
         * @return true if the room is dark, false otherwise. 
        */
        virtual bool isDark() = 0;
};

#endif