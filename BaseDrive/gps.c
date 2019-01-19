#include "gps.h"



const u32 BAUD_id[9]={4800,9600,19200,38400,57600,115200,230400,460800,921600};

u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//Óöµ½'*'»òÕß·Ç·¨×Ö·û,Ôò²»´æÔÚµÚcx¸ö¶ººÅ
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}


u32 NMEA_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

int NMEA_Str2num(u8 *buf,u8*dx)
{
	u8 *p=buf;
	u32 ires=0,fres=0;
	u8 ilen=0,flen=0,i;
	u8 mask=0;
	int res;
	while(1) //µÃµ½ÕûÊýºÍÐ¡ÊýµÄ³¤¶È
	{
		if(*p=='-'){mask|=0X02;p++;}//ÊÇ¸ºÊý
		if(*p==','||(*p=='*'))break;//Óöµ½½áÊøÁË
		if(*p=='.'){mask|=0X01;p++;}//Óöµ½Ð¡ÊýµãÁË
		else if(*p>'9'||(*p<'0'))	//ÓÐ·Ç·¨×Ö·û
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//È¥µô¸ººÅ
	for(i=0;i<ilen;i++)	//µÃµ½ÕûÊý²¿·ÖÊý¾Ý
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//×î¶àÈ¡5Î»Ð¡Êý
	*dx=flen;	 		//Ð¡ÊýµãÎ»Êý
	for(i=0;i<flen;i++)	//µÃµ½Ð¡Êý²¿·ÖÊý¾Ý
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	  							 

void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p,*p1,dx;
	u8 len,i,j,slx=0;
	u8 posx;   	 
	p=buf;
	p1=(u8*)strstr((const char *)p,"$GPGSV");
	len=p1[7]-'0';								//µÃµ½GPGSVµÄÌõÊý
	posx=NMEA_Comma_Pos(p1,3); 					//µÃµ½¿É¼ûÎÀÐÇ×ÜÊý
	if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
	for(i=0;i<len;i++)
	{	 
		p1=(u8*)strstr((const char *)p,"$GPGSV");  
		for(j=0;j<4;j++)
		{	  
			posx=NMEA_Comma_Pos(p1,4+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);	//µÃµ½ÎÀÐÇ±àºÅ
			else break; 
			posx=NMEA_Comma_Pos(p1,5+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);//µÃµ½ÎÀÐÇÑö½Ç 
			else break;
			posx=NMEA_Comma_Pos(p1,6+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);//µÃµ½ÎÀÐÇ·½Î»½Ç
			else break; 
			posx=NMEA_Comma_Pos(p1,7+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);	//µÃµ½ÎÀÐÇÐÅÔë±È
			else break;
			slx++;	   
		}   
 		p=p1+1;//ÇÐ»»µ½ÏÂÒ»¸öGPGSVÐÅÏ¢
	}   
}

void NMEA_BDGSV_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p,*p1,dx;
	u8 len,i,j,slx=0;
	u8 posx;   	 
	p=buf;
	p1=(u8*)strstr((const char *)p,"$BDGSV");
	len=p1[7]-'0';								//µÃµ½BDGSVµÄÌõÊý
	posx=NMEA_Comma_Pos(p1,3); 					//µÃµ½¿É¼û±±¶·ÎÀÐÇ×ÜÊý
	if(posx!=0XFF)gpsx->beidou_svnum=NMEA_Str2num(p1+posx,&dx);
	for(i=0;i<len;i++)
	{	 
		p1=(u8*)strstr((const char *)p,"$BDGSV");  
		for(j=0;j<4;j++)
		{	  
			posx=NMEA_Comma_Pos(p1,4+j*4);
			if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_num=NMEA_Str2num(p1+posx,&dx);	//µÃµ½ÎÀÐÇ±àºÅ
			else break; 
			posx=NMEA_Comma_Pos(p1,5+j*4);
			if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_eledeg=NMEA_Str2num(p1+posx,&dx);//µÃµ½ÎÀÐÇÑö½Ç 
			else break;
			posx=NMEA_Comma_Pos(p1,6+j*4);
			if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_azideg=NMEA_Str2num(p1+posx,&dx);//µÃµ½ÎÀÐÇ·½Î»½Ç
			else break; 
			posx=NMEA_Comma_Pos(p1,7+j*4);
			if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_sn=NMEA_Str2num(p1+posx,&dx);	//µÃµ½ÎÀÐÇÐÅÔë±È
			else break;
			slx++;	   
		}   
 		p=p1+1;//ÇÐ»»µ½ÏÂÒ»¸öBDGSVÐÅÏ¢
	}   
}
void NMEA_GNGGA_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GNGGA");
	posx=NMEA_Comma_Pos(p1,6);								//µÃµ½GPS×´Ì¬
	if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,7);								//µÃµ½ÓÃÓÚ¶¨Î»µÄÎÀÐÇÊý
	if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx); 
	posx=NMEA_Comma_Pos(p1,9);								//µÃµ½º£°Î¸ß¶È
	if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);  
}
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx; 
	u8 i;   
	p1=(u8*)strstr((const char *)buf,"$GNGSA");
	posx=NMEA_Comma_Pos(p1,2);								//µÃµ½¶¨Î»ÀàÐÍ
	if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);	
	for(i=0;i<12;i++)										//µÃµ½¶¨Î»ÎÀÐÇ±àºÅ
	{
		posx=NMEA_Comma_Pos(p1,3+i);					 
		if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
		else break; 
	}				  
	posx=NMEA_Comma_Pos(p1,15);								//µÃµ½PDOPÎ»ÖÃ¾«¶ÈÒò×Ó
	if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,16);								//µÃµ½HDOPÎ»ÖÃ¾«¶ÈÒò×Ó
	if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,17);								//µÃµ½VDOPÎ»ÖÃ¾«¶ÈÒò×Ó
	if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);  
}
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;     
	u32 temp;	   
	float rs;  
	p1=(u8*)strstr((const char *)buf,"$GNRMC");//"$GNRMC",¾­³£ÓÐ&ºÍGNRMC·Ö¿ªµÄÇé¿ö,¹ÊÖ»ÅÐ¶ÏGPRMC.
	posx=NMEA_Comma_Pos(p1,1);								//µÃµ½UTCÊ±¼ä
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//µÃµ½UTCÊ±¼ä,È¥µôms
		gpsx->utc.hour=temp/10000;
		gpsx->utc.min=(temp/100)%100;
		gpsx->utc.sec=temp%100;	 	 
	}	
	posx=NMEA_Comma_Pos(p1,3);								//µÃµ½Î³¶È
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->latitude=temp/NMEA_Pow(10,dx+2);	//µÃµ½¡ã
		rs=temp%NMEA_Pow(10,dx+2);				//µÃµ½'		 
		gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//×ª»»Îª¡ã 
	}
	posx=NMEA_Comma_Pos(p1,4);								//ÄÏÎ³»¹ÊÇ±±Î³ 
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);					 
 	posx=NMEA_Comma_Pos(p1,5);								//µÃµ½¾­¶È
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp/NMEA_Pow(10,dx+2);	//µÃµ½¡ã
		rs=temp%NMEA_Pow(10,dx+2);				//µÃµ½'		 
		gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//×ª»»Îª¡ã 
	}
	posx=NMEA_Comma_Pos(p1,6);								//¶«¾­»¹ÊÇÎ÷¾­
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);		 
	posx=NMEA_Comma_Pos(p1,9);								//µÃµ½UTCÈÕÆÚ
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//µÃµ½UTCÈÕÆÚ
		gpsx->utc.date=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=2000+temp%100;	 	 
	} 
}
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GNVTG");							 
	posx=NMEA_Comma_Pos(p1,7);								//µÃµ½µØÃæËÙÂÊ
	if(posx!=0XFF)
	{
		gpsx->speed=NMEA_Str2num(p1+posx,&dx);
		if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);	 	 		//È·±£À©´ó1000±¶
	}
}  

