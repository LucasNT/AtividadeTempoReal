/* loop.c - Measure the time spend in some computations.

   Copyright (c) 2016, Monaco F. J. - <moanco@icmc.usp.br>

   This file is part of Real-Time Sand Box - RTSB

   RTSB is free software: you can redistribute it and/or modify
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

   Can you optimize this code?

*/


#include <stdio.h>
#include <math.h>
#include "utils.h"

#define MAX_OUTER_LOOP 100
#define MAX_INNER_LOOP 1000
#define PI 3.1416
#define LENGTH 100


const float cos_lookup_table[12] = {
	1.0,
	0.8660254037844387,
	0.5000000000000001,
	6.123233995736766e-17,
	-0.4999999999999998,
	-0.8660254037844387,
	-1.0,
	-0.8660254037844386,
	-0.5000000000000004,
	-1.8369701987210297e-16,
	0.5000000000000001,
	0.8660254037844384
};


/* This function returns length*cos(angle). */

float foo (int length, int angle)
{
	float b;
	b = length * cos_lookup_table[angle/30];
	return b;
}

/* Main program. */

int main ()
{
	int angle;
	float val;
	long int i, j;
	struct timeval now, before, elapsed;
	double average_loop_time, min_loop_time, max_loop_time;

	average_loop_time = 0;
	min_loop_time = average_loop_time;
	max_loop_time = average_loop_time;

	/* Outer loop, to compute averages. */

	for (i = 0; i < MAX_OUTER_LOOP; i++)
	{
		angle = 0;

		/* Get current time before computation work. */

		gettimeofday (&before, NULL);

		/* Exec computation several times. */

		for (j=0; j<MAX_INNER_LOOP; j++)
		{
			val = foo (LENGTH, angle);
			angle = (angle + 30) % 360;	/* 0, 30, 60, 90 ... 330, 0, 30 ... */

#if DEBUG
			fprintf (stdout, "(%lu) %3d %8.3f %u\n",
					i, angle, val, (unsigned int) elapsed.tv_usec);
#endif

		}

		/* Get current time after computation work. */

		gettimeofday (&now, NULL);
		timeval_subtract (&elapsed, &now, &before);


		/* Do some statistics. */

		average_loop_time += elapsed.tv_usec;

		if (i==0)
			min_loop_time = elapsed.tv_usec;
		else
			if (elapsed.tv_usec < min_loop_time)
				min_loop_time = elapsed.tv_usec;
		if (elapsed.tv_usec > max_loop_time)
			max_loop_time = elapsed.tv_usec;

	}

	/* Show statistics. */

	average_loop_time = average_loop_time / MAX_OUTER_LOOP;

	printf ("Average loop time: %u\n", (int) average_loop_time);
	printf ("Minimum loop time: %u\n", (int) min_loop_time);
	printf ("Maximum loop time: %u\n", (int) max_loop_time);

	return 0;
}
