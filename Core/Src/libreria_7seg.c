/*
 * libreria_7seg.c
 *
 *  Created on: Apr 4, 2024
 *      Author: tomas
 */

#include "main.h"

#define timeMax 4
#define disp1 GPIOA, GPIO_PIN_3
#define disp2 GPIOA, GPIO_PIN_2
#define disp3 GPIOA, GPIO_PIN_12
#define disp4 GPIOA, GPIO_PIN_15

#define a  GPIOA, GPIO_PIN_4
#define f  GPIOA, GPIO_PIN_5
#define g  GPIOA, GPIO_PIN_6
#define b  GPIOA, GPIO_PIN_7
#define c  GPIOA, GPIO_PIN_8
#define d  GPIOA, GPIO_PIN_9
#define e  GPIOA, GPIO_PIN_10
#define p  GPIOA, GPIO_PIN_11

typedef enum{Display1,Display2,Display3,Display4}estados_e;
typedef enum{timeOut,null}evento_e;
estados_e estado;
evento_e evento;
TIM_HandleTypeDef htim2;
volatile uint32_t ticki;
volatile uint32_t GCounter=0;
volatile float numeroGlobal=0;
volatile uint32_t flag=0;
volatile int p1=0;
volatile int p2=0;
volatile int p3=0;


