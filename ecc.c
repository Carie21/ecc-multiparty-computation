#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
// equation of elliptic curve => y^2 = x^3 + ax + b 
//secp256k1 curve
int a = 0;
int b = 7;
int p = 17; // order
int generator[2] = {15,13}; //initial point
int points[1000][2]; // used to store field limit points of the elliptic curve 
int count = 0;
void generate()
{
    int rhs,lhs;
    for(int x=0;x<p;x++)
    {
        rhs=((x*x*x)+(a*x)+b)%p;
        for(int y=0;y<p;y++)
        {
            lhs=(y*y)%p;
            if (lhs==rhs)
            {
                points[count][0]=x;
                points[count][1]=y;
                count+=1;
            }
        }
    }
    
}
void print_points(){
    printf("\nNumber of points in the chosen field on the curve is %d \n",count);
    for(int k=0;k<count;k++)
    {
        printf("%d(%d,%d)\n",(k),points[k][0],points[k][1]);
    }
}
bool isPAI(int *point)  //PAI => Point at infinity
{
    return point[0] == 0 && point[1] == 0;
}

int* getPAI(int *point) //PAI => Point at infinity, assumed to be (0,0) as it does not lie on the elliptic curve
{
    point[0] = 0;
    point[1] = 0;
    return point;
}
int inverse(int num)
{
    int i=1;
    if (num<0)
        num=p+num;
    for (i=1;i<p;i++)
    {
        if(((num*i)%p)==1)
            break;
    }
    
    return i;
}
int * add(int A[2],int B[2]) //add two points on the elliptic curve
{
    int *C = calloc(2,sizeof(int));
    int x=0;
    if (isPAI(A) && isPAI(B))  
    {
        return getPAI(C);
    }
    else if (isPAI(A))        
    {
        return B;
    }
    else if (isPAI(B))      
    {
        return A;
    }
    if (A[0]==0 || A[1]==0)
    {
        return B;
    }
    if (B[0]==0 || B[1]==0)
    {
        return A;
    }
    if (A[1]==(p-B[1]))
    {
        return C;
    }
    if ((A[0]==B[0]) && (A[1]==B[1]))
    {   
        if (A[1] % p == 0)           
        {
            return getPAI(C);
        }
        x=((3*(A[0]*A[0]))+a)*inverse(2*A[1]);
        C[0]=((x*x)-(2*A[0]))%p;
        C[1]=((x*(A[0]-C[0]))-A[1])%p;
        
    }
    else
    {
        if ((B[0] - A[0]) % p == 0)    
        {
            return getPAI(C);
        }
        x=(B[1]-A[1])*inverse(B[0]-A[0]);
        C[0]=((x*x)-(A[0]+B[0]))%p;
        C[1]=((x*(A[0]-C[0]))-A[1])%p;
    }
    if (C[0]<0)
        C[0]=p+C[0];
    if (C[1]<0)
        C[1]=p+C[1];
    return C;
}
int * sclr_mult(int k,int P[2])//using LSB first algorithm
{
    int *temp,i;
    int *Q = calloc(2,sizeof(int));
    Q[0]=0;
    Q[1]=0;
    for(i=31;i>=0;i--)
    {
        if((k>>i)&1)
            break;
    }
    for(int j=0;j<=i;j++)
    {
        if((k>>j)&1)
        {
            temp=add(Q,P);
            Q[0]=*temp;
            Q[1]=*(temp+1);
        }
        temp=add(P,P);
        P[0]=*temp;
        P[1]=*(temp+1);
    }
    free(temp);
    return Q;
}
int *subtract(int A[2],int B[2]){
    B[1]=-1*B[1];
    return add(A,B);
}
void test_gen(int *gen){
    int *Q = calloc(2,sizeof(int));
    
        Q = sclr_mult(25,gen);
        printf("%d,%d)\n",Q[0],Q[1]);
         free(Q);
  
}
void calculate_additive_shares(int x, int y, int k) {
    int U; // Alice's additive share
    int V; // Bob's additive share
    int c = 0;
    while(c++ < 50){
        U=0, V=0;
        for (int i = 0; i < k; i++) {
        //random scalar Ui
        int Ui = rand() % 2; 
        //chosen bit c -> ith bit of y
        int ci = (y >> i)&1;
       
        int mci;
        if (ci == 0)
            mci = Ui;
        else
            mci = Ui + x;

       
        U -= ((int)pow(2,i+1)) * Ui;
        V += ((int)pow(2,i+1)) * mci;
     

    }
    if((V*V*V)%p == ((U*U)+(a*U)+b)%p){
        printf("Alice's additive share (U): %d\n", U);
        printf("Bob's additive share (V): %d\n", V);
        return;
    }
    }
    
    printf("Could not find U and V values such that x*y=U+V and (U,V) in chosen field of elliptic curve, after 50 iterations\n");
    
}

int main(int argc, char *argv[]){
    int num1, num2;
    num1 = atoi(argv[1]);
    num2 = atoi(argv[2]);
    printf("secret multiplicative share x: %d, y: %d \n", num1,num2);
    
    generate(); // generate list of all points within field limit of elliptic curve
    srand(time(NULL));
    int lower = 1, upper = 100;
    int A_random = ( rand() % (upper - lower + 1)) + lower;
    int B_random = ( rand() % (upper - lower + 1)) + lower;

    printf("Random number a: %d, Random number b: %d \n", A_random,B_random);
    
    int * point_A= calloc(2,sizeof(int));
    int * point_B= calloc(2,sizeof(int));
    int * dcrypt_B= calloc(2,sizeof(int));

    point_A = sclr_mult(A_random, generator);
    point_B = sclr_mult(B_random, generator);
    printf("Point aG:(%d,%d), Point bG:(%d,%d) \n", point_A[0],point_A[1],point_B[0], point_B[1]);
    dcrypt_B = sclr_mult(B_random, point_A);

    int c = 0; //choice
    if(c==1) point_B = add(point_A,point_B);

//Diffie Hellman Key Secret Key Generation
    //Alice key generation and encryption
    int k0, k1;
    int *enc_A0 = calloc(2,sizeof(int));
    int *enc_A1 = calloc(2,sizeof(int));
    enc_A0 = sclr_mult(A_random, point_B);
    enc_A1 = sclr_mult(A_random, subtract(point_B,point_A));
    k0 = enc_A0[0];
    k1 = enc_A1[0];
    int enc_m0 = num1^k0;
    int enc_m1 = num2^k1;

    //Bob decryption
    int dec_m0 = enc_m0 ^ dcrypt_B[0];
    int dec_m1 = enc_m1 ^ dcrypt_B[0];
    // printf("(%d,%d)\n",dec_m0, dec_m1);
    free(point_A); 
    free(point_B);
    free(dcrypt_B);
    //multiplicative term successfully decrypted. Now Calculate additive shares.
    int x = num1, y = num2;
    calculate_additive_shares(x,y,sizeof(int));
  
     

    return 0;
   
}