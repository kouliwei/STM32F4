#ifndef GPS_H
#define GPS_H
#include "user.h"
__packed typedef struct  
{										    
        u8 num;		//ÎÀÐÇ±àºÅ
        u8 eledeg;	//ÎÀÐÇÑö½Ç
        u16 azideg;	//ÎÀÐÇ·½Î»½Ç
        u8 sn;		//ÐÅÔë±È		   
}nmea_slmsg; 
__packed typedef struct  
{	
        u8 beidou_num;		//ÎÀÐÇ±àºÅ
        u8 beidou_eledeg;	//ÎÀÐÇÑö½Ç
        u16 beidou_azideg;	//ÎÀÐÇ·½Î»½Ç
        u8 beidou_sn;		//ÐÅÔë±È		   
}beidou_nmea_slmsg; 
__packed typedef struct  
{										    
        u16 year;	//Äê·Ý
        u8 month;	//ÔÂ·Ý
        u8 date;	//ÈÕÆÚ
        u8 hour; 	//Ð¡Ê±
        u8 min; 	//·ÖÖÓ
        u8 sec; 	//ÃëÖÓ
}nmea_utc_time;   	   
__packed typedef struct  
{										    
        u8 svnum;					//¿É¼ûGPSÎÀÐÇÊý
        u8 beidou_svnum;					//¿É¼ûGPSÎÀÐÇÊý
        nmea_slmsg slmsg[12];		//×î¶à12¿ÅGPSÎÀÐÇ
        beidou_nmea_slmsg beidou_slmsg[12];		//ÔÝÇÒËã×î¶à12¿Å±±¶·ÎÀÐÇ
        nmea_utc_time utc;			//UTCÊ±¼ä
        u32 latitude;				//Î³¶È ·ÖÀ©´ó100000±¶,Êµ¼ÊÒª³ýÒÔ100000
        u8 nshemi;					//±±Î³/ÄÏÎ³,N:±±Î³;S:ÄÏÎ³				  
        u32 longitude;			    //¾­¶È ·ÖÀ©´ó100000±¶,Êµ¼ÊÒª³ýÒÔ100000
        u8 ewhemi;					//¶«¾­/Î÷¾­,E:¶«¾­;W:Î÷¾­
        u8 gpssta;					//GPS×´Ì¬:0,Î´¶¨Î»;1,·Ç²î·Ö¶¨Î»;2,²î·Ö¶¨Î»;6,ÕýÔÚ¹ÀËã.				  
        u8 posslnum;				//ÓÃÓÚ¶¨Î»µÄGPSÎÀÐÇÊý,0~12.
        u8 possl[12];				//ÓÃÓÚ¶¨Î»µÄÎÀÐÇ±àºÅ
        u8 fixmode;					//¶¨Î»ÀàÐÍ:1,Ã»ÓÐ¶¨Î»;2,2D¶¨Î»;3,3D¶¨Î»
        u16 pdop;					//Î»ÖÃ¾«¶ÈÒò×Ó 0~500,¶ÔÓ¦Êµ¼ÊÖµ0~50.0
        u16 hdop;					//Ë®Æ½¾«¶ÈÒò×Ó 0~500,¶ÔÓ¦Êµ¼ÊÖµ0~50.0
        u16 vdop;					//´¹Ö±¾«¶ÈÒò×Ó 0~500,¶ÔÓ¦Êµ¼ÊÖµ0~50.0 
        int altitude;			 	//º£°Î¸ß¶È,·Å´óÁË10±¶,Êµ¼Ê³ýÒÔ10.µ¥Î»:0.1m	 
        u16 speed;					//µØÃæËÙÂÊ,·Å´óÁË1000±¶,Êµ¼Ê³ýÒÔ10.µ¥Î»:0.001¹«Àï/Ð¡Ê±	 
}nmea_msg;
__packed typedef struct
{
        u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
        u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0004£» 
        u8 id;             //ID£¬¹Ì¶¨Îª0X05
        u8 com_port;       //COM¿Ú£¬¹Ì¶¨Îª0X00£¬¼´COM1   
        u8 Baud_id;       //²¨ÌØÂÊ£¨0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600£©
        u8 Attributes;     //ÅäÖÃÊý¾Ý±£´æÎ»ÖÃ ,0±£´æµ½SRAM£¬1±£´æµ½SRAM&FLASH£¬2ÁÙÊ±±£´æ
        u8 CS;             //Ð£ÑéÖµ
        u16 end;            //½áÊø·û:0X0D0A  
}SkyTra_baudrate;
__packed typedef struct
{
        u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
        u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0009£» 
        u8 id;             //ID£¬¹Ì¶¨Îª0X08
        u8 GGA;            //1~255£¨s£©,0:disable
        u8 GSA;            //1~255£¨s£©,0:disable
        u8 GSV;            //1~255£¨s£©,0:disable
        u8 GLL;            //1~255£¨s£©,0:disable
        u8 RMC;            //1~255£¨s£©,0:disable
        u8 VTG;            //1~255£¨s£©,0:disable
        u8 ZDA;            //1~255£¨s£©,0:disable
        u8 Attributes;     //ÅäÖÃÊý¾Ý±£´æÎ»ÖÃ ,0±£´æµ½SRAM£¬1±£´æµ½SRAM&FLASH£¬2ÁÙÊ±±£´æ
        u8 CS;             //Ð£ÑéÖµ
        u16 end;            //½áÊø·û:0X0D0A  
}SkyTra_outmsg;
__packed typedef struct
{
        u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
        u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0003£» 
        u8 id;             //ID£¬¹Ì¶¨Îª0X0E
        u8 rate;           //È¡Öµ·¶Î§:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
        u8 Attributes;     //ÅäÖÃÊý¾Ý±£´æÎ»ÖÃ ,0±£´æµ½SRAM£¬1±£´æµ½SRAM&FLASH£¬2ÁÙÊ±±£´æ
        u8 CS;             //Ð£ÑéÖµ
        u16 end;            //½áÊø·û:0X0D0A  
}SkyTra_PosRate;
__packed typedef struct
{
        u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
        u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0007£» 
        u8 id;             //ID£¬¹Ì¶¨Îª0X65
        u8 Sub_ID;         //0X01
        u32 width;        //1~100000(us)
        u8 Attributes;     //ÅäÖÃÊý¾Ý±£´æÎ»ÖÃ ,0±£´æµ½SRAM£¬1±£´æµ½SRAM&FLASH£¬2ÁÙÊ±±£´æ
        u8 CS;             //Ð£ÑéÖµ
        u16 end;            //½áÊø·û:0X0D0A 
}SkyTra_pps_width;
__packed typedef struct
{
        u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
        u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0002£» 
        u8 id;             //ID£¬¹Ì¶¨Îª0X83
        u8 ACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
        u8 CS;             //Ð£ÑéÖµ
        u16 end;            //½áÊø·û 
}SkyTra_ACK;
__packed typedef struct
{
        u16 sos;            //Æô¶¯ÐòÁÐ£¬¹Ì¶¨Îª0XA0A1
        u16 PL;             //ÓÐÐ§Êý¾Ý³¤¶È0X0002£» 
        u8 id;             //ID£¬¹Ì¶¨Îª0X84
        u8 NACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
        u8 CS;             //Ð£ÑéÖµ
        u16 end;            //½áÊø·û 
}SkyTra_NACK;
extern nmea_msg gpsx;
int NMEA_Str2num(u8 *buf,u8*dx);
void GPS_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_BDGSV_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNGGA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf);
u8* data_Little_endian(u8* data,u16 len);
void DataInter_GPS(void);
#endif
