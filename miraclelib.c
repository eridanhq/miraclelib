/** MIRACLE API */
#include "miraclelib.h"

/* spi_header_tx write command to memory address mapped_base+200 */
void spi_header_tx(int rw, int xfr_size, int target, int addr, void *mapped_base, int verbose)
{
	/*          ----------------------------------
	 *          |  31 |   30:19  | 18:16  | 15:0 |
	 *          | r/w | xfr_size | target | addr |
	 *          ----------------------------------
	 * bits  parameter
	 *  1    r/w         0 = read, 1 = write
	 *  12   xfr_size    The number of spi_data_frames following the spi_ctrl_frame =  xfr_size + 1
	 *  3    target      0 = system register map
	 *  16   addr        addr = { bank[3:0], addr[11:0] }
	 */

	int headerval;

	headerval = 0x00000000;
	// [31]: read/write
	rw = (rw & 0x1) << 31;
	// [30:19]: number of spi data frames
	xfr_size = (xfr_size & 0xfff) << 19;
	// [18:16]: target structure
	target = (target & 0x7) << 16;
	// [15:0]: addr of register or table
	addr = addr & 0xffff;
	// combine into headerval
	headerval = headerval | rw | xfr_size | target | addr;

	if (verbose) printf("headerval: %x\n", headerval);

	// write header
	*((unsigned int *) (mapped_base)) = headerval;
	return;
}

/** Sets TX baseband signal sample rate. */
void set_sample_rate_tx(double rate, void *mapped_base)
{

	uint64_t val;
	unsigned int val_msb;
	unsigned int val_lsb;

	val = (uint64_t) round(ldexp(rate/160e6,40)); // do rate calc
	val_msb = (val >> 32) & 0xFF; // upper 8 bits
	val_lsb = val & 0xFFFFFFFF; // lower 32 bits
	/*
	   printf("rate: %f\n",rate);
	   printf("val=0x%" PRIX64 "\n", val);
	   printf("MSB: 0x%X\n",val_msb);
	   printf("LSB: 0x%X\n",val_lsb);
	   */

	*((unsigned int *) (mapped_base + (MAP_SIZE - (2<<2)))) = val_lsb; // write rate val LSB to maxaddr - 2
	*((unsigned int *) (mapped_base + (MAP_SIZE - (3<<2)))) = val_msb; // write rate val MSB to maxaddr - 3
}

/** Prints a list of signals located in the root directory of the SD card. */
void get_sigs_tx()
{
	//system("umount /mnt");
	system("mount /dev/mmcblk0p1 /mnt");
	/* Note: following line will throw warning about an escape character. Ignore
	 * this warning.  C does not recognize the escape character, but linux will. */
	system("ls -1 /mnt | grep .bin | sed -e 's/\.bin$//'");        
	system("umount /mnt");
}

/** Loads in a signal file into the FPGA to be streamed across the FMC DDR interface. */
void set_sig_tx(char *filename, void *mapped_base)
{
	int wave[WAVE_SIZE];
	int N, counter;

	
	read_bin_file(filename, wave, &N);	

	// write array to FIFO memory
	for (counter = 0; counter <= N; counter = counter +1)
	{
		*((unsigned int *) (mapped_base+(counter << 2))) = wave[counter];
	}

	*((unsigned int *) (mapped_base+ (MAP_SIZE - (1<<2)))) = N; // write to maxaddr


}

/** Writes register value. */
void write_reg_tx(int reg_num, int value, void *mapped_base)
{
	// create and write spi header
	spi_header_tx(SPIWRITE, 0, REGMAP, reg_num, mapped_base, 0);  // write, 1 frame, regmap, reg_num

	*((unsigned int *) (mapped_base+204)) = (unsigned int) (value); // write register value to next memory location
	return;
}

/** Reads register value. */
int read_reg_tx(int reg_num, void *mapped_base)
{
	spi_header_tx(SPIREAD, 0, REGMAP, reg_num, mapped_base, 0);  // read, 1 frame, regmap, reg_num

	usleep(250); // delay to ensure return value is written to memory
	return(*((unsigned int *) (mapped_base)));
}

