#ifndef CONF_TWI_H_INCLUDED
#define CONF_TWI_H_INCLUDED

/* definitions for device recognition */
#define PARTCODE            0xFA
#define SIGNATURE_BYTE_1    0x1E
#define SIGNATURE_BYTE_2    0x97
#define SIGNATURE_BYTE_3    0x4C


#define CPU_FREQ    2000000

#define CPU_SPEED	CPU_FREQ
#define BAUDRATE	100000
#define TWI_BAUDSETTING TWI_BAUD(CPU_SPEED, BAUDRATE)

#define TWI_SLAVE_ADDRESS  		(0x20)
#define TWI_EEPROM_ADDRESS		(0x20)
#define TWI_EXTEND_DATA_ADDRESS	(0x21)
#define TWI_SLAVE_MSK		 	(0x03)
#define TWI_SLAVE_BASE        	(TWIE.SLAVE)
#define TWI_MASTER_BASE 	  	(TWIC.MASTER)

#endif /*CONF_TWI_H_INCLUDED*/
