/* Loads signal vector from SD card */

#include "miraclelib.h"

int main(int argc, char *argv[])
{
	char filename[100];
	int memfd;
	void *mapped_base;

	strcpy(filename,"/mnt/");
	strcat(filename,argv[1]);
	strcat(filename,".bin");

	memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if (memfd == -1) {
		printf("Can't open /dev/mem.\n");
		exit(0);
	}

	// Map one page of memory into user space such that the device is in that page, but it may not
	// be at the start of the page

	mapped_base = mmap( (void *) MEM0_BASE_ADDRESS, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, memfd, FIFO_BASE_ADDRESS);
	if (mapped_base == (void *) -1) {
		printf("Can't map the memory to user space.\n");
		exit(0);
	}
	
	/* ----------- INSERT CODE BELOW THIS LINE ------------ */

	set_sig_tx(filename, mapped_base);

	/* ----------- INSERT CODE ABOVE THIS LINE ------------ */
	if (munmap(mapped_base, MAP_SIZE) == -1) {
		printf("Can't unmap memory from user space.\n");
		exit(0);
	}
	close(memfd);
	return 0;
}
