#include<xmmintrin.h> //SSE
#include<emmintrin.h> //SSE2
#include<pmmintrin.h> //SSE3
#include<tmmintrin.h> //SSSE3
#include<smmintrin.h>
#include<nmmintrin.h>
#include<immintrin.h> //AVX、AVX2
#include<iostream>
#include<time.h>

using namespace std;
//struct timeval val;
//struct timeval newval;

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
void gauss(float **A,int n)
{
        for(int k=0;k<n;k++)
        {
                //vt ← dupToFloat(A[k,k]);
                __m256 vt=_mm256_set1_ps(A[k][k]);
                int j;
		for(j=k+1;j+8<=n;j+=8)
                {
                        //将四个单精度浮点数从内存加载到向量寄>存器中
                        //va<-load4FloatFrom(&A[k,j]);
                        __m256 va=_mm256_loadu_ps(&A[k][j]);
                        //向量对位相除
                        //va<-va/vt;
                        va=_mm256_div_ps(va,vt);
                        //将四个单精度浮点数从向量寄存器存储到>内存
                        _mm256_store_ps(&A[k][j],va);
                }
                for(;j<n;j++)
                        A[k][j]=A[k][j]/A[k][k];
                A[k][k]=1.0;
                for(int i=k+1;i<n;i++)
                {
                        __m256 vaik=_mm256_set1_ps(A[i][k]);
                        for(j=k+1;j+8<=n;j+=8)
                        {
                                __m256 vakj=_mm256_loadu_ps(&A[k][j]);
                                __m256 vaij=_mm256_loadu_ps(&A[i][j]);
                                //vx ← vakj*vaik;
                                __m256 vx=_mm256_mul_ps(vakj,vaij);
                                vaij=_mm256_sub_ps(vaij,vx);
                                //store4FloatTo(&A[i,j],vaij);
                                _mm256_store_ps(&A[i][j],vaij);
                        }
                        for(;j<n;j++)
                                A[i][j]=A[i][j]-A[k][j]*A[i][j];
			A[i][k]=0;
                 }
        }
}


int main()
{
        int N;
	cin>>N;
	float** m;
//        float m[500][500];
        for(int i=0;i<N;i++)
                m=new float*[N];
        for(int i=0;i<N;i++)
                m[i]=new float[N];
        int n;//执行次数
        m_reset(m,N);//测试用例生成
        clock_t start,finish;
//      QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
//      QueryPerformanceCounter((LARGE_INTEGER *)&head);
//      int ret=gettimeofday(&val,NULL);
        start=clock();
        for(int i=0;i<n;i++)
           gauss(m,N);
        finish=clock();
        cout<<n<<":"<<(finish-start)/float(CLOCKS_PER_SEC)<<endl;
//      QueryPerformanceCounter((LARGE_INTEGER *)&tail);
//      cout<<(tail-head)*1000.0 / freq<<"ms"<<endl;
//      ret=gettimeofday(&newval,NULL);
//      cout<<"diff:sec---"<<newval.tv_sec-val.tv_sec<<" microsec---"<<newval.tv_usec
//             -val.tv_usec<<endl;
        return 0;
}
