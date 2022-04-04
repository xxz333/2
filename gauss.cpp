#include<iostream>
#include<sys/time.h>
using namespace std;
struct timeval val;
struct timeval newval;
//测试用例生成
void m_reset(float** m,int N)
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
//高斯消去串行算法
void LU(float** A,int n)
{
	for(int k=0;k<n;k++)
	{
		for(int j=k+1;j<n;j++)
			A[k][j]=A[k][j]/A[k][k];
		A[k][k]=1.0;
		for(int i=k+1;i<n;i++)
		{
			for(int j=k+1;j<n;j++)
				A[i][j]=A[i][j]-A[i][k]*A[k][j];
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
	int n;//循环次数
	cin>>n;
	m_reset(m,N);
	int ret=gettimeofday(&val,NULL);
	for(int x=0;x<n;x++)
	{
		LU(m,N);
	}
	ret=gettimeofday(&newval,NULL);
	cout<<"diff:sec---"<<newval.tv_sec-val.tv_sec<<" microsec---"<<newval.tv_usec
		            -val.tv_usec<<endl;
	for(int k=0;k<N;k++)
		delete m[k];
	delete []m;
}