void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
	NMEA_GPGSV_Analysis(gpsx,buf);	//GPGSV½âÎö
	NMEA_BDGSV_Analysis(gpsx,buf);	//BDGSV½âÎö
	NMEA_GNGGA_Analysis(gpsx,buf);	//GNGGA½âÎö 	
	NMEA_GNGSA_Analysis(gpsx,buf);	//GPNSA½âÎö
	NMEA_GNRMC_Analysis(gpsx,buf);	//GPNMC½âÎö
	NMEA_GNVTG_Analysis(gpsx,buf);	//GPNTG½âÎö
}

u8 SkyTra_Cfg_Ack_Check(void)
{			 
	u16 len=0,i;
	u8 rval=0;
	while((USART3_RX_STA&0X8000)==0 && len<100)//µÈ´ý½ÓÊÕµ½Ó¦´ð   
	{
		len++;
		delay_ms(5);
	}		 
	if(len<100)   	//³¬Ê±´íÎó.
	{
		len=USART3_RX_STA&0X7FFF;	//´Ë´Î½ÓÊÕµ½µÄÊý¾Ý³¤¶È 
		for(i=0;i<len;i++)
		{
			if(USART3_RX_BUF[i]==0X83)break;
			else if(USART3_RX_BUF[i]==0X84)
			{
				rval=3;
				break;
			}
		}
		if(i==len)rval=2;						//Ã»ÓÐÕÒµ½Í¬²½×Ö·û
	}else rval=1;								//½ÓÊÕ³¬Ê±´íÎó
    USART3_RX_STA=0;							//Çå³ý½ÓÊÕ
	return rval;  
}





