/* Created by William Rabbermann */
/* Refractored by Songli Yu */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../util.h"

#define BUFSIZE 15
#define ITEMS 3 // volube, db, mute

const char *
alsa_master_vol(void)
{
	char recorder[ITEMS][BUFSIZE];
	int isrecord = 0;
	int i; // inner pointer
	int j = 0; // outer pointer

	FILE *fp = popen("amixer -M get Master | tail -n 1", "r"); // Get the last line of command output.
	char ch;

	while ((ch = fgetc(fp)) != EOF && i < BUFSIZE && j < ITEMS) {
		if ('[' == ch) {
			isrecord = 1;
			i = 0;
			continue; // Don't record the '['
		} else if (']' == ch) {
			recorder[j][i] = '\0';
			j++;
			isrecord = 0;
		}
		if (isrecord) {
			recorder[j][i++] = ch;
		}
	}
	pclose(fp);

	// If equal, the strcmp() will return 0;
	// So here if not equal, the if will capture.
	if (strcmp("on", recorder[ITEMS - 1])) // If not equal.
		return bprintf("MUTE");
	return bprintf("%s", recorder[0]);
}
