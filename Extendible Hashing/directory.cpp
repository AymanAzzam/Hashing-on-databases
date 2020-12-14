#include "directory.h"
#include <iostream>

using namespace std;


// return n Least Significant Bits (LSB) for the key
int hashing(int key, int n)     {   return key % (1 << n);  }

int Directory::getGlobalDepth() {   return global_depth;                }

Directory::Directory()
{   
    size = 2;
    global_depth = 1;  

    buckets = new Bucket*[size];

    for(int i=0; i<size; i++)   buckets[i] = new Bucket(i);
}

Directory::~Directory()
{
    for(int i=0; i<size; i++)   delete buckets[i];

    delete[] buckets;
}

// it increments the depth then rehash and split the bucket with id = bucket_id 
void Directory::expand(int bucket_id)
{

}

void Directory::printKeys()
{
    for(int i=0; i<size; i++)
        if(buckets[i] != nullptr)
        {
            cout<<"id = "<<buckets[i]->getId()<<", Keys = ";
            buckets[i]->printKeys();
        }
}

void Directory::printData()
{
   for(int i=0; i<size; i++)
        if(buckets[i] != nullptr)
        {
            cout<<"id = "<<buckets[i]->getId()<<", Data = ";
            buckets[i]->printData();
        }
}

void Directory::print()
{
    for(int i=0; i<size; i++)
        if(buckets[i] != nullptr)
        {
            cout<<"id = "<<buckets[i]->getId()<<": ";
            buckets[i]->print();
        }
}


bool Directory::searchItem(int key)
{
    int hash = hashing(key,global_depth);

    if(buckets[hash] != nullptr && buckets[hash]->searchItem(key))
    {
        cout<<"Item "<<key<<" found at bucket id "<<hash<<endl;
        return true;
    }
    cout<<"Item "<<key<<" not found"<<endl;
    return false;   
}

/* return the number of deleted Items
   Note: it can delete several items that have the same key */
int Directory::deleteItem(int key)
{
    int hash = hashing(key,global_depth), counter = 0, deleted;

    if(buckets[hash] == nullptr)    return 0;
    
    deleted = buckets[hash]->deleteItem(key);
    if(deleted)
    {
        for(int i=1; i<size; i+=2)
        {
            if(buckets[i-1] != buckets[i] && buckets[i]->getSize() == 0)
            {
                delete buckets[i];
                buckets[i] = buckets[i-1];
            }
            else if(buckets[i-1] != buckets[i] && buckets[i-1]->getSize() == 0)
            {
                delete buckets[i-1];
                buckets[i-1] = buckets[i];
            }

            if(buckets[i] != nullptr && buckets[i-1] == buckets[i])  counter++;
        }

        if( size == counter* 2) shrink();
    }
    return deleted;
}

// it decrease the depth of the Directory by one and merge buckets
void Directory::shrink()
{
    int new_size = size/2;
    Bucket** new_bucket = new Bucket*[new_size];

    for(int i=0,j=0;i<size;i+=2,j++)
    {
        new_bucket[j] = buckets[i];
        delete buckets[i];
    }

    delete[] buckets;
    buckets = new_bucket;
    size = new_size;
    global_depth--;
}