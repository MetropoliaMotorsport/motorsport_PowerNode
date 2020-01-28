#include "main.h"
#include "transfer_functions.h"


//lookup tables, first 2^x values is raw, second 2^x values is calculated
uint32_t NTC_NTC1_680_LUT[2*16] =	{	419, 629, 757, 985, 1223, 1582, 1953, 2310, 2641, 2918, 3153, 3343, 3496, 3616, 3711, 3785,
										100, 200, 250, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500 }; //in .1 °C
uint32_t NTC_NTC1_360_LUT[2*16] =	{	233, 359, 439, 588, 753, 1023, 1333, 1665, 2008, 2324, 2617, 2874, 3093, 3275, 3425, 3546,
										100, 200, 250, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500	};

uint32_t TF_Select(uint8_t bytes, uint8_t sensor, uint16_t raw)
{
	uint32_t transmit = 0;

	switch(sensor)
	{
	case VOLTAGE_3V3_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_4V5_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_5V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_9V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_12V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_24V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case VOLTAGE_30V_UNCAL:
		transmit = TF_Voltage(bytes, sensor, raw);
		break;
	case NTC_NTC1_680:
		transmit = TF_NTC(bytes, sensor, raw);
		break;
	case NTC_NTC1_360:
		transmit = TF_NTC(bytes, sensor, raw);
		break;
	default:
		Set_Error(ERR_INCORRECT_TF);
		break;
	}

	return transmit;
}


//note that these will overflow if higher than 12 bit resolution on the adc is used
uint32_t TF_Voltage(uint8_t bytes, uint8_t divider, uint16_t raw)
{
	uint32_t voltage = 0;

	switch(divider) //calculate voltage in uV
	{
	case VOLTAGE_3V3_UNCAL:
		voltage = (raw*103125)/128; // C = 3.3*1000000/2^12 //3.3/2^12 gives in V
		switch(bytes)
		{
		case 1:
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_4V5_UNCAL:
		voltage = (raw*55327)/50; // C*(1.21+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_5V_UNCAL:
		voltage = (raw*247667)/20; // C*(1.74+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_9V_UNCAL:
		voltage = (raw*44759)/20; // C*(5.76+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_12V_UNCAL:
		voltage = (raw*301129)/100; // C*(8.87+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_24V_UNCAL:
		voltage = (raw*150689)/25; // C*(21+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage+=500000;
			voltage/=1000000; //voltage in 100's of mV
			break;
		case 2:
			voltage+=500;
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	case VOLTAGE_30V_UNCAL:
		voltage = (raw*744493)/100; // C*(26.7+3.24)/3.24
		switch(bytes)
		{
		case 1:
			voltage+=5000000;
			voltage/=10000000; //voltage in V
			break;
		case 2:
			voltage+=500;
			voltage/=1000; //voltage in mV
			break;
		default:
			Set_Error(ERR_WRONG_BYTES);
			break;
		}
		break;
	default:
		Set_Error(ERR_INCORRECT_TF_VOLTAGE);
		break;
	}

	return voltage;
}

uint32_t TF_NTC(uint8_t bytes, uint8_t resistor, uint16_t raw)
{
	uint32_t temperature = 0;

	switch(resistor)
	{
	case NTC_NTC1_680:
		temperature = LUT(raw, NTC_NTC1_680_LUT, 4);
		break;
	case NTC_NTC1_360:
		temperature = LUT(raw, NTC_NTC1_360_LUT, 4);
		break;
	default:
		Set_Error(ERR_INCORRECT_TF_NTC);
		break;
	}

	return temperature;
}


uint32_t LUT(uint16_t input, uint32_t* LUT, uint8_t LUT_length_LN2)
{
	uint32_t result = 0;

	uint8_t length = 2;
	for(uint32_t i=0; i<(LUT_length_LN2-1); i++) { length*=2; }

	uint8_t bound_low = 0;
	uint8_t bound_high = length-1;
	uint8_t mid = 0;

	if (LUT[bound_low]>input)
	{
		return LUT[bound_low+length];
	}
	else if (LUT[bound_high]<input)
	{
		return LUT[bound_high+length];
	}

	for(uint32_t i=0; i<LUT_length_LN2; i++)
	{
		mid = ((bound_high-bound_low)/2)+bound_low;

		switch( (LUT[mid]>input) | ((LUT[mid+1]<input)<<1))
		{
		case 0: //result between mid and mid+1
			i=LUT_length_LN2; //mid is correct, so exit the loop
			break;
		case 1: //input < mid value
			bound_high = mid;
			break;
		case 2: //input > mid+1 value
			bound_low = mid+1;
			break;
		case 3: //impossible case
			Error_Handler();
			break;
		default:
			Error_Handler();
			break;
		}
	}

	result = (int)((((float)(input-LUT[mid]))/((float)(LUT[mid+1]-LUT[mid])))*(LUT[mid+1+length]-LUT[mid+length]))+LUT[mid+length];

	return result;
}