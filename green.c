#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "ansi.h"

int wire_green = 1;

const char password[] = "dcaotdae";

void green_preflight(char *buffer)
{
	printf(A(DB,GR) " " RST A(DF,GR) " ENTER UNLOCK PASSWORD 2: " RST);
	fgets(buffer, 20, stdin);
}

void green()
{
	// I put these in a struct because GCC was re-arranging them on the stack
	// ... probably just some flag I need to set.
	struct
	{
		char buffer[12];
		int locked;
	} lock;
	lock.locked = 1;
	
	green_preflight(lock.buffer);
		
	if (strncmp(password, lock.buffer, strlen(password)) == 0)
	{
		printf(A(DB,GR) " " RST A(DF,GR) " UNLOCK PASSWORD 2 ACCEPTED, LOCK DISENGAGED" RST "\n");
		lock.locked = ~lock.locked & 1;
		usleep(500000);
		printf(A(DB,GR) " " RST A(DF,GR) " ACTION OVERRIDDEN FROM USER NOIZEV, LOCK RE-ENGAGED" RST "\n");
		lock.locked = ~lock.locked & 1;
	}
	else
	{
		// Blow the bomb up! Wee
		wire_green <<= 1;
	}
	
	if (!lock.locked)
		wire_green >>= 1;
}