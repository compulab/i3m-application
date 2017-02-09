#ifndef _H_SYNTAX_
#define _H_SYNTAX_

#define array_foreach(member_type, array, index)	\
	for (int index = 0; index < sizeof(array) / sizeof(member_type); index++)

#endif