/** Safely turns off system supplies with proper sequencing. */
void safe_off_tx(void *mapped_base)
{
	int pwr_supp_reg;

	pwr_supp_reg = read_reg_tx(36, mapped_base); // Get supply state
	write_reg_tx(66,0,mapped_base); // Turn off signal

	pwr_supp_reg = pwr_supp_reg | 0x1; // Turn on filter
	write_reg_tx(36,pwr_supp_reg,mapped_base);
	usleep(100000); // 100ms
	//printf("pwr_supp_reg = %d\n", pwr_supp_reg);

	pwr_supp_reg = pwr_supp_reg & 0x7; // Turn off comparator
	write_reg_tx(36,pwr_supp_reg,mapped_base);
	usleep(100000); // 100ms
	//printf("pwr_supp_reg = %d\n", pwr_supp_reg);

	pwr_supp_reg = pwr_supp_reg & 0x5; // Turn off 20V
	write_reg_tx(36,pwr_supp_reg,mapped_base);
	usleep(100000); // 100ms
	//printf("pwr_supp_reg = %d\n", pwr_supp_reg);

	pwr_supp_reg = pwr_supp_reg & 0x1; // Turn off 5V
	write_reg_tx(36,pwr_supp_reg,mapped_base);
	//printf("pwr_supp_reg = %d\n", pwr_supp_reg);
	return;
}

/** Safely turns off the system by sequencing digitally controlled power supplies. */
void sys_off_tx(void *mapped_base)
{
	int pwr_supp_reg;
	// Get current state of power supplies (pwr_supp_reg)
	pwr_supp_reg = read_reg_tx(36, mapped_base);
	//printf("pwr_supp_reg = %d\n", pwr_supp_reg);
	if (pwr_supp_reg >= 14)
	{
		printf("Turning OFF system...");
		safe_off_tx(mapped_base);
		write_reg_tx(32,0x1,mapped_base); // CS select bit
		write_reg_tx(33,0x000840,mapped_base); // Page DAC0 for TXEN control update.
		write_reg_tx(33,0x059604,mapped_base); // SPI turn on TXENx feature, but turn DAC0 off.
	
		printf("done.\n");
	}
	else if (pwr_supp_reg == 1)
	{
		printf("System is already off.\n");
	}
	else if (pwr_supp_reg == 0)
	{
		printf("CANNOT CONNECT TO MIRACLE. CHECK FMC CONNECTION,\n");
		printf("MAKE SURE MIRACLE POWER IS ON, AND TRY AGAIN.\n");
	}
	else
	{
		printf("SYSTEM IN UNDEFINED STATE\n");
		printf("SAFELY TURNING OFF...");
		safe_off_tx(mapped_base);
		printf("done.\n");
	}
}

/** Safely turns on the system by sequencing digitally controlled power supplies.*/
void sys_on_tx(void *mapped_base)
{
	int pwr_supp_reg;
	
	pwr_supp_reg = read_reg_tx(36,mapped_base); // Check state of power supplies
	//printf("pwr_supp_reg = %d\n", pwr_supp_reg);
	if (pwr_supp_reg == 1 || pwr_supp_reg == 5) // Acceptable power states
	{
		printf("Turning ON system...");
		write_reg_tx(36,5,mapped_base); // ensure 5V supply and filter is on
		usleep(100000); // 100ms
		//printf("pwr_supp_reg = %d\n", pwr_supp_reg);
		write_reg_tx(36,7,mapped_base); // Turn on 25V supply
		usleep(100000); // 100ms
		//printf("pwr_supp_reg = %d\n", pwr_supp_reg);
		write_reg_tx(36,15,mapped_base); // Turn on comparator
		usleep(100000); // 100ms
		//printf("pwr_supp_reg = %d\n", pwr_supp_reg);
		write_reg_tx(36,14,mapped_base); // Turn off filter
		//printf("pwr_supp_reg = %d\n", pwr_supp_reg);
		
		// Enabled AD9173 output
		write_reg_tx(32,0x1,mapped_base); // CS select bit
		write_reg_tx(33,0x000840,mapped_base); // Page DAC0 for TXEN control update.
		write_reg_tx(33,0x05960C,mapped_base); // SPI turn on TXENx feature
		printf("done.\n");
	}
	else if (pwr_supp_reg == 14)
	{
		printf("System is already on.\n");
	}
	else if (pwr_supp_reg == 0) // Cannot connect to interposer
	{
		printf("CANNOT CONNECT TO MIRACLE. CHECK FMC CONNECTION,\n");
		printf("MAKE SURE MIRACLE POWER IS ON, AND TRY AGAIN.\n");
	}
	else // Weird power states.  Ideally should never get to this state
	{
		printf("SYSTEM IN UNDEFINED STATE\n");
		printf("SAFELY TURNING OFF...");
		safe_off_tx(mapped_base);
		printf("done.\n");
		printf("PLEASE TRY AGAIN.\n");
	}
}

