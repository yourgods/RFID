#include "stdafx.h"
#include "steganogram.h"

static double x;
static double y;
static double z;
static double xll;
static int num = 1;

/*ͨ��Euclidean gcd�㷨����key�ĵ���*/
unsigned int inv(unsigned int key)
{
	long n1,n2,q,r,b1,b2,t;
    if(key==0)
    b2=0;
    else
    {
		n1=maxim;
		n2=key;
		b2=1;
		b1=0;
        do {
               r=(n1%n2);
		       q=(n1-r)/n2;
               if(r==0)
			   {
                   if(b2<0)
				       b2=maxim+b2;
			   }
               else
			   {
			       n1=n2;
			       n2=r;
                   t=b2;
                   b2=b1-q*b2;
				   b1=t;
			   }
        } while(r!=0);
      
	}
    return (unsigned long int)b2;
}

/*���ܺ���code*/
void code(CString input,char output[28],unsigned int keybox[7][10])
{
	unsigned int r,x1,x2,x3,x4,kk,t1,t2,a,k,g;

	unsigned int fin[4];
    unsigned int fout[5];
	CString Cinput;
	char foutput[28],finput[16];
	int state;

	k=0;
	state =1;//��Ǽ��ܵ�����
	for(int j=0;j<2;j++)
	{
		if(state==1)
		{
			/*�����һ��Ҫ���ܵ�����*/
			Cinput=input.Mid(0,16);//�������CString��ȡ16������
			memcpy(&finput,Cinput,sizeof(finput));//����ת�����ַ������н��д���

			for(int h=0;h<16;)
			{
				/*����finput[h+t]�е�����Ϊʮ�����Ƶ�����*/
				for (int t=0;t<=3;t++)
				{
					if (finput[h+t]>='A' && finput[h+t]<='F')
					{
						finput[h+t]=finput[h+t]-7;
					}					
				}
				
				fin[k]=(finput[h]-48)<<12;
				fin[k]=fin[k]^(finput[h+1]-48)<<8;
				fin[k]=fin[k]^(finput[h+2]-48)<<4;
				fin[k]=fin[k]^(finput[h+3]-48);
				fin[k]=fin[k];
				
				h+=4;				
			    k++;			
			}		
		}

		/*����ڶ���Ҫ���ܵ�����*/		
	    else
		{
			Cinput=input.Mid(16,12);//�������CString��ȡ12������
			memcpy(&finput,Cinput,sizeof(finput));
			k=0;
			for(int h=0;h<12;)
			{
				/*����finput[h+t]�е�����Ϊʮ�����Ƶ�����*/
				for (int t=0;t<=2;t++)
				{
					if (finput[h+t]>='A' && finput[h+t]<='F')
					{
						finput[h+t]=finput[h+t]-7;
					}					
				}
				
				fin[k]=(finput[h]-48)<<8;
				fin[k]=fin[k]^(finput[h+1]-48)<<4;
				fin[k]=fin[k]^(finput[h+2]-48);				
				fin[k]=fin[k];
				
				h+=3;			
				k++;				
			}		
		}		

		x1=fin[0];x2=fin[1];x3=fin[2];x4=fin[3];
		for(r=1;r<=8;r++)
		{ 
			/* ��64λ�Ŀ���з�������*/
			x1=chengfa(x1,keybox[1][r]);
			x4=chengfa(x4,keybox[4][r]);
			x2=(x2+keybox[2][r])&ones;
			x3=(x3+keybox[3][r])&ones;

			/* MA�ṹ�ĺ��� */
			kk=chengfa(keybox[5][r],(x1^x3));
			t1=chengfa(keybox[6][r],(kk+(x2^x4))&ones);
			t2=(kk+t1)&ones;
			
			/* ����任PI*/
			x1=x1^t1;
			x4=x4^t2;
			a=x2^t2;
			x2=x3^t1;
			x3=a;
		}

		/* ���ת��*/
		fout[1]=chengfa(x1,keybox[1][round+1]);
		fout[4]=chengfa(x4,keybox[4][round+1]);
		fout[2]=(x3+keybox[2][round+1])&ones;
		fout[3]=(x2+keybox[3][round+1])&ones;

         /*�����һ�龭�����ܵõ������ݣ���������飬�γ�16���ַ�����*/	
		if(state==1)
		{
				
			g=0;
			for (int y=1;y<5;)
			{		
				foutput[g]=fout[y]>>12;//��16λ��������fout[y]�ĵ�13-16λ�����ݷֽ����
				foutput[g]+=65;//ת��Ϊ��ĸ�ַ�
				output[g]=(char)foutput[g];
				foutput[g+1]=(fout[y]&gainthree)>>8;//��16λ��������fout[y]�ĵ�9-12λ�����ݷֽ����
				foutput[g+1]+=65;
				output[g+1]=(char)foutput[g+1];
				foutput[g+2]=(fout[y]&gaintwo)>>4;//��16λ��������fout[y]�ĵ�5-8λ�����ݷֽ����
				foutput[g+2]+=65;
				output[g+2]=(char)foutput[g+2];
				foutput[g+3]+=65;
				foutput[g+3]=fout[y]&gainone;//��16λ��������fout[y]�ĵ�1-4λ�����ݷֽ����
				foutput[g+3]+=65;
				output[g+3]=(char)foutput[g+3];	
				
				g+=4;				
				y++;				
			}			
		}

		/*����ڶ��龭�����ܵõ������ݣ���������飬�γ�12���ַ�����*/
		else
		{
			g=16;
			for (int y=1;y<5;)
			{		
				foutput[g]=fout[y]>>10;//��16λ��������fout[y]�ĵ�11-16λ�����ݷֽ����
				foutput[g]+=33;
				output[g]=(char)foutput[g];
				foutput[g+1]=(fout[y]&gain)>>4;//��16λ��������fout[y]�ĵ�5-10λ�����ݷֽ����
				foutput[g+1]+=33;
				output[g+1]=(char)foutput[g+1];
				foutput[g+2]=(fout[y]&gainone);//��16λ��������fout[y]�ĵ�1-4λ�����ݷֽ����
				foutput[g+2]+=33;
				output[g+2]=(char)foutput[g+2];
					
				g+=3;				
				y++;				
			}
		}

		state++;
	}
}

