#include <string.h>
#include <math.h>
#include "twi/twi-slave.h"
#include "gfx/menu.h"
#include "gfx/gfx_components.h"
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

void updateData(uint8_t data){}


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
}


void printWindow(){
	gfx_window window;
	gfx_window_init(&window,0,0,true,true,10,10);
	gfx_window_draw(&window);
}

void testTWI(){
	sendWritePackage();
	sendReadPackage(updateData);
}


int main(void){
	init();
	initMenu();
//	testTWI();
	while(true)
	{
		menu_handler();
	int returnToMenuCount = 0;
	}

}
