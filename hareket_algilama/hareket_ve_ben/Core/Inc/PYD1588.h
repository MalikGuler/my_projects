/*
 * PYD1588.h
 *
 *  Created on: Jul 30, 2025
 *      Author: malikdev
 */

#ifndef INC_PYD1588_H_
#define INC_PYD1588_H_

//kendinize göre değiştirin
#define PYD_DIRECT_PIN        	GPIO_PIN_0
#define PYD_DIRECT_PORT       	GPIOA
#define PYD_SERIN_PIN         	GPIO_PIN_1
#define PYD_SERIN_PORT        	GPIOA


// Threshold: Bits [24:17]  (0-255) arası sayı tutun
#define PYD_THRESHOLD_SHIFT        17
#define PYD_THRESHOLD(value)       ((value & 0xFF) << PYD_THRESHOLD_SHIFT)

// ------------------------
// Blind Time: Bits [16:13]
#define PYD_BLIND_TIME_SHIFT       13
#define PYD_BLIND_TIME_SHIFT       13

#define PYD_BLIND_TIME_0_5S        (0 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_1S          (1 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_1_5S        (2 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_2S          (3 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_2_5S        (4 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_3S          (5 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_3_5S        (6 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_4S          (7 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_4_5S        (8 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_5S          (9 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_5_5S        (10 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_6S          (11 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_6_5S        (12 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_7S          (13 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_7_5S        (14 << PYD_BLIND_TIME_SHIFT)
#define PYD_BLIND_TIME_8S          (15 << PYD_BLIND_TIME_SHIFT)

// ------------------------
// Pulse Counter: Bits [12:11]
#define PYD_PULSE_CNT_SHIFT        11

#define PYD_PULSE_CNT_1            (0 << PYD_PULSE_CNT_SHIFT)
#define PYD_PULSE_CNT_2            (1 << PYD_PULSE_CNT_SHIFT)
#define PYD_PULSE_CNT_3            (2 << PYD_PULSE_CNT_SHIFT)
#define PYD_PULSE_CNT_4            (3 << PYD_PULSE_CNT_SHIFT)

// ------------------------
// Window Time: Bits [10:9]
#define PYD_WINDOW_TIME_SHIFT      9

#define PYD_WINDOW_TIME_2S         (0 << PYD_WINDOW_TIME_SHIFT)  // 2 seconds
#define PYD_WINDOW_TIME_4S         (1 << PYD_WINDOW_TIME_SHIFT)  // 4 seconds
#define PYD_WINDOW_TIME_6S         (2 << PYD_WINDOW_TIME_SHIFT)  // 6 seconds
#define PYD_WINDOW_TIME_8S         (3 << PYD_WINDOW_TIME_SHIFT)  // 8 seconds
// ------------------------
// Operation Mode: Bits [8:7]
#define PYD_OP_MODE_SHIFT          7

#define PYD_OP_MODE_FORCED         (0 << PYD_OP_MODE_SHIFT)
#define PYD_OP_MODE_INTERRUPT      (1 << PYD_OP_MODE_SHIFT)
#define PYD_OP_MODE_WAKEUP         (2 << PYD_OP_MODE_SHIFT)
#define PYD_OP_MODE_RESERVED       (3 << PYD_OP_MODE_SHIFT)

// ------------------------
// Signal Source: Bits [6:5]
#define PYD_SIGNAL_SRC_SHIFT       5

#define PYD_SIGNAL_SRC_BPF         (0 << PYD_SIGNAL_SRC_SHIFT)
#define PYD_SIGNAL_SRC_LPF         (1 << PYD_SIGNAL_SRC_SHIFT)
#define PYD_SIGNAL_SRC_RESERVED    (2 << PYD_SIGNAL_SRC_SHIFT)
#define PYD_SIGNAL_SRC_TEMP        (3 << PYD_SIGNAL_SRC_SHIFT)

// ------------------------
// Reserved Bits [4:3] -> Must be 2 (binary 10)
#define PYD_RESERVED_BITS_SHIFT    3
#define PYD_RESERVED_BITS          (2 << PYD_RESERVED_BITS_SHIFT)

// ------------------------
// HPF Cut-Off: Bit [2]
#define PYD_HPF_SHIFT              2

#define PYD_HPF_0_4HZ              (0 << PYD_HPF_SHIFT)
#define PYD_HPF_0_2HZ              (1 << PYD_HPF_SHIFT)

// ------------------------
// Reserved Bit [1] -> Must be 0
#define PYD_RESERVED1_BIT          (0 << 1)

// ------------------------
// Count Mode: Bit [0]
#define PYD_COUNT_MODE_BIT         0

#define PYD_COUNT_WITH_SIGN        (0 << PYD_COUNT_MODE_BIT)
#define PYD_COUNT_NO_SIGN          (1 << PYD_COUNT_MODE_BIT)

void pyd_init(uint32_t threshold,
	    uint32_t blind_time,
	    uint32_t pulse_count,
	    uint32_t window_time,
	    uint32_t op_mode,
	    uint32_t signal_src,
	    uint32_t hpf_cutoff,
	    uint32_t count_mode);


void pyd_SI_select(void);
void pyd_SI_unselect(void);
void pyd_write_config(uint32_t config);
uint64_t pyd_read_data(void);
void pyd_delay_us(uint16_t us);










#endif /* INC_PYD1588_H_ */