/*����������Կ��keybox[7][10] */
void key(CString cskey,unsigned int keybox[7][10])
{
	char streamkey[16];
	memcpy(&streamkey,cskey,sizeof(streamkey));
	
	unsigned int S[54];
	unsigned int fkey[8];
    int i,j,r;
	
	/*��16��8λ�����ַ�����ת��Ϊ8��16λ�����޷������ݲ�������fkey[0],fkey[1],fkey[2]...fkey[7]*/
	int m=0;
	for(int n=0;n<16;)
	{
		fkey[m]=(unsigned int)((int)streamkey[n]<<8);
		fkey[m]=fkey[m]^(unsigned int)((int)streamkey[n+1]);
		
		n+=2;		
		m++;
			
	}

	/*�γɴ�������ڼ��ܵ�������Կ����Կ��keybox[7][10]*/
    for(i=0;i<8;i++)
	{		
        S[i]=fkey[i];
	}
	
    /* shifts */
    for(i=8;i<54;i++)
    {
		int c;
        if((i+2)%8==0)/* ����S[14],S[22],...���м��� */
            S[i]=((S[i-7]<<9)^(S[i-14]>>7))&ones;
        else if((i+1)%8==0)/* ����S[15],S[23],...���м��� */
            S[i]=((S[i-15]<<9)^(S[i-14]>>7))&ones;
        else
		{
            S[i]=((S[i-7]<<9)^(S[i-6]>>7))&ones;
			c=S[i];
		}
		    
    }
     /*ȡ������Կ����������Կ��keybox[7][10]*/
    for(r=1;r<=round+1;r++)
	{
        for(j=1;j<7;j++)
		{			
            keybox[j][r]=S[6*(r-1)+j-1];		
		}
	}
		   

}

