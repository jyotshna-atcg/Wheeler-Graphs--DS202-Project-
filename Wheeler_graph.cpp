#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <string.h>

#include <divsufsort.h>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/bit_vectors.hpp>


using namespace std;
using namespace sdsl;

bit_vector rank_char(int x, string str){
    bit_vector BV_c;
    int k=str.size();
    if(x==0){
        for(int i=0; i<k; i++){
            if(str[i]=='A')
                BV_c[i]=1;
            else
                BV_c[i]=0;
        }
    }
    else if(x==1){
        for(int i=0; i<k; i++){
            if(str[i]=='C')
                BV_c[i]=1;
            else
                BV_c[i]=0;
        }
    }
    else if(x==2){
        for(int i=0; i<k; i++){
            if(str[i]=='G')
                BV_c[i]=1;
            else
                BV_c[i]=0;
        }
    }
    else if(x==3){
        for(int i=0; i<k; i++){
            if(str[i]=='T')
                BV_c[i]=1;
            else
                BV_c[i]=0;
        }
    }
    return BV_c;
}

int main()
{
    string sqc, s;
    cin >> sqc;
    while(cin >> s){
        sqc.append(s);
    }
    

    int n = sqc.length();
    int *SA =(int *)malloc(n* sizeof(int));
    int *BWT =(int *)malloc(n * sizeof(int));
    
    //Computing SA
    divsufsort((unsigned char *)sqc.c_str(), SA, n);
    cout<<"Length of given sequence is: "<<n<<endl;
    for(int i=0; i<n; i++){
        if(SA[i]==0)
            BWT[i]=n-1;
        else
            BWT[i]= SA[i]-1;
    }

    //C array to count the cummulative frequencies of each character in sequence
    int C[4]={0};
    for(int k=0; k<n-1; k++){
        if(sqc[k]=='A')
            {C[1]+=1;}
        else if(sqc[k]=='C')
            {C[2]+=1;}
        else if(sqc[k]=='G')
            {C[3]+=1;}
    }
    for(int k=1; k<4; k++)
    {
        C[k]+=C[k-1];
    }


    //constructing rank vector for given sequennce where \sigma={A, c, G, T}
    bit_vector BV_A, BV_C, BV_G, BV_T;
    BV_A=rank_char(0, sqc);
    rank_support_v<1> rank_A(&BV_A); 
    BV_C=rank_char(1, sqc);
    rank_support_v<1> rank_C(&BV_C); 
    BV_G=rank_char(2, sqc);
    rank_support_v<1> rank_G(&BV_G); 
    BV_T=rank_char(3, sqc);
    rank_support_v<1> rank_T(&BV_T); 
    
    //computing indegree and out degree vectors
    bit_vector outd;
    bit_vector ind;
    int p=0, q=0;
    vector<char>F, L;
    auto strt = std::chrono::steady_clock::now();
    for(int j=0; j<n; j++)
    {
        if((sqc[SA[j]]!='$') && (sqc[BWT[j]]!='$')){
                outd[p]=0;
                p+=1;
                outd[p]=1;
                p+=1;
                //
                ind[q]=0;
                q+=1;
                ind[p]=1;
                q+=1;
                //
                L.push_back(sqc[BWT[j]]);
                F.push_back(sqc[SA[j]]);
            }
        else if((sqc[SA[j]]=='$') && (sqc[BWT[j]]!='$')){
                outd[p]=0;
                p+=1;
                outd[p]=1;
                p+=1;
                //
                ind[p]=1;
                q+=1;
                L.push_back(sqc[BWT[j]]);
            }
        else if((sqc[SA[j]]!='$') && (sqc[BWT[j]]=='$')){
                outd[p]=1;
                p+=1;
                //
                ind[q]=0;
                q+=1;
                ind[p]=1;
                q+=1;
                //
                F.push_back(sqc[SA[j]]);
            }
    }
auto finish = std::chrono::steady_clock::now();
cout	<< "\nTime to buld I, O bitvectors and L array is: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - strt).count()<< "ns" <<endl;
cout<<"In-degree vector has been built successfully..."<<endl;
cout<<"Out-degree vector has ben built successfully..."<<endl;
cout<<"F & L column has been built successfully..."<<endl;

//rank_0 & rank_1 vector of out-degreee vector
rank_support_v<0> outd_rank_0(&outd);
rank_support_v<1> outd_rank_1(&outd);
cout<<"rank_0 & rank_1 vector for out-degree vector has been built successfully..."<<endl;

//rank_0 & rank_1 vector of in-degreee vector
rank_support_v<0> ind_rank_0(&ind);
rank_support_v<1> ind_rank_1(&ind);
cout<<"rank_0 & rank_1 vector for in-degree vector has been built successfully..."<<endl;
return 0;
}
