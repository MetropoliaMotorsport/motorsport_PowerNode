#ifndef CONFIG_H_
#define CONFIG_H_

//ID definition
#define ID 0

//definitions
#define ROLLING_AVERAGE_MAX 128
#define T_ROLLING_AVERAGE	16
#define I_ROLLING_AVERAGE	32

//function prototypes
void Config_Setup(void);

void Config_0(void);
void Config_1(void);

void Config_Write_Flash(void);
void Config_Read_Flash(void);

void Flash_Write(uint32_t, uint32_t, uint32_t[512], int);
uint32_t Flash_Read(uint32_t);

#endif /* CONFIG_H_ */
