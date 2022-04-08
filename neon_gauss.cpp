//普通高斯消去Neon（ARM平台）
#include<iostream>
#include<sys/time.h>
#include<arm_neon.h>
using namespace std;
struct timeval val;
struct timeval newval;

//测试用例生成
void m_reset(float **m,int N)
{
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<i;j++)
			m[i][j]=0;
		m[i][i]=1.0;
		for(int j=i+1;j<N;j++)
			m[i][j]=rand();
	}
	for(int k=0;k<N;k++)
		for(int i=k+1;i<N;i++)
			for(int j=0;j<N;j++)
				m[i][j]+=m[k][j];
}

//普通高斯消元
void gauss(float** A,int n)
{
	for(int k=0;k<n;k++)
	{
		//将四个单精度浮点数从内存加载到向量寄存器
		//vt ← dupToFloat(A[k,k]);
		float32x4_t vt=vdupq_n_f32(A[k][k]);//定义寄存器并将其初始化为A[k,k]
		int j;
		for(j=k+1;j+4<=n;j+=4)
		{
			//将四个单精度浮点数从内存加载到向量寄存器中
			//va<-load4FloatFrom(&A[k,j]);
			float32x4_t va=vld1q_f32(&A[k][j]);
			//向量对位相除
			//va<-va/vt;
			va=vdivq_f32(va,vt);
			//将四个单精度浮点数从向量寄存器存储到内存
			//store4FloatTo(&A[k,j],va);
			vst1q_f32(&A[k][j],va);
		}
		for(;j<n;j++)
			A[k][j]=A[k][j]/A[k][k];//该行结尾处有几个元素还未计算
		A[k][k]=1.0;
		for(int i=k+1;i<n;i++)
		{
			float32x4_t vaik=vdupq_n_f32(A[i][k]);
			for(j=k+1;j+4<=n;j+=4)
			{
				float32x4_t vakj=vld1q_f32(&A[k][j]);
				float32x4_t vaij=vld1q_f32(&A[i][j]);
				//vx ← vakj*vaik;
				float32x4_t vx=vmulq_f32(vakj,vaij);
				vaij=vsubq_f32(vaij,vx);
				//store4FloatTo(&A[i,j],vaij);
				vst1q_f32(&A[i][j],vaij);
			}
			for(;j<n;j++)
				A[i][j]=A[i][j]-A[k][j]*A[i][j];
			A[i][k]=0;
		}
	}	
}

int main()
{
	int N=500;
	float **m;
	for(int i=0;i<N;i++)
		m=new float*[N];
	for(int i=0;i<N;i++)
		m[i]=new float[N];
	int n;//执行次数
	cin>>n;
	m_reset(m,N);//测试用例生成
	int ret=gettimeofday(&val,NULL);
	for(int h=0;h<n;h++)
		gauss(m,N);
	ret=gettimeofday(&newval,NULL);
	cout<<"diff:sec---"<<newval.tv_sec-val.tv_sec<<" microsec---"<<newval.tv_usec
		-val.tv_usec<<endl;
	return 0;
}
