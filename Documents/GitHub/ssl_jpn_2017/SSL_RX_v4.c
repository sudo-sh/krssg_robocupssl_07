#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>

#include "misc.h"

#include "defines.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_nrf24l01.h"

#include "tm_stm32f4_gpio.c"
#include "tm_stm32f4_delay.c"
#include "tm_stm32f4_spi.c"
#include "tm_stm32f4_nrf24l01.c"

#define pld 26
#define chnl 5
#define TRUE 1
#define FALSE 0

int bot_id;  //Define bot id here. BOT id switch does not work. Hardcoding per bot.
int team_id = 127;
int i=0;
volatile int c=0;
volatile int BotData[32]={0};
int duty,pulse_l,temp,x,pwm;
int temp=0;
int count=0;

uint8_t MyAddress[] = {
    0x7E,
    0x7E,
    0x7E,
    0x7E,
    0x7E

 };

uint8_t  Receive[32]={0};

void init_kicker()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void init_botid()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

//    bot_id = ((GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)*4) + (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)*2) + GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5));
//    team_id = (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2));
}

void GPIOLED_TEST()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void GPIOB_Motors()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_0 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void GPIOE_Motors()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void setFWDbit(int motorID, int value)
{
    switch (motorID){
        case 1:
            if(value==1){
                GPIO_SetBits(GPIOB, GPIO_Pin_0);
            }else if(value==0){
                GPIO_ResetBits(GPIOB, GPIO_Pin_0);
            }break;
        case 2:
            if(value==1){
                GPIO_SetBits(GPIOE, GPIO_Pin_7);
            }else if(value==0){
                GPIO_ResetBits(GPIOE, GPIO_Pin_7);
            }break;
        case 3:
            if(value==1){
                GPIO_SetBits(GPIOE, GPIO_Pin_9);
            }else if(value==0){
                GPIO_ResetBits(GPIOE, GPIO_Pin_9);
            }break;
        case 4:
            if(value==1){
                GPIO_SetBits(GPIOE, GPIO_Pin_11);
            }else if(value==0){
                GPIO_ResetBits(GPIOE, GPIO_Pin_11);
            }break;
        case 5:
            if(value==1){
                GPIO_SetBits(GPIOE, GPIO_Pin_13);
            }else if(value==0){
                GPIO_ResetBits(GPIOE, GPIO_Pin_13);
            }break;
    }
}
void setBKDbit(int motorID, int value)
{
    switch (motorID){
        case 1:
            if(value==1){
                GPIO_SetBits(GPIOB, GPIO_Pin_1);
            }else if(value==0){
                GPIO_ResetBits(GPIOB, GPIO_Pin_1);
            }break;
        case 2:
            if(value==1){
                GPIO_SetBits(GPIOE, GPIO_Pin_8);
            }else if(value==0){
                GPIO_ResetBits(GPIOE, GPIO_Pin_8);
            }break;
        case 3:
            if(value==1){
                GPIO_SetBits(GPIOE, GPIO_Pin_10);
            }else if(value==0){
                GPIO_ResetBits(GPIOE, GPIO_Pin_10);
            }break;
        case 4:
            if(value==1){
                GPIO_SetBits(GPIOE, GPIO_Pin_12);
            }else if(value==0){
                GPIO_ResetBits(GPIOE, GPIO_Pin_12);
            }break;
        case 5:
            if(value==1){
                GPIO_SetBits(GPIOE, GPIO_Pin_14);
            }else if(value==0){
                GPIO_ResetBits(GPIOE, GPIO_Pin_14);
            }break;
    }
}

