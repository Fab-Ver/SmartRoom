#ifndef __TASK__
#define __TASK__

class Task {
    private:
        virtual void tick() = 0;
        virtual static void tickWrapper(void* _this) = 0;
    public: 
        virtual void init() = 0;    
};

#endif