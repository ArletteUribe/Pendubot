/*
 * controlador.c
 *
 *  Created on: 10 may 2024
 *
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 */


#include "controlador.h"

/******************************************************************************
 * Global variables:
******************************************************************************/

float k_lqr[4] = {-16.6265,-16.6020,-3.2059 ,-2.1024};
float k_AV = 0.95;
float kT = 0.4006;
float par_motor = 0;
float Vin = 0;

/******************************************************************************
 * Function code:
******************************************************************************/

float LQR(float q1,float q2,float dq1,float dq2){

	par_motor = -1*(k_lqr[0]*q1 + k_lqr[1]*q2 + k_lqr[2]*dq1 + k_lqr[3]*dq2);
	Vin = par_motor/(kT*k_AV);

	return Vin;
}
