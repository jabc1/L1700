#include "type.h"
void delayms(uint32_t ms)
{
     uint32_t i;
	 for(i=0;i<ms*10300;i++);
}
void delayus(uint32_t us)
{
     uint32_t i;
	 for(i=0;i<us*10;i++);
}

