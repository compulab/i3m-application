#include <string.h>
#include <math.h>
#include "asf.h"
#include "twi/twi-slave.h"
#include "twi/twi-master.h"
#include "gfx/logos.h"
#include "debug.h"
#include "gfx/test/sampleMainMenu.h"
#include "gfx/menu-handler.h"
#include "twi/testTwi.h"
#include "timer/tc.h"
#include "power-state.h"
#include "adc/adc.h"

#define PORT_SetPinsAsInput( _port, _inputMask) ( (_port)->DIRCLR = _inputMask )


bool buttonStatus;
int count = 0;

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

//ISR(PORTF_INT1_vect){
//	leftButtonPressed();
//}
//
//ISR(PORTE_INT0_vect){
//	selectButtonPressed();
//}
//
//ISR(TCC0_OVF_vect){
//	count++;
//}
//

void powerState_interrupts_init(){
	PORTF.INTCTRL =  PORT_INT0LVL_MED_gc;
	PORTF.INT0MASK = PIN0_bm | PIN1_bm |PIN2_bm;
	changePowerState();
}

void enable_interrupts(){
	PMIC.CTRL |= PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	powerState_interrupts_init();
		sei();
}

void updateData(uint8_t data){
	char debug[4];
	CLEAR
	MSG("UPDATED DATA")
	MSG2(data,debug)
}


void power_state_init(){
	uint8_t sreg = SREG;
	uint8_t pinCfg =(uint8_t)  PORT_ISC_BOTHEDGES_gc | PORT_OPC_PULLUP_gc;
	cli();
	PORTCFG.MPCMASK = PIN0_bm | PIN1_bm | PIN2_bm;
	PORTF.PIN0CTRL = pinCfg;
	SREG = sreg;
}

void init(){
	enable_interrupts();
	board_init();
	gfx_mono_init();
	adc_init();
	pmic_init();
	power_state_init();
//	tc_init();
	twi_slave_init();
	twi_master_init();
}

void testTWI(){
	sendWritePackage();
	sendReadPackage(updateData);
}


void initMenu(){
	gfx_action_menu_init(getSampleMenu());
}

int main(void){
	init();
	initMenu();

//	testTWI();
	while(true)
	{
		menu_handler();
		delay_us(100000); //10ms
	}

}