/*���ܺ���uncode*/
void uncode(CString miweninput,char output[28],unsigned int keybox[7][10])
{
	unsigned int r,x1,x2,x3,x4,kk,t1,t2,a;



	char input[28];
	memcpy(&input,miweninput,sizeof(input));//����ת�����ַ������н��д���
	unsigned int fin[4],fout[4];
	int g,state;


	state=1;//��Ǽ��ܵ�����
	for (int m=0;m<2;m++)
	{
		int j=0;
		
		if(state==1)
		{
			/*��ǰ16���ַ�ת����4��16λ���޷������ݣ�����ŵ�fout[0],fout[1],fout[2],fout[3]��*/
			for(int i=0;i<16;)						
			{
				fout[j]=(unsigned int)(((int)(input[i])-65)<<12);
		        fout[j]=fout[j]^(unsigned int)(((int)(input[i+1])-65)<<8);
		        fout[j]=fout[j]^(unsigned int)(((int)(input[i+2])-65)<<4);
		        fout[j]=fout[j]^(unsigned int)((int)(input[i+3])-65);
		        fout[j]=fout[j];

				i+=4;				
				j++;							
			}
		}

		/*����12���ַ�ת����4��16λ���޷������ݣ�����ŵ�fout[0],fout[1],fout[2],fout[3]��*/
		else
		{
			for (int i=16;i<28;)
			{
				fout[j]=(unsigned int)(((int)(input[i]-33))<<10);
		        fout[j]=fout[j]^(unsigned int)(((int)(input[i+1]-33))<<4);
		        fout[j]=fout[j]^(unsigned int)((int)(input[i+2]-33));		        
		        fout[j]=fout[j];

		        i+=3;				
			    j++;					
			}			
		}	    
	

        x1=fout[0];x2=fout[1];x3=fout[2];x4=fout[3];
        for(r=1;r<=8;r++)
		{ 
			/* ��64λ�Ŀ���з�������*/
            x1=chengfa(x1,keybox[1][r]);
	        x4=chengfa(x4,keybox[4][r]);
            x2=(x2+keybox[2][r])&ones;
	        x3=(x3+keybox[3][r])&ones;
			
			/* MA�ṹ�ĺ��� */    
            kk=chengfa(keybox[5][r],(x1^x3));
            t1=chengfa(keybox[6][r],(kk+(x2^x4))&ones);
		    t2=(kk+t1)&ones;

			/* ����任PI*/    
            x1=x1^t1;
	        x4=x4^t2;
            a=x2^t2;
	        x2=x3^t1;
	        x3=a;
		}

		/* ���ת��*/  
        fin[1]=chengfa(x1,keybox[1][round+1]);
        fin[4]=chengfa(x4,keybox[4][round+1]);
        fin[2]=(x3+keybox[2][round+1])&ones;
        fin[3]=(x2+keybox[3][round+1])&ones;

		/*�����һ�龭�����ܵõ������ݣ���������飬�γ�16���ַ�����*/	
	    if (state==1)
	    {
			g=0;
    	    for (int y=1;y<5;)
			{				
		        output[g]=((fin[y]&gainfour)>>12)+48;//��16λ��������(finput[i])-65)�ĵ�11-16λ�����ݷֽ����			
		        output[g+1]=((fin[y]&gainthree)>>8)+48;//��16λ��������(finput[i])-65)�ĵ�9-12λ�����ݷֽ����		
		        output[g+2]=((fin[y]&gaintwo)>>4)+48;//��16λ��������(finput[i])-65)�ĵ�5-8λ�����ݷֽ����		
		        output[g+3]=(fin[y]&gainone)+48;//��16λ��������(finput[i])-65)�ĵ�1-4λ�����ݷֽ����
				
				for (int t=0;t<=3;t++)
				{
					if (output[g+t]>=58 && output[g+t]<=63)
					{
						output[g+t]=output[g+t]+7;
					}
					
				}
			
		        g+=4;		        
		    	y++;			
			}
	    }
		
		/*����ڶ��龭�����ܵõ������ݣ���������飬�γ�12���ַ�����*/	
		else
		{
			g=16;
    	    for (int y=1;y<5;)
			{		
		        output[g]=((fin[y]&gainthree)>>8)+48;//��16λ��������(finput[i])-65)�ĵ�11-16λ�����ݷֽ����			
		        output[g+1]=((fin[y]&gaintwo)>>4)+48;//��16λ��������(finput[i])-65)�ĵ�5-10λ�����ݷֽ����		
		        output[g+2]=(fin[y]&gainone)+48;//��16λ��������(finput[i])-65)�ĵ�1-4λ�����ݷֽ����

				for (int t=0;t<=2;t++)
				{
					if (output[g+t]>=58 && output[g+t]<=63)
					{
						output[g+t]=output[g+t]+7;
					}					
				}
			
		        g+=3;				
		    	y++;
			}
		}
		
		state++;
	}
}

