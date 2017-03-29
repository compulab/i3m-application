#ifndef _H_SYNTAX_
#define _H_SYNTAX_

#define array_foreach(member_type, array, index)	\
	for (int index = 0; index < sizeof(array) / sizeof(member_type); index++)

#define array_length(array)	(sizeof(array) / sizeof((array)[0]))

#define list_foreach(member_type, list, index)		\
	for (member_type index = list; index != NULL; index = index->next)

#define BIT_ON(value, bitnum)	((value) & (1 << (bitnum)))

#endif
