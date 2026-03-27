#include "adc.h"

#define AD_Val_max 4095
#define N 5
#define NUM 5


float AD_value[NUM][5]={{0},{0},{0},{0},{0}} , adtemp , ad_sum[NUM] , ad_ave[NUM];
float AD_V[NUM], AD_last[NUM] = {0};
float RAW[NUM];
float ad0,ad1,ad2,ad3,ad4 ; //0左后 1左前 2 前中 3 右前 4右后	0，2，4横；1，3竖
//float MAX[NUM] = {2850, 2850, 2500, 2850, 2800};
float MAX[NUM] = {3802, 3800, 3520, 3810, 3805}; //实际情况改值，实际采集的最大值
int8 count = 0;

void Adinductance_init(void) /*初始化*///AD电感
{
    adc_init(ADC_P06, ADC_SYSclk_DIV_2);//
    adc_init(ADC_P05, ADC_SYSclk_DIV_2);//
	adc_init(ADC_P15, ADC_SYSclk_DIV_2);//
    adc_init(ADC_P00, ADC_SYSclk_DIV_2);//
    adc_init(ADC_P14, ADC_SYSclk_DIV_2);//
}

void read_AD(void)
{
	int i,j,k;
	float AD_ONE[5]={0.0,0.0,0.0,0.0,0.0};
	float AD_sum[NUM] = {0};
	for(i=0;i<5;i++)
	{
//  取值
	AD_value[0][i] = adc_once(ADC_P06, ADC_12BIT);//Adaverage(0);
    AD_value[1][i] = adc_once(ADC_P05, ADC_12BIT);//Adaverage(1);
	AD_value[2][i] = adc_once(ADC_P15, ADC_12BIT);//Adaverage(2);
    AD_value[3][i] = adc_once(ADC_P00, ADC_12BIT);//Adaverage(3);
    AD_value[4][i] = adc_once(ADC_P14, ADC_12BIT);//Adaverage(4);      //取五个电感（已平均）
//      AD_value[5][i] = Adaverage(5);
   }

   for(i=0;i<NUM;i++)
   {
       for(j=0;j<4;j++)
           {
               for(k=0;k<4-j;k++)
               {
                   if(AD_value[i][k] > AD_value[i][k+1])   ///从小到大排序
                   {
                       adtemp = AD_value[i][k];
                       AD_value[i][k] = AD_value[i][k+1];
                       AD_value[i][k+1] = adtemp;
                   }
               }
           }
   }
//  取中间三个值平均
   for(i=0;i<NUM;i++)
   {
       ad_sum[i] = AD_value[i][1] + AD_value[i][2] + AD_value[i][3];
       ad_ave[i] = ad_sum[i]/3;
   }
   //把平均值赋给最后一个
   for(i=0;i<NUM;i++)
   {
       AD_value[i][4] = ad_ave[i];
   }
   
//   滑动覆盖
   for(i=0;i<4;i++)
   {
       AD_value[0][i] = AD_value[0][i+1];
       AD_value[1][i] = AD_value[1][i+1];
       AD_value[2][i] = AD_value[2][i+1];
       AD_value[3][i] = AD_value[3][i+1];
       AD_value[4][i] = AD_value[4][i+1];
   }

//五个电感再次求和
   for(i=0;i<5;i++)
   {
       AD_sum[0] += AD_value[0][i];
       AD_sum[1] += AD_value[1][i];
       AD_sum[2] += AD_value[2][i];
       AD_sum[3] += AD_value[3][i];
       AD_sum[4] += AD_value[4][i];
//     AD_sum[5] += AD_value[5][i];
   }
//取平均
   for(i=0;i<NUM;i++)
   {
       AD_V[i] =  AD_sum[i]/NUM;
	   RAW[i] = AD_sum[i]/NUM;
	   if(AD_V[i] > MAX[i]) 
		   AD_V[i] = MAX[i];
       AD_sum[i] = 0;
   }
	 

   for(i=0;i<NUM;i++)
   {   /*************归一化****************/
     AD_ONE[i]=(100*AD_V[i]/MAX[i]);    ///AD_V[i]电感读回值（百分比）
   }
	
   ad0=AD_ONE[0];
   ad1=AD_ONE[1];
   ad2=AD_ONE[2];
   ad3=AD_ONE[3];
   ad4=AD_ONE[4];
	
}
