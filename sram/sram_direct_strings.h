/*
 * sram_direct_strings.h
 *
 * Created: 14-Dec-17 6:19:06 PM
 *  Author: Nikita
 */


#ifndef SRAM_DIRECT_STRINGS_H_
#define SRAM_DIRECT_STRINGS_H_

void init_direct_write_vars(void);
void write_direct(enum i2c_addr_space write_address);

#endif /* SRAM_DIRECT_STRINGS_H_ */