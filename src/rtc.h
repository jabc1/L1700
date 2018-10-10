#ifdef __SRC_RTC
#define EXTERN
#else
#define EXTERN		extern
#endif

typedef struct
{
	unsigned int RTC_Sec;     /* Second value - [0,59] */
	unsigned int  RTC_Min;     /* Minute value - [0,59] */
	unsigned int  RTC_Hour;    /* Hour value - [0,23] */
	unsigned int  RTC_Mday;    /* Day of the month value - [1,31] */
	unsigned int  RTC_Mon;     /* Month value - [1,12] */
	unsigned int  RTC_Year;    /* Year value - [0,4095] */
	unsigned int  RTC_Wday;    /* Day of week value - [0,6] */
	unsigned int  RTC_Yday;    /* Day of year value - [1,365] */
}RTCTime;

#define IMSEC		0x00000001
#define IMMIN		0x00000002
#define IMHOUR		0x00000004
#define IMDOM		0x00000008
#define IMDOW		0x00000010
#define IMDOY		0x00000020
#define IMMON		0x00000040
#define IMYEAR		0x00000080

#define AMRSEC		0x00000001  /* Alarm mask for Seconds */
#define AMRMIN		0x00000002  /* Alarm mask for Minutes */
#define AMRHOUR		0x00000004  /* Alarm mask for Hours */
#define AMRDOM		0x00000008  /* Alarm mask for Day of Month */
#define AMRDOW		0x00000010  /* Alarm mask for Day of Week */
#define AMRDOY		0x00000020  /* Alarm mask for Day of Year */
#define AMRMON		0x00000040  /* Alarm mask for Month */
#define AMRYEAR		0x00000080  /* Alarm mask for Year */

#define ILR_RTCCIF	0x01
#define ILR_RTCALF	0x02

#define CCR_CLKEN	0x01
#define CCR_CCALEN	0x02

void RTCInit(void);
void RTCStart(void);
void RTCStop(void);
void  RTCGetTime(RTCTime *LocalTime) ;
void RTCSetTime(RTCTime);
EXTERN RTCTime SystemTime;
EXTERN unsigned char SystemTimeCal;
#undef EXTERN 

