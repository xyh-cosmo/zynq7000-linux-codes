#!/bin/bash

reset

CC="/home/xyh/Xilinx/SDK/2017.4/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin/arm-linux-gnueabihf-gcc"

${CC} -c main_ad5628.c
${CC} -c xil_printf.c
${CC} -c xil_assert.c
${CC} -c xspips.c
${CC} -c xspips_sinit.c
${CC} -c xspips_options.c
${CC} -c outbyte.c
${CC} -c xspips_selftest.c
${CC} -c xspips_g.c
${CC} -c xuartps_hw.c

${CC} libxil.a main_ad5628.o xil_printf.o xil_assert.o xspips.c xspips_sinit.o xspips_options.o \
	outbyte.o xspips_selftest.o xspips_g.o xuartps_hw.o -o SPI-test
