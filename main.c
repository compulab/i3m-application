#include <string.h>
#include <math.h>
#include "twi/twi-slave.h"
#include "gfx/gfx_components.h"
#include "debug.h"
#include "gfx/menu-handler.h"
#include "twi/testTwi.h"
#include "timer/tc.h"
#include "adc/adc.h"
#include "config/cnf_blk_components.h"
#include "gfx/gfx_utils.h"
#include "gfx/sampleMenus/sampleConfig.h"
#include "gfx/sampleMenus/sampleMainMenu.h"

ISR(TWIE_TWIS_vect)
{
	TWI_interrupt_handler();
}

ISR(TWIC_TWIM_vect){
	InterruptHandler();
}


ISR(PORTF_INT0_vect){
	changePowerState();
}

ISR(TCC0_OVF_vect){
	menu_handler();
}

void initMenu(){
	loadConfigBlock();
	setMenuById(&presentMenu,0);
	gfx_action_menu_init(presentMenu);
}

void powerState_interrupts_init(){
	PORTF.INTCTRL =  PORT_INT0LVL_MED_gc;
	PORTF.INT0MASK = PIN0_bm | PIN1_bm |PIN2_bm;
	changePowerState();
}



void updateData(uint8_t data){
	MSG_T_N("data updated:" ,data)
	delay_s(3);
	initMenu();
}

void power_state_init(){
	powerState_interrupts_init();
	uint8_t sreg = SREG;
	uint8_t pinCfg =(uint8_t)  PORT_ISC_BOTHEDGES_gc | PORT_OPC_PULLUP_gc;
	cli();
	PORTCFG.MPCMASK = PIN0_bm | PIN1_bm | PIN2_bm;
	PORTF.PIN0CTRL = pinCfg;
	SREG = sreg;
}

void init(){
	board_init();
	sysclk_init();
	gfx_mono_init();
	adc_init();
	pmic_init();
	power_state_init();
	tc_init();
	sei();
	twi_slave_init();
	twi_master_init();
}

void testTWI(){
	sendWritePackage();
	delay_s(1);
	sendReadPackage(updateData);
}



int main(void){
	init();
	initMenu();
//	testTWI();
//	delay_s(5);
//	initMenu();
	PORTC.DIR |= PIN4_bm;
	PORTC.OUT &= ~PIN4_bm;
	while(true)
	{}
}