u8 SkyTra_Cfg_Prt(u32 baud_id)
{
	SkyTra_baudrate *cfg_prt=(SkyTra_baudrate *)USART3_TX_BUF;
	cfg_prt->sos=0XA1A0;		//Òýµ¼ÐòÁÐ(Ð¡¶ËÄ£Ê½)
	cfg_prt->PL=0X0400;			//ÓÐÐ§Êý¾Ý³¤¶È(Ð¡¶ËÄ£Ê½)
	cfg_prt->id=0X05;		    //ÅäÖÃ²¨ÌØÂÊµÄID 
	cfg_prt->com_port=0X00;			//²Ù×÷´®¿Ú1
	cfg_prt->Baud_id=baud_id;	 	////²¨ÌØÂÊ¶ÔÓ¦±àºÅ
	cfg_prt->Attributes=1; 		  //±£´æµ½SRAM&FLASH
	cfg_prt->CS=cfg_prt->id^cfg_prt->com_port^cfg_prt->Baud_id^cfg_prt->Attributes;
	cfg_prt->end=0X0A0D;        //·¢ËÍ½áÊø·û(Ð¡¶ËÄ£Ê½)
	SkyTra_Send_Date((u8*)cfg_prt,sizeof(SkyTra_baudrate));//·¢ËÍÊý¾Ý¸øSkyTra   
	delay_ms(200);				//µÈ´ý·¢ËÍÍê³É 
	usart3_init(BAUD_id[baud_id]);	//ÖØÐÂ³õÊ¼»¯´®¿Ú3  
	return SkyTra_Cfg_Ack_Check();//ÕâÀï²»»á·´»Ø0,ÒòÎªUBLOX·¢»ØÀ´µÄÓ¦´ðÔÚ´®¿ÚÖØÐÂ³õÊ¼»¯µÄÊ±ºòÒÑ¾­±»¶ªÆúÁË.
} 

u8 SkyTra_Cfg_Tp(u32 width)
{
	u32 temp=width;
	SkyTra_pps_width *cfg_tp=(SkyTra_pps_width *)USART3_TX_BUF;
	temp=(width>>24)|((width>>8)&0X0000FF00)|((width<<8)&0X00FF0000)|((width<<24)&0XFF000000);//Ð¡¶ËÄ£Ê½
	cfg_tp->sos=0XA1A0;		    //cfg header(Ð¡¶ËÄ£Ê½)
	cfg_tp->PL=0X0700;        //ÓÐÐ§Êý¾Ý³¤¶È(Ð¡¶ËÄ£Ê½)
	cfg_tp->id=0X65	;			    //cfg tp id
	cfg_tp->Sub_ID=0X01;			//Êý¾ÝÇø³¤¶ÈÎª20¸ö×Ö½Ú.
	cfg_tp->width=temp;		  //Âö³å¿í¶È,us
	cfg_tp->Attributes=0X01;  //±£´æµ½SRAM&FLASH	
	cfg_tp->CS=cfg_tp->id^cfg_tp->Sub_ID^(cfg_tp->width>>24)^(cfg_tp->width>>16)&0XFF^(cfg_tp->width>>8)&0XFF^cfg_tp->width&0XFF^cfg_tp->Attributes;    	//ÓÃ»§ÑÓÊ±Îª0ns
	cfg_tp->end=0X0A0D;       //·¢ËÍ½áÊø·û(Ð¡¶ËÄ£Ê½)
	SkyTra_Send_Date((u8*)cfg_tp,sizeof(SkyTra_pps_width));//·¢ËÍÊý¾Ý¸øNEO-6M  
	return SkyTra_Cfg_Ack_Check();
}

u8 SkyTra_Cfg_Rate(u8 Frep)
{
	SkyTra_PosRate *cfg_rate=(SkyTra_PosRate *)USART3_TX_BUF;
 	cfg_rate->sos=0XA1A0;	    //cfg header(Ð¡¶ËÄ£Ê½)
	cfg_rate->PL=0X0300;			//ÓÐÐ§Êý¾Ý³¤¶È(Ð¡¶ËÄ£Ê½)
	cfg_rate->id=0X0E;	      //cfg rate id
	cfg_rate->rate=Frep;	 	  //¸üÐÂËÙÂÊ
	cfg_rate->Attributes=0X01;	   	//±£´æµ½SRAM&FLASH	.
	cfg_rate->CS=cfg_rate->id^cfg_rate->rate^cfg_rate->Attributes;//Âö³å¼ä¸ô,us
	cfg_rate->end=0X0A0D;       //·¢ËÍ½áÊø·û(Ð¡¶ËÄ£Ê½)
	SkyTra_Send_Date((u8*)cfg_rate,sizeof(SkyTra_PosRate));//·¢ËÍÊý¾Ý¸øNEO-6M 
	return SkyTra_Cfg_Ack_Check();
}

void SkyTra_Send_Date(u8* dbuf,u16 len)
{
	u16 j;
	for(j=0;j<len;j++)//Ñ­»··¢ËÍÊý¾Ý
	{
		while((USART3->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
		USART3->DR=dbuf[j];  
	}	
}


