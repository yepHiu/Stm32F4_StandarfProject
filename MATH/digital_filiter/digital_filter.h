/*
	Copyright 2012-2014 Benjamin Vedder	benjamin@vedder.se

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

/*
 * digital_filter.h
 *
 *  Created on: 24 nov 2012
 *      Author: benjamin
 */

#ifndef DIGITAL_FILTER_H_
#define DIGITAL_FILTER_H_

#include <stdint.h>
//filter
typedef struct
{
 float Input_Butter[3];
 float Output_Butter[3];
}Butter_BufferData;


typedef struct
{
  float a[3];
  float b[3];
}Butter_Parameter;

// Functions
void filter_fft(int dir, int m, float *real, float *imag);
void filter_dft(int dir, int len, float *real, float *imag);
void filter_fftshift(float *data, int len);
void filter_hamming(float *data, int len);
void filter_zeroPad(float *data, float *result, int dataLen, int resultLen);
void filter_create_fir_lowpass(float *filter_vector, float f_break, int bits, int use_hamming);
float filter_run_fir_iteration(float *vector, float *filter, int bits, uint32_t offset);
void filter_add_sample(float *buffer, float sample, int bits, uint32_t *offset);
float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter);
void Set_Cutoff_Frequency(float sample_frequent, float cutoff_frequent,Butter_Parameter *LPF);

#endif /* DIGITAL_FILTER_H_ */
