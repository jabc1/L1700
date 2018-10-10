#ifdef __SRC_SSP
#define EXTERN
#else
#define EXTERN extern
#endif

void  SSP1_Init(void);
unsigned char SSP1_WR(unsigned char dat);
void SSP0_Init(void);
unsigned char SSP0_WR(unsigned char dat);

#undef EXTERN