/* �øߵ��㷨��ʵ�ֳ˷�����*/
unsigned int chengfa(unsigned int a,unsigned int b)
{
	long int p;
    long unsigned q;
    if(a==0) p=maxim-b;
    else if(b==0) 
		p=maxim-a;
    else
    {
    q=(unsigned long)a*(unsigned long)b;
    p=(q&ones)-(q>>16);
    if(p<=0) p=p+maxim;
    }
    return (unsigned) (p&ones);
}

/*����������Կ��dekeybox[7][10] */
void de_key(unsigned int keybox[7][10],unsigned int dekeybox[7][10])
{
	int j;
	
    for(j=1;j<=round+1;j++)
    {		
		dekeybox[1][round-j+2]=inv(keybox[1][j]);		
        dekeybox[4][round-j+2]=inv(keybox[4][j]);
		
        if((j==1)|(j==round+1))
		{		
            dekeybox[2][round-j+2]=(fuyi-keybox[2][j])&ones;		
            dekeybox[3][round-j+2]=(fuyi-keybox[3][j])&ones;		
		}
        else
		{
            dekeybox[2][round-j+2]=(fuyi-keybox[3][j])&ones;
            dekeybox[3][round-j+2]=(fuyi-keybox[2][j])&ones;
		}
    }
    for(j=1;j<=round+1;j++)
    {
        dekeybox[5][round+1-j]=keybox[5][j];
        dekeybox[6][round+1-j]=keybox[6][j];
    }
}




//ȡ����������                  
double GetInt(double dbX)   
{   
    double dcs;   
    double fcs = modf(dbX, &dcs);   
    return dcs;   
}   

//ȡ��С����8λ������Ϊһ����Կ
CString GetChaosKey(CString strInput)   
{   
    int site = strInput.Find('.');   
    CString strBack = strInput.Mid(++site, 8);   
       
    return strBack;   
} 

