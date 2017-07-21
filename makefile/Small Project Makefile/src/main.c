#include <stdio.h>

#include "square.h"
#include "sphere.h"

int main (int argc, char **argv)
{
	double square_side = 2.00;
	double sphere_rad  = 9.30;

	printf ("Area of a square with side lenght of %f is %f\n", 
				square_side, square_area (square_side));

	printf ("Surface area of a sphere with radius of %f is %f\n", 
				sphere_rad, sphere_area (sphere_rad));

	return 0;
}