void pwmToWheels_init()
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        TIM_OCInitTypeDef  TIM_OCInitStructure;
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
        /* Time base configuration */
        TIM_TimeBaseStructure.TIM_Period = 127;
        TIM_TimeBaseStructure.TIM_Prescaler = 131;
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OC1Init(TIM4, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
        /* PWM1 Mode configuration: Channel2 */
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;
        TIM_OC2Init(TIM4, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
        /* PWM1 Mode configuration: Channel3 */
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;
        TIM_OC3Init(TIM4, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
        /* PWM1 Mode configuration: Channel4 */
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;
        TIM_OC4Init(TIM4, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
        TIM_ARRPreloadConfig(TIM4, ENABLE);
        /* TIM4 enable counter */
        TIM_Cmd(TIM4, ENABLE);
}
void pwmToWheels_set(int channel, int duty_cycle)
{
    switch (channel){
        case 1: TIM4->CCR1 = duty_cycle; break;
        case 2: TIM4->CCR2 = duty_cycle; break;
        case 3: TIM4->CCR3 = duty_cycle; break;
        case 4: TIM4->CCR4 = duty_cycle; break;
    }
}
void pwmToDribbler_init()
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        TIM_OCInitTypeDef  TIM_OCInitStructure;
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
        /* Time base configuration */
        TIM_TimeBaseStructure.TIM_Period = 255;
        TIM_TimeBaseStructure.TIM_Prescaler = 66;
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OC1Init(TIM2, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
        /* PWM1 Mode configuration: Channel3 */
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;
        TIM_OC3Init(TIM2, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
        TIM_ARRPreloadConfig(TIM2, ENABLE);
        /* TIM2 enable counter */
        TIM_Cmd(TIM2, ENABLE);
}
void pwmToDribbler_set(int duty_cycle)
{
    TIM2->CCR3 = duty_cycle;
}

int delay=0;

void NRF_init()
{
    TM_NRF24L01_Init(18, 32);
        //TM_NRF24L01_SetCrcLength(NRF24L01_CRC_8);
        TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm); /* Set RF settings, Data rate to 2Mbps, Output power to -18dBm */
        TM_NRF24L01_SetMyAddress(MyAddress);/* Set my address, 5 bytes */
         TM_NRF24L01_PowerUpRx();
}

void DataCheck()
{
    int i=0;
    for(i=0;i<32;i++)
    {
        BotData[i] = Receive[i];
    }
}

void kicker()
{
    GPIO_ResetBits(GPIOD,GPIO_Pin_0);  //Giving a falling edge interrupt for kicker.
}

void Dribbler(int status)
{
    switch (status){
        case 1: pwmToDribbler_set(128);  //status = 0 -> Dribbler is off
                setBKDbit(5,1);          //Status = 1 -> Dribbler is on
                setFWDbit(5,0);
                break;

        case 0: pwmToDribbler_set(10);
                setBKDbit(5,1);
                setFWDbit(5,0);
                break;
    }
}

void SetDataValues(int botid_dataStart)
{
    if((BotData[0] == team_id) )
                                {
                                     /* if(BotData[botid_dataStart+5-1]==1)
                                    {
                                        kicker();
                                        GPIO_ResetBits(GPIOE,GPIO_Pin_0);
                                    }
                                    if(BotData[botid_dataStart+6-1]==1)
                                    {
                                        pwmToDribbler_set(128);
                                        setBKDbit(5,1);
                                        setFWDbit(5,0);
                                    }
                                    else
                                    {
                                        pwmToDribbler_set(10);
                                        setBKDbit(5,1);
                                        setFWDbit(5,0);
                                    }*/
                                    switch (BotData[botid_dataStart+5-1])   //Switch Case for running Dribbler and Kicker
                                    {
                                        case 0: Dribbler(0);  //Both dribbler and kicker are deactivated
                                                break;
                                        case 1: kicker();    //Kicker is activated, Dribbler is deactivated
                                                Dribbler(0);
                                                break;
                                        case 2: Dribbler(1);  // Dribbler is activated, Kicker is deactivated
                                                break;
                                        case 3: kicker();     //Both kicker and dribbler are activated.
                                                Dribbler(1);
                                                break;
                                    }

                                    for(i=1; i<5; i++)  //Loop for running the 4 motors.
                                    {
                                    	GPIO_ToggleBits(GPIOE,GPIO_Pin_1);
                                        if(BotData[botid_dataStart+i-1]>127)
                                        {
                                            BotData[botid_dataStart+i-1]=255-BotData[botid_dataStart+i-1];
                                            setFWDbit(i,1);
                                            setBKDbit(i,0);
                                        }
                                        else
                                        {
                                            setFWDbit(i,0);
                                            setBKDbit(i,1);
                                        }
                                        if(BotData[botid_dataStart+i-1]==0)
                                        {
                                            pwm = 10;
                                        }
                                        else
                                        {
                                            pwm = (int) 13+(BotData[botid_dataStart+i-1]*100.0)/126.0;
                                        }
                                        pwmToWheels_set(i,pwm);
                                    }
                                }
}

int main(void)
    {
        SystemInit();
        init_kicker();
        GPIOB_Motors();
        GPIOE_Motors();
        GPIOLED_TEST();
        init_botid();
        NRF_init();
        pwmToWheels_init();
        pwmToDribbler_init();

//        GPIO_SetBits(GPIOE, GPIO_Pin_0);
//        GPIO_SetBits(GPIOE, GPIO_Pin_1);

       team_id = 127;
        bot_id = 1;
        uint32_t incr=0;
          uint32_t incr_nrf_sleep=0;

        while(1)
        {
            GPIO_SetBits(GPIOD, GPIO_Pin_0);  //Setting pin for kicker high to allow for falling edge.

            while (!TM_NRF24L01_DataReady()) {
                /*
                When The STM Does not Recieve Any Packet We need to 
                Include the Criteria Such that NRF Should Not go into 
                Sleep.So If this Loop is Executed many a times we need to need 
                Initialise the NRF Again
                */   
                            incr_nrf_sleep++;

                            if(incr_nrf_sleep>100000000)
                            {
                                NRF_init();
                                incr_nrf_sleep=0;
                            }




             }
                            TM_NRF24L01_GetData(Receive);
                            DataCheck();
                            //TM_NRF24L01_PowerUpRx();
                            GPIO_ToggleBits(GPIOE,GPIO_Pin_0);

                            count=0;

                            //SetDataValues(*integer*);
                            switch (bot_id){                //Each bot takes 5 Bytes of data.
                                case 1: SetDataValues(1);
                                        break;
                                case 2: SetDataValues(6);
                                        break;
                                case 3: SetDataValues(11);
                                        break;
                                case 4: SetDataValues(16);
                                        break;
                                case 5: SetDataValues(21);
                                        break;
                                case 6: SetDataValues(26);
                                        break;
                            }

                            incr++;

                            if(incr>100000000)
                            {
                            	NRF_init();
                            	incr=0;
                            }



              }

}