void deco(uint32_t numero);
static void MX_TIM2_Init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void onD1(uint32_t numero){
	HAL_GPIO_WritePin(disp1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(disp4, GPIO_PIN_SET);
	deco(numero/1000);
	HAL_GPIO_WritePin(p, p1);
}
void onD2(uint32_t numero){
	HAL_GPIO_WritePin(disp1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(disp2, GPIO_PIN_RESET);
	deco((numero%1000)/100);
	HAL_GPIO_WritePin(p, p2);
}
void onD3(uint32_t numero){
	HAL_GPIO_WritePin(disp2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(disp3, GPIO_PIN_RESET);
	deco((numero%100)/10);
	HAL_GPIO_WritePin(p, p3);
}
void onD4(uint32_t numero){
	HAL_GPIO_WritePin(disp3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(disp4, GPIO_PIN_RESET);
	deco(numero%10);
}

void offdisplays(){
	HAL_GPIO_WritePin(disp1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(disp2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(disp3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(disp4, GPIO_PIN_SET);
}

void DisplaysInit(){
	onD1(numeroGlobal);
	estado=Display1;
	evento=null;
	offdisplays();
	MX_TIM2_Init();
	HAL_TIM_Base_Start_IT(&htim2);
}
void DisplaysOFF(){
	offdisplays();
	HAL_TIM_Base_Stop_IT(&htim2);
}

void clear(){
	HAL_GPIO_WritePin(a, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(b, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(c, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(d, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(e, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(f, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(g, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(p, GPIO_PIN_RESET);

}
void cero(){

	HAL_GPIO_WritePin(a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(b, GPIO_PIN_SET);
	HAL_GPIO_WritePin(c, GPIO_PIN_SET);
	HAL_GPIO_WritePin(d, GPIO_PIN_SET);
	HAL_GPIO_WritePin(e, GPIO_PIN_SET);
	HAL_GPIO_WritePin(f, GPIO_PIN_SET);

}
void uno(){

	HAL_GPIO_WritePin(c, GPIO_PIN_SET);
	HAL_GPIO_WritePin(b, GPIO_PIN_SET);
}
void dos(){

	HAL_GPIO_WritePin(a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(b, GPIO_PIN_SET);
	HAL_GPIO_WritePin(d, GPIO_PIN_SET);
	HAL_GPIO_WritePin(e, GPIO_PIN_SET);
	HAL_GPIO_WritePin(g, GPIO_PIN_SET);
}
void tres(){

	HAL_GPIO_WritePin(a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(b, GPIO_PIN_SET);
	HAL_GPIO_WritePin(c, GPIO_PIN_SET);
	HAL_GPIO_WritePin(d, GPIO_PIN_SET);
	HAL_GPIO_WritePin(g, GPIO_PIN_SET);
}
void cuatro(){

	HAL_GPIO_WritePin(b, GPIO_PIN_SET);
	HAL_GPIO_WritePin(c, GPIO_PIN_SET);
	HAL_GPIO_WritePin(f, GPIO_PIN_SET);
    HAL_GPIO_WritePin(g, GPIO_PIN_SET);
}
void cinco(){

	HAL_GPIO_WritePin(a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(f, GPIO_PIN_SET);
	HAL_GPIO_WritePin(c, GPIO_PIN_SET);
	HAL_GPIO_WritePin(d, GPIO_PIN_SET);
	HAL_GPIO_WritePin(g, GPIO_PIN_SET);
}
void seis(){
	HAL_GPIO_WritePin(a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(f, GPIO_PIN_SET);
	HAL_GPIO_WritePin(c, GPIO_PIN_SET);
	HAL_GPIO_WritePin(d, GPIO_PIN_SET);
	HAL_GPIO_WritePin(g, GPIO_PIN_SET);
	HAL_GPIO_WritePin(e, GPIO_PIN_SET);
}
void siete(){

	HAL_GPIO_WritePin(a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(b, GPIO_PIN_SET);
	HAL_GPIO_WritePin(c, GPIO_PIN_SET);
}
void ocho(){
	HAL_GPIO_WritePin(a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(b, GPIO_PIN_SET);
	HAL_GPIO_WritePin(c, GPIO_PIN_SET);
	HAL_GPIO_WritePin(d, GPIO_PIN_SET);
	HAL_GPIO_WritePin(e, GPIO_PIN_SET);
	HAL_GPIO_WritePin(f, GPIO_PIN_SET);
	HAL_GPIO_WritePin(g, GPIO_PIN_SET);
}
void nueve(){
	HAL_GPIO_WritePin(a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(b, GPIO_PIN_SET);
	HAL_GPIO_WritePin(c, GPIO_PIN_SET);
	HAL_GPIO_WritePin(f, GPIO_PIN_SET);
    HAL_GPIO_WritePin(g, GPIO_PIN_SET);
}

void deco(uint32_t numero){
	switch(numero){
	case 0:
		clear();
		cero();
	break;
	case 1:
		clear();
		uno();
	break;
	case 2:
		clear();
		dos();
	break;
	case 3:
		clear();
		tres();
	break;
	case 4:
		clear();
		cuatro();
	break;
	case 5:
		clear();
		cinco();
	break;
	case 6:
		clear();
		seis();
	break;
	case 7:
		clear();
		siete();
	break;
	case 8:
		clear();
		ocho();
	break;
	case 9:
		clear();
		nueve();
	break;
	}
	}

uint32_t acondicionamiento(float numero){
	p1=0;
	p2=0;
	p3=0;
	uint32_t numeroInt=0;

	//Verifico donde poner el decimal
	if(numero>=0 && numero<10){
		numeroInt=numero*1000;				//x.xxx
		p1=1;
	}
	if(numero>=10 && numero<100){			//xx.xx
		numeroInt=numero*100;
		p2=1;
	}
	if(numero>=100 && numero<1000){			//xxx.x
		numeroInt=numero*10;
		p3=1;
	}
	if(numero>=1000 && numero <= 9999){		//xxxx
		numeroInt = numero;
	}
	if(numero>9999){						//saturacion maxima
	numeroInt=9999;
	}
	if(numero<0.001){						//saturacion minima
		numeroInt=0;
	}
	return numeroInt;
}

void periferico_7Segmentos(float numero,estados_e *actual,evento_e evt){

	uint32_t numeroint=acondicionamiento(numero);

	switch(*actual){
	case Display1:
		if(evt==timeOut){
		onD2(numeroint);
		*actual=Display2;
		}
	break;
	case Display2:
		if(evt==timeOut){
		onD3(numeroint);
		*actual=Display3;
		}
	break;
	case Display3:
		if(evt==timeOut){
		onD4(numeroint);
		*actual=Display4;
		}
	break;
	case Display4:
		if(evt==timeOut){
		onD1(numeroint);
		*actual=Display1;
		}
	break;
	}
}

void print_displays_float(float numero){
	numeroGlobal=numero;
}

void print_displays_cad(char *numero){
	uint32_t j=0;
	uint32_t aux=0;
	float numeroRE=0;
	while(numero[j]!='\0'){
		if(numero[j]!='.'){
			numeroRE=numeroRE*10+(float)(numero[j]-'0');
		}
		else{
			aux=j;
		}
		j++;
	}
	if(numeroRE<1000 && numeroRE>=100){
		numeroRE=numeroRE*10;
	}
	if(numeroRE<100 && numeroRE>=10){
		numeroRE=numeroRE*100;
	}
	if(numeroRE<10 && numeroRE>=0){
		numeroRE=numeroRE*1000;
	}
	switch(aux){
	case 0:
		numeroRE=numeroRE;
	break;
	case 1:
		numeroRE=numeroRE/1000;
	break;
	case 2:
		numeroRE=numeroRE/100;
	break;
	case 3:
		numeroRE=numeroRE/10;
	break;
	}
	print_displays_float(numeroRE);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
    	GCounter++;
    	if(GCounter>timeMax){
    		GCounter=0;
    		evento = timeOut;
    	}
    	periferico_7Segmentos(numeroGlobal,&estado,evento);
    	evento=null;
    }
}

static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}
