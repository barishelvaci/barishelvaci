#include "main.h"
#include "usb_host.h"

ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim3;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
int computeOutput(int inputs[]);

// Perceptron Variables
#define INPUT_COUNT 2
#define HIDDEN_COUNT 2
#define THRESHOLD 0.5f

int weights_hidden[HIDDEN_COUNT][INPUT_COUNT] = {{20, 20}, {-20, -20}};
int bias_hidden[HIDDEN_COUNT] = {-10, 30};
int weights_output[HIDDEN_COUNT] = {20, 20};
int bias_output = -30;

volatile int inputs[2];
volatile int perceptronOutput;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();

  if (HAL_ADC_Start_IT(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if (hadc == &hadc1)
  {
    HAL_ADC_Start(&hadc1);
    int adcValue1 = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Start(&hadc1);
    int adcValue2 = HAL_ADC_GetValue(&hadc1);

    inputs[0] = adcValue1 > 2048 ? 1 : 0;
    inputs[1] = adcValue2 > 2048 ? 1 : 0;

    perceptronOutput = computeOutput((int*)inputs);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, perceptronOutput == 1 ? htim3.Init.Period : 0);
  }
}

int computeOutput(int inputs[])
{
  int hidden_output[HIDDEN_COUNT];
  for (int i = 0; i < HIDDEN_COUNT; i++)
  {
    hidden_output[i] = inputs[0] * weights_hidden[i][0] + inputs[1] * weights_hidden[i][1] + bias_hidden[i] > THRESHOLD ? 1 : 0;
  }

  int output = hidden_output[0] * weights_output[0] + hidden_output[1] * weights_output[1] + bias_output > THRESHOLD ? 1 : 0;
  return output;
}

void SystemClock_Config(void)
{

}

static void MX_ADC1_Init(void)
{

}

static void MX_TIM3_Init(void)
{

}

static void MX_GPIO_Init(void)
{

}

void Error_Handler(void)
{
  while (1)
  {

  }
}
