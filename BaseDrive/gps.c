#include "gps.h"
nmea_msg gpsx; 
const u32 BAUD_id[9]={4800,9600,19200,38400,57600,115200,230400,460800,921600};
u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
        u8 *p=buf;
        while(cx)
        {		 
                if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;
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
        while(1) 
        {
                if(*p=='-'){mask|=0X02;p++;}
                if(*p==','||(*p=='*'))break;
                if(*p=='.'){mask|=0X01;p++;}
                else if(*p>'9'||(*p<'0'))
                {	
                        ilen=0;
                        flen=0;
                        break;
                }	
                if(mask&0X01)flen++;
                else ilen++;
                p++;
        }
        if(mask&0X02)buf++;
        for(i=0;i<ilen;i++)
        {  
                ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
        }
        if(flen>5)flen=5;
        *dx=flen;
        for(i=0;i<flen;i++)	
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
        len=p1[7]-'0';	
        posx=NMEA_Comma_Pos(p1,3);
        if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
        for(i=0;i<len;i++)
        {	 
                p1=(u8*)strstr((const char *)p,"$GPGSV");  
                for(j=0;j<4;j++)
                {	  
                        posx=NMEA_Comma_Pos(p1,4+j*4);
                        if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);
                        else break; 
                        posx=NMEA_Comma_Pos(p1,5+j*4);
                        if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);
                        else break;
                        posx=NMEA_Comma_Pos(p1,6+j*4);
                        if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);
                        else break; 
                        posx=NMEA_Comma_Pos(p1,7+j*4);
                        if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);
                        else break;
                        slx++;	   
                }   
                p=p1+1;
        }   
}
void NMEA_BDGSV_Analysis(nmea_msg *gpsx,u8 *buf)
{
        u8 *p,*p1,dx;
        u8 len,i,j,slx=0;
        u8 posx;   	 
        p=buf;
        p1=(u8*)strstr((const char *)p,"$BDGSV");
        len=p1[7]-'0';
        posx=NMEA_Comma_Pos(p1,3); 
        if(posx!=0XFF)gpsx->beidou_svnum=NMEA_Str2num(p1+posx,&dx);
        for(i=0;i<len;i++)
        {	 
                p1=(u8*)strstr((const char *)p,"$BDGSV");  
                for(j=0;j<4;j++)
                {	  
                        posx=NMEA_Comma_Pos(p1,4+j*4);
                        if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_num=NMEA_Str2num(p1+posx,&dx);
                        else break; 
                        posx=NMEA_Comma_Pos(p1,5+j*4);
                        if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_eledeg=NMEA_Str2num(p1+posx,&dx);
                        else break;
                        posx=NMEA_Comma_Pos(p1,6+j*4);
                        if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_azideg=NMEA_Str2num(p1+posx,&dx);
                        else break; 
                        posx=NMEA_Comma_Pos(p1,7+j*4);
                        if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_sn=NMEA_Str2num(p1+posx,&dx);
                        else break;
                        slx++;	   
                }   
                p=p1+1;
        }   
}
void NMEA_GNGGA_Analysis(nmea_msg *gpsx,u8 *buf)
{
        u8 *p1,dx;			 
        u8 posx;    
        p1=(u8*)strstr((const char *)buf,"$GNGGA");
        posx=NMEA_Comma_Pos(p1,6);
        if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);	
        posx=NMEA_Comma_Pos(p1,7);
        if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx); 
        posx=NMEA_Comma_Pos(p1,9);
        if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);  
}
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf)
{
        u8 *p1,dx;			 
        u8 posx; 
        u8 i;   
        p1=(u8*)strstr((const char *)buf,"$GNGSA");
        posx=NMEA_Comma_Pos(p1,2);
        if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);	
        for(i=0;i<12;i++)
        {
                posx=NMEA_Comma_Pos(p1,3+i);					 
                if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
                else break; 
        }				  
        posx=NMEA_Comma_Pos(p1,15);
        if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);  
        posx=NMEA_Comma_Pos(p1,16);
        if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);
        posx=NMEA_Comma_Pos(p1,17);						
        if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);  
}
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
        u8 *p1,dx;			 
        u8 posx;     
        u32 temp;	   
        float rs;  
        p1=(u8*)strstr((const char *)buf,"$GNRMC");
        posx=NMEA_Comma_Pos(p1,1);							
        if(posx!=0XFF)
        {
                temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);
                gpsx->utc.hour=temp/10000;
                gpsx->utc.min=(temp/100)%100;
                gpsx->utc.sec=temp%100;	 	 
        }	
        posx=NMEA_Comma_Pos(p1,3);			// weidu	
        if(posx!=0XFF)
        {
                temp=NMEA_Str2num(p1+posx,&dx);		 	 
                gpsx->latitude=temp/NMEA_Pow(10,dx+2);
                rs=temp%NMEA_Pow(10,dx+2);		
                gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;
        }
        posx=NMEA_Comma_Pos(p1,4);					//N or S
        if(posx!=0XFF)gpsx->nshemi=*(p1+posx);					 
        posx=NMEA_Comma_Pos(p1,5);						//longitude
        if(posx!=0XFF)
        {												  
                temp=NMEA_Str2num(p1+posx,&dx);		 	 
                gpsx->longitude=temp/NMEA_Pow(10,dx+2);	
                rs=temp%NMEA_Pow(10,dx+2);				
                gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;
        }
        posx=NMEA_Comma_Pos(p1,6);					// E or W
        if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);		 
        posx=NMEA_Comma_Pos(p1,9);
        if(posx!=0XFF)
        {
                temp=NMEA_Str2num(p1+posx,&dx);	
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
        posx=NMEA_Comma_Pos(p1,7);					
        if(posx!=0XFF)
        {
                gpsx->speed=NMEA_Str2num(p1+posx,&dx);
                if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);
        }
}  
void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
        NMEA_GPGSV_Analysis(gpsx,buf);
        NMEA_BDGSV_Analysis(gpsx,buf);
        NMEA_GNGGA_Analysis(gpsx,buf);
        NMEA_GNGSA_Analysis(gpsx,buf);
        NMEA_GNRMC_Analysis(gpsx,buf);
        NMEA_GNVTG_Analysis(gpsx,buf);
}
void DataInter_GPS(void)
{
        uint16_t rxlen;
        uint16_t i;
        if(USART3_RX_STA&0X8000)		//接收到一次数据了
        {
                rxlen=USART3_RX_STA&0X7FFF;	//得到数据长度
                for(i=0;i<rxlen;i++)USART3_TempBUF[i]=USART3_RX_BUF[i];	   
                USART3_TempBUF[i]=0;			//自动添加结束符
                GPS_Analysis(&gpsx,(u8*)USART3_TempBUF);//分析字符串
                USART3_RX_STA=0;		   	//启动下一次接收
        } 
}



