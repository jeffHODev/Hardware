/*!
    \file    main.c
    \brief   main flash program, erase

    \version 2014-12-26, V1.0.0, firmware for GD32F10x
    \version 2017-06-20, V2.0.0, firmware for GD32F10x
    \version 2018-07-31, V2.1.0, firmware for GD32F10x
    \version 2020-09-30, V2.2.0, firmware for GD32F10x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f10x.h" 
#include "gd32f10x_eval.h"
#include "main.h" 
#include "bsp_cpu_flash.h"

#define FMC_PAGE_SIZE           ((uint16_t)0x400U)
//#define FMC_WRITE_START_ADDR    ((uint32_t)0x0801FC00 )
//#define FMC_WRITE_END_ADDR      ((uint32_t)0x0801FFFF)


uint32_t address = 0x00;


/* calculate the num of page to be programmed/erased */
uint32_t page_num = (FMC_WRITE_END_ADDR - FLASH_BASE_ADDR+1) / FMC_PAGE_SIZE;
/* calculate the num of page to be programmed/erased */
//uint32_t word_num = ((FMC_WRITE_END_ADDR - FLASH_BASE_ADDR) >> 2);

/*!
    \brief      erase fmc pages from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_erase_pages(uint32_t addr)
{
    uint32_t erase_counter;

    /* unlock the flash program/erase controller */
    fmc_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    
    /* erase the flash pages */
	  addr = addr + FLASH_BASE_ADDR;
    for(erase_counter = 0; erase_counter < page_num; erase_counter++){
        fmc_page_erase(addr + (FMC_PAGE_SIZE * erase_counter));
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    }

    /* lock the main FMC after the erase operation */
    fmc_lock();
}

/*!
    \brief      program fmc word by word from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_program(uint32_t addr ,uint16_t buf)
{
    /* unlock the flash program/erase controller */
   
	address = addr*2 + FLASH_BASE_ADDR;


    /* program flash */
    //while(address < FMC_WRITE_END_ADDR){
        fmc_halfword_program(address, buf);
        //address += 2;
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR); 
  

    /* lock the main FMC after the program operation */
    
}

/*!
    \brief      check fmc erase result
    \param[in]  none
    \param[out] none
    \retval     none
*/


/*!
    \brief      check fmc program result
    \param[in]  none
    \param[out] none
    \retval     none
*/


/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int test(void)
{


//    /* step1: erase pages and check if it is successful. if not, light the LED2 */
//    fmc_erase_pages();

//    /* step2: program and check if it is successful. if not, light the LED3 */
//    fmc_program();
//    fmc_program_check();

    /* if all the operations are successful, light the LED4 */
   // gd_eval_led_on(led_num);

    while(1);
}
