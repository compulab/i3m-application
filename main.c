#include <string.h>
#include <math.h>
#include "twi/twi-slave.h"
#include "gfx/gfx_components.h"
#include "debug.h"
#include "gfx/menu-handler.h"
#include "twi/testTwi.h"
#include "timer/tc.h"
#include "power-state/power-state.h"
#include "adc/adc.h"
#include "gfx/gfx_utils.h"
#include "config/cnf_blk_header.h"
#include "gfx/sampleMenus/sampleConfig.h"
#include "gfx/sampleMenus/sampleMainMenu.h"

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

ISR(TCC0_OVF_vect){
	menu_handler();
}

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
	CLEAR
	MSG("Yeah!!!")
}


#define PAGE_SPACING 8

void printitem(){
//	gfx_image image;
//	gfx_image_init(&image,&voltage,0,0,false);
//	gfx_image_draw(&image);
//
//	gfx_label welcome,screen;
//	gfx_label_init(&welcome,"Welcome to ",0,voltage.height+PAGE_SPACING,false);
//	gfx_label_draw(&welcome);
//	gfx_label_init(&screen,"new screen view!",0,voltage.height+2*PAGE_SPACING,false);
//	gfx_label_draw(&screen);
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
	sysclk_init();
	gfx_mono_init();
	adc_init();
	pmic_init();
	power_state_init();
	tc_init();
	twi_slave_init();
	twi_master_init();
}

void testTWI(){
	sendWritePackage();
	sendReadPackage(updateData);
}


void initMenu(){
//	struct cnf_blk * config = pgm_read_ptr(&sampleConfig);
//	struct cnf_blk * config = &sampleConfig;
//	void * mainMenuPgm = pgm_read_byte(&sampleConfig.mainMenu);
//	gfx_action_menu_init(config->mainMenu);
}

int main(void){
	init();
	initMenu();
//	testTWI();
//	printitem();

	while(true)
	{}
}
