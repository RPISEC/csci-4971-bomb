#include <stdio.h>
#include "ansi.h"

int wire_yellow = 1;

char buffer[10];

void yellow_preflight()
{
	printf(A(DB,YL) " " RST A(DF,YL) " ENTER UNLOCK PASSWORD 1: " RST);
	fgets(buffer, 10, stdin);
}

void yellow()
{
	yellow_preflight();
	
	if (buffer[0] != '8' ||
	    buffer[1] != '4' ||
	    buffer[2] != '3' ||
	    buffer[3] != '7' ||
	    buffer[4] != '1' ||
	    buffer[5] != '0' ||
	    buffer[6] != '6' ||
	    buffer[7] != '5')
	{
		wire_yellow *= 1024;
		return;
	}
	
	printf(A(DB,YL) " " RST A(DF,YL) " UNLOCK PASSWORD 1 ACCEPTED, LOCK DISENGAGED" RST "\n");
	wire_yellow = 0;
}