#include "bucket.h"
#include <iostream>

using namespace std;

// construct empty bucket with depth = 1
Bucket::Bucket(int iid = 0)
{   size = 0;   local_depth = 1;    id =iid;    }

// return true if the bucket is full, false otherwise
bool Bucket::isFull()       {   return size == BUCKETSIZE;  }

int Bucket::getLocalDepth() {   return local_depth;         }

void Bucket::incDepth()     {   local_depth += 1;           }

void Bucket::setId(int iid) {   id = iid;                   }

int Bucket::getId()         {   return id;                  }

// return true if the key is exist, false if isn't exist
bool Bucket::searchItem(int key)    {
    for(int i=0; i<size; i++)
        if(data[i].getKey() == key)
            return true;
    return false;
}
 
// print the Bucket Keys seperated by comma
void Bucket::printKeys()
{
    for(int i=0; i<size; i++)   cout<<data[i].getKey()<<", ";
    
    cout<<endl;
}

// print the Bucket Data seperated by comma
void Bucket::printData()
{
    for(int i=0; i<size; i++)   cout<<data[i].getData()<<", ";
    
    cout<<endl;
}

// print the Bucket Content seperated by comma
void Bucket::print()
{
    for(int i=0; i<size; i++)   cout<<"("<<data[i].getKey()<<","<<data[i].getData()<<"), ";
    
    cout<<endl;
}

/* if the Bucket is full, return false. otherwise insert it and return true. 
   Note: if the item key is exist, I ignore the new item and return true.   */ 
bool Bucket::insertItem(DataItem item)    
{
    if(size == BUCKETSIZE)              return false;   
    
    if(searchItem(item.getKey()))       return true;
    
    data[size++] = item;
    return true; 
}

/* return the number of deleted Items
   Note: it can delete several items that have the same key */
int Bucket::deleteItem(int key)
{
    int i = 0,j, count = 0;

    if(!searchItem(key))    return false;

    while(i < size && data[i].getKey() != key) i++;

    (i == size)? count = 0 : count = 1;
    j = i + count;
    
    while(j < size)
    {
        if(data[j].getKey() != key) data[i++] = data[j++];
        else                        count++,j++;
    }

    size -= count;

    return count;
}