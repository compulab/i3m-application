/*
 * memory.c
 *
 * Created: 12-Dec-17 4:21:27 PM
 *  Author: Nikita
 */
#include "asf.h"

/*
 * Implemented with disabling interrupts.
 */
void *malloc_locked(size_t size)
{
	bool preenabled_interrupts = cpu_irq_is_enabled();
	if (preenabled_interrupts)
		cli();

	void *ret = malloc(size);
	if (preenabled_interrupts)
		sei();
	return ret;
}
