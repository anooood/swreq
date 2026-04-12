/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <FPGA.h>
 *
 *  Description:
 *  ------------
 *  Header file of the FPGA.
 *
 ******************************************************************************/

#ifndef FPGA_H
#define FPGA_H

// FPGA Control and Status registers
#define FPGA_FW_VERSION_ADDR                  0x40000000
#define FPGA_STATUS_REG_ADDR                  (FPGA_FW_VERSION_ADDR + 0x0004)
#define FPGA_CONTROL_REG_ADDR  	 	            (FPGA_FW_VERSION_ADDR + 0x0008)

// SYNC
#define FPGA_SYNC_TIMER_ADDR                  0x40400000
#define FPGA_SYNC_SEEKER_INT_TIME_0_ADDR      (FPGA_SYNC_TIMER_ADDR + 0x00)
#define FPGA_SYNC_SEEKER_INT_TIME_1_ADDR      (FPGA_SYNC_TIMER_ADDR + 0x04)
#define FPGA_SYNC_SEEKER_INT_TIME_2_ADDR      (FPGA_SYNC_TIMER_ADDR + 0x08)
#define FPGA_SYNC_SEEKER_INT_TIME_3_ADDR      (FPGA_SYNC_TIMER_ADDR + 0x0C)
#define FPGA_SYNC_SEEKER_INT_TIME_4_ADDR      (FPGA_SYNC_TIMER_ADDR + 0x10)
#define FPGA_SYNC_SEEKER_INT_TIME_5_ADDR      (FPGA_SYNC_TIMER_ADDR + 0x14)
#define FPGA_SYNC_SEEKER_INT_TIME_6_ADDR      (FPGA_SYNC_TIMER_ADDR + 0x18)
#define FPGA_SYNC_SEEKER_INT_TIME_7_ADDR      (FPGA_SYNC_TIMER_ADDR + 0x1C)
#define FPGA_SYNC_TEL_INT_TIME_0_ADDR         (FPGA_SYNC_TIMER_ADDR + 0x20)
#define FPGA_SYNC_TEL_INT_TIME_1_ADDR         (FPGA_SYNC_TIMER_ADDR + 0x24)
#define FPGA_SYNC_TEL_INT_TIME_2_ADDR         (FPGA_SYNC_TIMER_ADDR + 0x28)
#define FPGA_SYNC_ENABLE_INTERRUPT_ADDR       (FPGA_SYNC_TIMER_ADDR + 0x2C)
#define FPGA_SYNC_CLEAR_INTERRUPT_ADDR        (FPGA_SYNC_TIMER_ADDR + 0x30)
#define FPGA_SYNC_ACTIVE_INTERRUPT_ADDR       (FPGA_SYNC_TIMER_ADDR + 0x34)
#define FPGA_SYNC_SEEKER_TIMER_VALUE_ADDR     (FPGA_SYNC_TIMER_ADDR + 0x38)
#define FPGA_SYNC_TEL_TIMER_VALUE_ADDR        (FPGA_SYNC_TIMER_ADDR + 0x3C)
#define FPGA_SYNC_TEL_VALUE_SET_ADDR          (FPGA_SYNC_TIMER_ADDR + 0x40)
#define FPGA_SYNC_SIGNAL_READ_ADDR            (FPGA_SYNC_TIMER_ADDR + 0x40)

// RAM
#define FPGA_RAM_BLOCK_ADDR                   0x40800000

// Timer
#define FPGA_TIMER_ADDR                       0x40C00000
#define FPGA_TIMER_PERIOD_ADDR                (FPGA_TIMER_ADDR + 0x04)
#define FPGA_TIMER_SET_ADDR                   (FPGA_TIMER_ADDR + 0x08)
#define FPGA_TIMER_LO_WORD_ADDR               (FPGA_TIMER_ADDR + 0x10)
#define FPGA_TIMER_HI_WORD_ADDR               (FPGA_TIMER_ADDR + 0x14)

// 485 BIT
#define FPGA_RS485_TEST_ADDR                  0x41400000
#define FPGA_RS485_STATUS_ADDR                FPGA_RS485_TEST_ADDR
#define FPGA_RS485_INITIATE_ADDR              (FPGA_RS485_TEST_ADDR + 0x08)

// SDLC
#define FPGA_SDLC_IMU_ADDR                    0x41800000

// UARTs
#define FPGA_UART_GNSS_ADDR                   0x42400000
#define FPGA_UART_ADS_ADDR                    0x42800000
#define FPGA_UART_INTERFACE_CARD_ADDR         0x42C00000
#define FPGA_UART_FTS_ADDR                    0x43000000
#define FPGA_UART_MPSAL_ADDR                  0x43000000  // base address of MPSAL UART
#define FPGA_UART_BATTERY_PACK_ADDR           0x43400000

// PLD Control Interface
#define FPGA_PLD_CONTROL_INTERFACE_ADDR       0x43800000

// Arcnet
#define FPGA_ARCNET_1_ADDR                    0x44400000
#define FPGA_ARCNET_2_ADDR                    0x44800000

// GPIO
#define FPGA_GPIO_ADDR                        0x45400000
#define FPGA_GPIO_ONE_WIRE_ADDR               FPGA_GPIO_ADDR
#define FPGA_GPIO_I2C_1_ADDR                  (FPGA_GPIO_ADDR + 0x08000)
#define FPGA_GPIO_I2C_2_ADDR                  (FPGA_GPIO_ADDR + 0x10000)
#define FPGA_GPIO_I2C_3_ADDR                  (FPGA_GPIO_ADDR + 0x18000)

// Interrupts
#define FPGA_INTERRUPT_ADDR                   0x47C00000
#define FPGA_INTERRUPT_STATUS_ADDR            (FPGA_INTERRUPT_ADDR + 0x00)
#define FPGA_INTERRUPT_CLEAR_ADDR             (FPGA_INTERRUPT_ADDR + 0x00)
#define FPGA_INTERRUPT_MASK_ADDR              (FPGA_INTERRUPT_ADDR + 0x04)
#define FPGA_INTERRUPT_ACTIVE_ADDR            (FPGA_INTERRUPT_ADDR + 0x08)
#define FPGA_INTERRUPT_LEVEL_ADDR             (FPGA_INTERRUPT_ADDR + 0x0C)


#endif
