#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>

#include <pthread.h>

#include <sys/types.h>
#include <sys/ptrace.h>

#include "ansi.h"

extern int wire_yellow;
extern int wire_green;
extern int wire_blue;
extern int wire_red;

extern void yellow();
extern void green();
extern void blue();
extern void red();

extern void postred();

char *display[3][11] =
	{
		{ "  ___", "     ", "  ___", "  ___", "     ", "  ___", "  ___", "  ___", "  ___", "  ___", "  ___"},
		{ " /  /", "    /", " ___/", " ___/", " /__/", " /__ ", " /__ ", " /  /", " /__/", " /__/", " /__ "},
		{ "/__/ ", "   / ", "/__  ", "___/ ", "   / ", "___/ ", "/__/ ", "   / ", "/__/ ", "___/ ", "/    "},
	};

/**
 * Computes the time left until the end of class on March 15, 2010.
 * Also seeds the random number generator to the time remaining.
 */
void gettimeleft(uint8_t *hr, uint8_t *min, uint8_t *sec)
{
	// Compute the time left until the end of class
	int32_t diff = 2268675400 - (int32_t)time(NULL);
	if (diff < 0)
		diff = 0;
	
	// Seed the random number generator
	srand(diff);
	
	// Split the time left into constituent parts
	if (hr && min && sec)
	{
		*sec = diff % 60;  diff /= 60;
		*min = diff % 60;  diff /= 60;
		*hr  = diff % 24;  diff /= 24;
	}
	
#warning "DEBUG MODE"
	srand(0);
}

void menu()
{
	// Get the time left for the clock
	uint8_t hr, min, sec;
	gettimeleft(&hr, &min, &sec);
	
	// First row
	printf("----------------------------------------");
	
	printf(AA(BF,YL,BB,YL) " " RST "--" AA(BF,GR,BB,GR) " " RST "--" AA(BF,CY,BB,CY) " " RST "--" AA(BF,RD,BB,RD) " " RST "---" "\n" );
	
	// Second through fourth rows
	int i;
	for (i = 0; i < 4; i ++)
	{
		if (i < 3)
			printf(A(BF,RD)"   %s%s  %s%s  %s%s   " RST,
			       display[i][ hr / 10],
			       display[i][ hr % 10],
			       display[i][min / 10],
			       display[i][min % 10],
			       display[i][sec / 10],
			       display[i][sec % 10]
			);
		else
			printf("     HOURS      MINUTES     SECONDS     ");
		
		printf("%s%c" RST "  ", (wire_yellow ? A(BB,YL) : A(BF,YL)), (wire_yellow ? ' ' : (i == 0 || i == 3) ? '*' : '.'));
		printf("%s%c" RST "  ", (wire_green  ? A(BB,GR) : A(BF,GR)), (wire_green  ? ' ' : (i == 0 || i == 3) ? '*' : '.'));
		printf("%s%c" RST "  ", (wire_blue   ? A(BB,CY) : A(BF,CY)), (wire_blue   ? ' ' : (i == 0 || i == 3) ? '*' : '.'));
		printf("%s%c" RST "  ", (wire_red    ? A(BB,RD) : A(BF,RD)), (wire_red    ? ' ' : (i == 0 || i == 3) ? '*' : '.'));
		printf("\n");
	}
	
	// Last row
	printf("+---------------------------------------");
	printf(AA(BF,YL,BB,YL) " " RST "--" AA(BF,GR,BB,GR) " " RST "--" AA(BF,CY,BB,CY) " " RST "--" AA(BF,RD,BB,RD) " " RST "--+" "\n" );
		
		
	// Menu
	printf("|                                                   |\n");
	printf("|      "
	       A(BF,YL) "," RST
	       "   DR. VON NOIZEMAN'S NUCLEAR BOMB   "
	       A(BF,YL) "," RST
	       "      |\n");
	printf("|     "
	       A(BF,YL) "/" A(DF,YL) "!" A(BF,YL) "\\" RST
	       " AUTHORIZED ACCESS ONLY "
	       A(DF,RD) "-" RST
	       " KEEP OUT "
	       A(BF,YL) "/" A(DF,YL) "!" A(BF,YL) "\\" RST
	       "     |\n");
	printf("|                                                   |\n");
	
	if (wire_red || wire_yellow || wire_blue || wire_green)
	{
		printf("|       "
			   "%s1%s YELLOW " RST
			   "%s2%s GREEN " RST
			   "%s3%s BLUE " RST
			   "%s4%s RED " RST
			   "      |\n",
			   wire_yellow ? A(DF,GR) "[" A(BF,GR) : A(BF,BK) "[", wire_yellow ? A(DF,GR) "]" RST : "]",
			   wire_green  ? A(DF,GR) "[" A(BF,GR) : A(BF,BK) "[", wire_green  ? A(DF,GR) "]" RST : "]",
			   wire_blue   ? A(DF,GR) "[" A(BF,GR) : A(BF,BK) "[", wire_blue   ? A(DF,GR) "]" RST : "]",
			   wire_red    ? A(DF,GR) "[" A(BF,GR) : A(BF,BK) "[", wire_red    ? A(DF,GR) "]" RST : "]"
			  );
	}
	else
	{
		printf("|                     " A(DF,GR) "[" A(BF,GR) "DISARM" A(DF,GR) "]" RST "                      |\n");
	}

	printf("|                                                   |\n");
	printf("+---------------------------------------------------+\n");
}

