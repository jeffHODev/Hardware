/*
 * config_usr.h
 *
 *  Created on: 2022-2-8
 *      Author: HDYL-YF
 */

#ifndef CONFIG_USR_H_
#define CONFIG_USR_H_

	#define SLAVE  0
	#define MASTER 1
	#define ROLE  MASTER


	#define MAX_DIS 100
	#define MIN_DIS 28

	#if ROLE ==MASTER
		#define	GPIO_LED_RED			GPIO_PD7
		#define LED_ON_LEVAL 			0 		//gpio output high voltage to turn on led
		#define ECHO                    GPIO_PC4

		#define CS102_EN			 GPIO_PC1
		#define CS102_T				 GPIO_PC0
		#define M_EN 				 GPIO_PD3
		#define KB                   GPIO_PD2

	#else

		#define	GPIO_LED_RED			GPIO_PB5
		#define LED_ON_LEVAL 			0 		//gpio output high voltage to turn on led
		#define ECHO                    GPIO_PC4
		#define CS102_EN			 GPIO_PC1
		#define CS102_T				 GPIO_PC0
		#define M_EN 				 GPIO_PD3
		#define KB                   GPIO_PD2

#endif

#endif /* CONFIG_USR_H_ */
