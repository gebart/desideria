/* *** Generated with Desideria SVD converter *** */
extern "C" {
using isr_func = void();
void isr_unused();
void isr_CLIC_INT_SFT();
void isr_CLIC_INT_TMR();
void isr_CLIC_INT_BWEI();
void isr_CLIC_INT_PMOVI();
void isr_WWDGT();
void isr_LVD();
void isr_Tamper();
void isr_RTC();
void isr_FMC();
void isr_RCU();
void isr_EXTI_Line0();
void isr_EXTI_Line1();
void isr_EXTI_Line2();
void isr_EXTI_Line3();
void isr_EXTI_Line4();
void isr_DMA0_Channel0();
void isr_DMA0_Channel1();
void isr_DMA0_Channel2();
void isr_DMA0_Channel3();
void isr_DMA0_Channel4();
void isr_DMA0_Channel5();
void isr_DMA0_Channel6();
void isr_ADC0_1();
void isr_CAN0_TX();
void isr_CAN0_RX0();
void isr_CAN0_RX1();
void isr_CAN0_EWMC();
void isr_EXTI_line9_5();
void isr_TIMER0_BRK();
void isr_TIMER0_UP();
void isr_TIMER0_TRG_CMT();
void isr_TIMER0_Channel();
void isr_TIMER1();
void isr_TIMER2();
void isr_TIMER3();
void isr_I2C0_EV();
void isr_I2C0_ER();
void isr_I2C1_EV();
void isr_I2C1_ER();
void isr_SPI0();
void isr_SPI1();
void isr_USART0();
void isr_USART1();
void isr_USART2();
void isr_EXTI_line15_10();
void isr_RTC_Alarm();
void isr_USBFS_WKUP();
void isr_TIMER4();
void isr_SPI2();
void isr_UART3();
void isr_UART4();
void isr_TIMER5();
void isr_TIMER6();
void isr_DMA1_Channel0();
void isr_DMA1_Channel1();
void isr_DMA1_Channel2();
void isr_DMA1_Channel3();
void isr_DMA1_Channel4();
void isr_CAN1_TX();
void isr_CAN1_RX0();
void isr_CAN1_RX1();
void isr_CAN1_EWMC();
void isr_USBFS();
__attribute__((__used__, __section__(".vectors")))
isr_func *const vector_table[] = {
    isr_unused,          // 0
    isr_unused,          // 1
    isr_unused,          // 2
    isr_CLIC_INT_SFT,    // 3
    isr_unused,          // 4
    isr_unused,          // 5
    isr_unused,          // 6
    isr_CLIC_INT_TMR,    // 7
    isr_unused,          // 8
    isr_unused,          // 9
    isr_unused,          // 10
    isr_unused,          // 11
    isr_unused,          // 12
    isr_unused,          // 13
    isr_unused,          // 14
    isr_unused,          // 15
    isr_unused,          // 16
    isr_CLIC_INT_BWEI,   // 17
    isr_CLIC_INT_PMOVI,  // 18
    isr_WWDGT,           // 19
    isr_LVD,             // 20
    isr_Tamper,          // 21
    isr_RTC,             // 22
    isr_FMC,             // 23
    isr_RCU,             // 24
    isr_EXTI_Line0,      // 25
    isr_EXTI_Line1,      // 26
    isr_EXTI_Line2,      // 27
    isr_EXTI_Line3,      // 28
    isr_EXTI_Line4,      // 29
    isr_DMA0_Channel0,   // 30
    isr_DMA0_Channel1,   // 31
    isr_DMA0_Channel2,   // 32
    isr_DMA0_Channel3,   // 33
    isr_DMA0_Channel4,   // 34
    isr_DMA0_Channel5,   // 35
    isr_DMA0_Channel6,   // 36
    isr_ADC0_1,          // 37
    isr_CAN0_TX,         // 38
    isr_CAN0_RX0,        // 39
    isr_CAN0_RX1,        // 40
    isr_CAN0_EWMC,       // 41
    isr_EXTI_line9_5,    // 42
    isr_TIMER0_BRK,      // 43
    isr_TIMER0_UP,       // 44
    isr_TIMER0_TRG_CMT,  // 45
    isr_TIMER0_Channel,  // 46
    isr_TIMER1,          // 47
    isr_TIMER2,          // 48
    isr_TIMER3,          // 49
    isr_I2C0_EV,         // 50
    isr_I2C0_ER,         // 51
    isr_I2C1_EV,         // 52
    isr_I2C1_ER,         // 53
    isr_SPI0,            // 54
    isr_SPI1,            // 55
    isr_USART0,          // 56
    isr_USART1,          // 57
    isr_USART2,          // 58
    isr_EXTI_line15_10,  // 59
    isr_RTC_Alarm,       // 60
    isr_USBFS_WKUP,      // 61
    isr_unused,          // 62
    isr_unused,          // 63
    isr_unused,          // 64
    isr_unused,          // 65
    isr_unused,          // 66
    isr_unused,          // 67
    isr_unused,          // 68
    isr_TIMER4,          // 69
    isr_SPI2,            // 70
    isr_UART3,           // 71
    isr_UART4,           // 72
    isr_TIMER5,          // 73
    isr_TIMER6,          // 74
    isr_DMA1_Channel0,   // 75
    isr_DMA1_Channel1,   // 76
    isr_DMA1_Channel2,   // 77
    isr_DMA1_Channel3,   // 78
    isr_DMA1_Channel4,   // 79
    isr_unused,          // 80
    isr_unused,          // 81
    isr_CAN1_TX,         // 82
    isr_CAN1_RX0,        // 83
    isr_CAN1_RX1,        // 84
    isr_CAN1_EWMC,       // 85
    isr_USBFS,           // 86
};
static_assert(sizeof(vector_table) == sizeof(isr_func *) * 87);
}  // extern "C"
