#ifndef MIRACLELIB_H_  /* Include guard */
#define MIRACLELIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <math.h>
#include <time.h>

// mmap parameters
#define SPI_BASE_ADDRESS  0x64000000
#define FIFO_BASE_ADDRESS 0x60000000
#define MEM0_BASE_ADDRESS 0x40000000
#define MAP_SIZE 	  0x400000UL
#define WAVE_SIZE         614400
// SPI commands
#define SPIWRITE 1
#define SPIREAD  0

#define REGMAP 0

// Get number of elements in array
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/* All accessible functions below */

void spi_header_tx(int rw, int xfr_size, int target, int addr, void *mapped_base, int verbose);
void set_sample_rate_tx(double rate, void *mapped_base);
void get_sigs_tx();
void set_sig_tx(char *filename, void *mapped_base);
void write_reg_tx(int reg_num, int value, void *mapped_base);
int read_reg_tx(int reg_num, void *mapped_base);
void safe_off_tx(void *mapped_base);
void sys_off_tx(void *mapped_base);
void sys_on_tx(void *mapped_base);
void stat_tx(void *mapped_base);
int init_tx(void *mapped_base);
void set_pwr_tx(int pwr_ctrl_reg, void *mapped_base);
void on_tx(void *mapped_base);
void off_tx(void *mapped_base);
double get_freq_tx();
void set_freq_tx(double fLO_in, void *mapped_base);
void read_bin_file(char *filename, int *array, int *N);

#endif // MIRACLELIB_H_
