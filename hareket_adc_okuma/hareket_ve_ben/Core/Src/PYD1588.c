/*
 * PYD1588.c
 *
 *  Created on: Jul 30, 2025
 *      Author: malikdev
 */

#include "main.h"
#include "stm32l0xx_hal.h"
#include "PYD1588.h"


extern TIM_HandleTypeDef htim2; // kendi timer ayarlarınıza göre değiştirin



void pyd_init(uint32_t threshold,
	    uint32_t blind_time,
	    uint32_t pulse_count,
	    uint32_t window_time,
	    uint32_t op_mode,
	    uint32_t signal_src,
	    uint32_t hpf_cutoff,
	    uint32_t count_mode)
{
    uint32_t config = 0;

    config |= PYD_THRESHOLD(threshold);
    config |= blind_time;
    config |= pulse_count;
    config |= window_time;
    config |= op_mode;
    config |= signal_src;
    config |= PYD_RESERVED_BITS;
    config |= hpf_cutoff;
    config |= PYD_RESERVED1_BIT;
    config |= count_mode;

    pyd_write_config(config);


}



static inline void pyd_set_DL_low(void)
{
    PYD_DIRECT_PORT->BSRR = ((uint32_t)PYD_DIRECT_PIN << 16U);
}

static inline void pyd_set_DL_high(void){

	PYD_DIRECT_PORT->BSRR = (uint32_t)PYD_DIRECT_PIN;

}


void pyd_write_config(uint32_t config)
{

    DL_OUTPUT_MODE();
	pyd_set_DL_low();

    uint32_t mask = 1UL << 24; // 25 bit MSB ilk


    for (int i = 0; i < 25; i++) {
        // 1. LOW → HIGH geçişi
        HAL_GPIO_WritePin(PYD_SERIN_PORT, PYD_SERIN_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PYD_SERIN_PORT, PYD_SERIN_PIN, GPIO_PIN_SET);


        GPIO_PinState bit = (config & mask) ? GPIO_PIN_SET : GPIO_PIN_RESET;
        HAL_GPIO_WritePin(PYD_SERIN_PORT, PYD_SERIN_PIN, bit);
        pyd_delay_us(80);

        mask >>= 1;

    }

    HAL_GPIO_WritePin(PYD_SERIN_PORT, PYD_SERIN_PIN, GPIO_PIN_RESET);
//    pyd_delay_us(650);

    pyd_delay_us(2400);
    //pyd_SI_unselect();

    DL_INPUT_MODE();

}

uint64_t pyd_try_read_data_if_valid(void)
{
    uint64_t data = 0;

    pyd_delay_us(130);


	uint64_t bit_mask = 1ULL << 39; // 40 bit MSB ilk


    for (int i = 0; i < 40; i++)
    {

		DL_OUTPUT_MODE();
		pyd_set_DL_low();
    	pyd_delay_us(1);

    	pyd_set_DL_high();
        pyd_delay_us(1);

        DL_INPUT_MODE();
        pyd_delay_us(5);


    	GPIO_PinState bit = HAL_GPIO_ReadPin(PYD_DIRECT_PORT, PYD_DIRECT_PIN);

    	if(bit == GPIO_PIN_SET)
    	{
            data |= bit_mask;
    	}

        bit_mask >>= 1;


        pyd_delay_us(14); // tBIT < 22µs


    }

    DL_OUTPUT_MODE();
	pyd_set_DL_low();

    pyd_delay_us(160);

    DL_INPUT_MODE();

    return data;
}




void pyd_delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_TIM_Base_Start(&htim2);
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
    HAL_TIM_Base_Stop(&htim2);
}

