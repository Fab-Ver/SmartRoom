#include "LedTask.h"

LedTask::LedTask(int ledPin){
    this->led = new Led(ledPin);
}

void LedTask::init(){
    xTaskCreatePinnedToCore(this->tickWrapper,"LedTask",1000,this,1,&LedHandle,1);
    delay(500);
}

void LedTask::tickWrapper(void* _this){
	((LedTask*)_this)->tick();
}

void LedTask::tick(){
	for(;;){
		int detectionState;
		xSemaphoreTake(xMutex, portMAX_DELAY);
		detectionState = currentDetectionState;
		xSemaphoreGive(xMutex);
    	if(detectionState){
      		led->switchOn();
    	} else {
      		led->switchOff();
    	} 
  	}
}