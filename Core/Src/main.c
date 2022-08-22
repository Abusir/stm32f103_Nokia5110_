/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/*
 * BlueTooth define
 */
#define RecDataSize 32

/*
 * Nokia 5110 define
 */
#define Nokia_RST_H()   HAL_GPIO_WritePin(Nokia_RST_GPIO_Port, Nokia_RST_Pin, GPIO_PIN_SET)
#define Nokia_RST_L()   HAL_GPIO_WritePin(Nokia_RST_GPIO_Port, Nokia_RST_Pin, GPIO_PIN_RESET)
#define Nokia_CE_H()    HAL_GPIO_WritePin(Nokia_CE_GPIO_Port, Nokia_CE_Pin, GPIO_PIN_SET)
#define Nokia_CE_L()    HAL_GPIO_WritePin(Nokia_CE_GPIO_Port, Nokia_CE_Pin, GPIO_PIN_RESET)
#define Nokia_DC_DATA() HAL_GPIO_WritePin(Nokia_DC_GPIO_Port, Nokia_DC_Pin, GPIO_PIN_SET)
#define Nokia_DC_CMD()  HAL_GPIO_WritePin(Nokia_DC_GPIO_Port, Nokia_DC_Pin, GPIO_PIN_RESET)
#define Nokia_Din_H()   HAL_GPIO_WritePin(Nokia_Din_GPIO_Port, Nokia_Din_Pin, GPIO_PIN_SET)
#define Nokia_Din_L()   HAL_GPIO_WritePin(Nokia_Din_GPIO_Port, Nokia_Din_Pin, GPIO_PIN_RESET)
#define Nokia_CLK_H()   HAL_GPIO_WritePin(Nokia_CLK_GPIO_Port, Nokia_CLK_Pin, GPIO_PIN_SET)
#define Nokia_CLK_L()   HAL_GPIO_WritePin(Nokia_CLK_GPIO_Port, Nokia_CLK_Pin, GPIO_PIN_RESET)
#define Nokia_VCC_H()   HAL_GPIO_WritePin(Nokia_VCC_GPIO_Port, Nokia_VCC_Pin, GPIO_PIN_SET)
#define Nokia_BL_H()    HAL_GPIO_WritePin(Nokia_BL_GPIO_Port, Nokia_BL_Pin, GPIO_PIN_SET)
#define Nokia_GND_L()   HAL_GPIO_WritePin(Nokia_GND_GPIO_Port, Nokia_GND_Pin, GPIO_PIN_RESET)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/*
 * BlueTooth macro
 */

/*
 * Nokia 5110 macro
 */
typedef enum
{
    DC_CMD  = 0,//写命令
    DC_DATA = 1 //写数据
}DCType;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/*
 * BlueTooth variables
 */
uint8_t RecBuffer;
uint8_t RecData[RecDataSize] = {0};
uint32_t DataPosition = 0;
int RecFlag =0;

/*
 * Nokia 5110 variables
 */

uint8_t Nokia_Init_Display[6][14] ={
        "**************",//line 1
        "**************",//line 2
        "  I Love YYY  ",//line 3
        "**************",//line 4
        "**************",//line 5
        "**************" //line 6
};

uint8_t Nokia_ADC_Display[6][14] ={
        "**************",//line 1
        "**************",//line 2
        "   Vx = 0000  ",//line 3
        "   Vy = 0000  ",//line 4
        "**************",//line 5
        "**************" //line 6
};

//char Nokia_Display[6][14] = {0};

