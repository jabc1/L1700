#ifdef __SRC_TIMER
#define EXTERN
#else
#define EXTERN	extern
#endif



void Timer1_init(void);
void TIMER1_IRQHandler (void);
void SysTickInit(void);
void SysTick_Handler(void);
#undef EXTERN
