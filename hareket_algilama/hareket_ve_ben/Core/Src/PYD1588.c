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

    pyd_delay_us(2400);
}


void pyd_SI_select(void)
{
    HAL_GPIO_WritePin(PYD_SERIN_PORT, PYD_SERIN_PIN, GPIO_PIN_RESET);
    pyd_delay_us(650);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // DIRECT_LINK pinini çıkış yap ve LOW çek
    GPIO_InitStruct.Pin = PYD_DIRECT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PYD_DIRECT_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(PYD_DIRECT_PORT, PYD_DIRECT_PIN, GPIO_PIN_RESET);

    pyd_delay_us(80);

}

void pyd_SI_unselect(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // DIRECT_LINK pinini tekrar kesmeye al
    GPIO_InitStruct.Pin = PYD_DIRECT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PYD_DIRECT_PORT, &GPIO_InitStruct);

//    __HAL_GPIO_EXTI_CLEAR_IT(PYD_DIRECT_PIN);

    pyd_delay_us(650);
}

void pyd_write_config(uint32_t config)
{
    pyd_SI_select();

    for (int i = 24; i >= 0; i--) {
        // 1. LOW → HIGH geçişi
        HAL_GPIO_WritePin(PYD_SERIN_PORT, PYD_SERIN_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PYD_SERIN_PORT, PYD_SERIN_PIN, GPIO_PIN_SET);


        GPIO_PinState bit = (config & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
        HAL_GPIO_WritePin(PYD_SERIN_PORT, PYD_SERIN_PIN, bit);
        pyd_delay_us(80);
    }

    HAL_GPIO_WritePin(PYD_SERIN_PORT, PYD_SERIN_PIN, GPIO_PIN_RESET);
    pyd_delay_us(650);

    pyd_SI_unselect();
}

uint64_t pyd_read_data(void)
{
    uint64_t data = 0;

    for (int i = 0; i < 40; i++)
    {
        data <<= 1;
        if (HAL_GPIO_ReadPin(PYD_DIRECT_PORT, PYD_DIRECT_PIN))
            data |= 0;

        pyd_delay_us(80);
    }

    return data;
}


void pyd_delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_TIM_Base_Start(&htim2);
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
    HAL_TIM_Base_Stop(&htim2);
}



