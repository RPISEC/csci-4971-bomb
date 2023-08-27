#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ansi.h"

int wire_red = 1;

unsigned r[3];
char buffer[21];

void red_preflight()
{
	// Generate 3 random values based on the time
	r[0] = rand() & ~0x80000000;
	r[1] = rand();
	r[2] = rand();
	
	// Print out values as "clock sync" messages
	int i;
	for(i = 0; i < 3; i ++)
	{
		printf(A(DB,RD) " " RST A(DF, RD) " CLOCK SYNC " RST "%08X\n", r[i]);
		usleep(500000);
	}
	printf(A(DB,RD) " " RST A(DF, RD) " ENTER CLOCK RESYNCHRONIZATION SEQUENCE: " RST);
	
	// Read in the user's response
	fgets(buffer, 21, stdin);
}

void red()
{
	red_preflight();
	
	// Check that their output matches the value we expect
	// for seed = 0, value = 9NNXS2N98LQDAB33N2H
	const char *alphabet = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
	
	int i;
	for (i = 0; i < 19; i ++)
	{
		if (buffer[i] != alphabet[r[2] & 0x1F])
		{
			++ wire_red;
			return;
		}
		
		r[2] = (r[2] >> 5) | ((r[1] & 0x1F) << 27);
		r[1] = (r[1] >> 5) | ((r[0] & 0x1F) << 27);
		r[0] >>= 5;
	}
	
	wire_red = 0;
}

void postred()
{
	// nop
}