/*����128λ��Կ*/ 
CString LogisticAlgorithm()   
{   
    double kx1, ky1, kz1;   
    double kx2, ky2, kz2;   
    double kx3, ky3, kz3;   
    double kx4, ky4, kz4;  
    x=y=z=1;
	xll=0;
	double x1cs;
	CString strKey2;
     //��������128����������Կ  
    for (int i=0; i<num; i++)
	{		
        kx1 = (y-x)*acs;   
        ky1 = rcs*x-y-x*z;   
        kz1 = x*y-bcs*z;   
   
        kx2 = acs*((y+hcs*ky1/2)-(x+hcs*kx1/2));//need advanced     
        ky2 = rcs*(x+hcs*kx1/2)-(y+hcs*ky1/2)-(x+hcs*kx1/2)*(z+hcs*kz1/2);   
        kz2 = (x+hcs*kx1/2)*(y+hcs*ky1/2)-bcs*(z+hcs*kz1/2);   
   
        kx3 = acs*((y+hcs*ky2/2)-(x+hcs*kx2/2));   
        ky3 = rcs*(x+hcs*kx2/2)-(y+hcs*ky2/2)-(x+hcs*kx2/2)*(z+hcs*kz2/2);   
        kz3 = (x+hcs*kx2/2)*(y+hcs*ky2/2)-bcs*(z+hcs*kz2/2);   
   
        kx4 = acs*((y+hcs*ky3)-(x+hcs*kx3));   
        ky4 = rcs*(x+hcs*kx3)-(y+hcs*ky3)-(x+hcs*kx3)*(z+hcs*kz3);   
        kz4 = (x+hcs*kx3)*(y+hcs*ky3)-bcs*(z+hcs*kz3);   
   
        x = x+hcs*(kx1+2*kx2+2*kx3+kx4)/6;   
        y = y+hcs*(ky1+2*ky2+2*ky3+ky4)/6;   
        z = z+hcs*(kz1+2*kz2+2*kz3+kz4)/6;   
   
        x1cs = 10*x-GetInt(10*x);   
   
        if (x1cs>=0)   
            xll = x1cs-0.5;   
        else   
            xll = x1cs+0.5;   
   
        CString strTemp;   
        strTemp.Format(_T("%.8f"), xll);   
   
        CString strKey1 = GetChaosKey(strTemp);    
   
        x = x+xll;   
        y = y+xll;   
        z = z+xll; 
		
		kx1 = (y-x)*acs;   
        ky1 = rcs*x-y-x*z;   
        kz1 = x*y-bcs*z;   
   
        kx2 = acs*((y+hcs*ky1/2)-(x+hcs*kx1/2));//need advanced     
        ky2 = rcs*(x+hcs*kx1/2)-(y+hcs*ky1/2)-(x+hcs*kx1/2)*(z+hcs*kz1/2);   
        kz2 = (x+hcs*kx1/2)*(y+hcs*ky1/2)-bcs*(z+hcs*kz1/2);   
   
        kx3 = acs*((y+hcs*ky2/2)-(x+hcs*kx2/2));   
        ky3 = rcs*(x+hcs*kx2/2)-(y+hcs*ky2/2)-(x+hcs*kx2/2)*(z+hcs*kz2/2);   
        kz3 = (x+hcs*kx2/2)*(y+hcs*ky2/2)-bcs*(z+hcs*kz2/2);   
   
        kx4 = acs*((y+hcs*ky3)-(x+hcs*kx3));   
        ky4 = rcs*(x+hcs*kx3)-(y+hcs*ky3)-(x+hcs*kx3)*(z+hcs*kz3);   
        kz4 = (x+hcs*kx3)*(y+hcs*ky3)-bcs*(z+hcs*kz3);   
   
        x = x+hcs*(kx1+2*kx2+2*kx3+kx4)/6;   
        y = y+hcs*(ky1+2*ky2+2*ky3+ky4)/6;   
        z = z+hcs*(kz1+2*kz2+2*kz3+kz4)/6;   
   
        x1cs = 10*x-GetInt(10*x);   
   
        if (x1cs>=0)   
            xll = x1cs-0.5;   
        else   
            xll = x1cs+0.5;   
   
          
        strTemp.Format(_T("%.8f"), xll);   
   
        strKey2 = GetChaosKey(strTemp)+strKey1;   
       // puts(strKey2);    
   
        x = x+xll;   
        y = y+xll;   
        z = z+xll;  
	
    }  
		return strKey2;
}  