/** Reports status of TX. */
void stat_tx(void *mapped_base)
{
	int pwr_supp_val, pwr_ctrl_val;
	double f;
	pwr_supp_val = read_reg_tx(36,mapped_base); // get system power state
	pwr_ctrl_val = read_reg_tx(66,mapped_base); // get power level
	f = get_freq_tx();
	if (pwr_supp_val == 1)
	{
		printf("System is OFF.\n");
	}
	else if (pwr_supp_val == 14)
	{
		printf("System is ON.\n"); 
		printf("Power level (0-32767): %d\n", pwr_ctrl_val);
		printf("TX frequency: %.0lf\n", f);
	}
	else if (pwr_supp_val == 0) // cannot connect to interposer
	{
		printf("CANNOT CONNECT TO MIRACLE. CHECK FMC CONNECTION,\n");
		printf("MAKE SURE MIRACLE POWER IS ON, AND TRY AGAIN.\n");
	}
	else // should never get to this, but just in case turn off safely
	{
		printf("SYSTEM IN UNDEFINED STATE\n");
		printf("SAFELY TURNING OFF...");
		safe_off_tx(mapped_base);
		printf("done.\n");
	}
}

/** Recursive function to return gcd of a and b */
uint64_t gcd(uint64_t a, uint64_t b) 
{ 
	if (b == 0)
		return a;
	else
		return gcd(b, a % b);
}

/** Calculates frequency parameters for AD9173 chip */
void ad9173_freq_calc(uint64_t fLO, uint64_t *A, uint64_t *B, uint64_t *X)
{
	uint64_t fDAC, c1, c2, M, N, Y; // need 64-bit computation

	fDAC = (uint64_t) 9.6e9; // needs to match AD9173 DAC frequency
	c1 = gcd(fLO,fDAC); // compute GCD of fLO and fDAC

	/* DDS modulus works using following equation:
	 * fLO/fDAC = M/N
	 *          = (X + A/B)/(2^48)
	 */

	M =  fLO/c1; // compute rational fraction M/N
	N = fDAC/c1;

	*X = (M << 48)/N; // Computer integer X
	Y = (M << 48) - (*X)*N; // Compute remainder y
	c2 = gcd(Y,N);
	*A = Y/c2; // Compute A/B modulus
	*B = N/c2;

	//printf("X=0x%" PRIX64 "\n", *X);
	//printf("A=0x%" PRIX64 "\n", *A);
	//printf("B=0x%" PRIX64 "\n", *B);
}

