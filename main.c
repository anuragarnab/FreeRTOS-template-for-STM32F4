#include "stm32f4xx.h"
#include "stm32f4_discovery_audio_codec/stm32f4_discovery_audio_codec.h"

#include "stm32f4_discovery.h"
#include "STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_tim.h"
#include "STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rcc.h"
#include <math.h>

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/task.h"
#include "FreeRTOS/include/semphr.h"

uint64_t u64IdleTicksCnt=0; // Counts when the OS has no task to execute.
uint64_t tickTime=0;        // Counts OS ticks (default = 1000Hz).

void GPIO_A0_Init(void);

void vButtonTask (void * pvparameters);


/*
 * When FreeRTOS crashes, you often end up in a hard fault.
 */
void HardFault_Handler (void){
	STM_EVAL_LEDOn(LED3);
	STM_EVAL_LEDOn(LED4);
	STM_EVAL_LEDOn(LED5);
	STM_EVAL_LEDOn(LED6);
}


int main(void)
{
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

	xTaskCreate( vButtonTask, ( signed char * ) "Button Task", 100, NULL, 1, NULL );

    vTaskStartScheduler(); // This should never return.


    while(1)
    {

    }

    return 1;
}

/*
 * A task that read reads and debounces the user's button
 */
void vButtonTask( void *pvparameters )
{

	uint8_t wasPressed = 0;

	for (;;){

		while (STM_EVAL_PBGetState(BUTTON_USER) == 1) {
		    wasPressed = 1;
		}

		if (wasPressed){
			STM_EVAL_LEDToggle(LED4);
    	    wasPressed = 0;
		}

		vTaskDelay(30 / portTICK_RATE_MS);
	}
}

// This FreeRTOS callback function gets called once per tick (default = 1000Hz).
// ----------------------------------------------------------------------------
void vApplicationTickHook( void ) {
    ++tickTime;
}

// This FreeRTOS call-back function gets when no other task is ready to execute.
// On a completely unloaded system this is getting called at over 2.5MHz!
// ----------------------------------------------------------------------------
void vApplicationIdleHook( void ) {
    ++u64IdleTicksCnt;
}

// A required FreeRTOS function.
// ----------------------------------------------------------------------------
void vApplicationMallocFailedHook( void ) {
    configASSERT( 0 );  // Latch on any failure / error.
}


/*
 * Required callbacks for audio playback
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
	return 1;
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
}

void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size)
{
}

void EVAL_AUDIO_Error_CallBack(void* pData)
{
	while(1);
}

uint32_t Codec_TIMEOUT_UserCallback(void)
{
	while(1);
	return 1;
}
