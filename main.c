#include <string.h>
#include <math.h>
#include "asf.h"
#include "twi-slave/twi-xmega.h"
#include "menu.h"

char powerstring[5];
bool leftButtonStatus = false,
rightButtonStatus= false,
selectButtonStatus= false;
bool menuEnable= true;
bool longSelectPressed = false;
long currentPower;

//***ADC configuration
#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0

static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;

	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_ON, ADC_RES_12, ADC_REF_VCC);  //signed, 12-bit resolution, VREF = 1V (band gap)
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SINGLE, 1, 0);
	adc_set_clock_rate(&adc_conf, 100000UL); // 100KHz. Up to 200000UL=200KHz

	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_PIN0, 0); // GAIN = 0.5
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}
//***END of ADC Config

void printLogo(struct gfx_mono_bitmap logo){
	gfx_mono_put_bitmap(&logo, 0, 8);
}

void writeText(char* text,int pageAddr,int colAddr){
	ssd1306_set_page_address(pageAddr);
	ssd1306_set_column_address(colAddr);
	ssd1306_write_text(text);
}

float adc_avg(){
	int i;
	long power_avg =0;
	for (i=0;i<5000;i++){
		adc_start_conversion(&MY_ADC, MY_ADC_CH);  //one conversion begins
		LED_On(LED3_GPIO);
//		adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);
		LED_Off(LED3_GPIO);
		int adca1_result = adc_get_result(&MY_ADC, MY_ADC_CH);
		power_avg=power_avg+adca1_result;
	}
	return power_avg/(i);
}

ISR(TWIC_TWIS_vect)
{
	TWI_interrupt_handler();
}

void enable_interrupts(){
	PMIC.CTRL |= PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
		sei();
}

void formatString(char *str, bool isFloatNumber){
	if (isFloatNumber >0){
		str[2]=str[1];
		str[3]=str[2];
		str[1]='.';
		str[3]=' ';
		str[4]='W';
		str[5]='\0';
	} else {
		str[1]=' ';
		str[2]='W';
		str[3]='\0';
	}
}

void printPower(){
    char resString[7];
    currentPower=round(adc_avg()*0.177); //*0.07731r
    if (currentPower < 0)
    	currentPower=0;
	itoa(currentPower, powerstring,10);//power_result
	ssd1306_clear();
	writeText("Power consumption: ",1,25);
	strcpy(resString,powerstring);
	formatString(resString,currentPower >0);
	writeText(resString,3,55);
}

bool isSelectButtonLongPressed(){
	longSelectPressed = selectButtonStatus && ioport_get_value(FP_OK_BUTTON);
	return longSelectPressed;
}

bool isButtonPressed(bool * lastStatus, uint8_t buttonPos){
	bool currentStatus = ioport_get_value(buttonPos);
	bool result = currentStatus && !(*lastStatus);
	*lastStatus = currentStatus;
	return result;
}

bool isLeftButtonPressed(){
	return isButtonPressed(&leftButtonStatus,FP_LEFT_BUTTON);
}

bool isRightButtonPressed(){
	return isButtonPressed(&rightButtonStatus,FP_RIGHT_BUTTON);
}

bool isSelectButtonPressed(){
	return isButtonPressed(&selectButtonStatus,FP_OK_BUTTON);
}

int main(void){
    enable_interrupts();

    board_init();
	sysclk_init();
	gfx_mono_init();
	adc_init();
	pmic_init();

	gfx_mono_menu_init(&testMenu);

	adc_enable(&MY_ADC);

	int returnToMenuCount = 0;
	while(true) 
	{
		if (menuEnable){
			if (longSelectPressed){


			} else {
				if (isLeftButtonPressed())
					gfx_mono_menu_process_key(&testMenu,GFX_MONO_MENU_KEYCODE_UP);
				else if(isRightButtonPressed())
					gfx_mono_menu_process_key(&testMenu,GFX_MONO_MENU_KEYCODE_DOWN);
				else if(isSelectButtonPressed()){
					uint8_t selection = gfx_mono_menu_process_key(&testMenu,GFX_MONO_MENU_KEYCODE_ENTER);
					menuEnable = false;
					if (selection == 0x00){
						ssd1306_clear();
						printLogo(compulab_logo);
					} else if (selection == 0x05){
						ssd1306_clear();
						printPower();
					} else {
						menuEnable = true;
					}
				}
			}
		} else if (isSelectButtonPressed()){
				if (returnToMenuCount == 1){
					returnToMenuCount=0;
					menuEnable = true;
					ssd1306_clear();
					gfx_mono_menu_init(&testMenu);
				}else
					returnToMenuCount++;
			}
		delay_us(100000); //10ms
		}
		return 0;
	}