void *watchred(void *crap)
{	
#warning "Don't forget to update the checksum!"
	const uint32_t checksum = 0xDFFEB544;
	
	// Continuously monitor it
	while (1)
	{
		// Has anything blown up?
		if (wire_red > 1 || wire_blue > 1 || wire_yellow > 1 || wire_green > 1)
		{
			goto __start;
		}
		
		uint32_t a = 1, b = 0;
		uint32_t *ptr = (uint32_t *)&red;
		while (ptr < (uint32_t *)postred)
		{
			a = (a + *ptr) % 65521;
			b = (b + a) % 65521;
			ptr ++;
		}
		
		uint32_t sum = (b << 16) | a;
		
		
		if (sum != checksum)
		{
#warning "FIXME"
			printf("CHECKSUM 0x%08X\n", sum);
			goto __start;
		}
		
		sleep(1);
	}
	
	return NULL;
	
__start:
	putchar('\n');
#warning "TODO: write RST \"\n\n\" A(BB,BK) \"  SOMEONE SET US UP THE BOMB  \" RST \"\n\n\""
#if 0
	__asm__(""
			"movl $0xDEADBEEF, %esp\n"
			"movl $0xCAFEBABE, %ebp\n"
			"xor %eax, %eax\n"
			"call *%eax"
			);
#else
	// Added 26 Aug 2023
	*(int *)0 = 0;
#endif
	return NULL; // shut up GCC
}

void disarm_handler (int sig)
{
	if (wire_blue || wire_green || wire_red || wire_yellow)
	{
		wire_blue = 2;
		return;
	}
	else
	{
		// First row
		printf("----------------------------------------");
		
		printf(AA(BF,YL,BB,YL) " " RST "--" AA(BF,GR,BB,GR) " " RST "--" AA(BF,CY,BB,CY) " " RST "--" AA(BF,RD,BB,RD) " " RST "---" "\n" );
		
		// Second through fourth rows
		int i;
		for (i = 0; i < 4; i ++)
		{
			if (i < 3)
				printf(A(BF,RD)"   %s%s  %s%s  %s%s   " RST,
					   display[i][10],
					   display[i][10],
					   display[i][10],
					   display[i][10],
					   display[i][10],
					   display[i][10]
					   );
			else
				printf("     HOURS      MINUTES     SECONDS     ");
			
			printf("%s%c" RST "  ", A(BF,YL), (i == 0 || i == 3) ? '*' : '.');
			printf("%s%c" RST "  ", A(BF,GR), (i == 0 || i == 3) ? '*' : '.');
			printf("%s%c" RST "  ", A(BF,CY), (i == 0 || i == 3) ? '*' : '.');
			printf("%s%c" RST "  ", A(BF,RD), (i == 0 || i == 3) ? '*' : '.');
			printf("\n");
		}
		
		// Last row
		printf("----------------------------------------");
		printf(AA(BF,YL,BB,YL) " " RST "--" AA(BF,GR,BB,GR) " " RST "--" AA(BF,CY,BB,CY) " " RST "--" AA(BF,RD,BB,RD) " " RST "---" "\n" );
		exit(0);
	}

}

int main(int argc, char *argv[])
{
	char buffer[20];
	
	// Create the bomb thread
	pthread_t watchthread;
	pthread_create(&watchthread, NULL, &watchred, NULL);
	
	// Register a signal handler
	signal(SIGUSR1, &disarm_handler);
	
	int showmenu = 1;
	while (1)
	{
		if (showmenu)
			menu();
		printf(A(BB,BK) " " RST A(BF,BK) " MENU SELECTION: " RST);
		fgets(buffer, 20, stdin);
	
		if (!strcmp(buffer, "1\n") || !strcasecmp(buffer, "yellow\n"))
			yellow();
		else if (!strcmp(buffer, "2\n") || !strcasecmp(buffer, "green\n"))
			green();
		else if (!strcmp(buffer, "3\n") || !strcasecmp(buffer, "blue\n"))
			blue();
		else if (!strcmp(buffer, "4\n") || !strcasecmp(buffer, "red\n"))
			red();
		else if (!strcasecmp(buffer, "exit\n") || !strcasecmp(buffer, "quit\n"))
			break;
		else if (!strcasecmp(buffer, "disarm\n"))
			raise(SIGUSR1);
		else if (buffer[0] == '\n')
		{
			showmenu = 0;
			continue;
		}
		else
			wire_green = 2;
		
		printf(A(BB,BK) " " RST A(BF,BK) " PRESS ENTER TO RETURN TO MENU " RST);
		getchar();
		showmenu = 1;
	}
	
	return 0;
}