const unsigned char  Font6x8[][6] ={
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
        { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
        { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
        { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
        { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
        { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
        { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
        { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
        { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
        { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
        { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
        { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
        { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
        { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
        { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
        { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
        { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
        { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
        { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
        { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
        { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
        { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
        { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
        { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
        { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
        { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
        { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
        { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
        { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
        { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
        { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
        { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
        { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
        { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
        { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
        { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
        { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
        { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
        { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
        { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
        { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
        { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
        { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
        { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
        { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
        { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
        { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
        { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
        { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
        { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
        { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
        { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
        { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
        { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
        { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
        { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
        { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
        { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
        { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
        { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
        { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
        { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
        { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
        { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
        { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
        { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
        { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
        { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
        { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
        { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
        { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
        { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
        { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
        { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
        { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
        { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
        { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
        { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
        { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
        { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
        { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
        { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
        { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
        { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
        { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
        { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
        { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
        { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
        { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
        { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
        { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
        { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
        };

/*
 * HW_504 variables
 */
uint16_t Vx_Value = 0;
uint16_t Voltage[2] ={0};
uint16_t Vy_Value = 0;
uint16_t AD_Flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/*
 * System function
 */
//HAL_StatusTypeDef user_delay_us(uint32_t us);
//HAL_StatusTypeDef user_delay_ms(uint32_t ms);
void user_print(const char *fmt,...);

/*
 * 中断回调函数，处理中断控制
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/*
 * Nokia 5110 LCD Function
 */
void Nokia_Init(void);
void Nokia_Clear(void);
void Nokia_SetXY(uint8_t X,uint8_t Y);
void Nokia_Wbyte(uint8_t data,DCType dc);
void Nokia_DisplayChar(uint8_t character);
void Nokia_DisplayString(uint8_t X,uint8_t Y,uint8_t * str);
void Nokia_DisplayFullFrame(uint8_t array[6][14]);
void Nokia_DisplayADC(void);

/*
 * HW-504 function
 */
void HW_504_Init(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
//    RetargetInit(&huart1);
    HAL_ADCEx_Calibration_Start(&hadc1);    //AD校准
    HAL_ADCEx_Calibration_Start(&hadc2);    //AD校准
    HW_504_Init();
//    HAL_UART_Receive_IT(&huart1,&RecBuffer,1);
    HAL_UART_Receive_IT(&huart1,&RecBuffer,1);
//    user_delay_ms(1);
    HAL_Delay(1);
    Nokia_Init();
//    user_delay_ms(1);
    HAL_Delay(1);
    Nokia_Clear();

    Nokia_DisplayString(0,0,(uint8_t *)"**************");//1
    Nokia_DisplayString(0,1,(uint8_t *)"**************");//2
    Nokia_DisplayString(0,2,(uint8_t *)"  I Love YYY  ");//3
    Nokia_DisplayString(0,3,(uint8_t *)"  I Love YYY  ");//4
    Nokia_DisplayString(0,4,(uint8_t *)"**************");//5
    Nokia_DisplayString(0,5,(uint8_t *)"**************");//6

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int adc;
  const char digits[11] ="0123456789";
  uint8_t adc_value[20] = "0123\r\n";
  while (1)
  {
      HAL_ADC_Start(&hadc1);
      HAL_ADC_PollForConversion(&hadc1,50);
      HAL_ADC_Start(&hadc2);
      HAL_ADC_PollForConversion(&hadc2,50);

      if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_REG_EOC))
      {
          adc = HAL_ADC_GetValue(&hadc1);
          for(int i =3;i>=0;i--)
          {
              adc_value[i]=digits[adc%10];
              adc /= 10;
          }
          HAL_UART_Transmit(&huart2 , (uint8_t *)"X = ",4,0xffff);
          HAL_UART_Transmit(&huart2 , (uint8_t *)adc_value,20,0xffff);
      }
 //     user_delay_ms(1000000);
      HAL_Delay(500);
      HAL_UART_Transmit(&huart2 , (uint8_t *)"\r\n",2,0xffff);
//      HAL_ADC_Start(&hadc2);
//      HAL_ADC_PollForConversion(&hadc2,50);
//
      if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2),HAL_ADC_STATE_REG_EOC))
      {
          adc = HAL_ADC_GetValue(&hadc2);
          for(int i =3;i>=0;i--)
          {
              adc_value[i]=digits[adc%10];
              adc /= 10;
          }
          HAL_UART_Transmit(&huart2 , (uint8_t *)"Y = ",4,0xffff);
          HAL_UART_Transmit(&huart2 , (uint8_t *)adc_value,20,0xffff);
      }
 //     user_delay_ms(1000000);
      HAL_Delay(500);
      HAL_UART_Transmit(&huart2 , (uint8_t *)"\r\n",2,0xffff);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//HAL_StatusTypeDef user_delay_us(uint32_t us)
//{
//    uint32_t temp;
//
//    if ((us*72 - 1UL) > SysTick_LOAD_RELOAD_Msk)
//    {
//        return HAL_ERROR;                                                   /* Reload value impossible */
//    }
//
//    SysTick->LOAD = (uint32_t)(us*72 - 1UL);
//    SysTick->VAL = 0;//清空计数器
//    SysTick->CTRL = 0x01;//0->enable 1->tickint(设置为1时开启中断请求) 2->clksource 16->countflag(只读，为1表明计数完成)
//
//
//    do {
//        temp = SysTick->CTRL;
//    }while((temp&0x01)&&(temp&(1<<16)));//等待计数完成
//
//    SysTick->CTRL=0x00;
//    SysTick->VAL=0x00;
//
//    return HAL_OK;
//}
//
//HAL_StatusTypeDef user_delay_ms(uint32_t ms)
//{
//    uint32_t i;
//    for( i=0;i<ms;i++)
//        user_delay_us(1000);
//    return HAL_OK;
//}

//void user_print(const char *fmt, ...)// custom printf() function
//{
//    unsigned char string[80];
//
//    va_list arg;
//    va_start(arg, fmt);
//    if (0 < vsprintf((char *)string, fmt, arg)) // build string
//        {
//        HAL_UART_Transmit(&huart1, string, strlen((const char *)string), 0xffffff); // send message via UART
//        } else {}
//    va_end(arg);
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        if(DataPosition >= RecDataSize-1)
        {
            DataPosition = 0;
            HAL_UART_Transmit(&huart1 , (uint8_t *)"Data overflow",20,0xffff);
        }
        else
        {
            RecData[DataPosition++] = RecBuffer;

            if(RecBuffer == (uint8_t)'1')
                RecFlag ++;
            else
                RecFlag = 0;

            //        HAL_UART_Transmit(&huart2,&RecBuffer,1,0xffff);

            if(RecFlag == 3)
            {
                if((RecData[DataPosition-6] == (uint8_t)'d')&&(RecData[DataPosition-5] == (uint8_t)'s')&&(RecData[DataPosition-4] == (uint8_t)'p'))
                {
                    Nokia_DisplayFullFrame(Nokia_Init_Display);
//                        Nokia_DisplayString(0,0,(uint8_t *)"**************");//1
//                        Nokia_DisplayString(0,1,(uint8_t *)"**************");//2
//                        Nokia_DisplayString(0,2,(uint8_t *)"  I Love YYY  ");//3
//                        Nokia_DisplayString(0,3,(uint8_t *)"  I Love YYY  ");//4
//                        Nokia_DisplayString(0,4,(uint8_t *)"**************");//5
//                        Nokia_DisplayString(0,5,(uint8_t *)"**************");//6
                }
                else if((RecData[DataPosition-6] == (uint8_t)'c')&&(RecData[DataPosition-5] == (uint8_t)'l')&&(RecData[DataPosition-4] == (uint8_t)'r'))
                {
                    Nokia_Clear();
                }
                HAL_UART_Transmit(&huart1,&RecData,DataPosition,0xffff);
                while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX);//等待发送完成
                RecFlag = 0;
                DataPosition = 0;
            }

            //        if((RecData[DataPosition-2] =0x0d)&&(RecData[DataPosition-1] =0x0a))//0x0d0a对应\r\n
            //        {
            //            HAL_UART_Transmit(&huart1,&RecData,RecDataSize,0xffff);
            //            while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX);//等待发送完成
            //            DataPosition = 0;
            //            memset(RecData,0x00,sizeof(RecDataSize));
            //        }
        }
        HAL_UART_Receive_IT(&huart1,&RecBuffer,1); //重新启动接收中断
    }

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == sw_Pin)
    {
//        user_delay_ms(20);
        HAL_Delay(20);
        if(HAL_GPIO_ReadPin(sw_GPIO_Port ,sw_Pin) == RESET)
        {
//            user_delay_ms(20);
            HAL_Delay(20);
            if(HAL_GPIO_ReadPin(sw_GPIO_Port ,sw_Pin) == RESET)
            HAL_GPIO_TogglePin(board_led_GPIO_Port, board_led_Pin);
        }
        __HAL_GPIO_EXTI_CLEAR_IT(sw_Pin);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        AD_Flag = 1;
        HAL_ADC_Stop_IT(&hadc1);    //关闭ADC1
        HAL_TIM_Base_Stop_IT(&htim3);  //关闭定时器
        Vx_Value = HAL_ADC_GetValue(&hadc1);//获取ADC1转换的值

        HAL_TIM_Base_Start_IT(&htim3);  //开启定时器
    }
    else if (hadc->Instance == ADC2)
    {
        AD_Flag = 2;
        HAL_ADC_Stop_IT(&hadc2);    //关闭ADC2
        HAL_TIM_Base_Stop_IT(&htim3);  //关闭定时器
        Vx_Value = HAL_ADC_GetValue(&hadc2);//获取ADC2转换的值

        HAL_TIM_Base_Start_IT(&htim3);  //开启定时器
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
        switch (AD_Flag) {
            case 0:
                HAL_ADC_Start_IT(&hadc1);
                break;
            case 1:
                HAL_ADC_Start_IT(&hadc2);
                break;
            default:
                Nokia_DisplayADC();
                break;
        }
    }
}

void Nokia_Wbyte(uint8_t data,DCType dc)
{
    //可以使用spi实现功能，也可以使用以下方式，实现过程更简单一点
    if(dc == DC_CMD)//写命令
        Nokia_DC_CMD();
    else if(dc == DC_DATA)//写数据
        Nokia_DC_DATA();
    //传输命令或数据
    for(uint8_t i=0;i<8;i++)
    {
        //检测数据时，在上升沿时获取数据，即应该在下降沿时切换数据，上升沿时会自动读取数据
        //并按照 MSB 先传输的方式进行数据的读取
        //要注意的是，下面可以不需要使用延时来控制clk的频率
        Nokia_CLK_L();
//        user_delay_us(1);
        if((data&0x80) == 0)
            Nokia_Din_L();
        else
            Nokia_Din_H();
//        user_delay_us(1);
        Nokia_CLK_H();
//        user_delay_us(1);
        data = data << 1;//数据移位
    }
}

void Nokia_Init(void)
{
    Nokia_VCC_H();
    Nokia_GND_L();

    //开启背景光
    Nokia_BL_H();

    Nokia_RST_L();
//    user_delay_ms(1);
    HAL_Delay(1);
    Nokia_RST_H();

    Nokia_CE_H();
    HAL_Delay(1);
//    user_delay_ms(1);
    Nokia_CE_L();

    Nokia_Wbyte(0x21,DC_CMD);//PD = 0，V = 0，H = 1 进入拓展指令设置
    Nokia_Wbyte(0xC8,DC_CMD);//设置偏置电压Vop
    // V_lcd=3.06+Vop*0.06 V_lcd[3.00,10.68] 取Vop=72D=48H->V_lcd=7.38
    Nokia_Wbyte(0x06,DC_CMD);//温度校正 温度系数为 2
    Nokia_Wbyte(0x13,DC_CMD);//根据手册直接查询即可 混合率为1:48 (对比度)
    Nokia_Wbyte(0x20,DC_CMD);//PD = 0，V = 0，H = 0 选择水平寻址 进入进本命令设置
    Nokia_Wbyte(0x0C,DC_CMD);//设定显示模式，0x0D反转显示.0X0C正常
}

void Nokia_SetXY(uint8_t X,uint8_t Y)
{
    //字符需要使用8*6个像素点,即用6字节表示一个英文字符，水平模式下X取值范围为[0,84/6 - 1=13] Y为[0,6-1=5]
    if(X>13)
        X = 13;
    if(Y>5)
        Y = 5;
    X *= 6;

    Nokia_Wbyte(0x20,DC_CMD);//设置H = 0，可以设置坐标的值
    Nokia_Wbyte(0x80 | X,DC_CMD);//设置x轴的坐标为X
    Nokia_Wbyte(0x40 | Y,DC_CMD);//设置y周的坐标为Y
}

void Nokia_Clear(void)
{
    uint16_t i;
    //H = 0 设置X，Y的地址为令，并依次清除数据
    Nokia_Wbyte(0x20,DC_CMD);//设置H = 0，可以设置坐标的值
    Nokia_Wbyte(0x80, DC_CMD);//RAM X=0
    Nokia_Wbyte(0x40, DC_CMD);//RAM Y=0
    for(i=0; i<504; i++)//6*84=504个byte
        Nokia_Wbyte(0, DC_DATA);
}

void Nokia_DisplayChar(uint8_t character)
{
    character -= 32;
    for(uint8_t n = 0; n < 6 ; n++)
        Nokia_Wbyte(Font6x8[character][n],DC_DATA);
}

void Nokia_DisplayString(uint8_t X,uint8_t Y,uint8_t * str)
{
    uint8_t * s = str;
    uint8_t send_buffer[504]={0};//一帧画面最多504个字节
    int buffer_index = 0;
    if(s == NULL)
    {
        return ;
    }

    while(*s)
    {
        //将字符数据转换为串行字节数据，以便数据发送
        for(int i=0;i<6;i++)
        {
            send_buffer[buffer_index ++] = Font6x8[*s -32][i];
        }
        s++;
    }

    send_buffer[buffer_index++] = 0;

    Nokia_SetXY(X,Y);
    for(int i=0;i<buffer_index;i++)
    {
        Nokia_Wbyte(send_buffer[i],DC_DATA);
    }
}

void Nokia_DisplayFullFrame(uint8_t array[6][14])
{
    for(uint8_t j=0;j<6;j++)
    {
        Nokia_DisplayString(0,j,array[j]);
    }
}

void Nokia_DisplayADC(void)
{
    HAL_TIM_Base_Stop_IT(&htim3);  //关闭定时器

    uint8_t VX[4],VY[4];
    uint16_t tempVx = Vx_Value;
    uint16_t tempVy = Vy_Value;
    for(uint8_t i;i<4;i++)
    {
        //数字加48对应其ascii码的值
        VX[i]=tempVx%10+48;
        VY[i]=tempVy%10+48;
        tempVx /= 10;
        tempVy /= 10;
    }
    /*
     * uint8_t Nokia_ADC_Display[6][14] ={
        "**************",//line 1
        "**************",//line 2
        "   Vx = 0000  ",//line 3
        "   Vy = 0000  ",//line 4
        "**************",//line 5
        "**************" //line 6
    };
     */
    for(uint8_t i=0;i<4;i++)
    {
        Nokia_ADC_Display[2][8+i]=VX[i];
        Nokia_ADC_Display[3][8+i]=VY[i];
    }

    Nokia_DisplayFullFrame(Nokia_ADC_Display);
    AD_Flag = 0;

    HAL_TIM_Base_Start_IT(&htim3);  //开启定时器
}

void HW_504_Init(void)
{
    HAL_GPIO_WritePin(HW_504_GND_GPIO_Port, HW_504_GND_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HW_503_VCC_GPIO_Port, HW_503_VCC_Pin, GPIO_PIN_SET);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
