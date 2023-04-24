#include "LedTask.h"

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
		/* Serial.println("Led"); */
		int detectionState;
		xSemaphoreTake(xMutex, portMAX_DELAY);
		detectionState = currentDetectionState;
		xSemaphoreGive(xMutex);
		currState = detectionState ? ON : OFF;
		if(currState != previousState){
			if(currState == ON){
      			led->switchOn();
    		} else {
      			led->switchOff();
    		} 
    		previousState = currState;
		}
		vTaskDelay(200);
  	}
}