/** Change frequencies on the AD9173 chip */
void ad9173_freq(uint64_t fLO, void *mapped_base)
{
	//uint64_t fDAC, c1, c2, M, N, X, Y, A, B; // need 64-bit computation
	uint64_t X, A, B; // need 64-bit computation
	int i;

	ad9173_freq_calc(fLO, &A, &B, &X); // frequency parameter calculations

	// spi words structure
	unsigned int spi_words[]  = {
		0x000840, 			// DAC0 page
		0x011300, 			// Clear DDSM_FTW_LOAD_ACK
		0x011400 | ( X        & 0xFF),  // X: DDSM_FTW[ 7: 0]
		0x011500 | ((X >>  8) & 0xFF),  // X: DDSM_FTW[15: 8]
		0x011600 | ((X >> 16) & 0xFF),  // X: DDSM_FTW[23:16]
		0x011700 | ((X >> 24) & 0xFF),  // X: DDSM_FTW[31:24]
		0x011800 | ((X >> 32) & 0xFF),  // X: DDSM_FTW[39:32]
		0x011900 | ((X >> 40) & 0xFF),  // X: DDSM_FTW[47:40]
		0x011C00, 			// DDSM_NCO_PHASE_OFFSET[ 7:0]
		0x011D00, 			// DDSM_NCO_PHASE_OFFSET[15:8]
		0x012400 | ( B        & 0xFF),	// B: DDSM_ACC_MODULUS[ 7: 0]
		0x012500 | ((B >>  8) & 0xFF),	// B: DDSM_ACC_MODULUS[15: 8]
		0x012600 | ((B >> 16) & 0xFF),	// B: DDSM_ACC_MODULUS[23:16]
		0x012700 | ((B >> 24) & 0xFF),	// B: DDSM_ACC_MODULUS[31:24]
		0x012800 | ((B >> 32) & 0xFF),	// B: DDSM_ACC_MODULUS[39:32]
		0x012900 | ((B >> 40) & 0xFF),	// B: DDSM_ACC_MODULUS[47:40]
		0x012A00 | ( A        & 0xFF),	// A: DDSM_ACC_DELTA[ 7: 0]
		0x012B00 | ((A >>  8) & 0xFF),	// A: DDSM_ACC_DELTA[15: 8]
		0x012C00 | ((A >> 16) & 0xFF),	// A: DDSM_ACC_DELTA[23:16]
		0x012D00 | ((A >> 24) & 0xFF),	// A: DDSM_ACC_DELTA[31:24]
		0x012E00 | ((A >> 32) & 0xFF),	// A: DDSM_ACC_DELTA[39:32]
		0x012F00 | ((A >> 40) & 0xFF),	// A: DDSM_ACC_DELTA[47:40]
		0x011301, 			// update datapath NCO & FTW words
	};

	int n = NELEMS(spi_words);
	write_reg_tx(32,0x1,mapped_base); // CS select bit
	for (i = 0; i<n; i++)
	{
		//printf("%2d: 0x%X\n", i, spi_words[i]);
		write_reg_tx(33,spi_words[i],mapped_base);
		usleep(500); // 500 usec pause
	}

	/*
	// Debug print statements
	printf("GCD of %" PRIu64 " and %" PRIu64 ": %" PRIu64 "\n", fLO, fDAC, c1);
	printf("M=%" PRIu64 " and N=%" PRIu64 "\n", M,N);
	printf("X=0x%" PRIx64 "\n", X);
	printf("M*2^48 = 0x%" PRIx64 "\n", (M<<48));
	printf("X*N    = 0x%" PRIx64 "\n", X*N);
	printf("Y=0x%" PRIx64 "\n", Y);
	printf("GCD of %" PRIu64 " and %" PRIu64 ": %" PRIu64 "\n", Y, N, c2);

	printf("A=0x%" PRIx64 "\n", A);
	printf("B=0x%" PRIx64 "\n", B);

	printf("Size of int64_t: %d bytes\n", sizeof(c1));
	*/
}


/** Resyncs the JESD link*/
void ad9173_resync(void *mapped_base)
{
	// Send sysref pulses from HMC7044
	int i;

	unsigned int spi_words_0[] = {
		0x003BF1, // Enable sync logic
		0x003A00, // Set up sync for monitor mode
		0x003A02, // Set up sync for one-shot sync mode
		// Send SYSREF pulse edges here for alignment
	};
	unsigned int spi_words_1[]  = {
		// -------------- Channel Parameters -----------------
		// Channel 3: AD9173 SYSREF
		0x00E65D, // channel enable, SYNC enable, dynamic startup.
		0x00EA11, // Coarse digital delay [4:0], max val = 0x11
		// Channel 13: FPGA SYSREF
		0x014A5D, // channel enable, SYNC enable, dynamic startup. default'014A71'
		0x0001E0, // reseed request
		0x000160, // stop reseed request
	};

	int n_spi_0 = NELEMS(spi_words_0);
	int n_spi_1 = NELEMS(spi_words_1);
	//int n_spi_2 = NELEMS(spi_words_2);

	write_reg_tx(32,0x1,mapped_base); // CS select bit
	// write to register 33
	for (i = 0; i<n_spi_0; i++)
	{
		write_reg_tx(33,spi_words_0[i],mapped_base);
		usleep(2000); // 1000 usec pause
	}

	usleep(10000); // 10 msec pause
	write_reg_tx(32,0x8,mapped_base); // CS select bit
	// write to register 33
	for (i = 0; i<n_spi_1; i++)
	{
		write_reg_tx(33,spi_words_1[i],mapped_base);
		usleep(2000); // 2000 usec pause
	}
}

