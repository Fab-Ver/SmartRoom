#include "LedTask.h"

unsigned long last = 0;

LedTask::LedTask(int ledPin){
    this->led = new Led(ledPin);
	this->currState = UNDEFINED;
	this->previousState = UNDEFINED;
}

void LedTask::init(){
    xTaskCreatePinnedToCore(this->tickWrapper,"LedTask",8192,this,1,&LedHandle,1);
    delay(500);
}

void LedTask::tickWrapper(void* _this){
	((LedTask*)_this)->tick();
}

void LedTask::tick(){
	for(;;){
		unsigned long now = millis();
		if(now - last >= TASK_PERIOD){
			last = now;
			xSemaphoreTake(xMutex, portMAX_DELAY);
			currState = currentDetectionState ? ON : OFF;
			xSemaphoreGive(xMutex);
			if(currState != previousState){
				if(currState == ON){
      				led->switchOn();
    			} else {
      				led->switchOff();
    			} 
    			previousState = currState;
			}
		}
		vTaskDelay(TASK_PERIOD);
  	}
}