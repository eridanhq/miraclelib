/* Prints center frequency to screen. */

#include "miraclelib.h"

int main(int argc, char *argv[])
{
	double f;
	f = get_freq_tx();

	printf("%.0lf\n",f);
	return 0;
}
