#include "directory.h"
#include <math.h>
#include <iostream>

using namespace std;


// return n Least Significant Bits (LSB) for the key
int hashing(int key, int n) {   return key % (int) (pow(2,n)+0.5); }

Directory::Directory()
{   
    int size = 2;
    global_depth = 1;  

    buckets = new Bucket*[size];

    for(int i=0; i<size; i++)   buckets[i] = new Bucket(i);
}

Directory::~Directory()
{
    int size = pow(2,global_depth);
    
    for(int i=0; i<size; i++)   delete[] buckets[i];

    delete[] buckets;
}