#include "directory.h"
#include <math.h>
#include <iostream>

using namespace std;


// return n Least Significant Bits (LSB) for the key
int hashing(int key, int n)     {   return key % (int) (pow(2,n)+0.5);  }

int Directory::getGlobalDepth() {   return global_depth;                }

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

// it increments the depth then rehash and split the bucket with id = bucket_id 
void Directory::expand(int bucket_id)
{

}

// it decrease the depth of the Directory by one and merge buckets
void Directory::shrink()
{
    
}

void Directory::printKeys()
{
    int dic_size = pow(2,global_depth),buck_size;

    for(int i=0; i<dic_size; i++)
        if(buckets[i])
        {
            cout<<"id = "<<buckets[i]->getId()<<", Keys = ";
            buckets[i]->printKeys();
        }
}

void Directory::printData()
{
    int dic_size = pow(2,global_depth);

    for(int i=0; i<dic_size; i++)
        if(buckets[i])
        {
            cout<<"id = "<<buckets[i]->getId()<<", Data = ";
            buckets[i]->printData();
        }
}

void Directory::print()
{
    int dic_size = pow(2,global_depth);

    for(int i=0; i<dic_size; i++)
        if(buckets[i])
        {
            cout<<"id = "<<buckets[i]->getId()<<": ";
            buckets[i]->print();
        }
}


bool Directory::searchItem(int key)
{
    int dic_size = pow(2,global_depth);
    int hash = hashing(key,global_depth);

    if(buckets[hash]->searchItem(key))
    {
        cout<<"Item "<<key<<" found at bucket id "<<hash<<endl;
        return true;
    }
    cout<<"Item "<<key<<" not found"<<endl;
    return false;   
}