/** Initializes the JESD204B link with the AD9173 DDS chip. */
int init_tx(void *mapped_base)
{
	int jesd_phase; // jesd phase readback value
	int done = 0; // done flag
	ad9173_resync(mapped_base); // Sync AD9173 subclass 1
	usleep(100000); //100 msec pause
	write_reg_tx(32,1,mapped_base); // CS to AD9173
	write_reg_tx(33,0x830200,mapped_base); // readback LMFC delay
	jesd_phase = read_reg_tx(34, mapped_base);
}

/** Sets power control register. */
void set_pwr_tx(int pwr_ctrl_reg, void *mapped_base)
{
	// Handle boundaries
	if (pwr_ctrl_reg > 32767) 
	{
		printf("Input is too large.\n\r");
		pwr_ctrl_reg = 32767;
	}
	else if(pwr_ctrl_reg < 0)
	{
		printf("Input is too small.\n\r");
		pwr_ctrl_reg = 0;
	}

	write_reg_tx(66, pwr_ctrl_reg, mapped_base); // set power level
	printf("Power is set to: %d\n", pwr_ctrl_reg);
}


/** Shortcut to transmit max power. */
void on_tx(void *mapped_base)
{
	int pwr_supp_reg;
	// Check state of power supplies
	pwr_supp_reg = read_reg_tx(36,mapped_base);
	if (pwr_supp_reg == 14)
	{
		write_reg_tx(66,32767,mapped_base); // signal gain max
	}
	else
	{
		printf("Supplies not in proper condition\n");
	}

}

/** Shortcut to transmit min power. */
void off_tx(void *mapped_base)
{
	write_reg_tx(66,0,mapped_base); // signal gain 0
}

/** Gets the current LO frequency by reading the FLO file in the home directory. */
double get_freq_tx()
{	
	FILE* file = fopen("FLO","r");
	double f;

	fscanf(file,"%lf",&f);
	return f;

}

/** Sets carrier frequency of MIRACLE TX. */
void set_freq_tx(double fLO_in, void *mapped_base)
{
	int tab_line, fLO_mhz;
	char cmd_buff[50];
	uint64_t fLO;

	fLO = (uint64_t) fLO_in;

	if (fLO > 3700e6) // if input freq is too high, set to 3000
	{
		printf("Freq is too large, calibrating to 3700\n");
		fLO = 3700e6;
	}
	else if (fLO < 600e6) // if input freq is too low, set to 200
	{
		printf("Freq is too small, calibrating to 600\n");
		fLO = 600e6;
	}
	fLO_mhz = (int) (fLO/1e6);
	tab_line = (fLO_mhz+12)/25-24; // calculate line in air param matrix 

	//printf("fLO_mhz: %d\n", fLO_mhz);
	//printf("tab_line: %d\n", tab_line);

	write_reg_tx(64, tab_line, mapped_base); // calibrate
	ad9173_freq(fLO, mapped_base);

	//printf("A=0x%" PRIx64 "\n", A);
	sprintf(cmd_buff, "echo %lf > FLO", fLO_in);
	system(cmd_buff);
	return;
} 



/** Reads binary file from SD card */
void read_bin_file(char *filename, int *array, int *N)
{
	FILE *fp;
	int c, c1, c2, counter;
	unsigned int c3, c4;
	int Continue;

	system("mount /dev/mmcblk0p1 /mnt");        

	counter = 0;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("FILE NOT FOUND\n");
		exit(0);
	}

	Continue = 1;
	while(Continue)
	{

		c1 = fgetc(fp);
		c2 = fgetc(fp);
		c2 = c2 << 8;
		c = c1 + c2;
		c3 = c & (0x0fff);  // bitwise AND 
		c3 = c3 << 16;    // I channel
		c1 = fgetc(fp);
		c2 = fgetc(fp);
		c2 = c2 << 8;
		c  = c1 + c2;
		c4 = c & (0x0fff);  // Q channel
		c4 = c3 + c4;
		array[counter] = c4;
		counter++;

		if (feof(fp))
		{
			//printf("\n\rfeof found\n\r");
			*N = counter-2;
			Continue = 0;
		}
	}
	fclose(fp);
	system("umount /mnt");
	return;
}

