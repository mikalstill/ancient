#include <stdio.h>
#include "x86info.h"

void binary32 (unsigned long value)
{
	int i;

	for(i=0;i<32;i++,value<<=1) {
		output(msg_accumulate, "%c", (1<<31 & value) ? '1' : '0' );

		if(i==23 || i==15 || i==7)
			output(msg_accumulate, " ");
	}
	output(msg_accumulate, "\n");
}

void binary64(unsigned long long value)
{
	binary32 (value>>32);
	output (msg_accumulate, "           ");
	binary32 (value);
}
