#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

//includes
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_pwr.h"
#include "config.h"
#include "transfer_functions.h"


//definitions
#define CANID_SYNC		0x80
#define CANID_CONFIG	0x700
#define CANID_ERROR		0x600

//function prototypes
void Error_Handler(void);
void Can_Send_Analog();
void Can_Send_Diagnostics();
void Set_Error(uint32_t);
void Send_Error(void);


//warning and error codes
#define ERR_CAN_FIFO_FULL			1
#define ERR_SEND_FAILED				2

#define ERR_WRONG_BYTES				33
#define ERR_INCORRECT_TF			34
#define ERR_INCORRECT_TF_VOLTAGE	35
#define ERR_INCORRECT_TF_NTC		36


//config externs
extern uint8_t MeasureDriverCurrent;
extern uint8_t MeasureTemperature;
extern uint16_t SensorRollingAverages[4];
extern uint8_t TransferFunctions[4];

extern uint16_t CanId_Analog;
extern uint8_t AnalogSensorBytes[4];
extern uint16_t CanId_Diagnostics;


#ifdef __cplusplus
}
#endif

#endif