#include "directory.h"
#include <math.h>
#include <iostream>

using namespace std;


int Directory::getGlobalDepth() {   return global_depth;    }

Directory::Directory()
{   
    global_depth = 1;  
    size = (1 << global_depth);

    buckets = new Bucket*[size];

    for(int i=0; i<size; i++)   buckets[i] = new Bucket(i);
}

Directory::~Directory()
{
    int size = pow(2,global_depth);
    
    for(int i=0; i<size; i++)   delete buckets[i];

    delete[] buckets;
}

bool Directory::insertItem(DataItem item)
{
    int hash = hashing(item.getKey(), global_depth);
    Bucket* oldBucket = buckets[hash];

    if (oldBucket->isFull())    // overflow -> split bucket
    {
        Bucket** newBuckets = oldBucket->split();

        if (oldBucket->getLocalDepth() == global_depth)     // expand directory
        {
            expand(newBuckets);
            hash = hashing(item.getKey(), global_depth);   //update hash value to that after expansion
        }
        else        // directory expansion not required
        {
            buckets[newBuckets[0]->getId()] = newBuckets[0];
            buckets[newBuckets[1]->getId()] = newBuckets[1];
        }
        
        delete[] newBuckets;
    }
    // splitting bucket not required or handled
    if (oldBucket->insertItem(item))
    {
        cout << "Record ";
        item.print();
        cout << " was inserted successfully into directory " << hash << endl;
        return true;
    }
    else
    {
        cout << "Failed to insert record ";
        item.print();
        cout  << " into directory " << hash << endl;
        return false;
    }
    
}

// expand (double) the directories and 
void Directory::expand(Bucket** newBuckets)
{
    int newSize = (1<<(global_depth+1));
    Bucket** expanded = new Bucket*[newSize];

    for (int i = 0; i < size; ++i)
    {
        if (i == newBuckets[0]->getId())
        {
            buckets[i] = newBuckets[0];
            buckets[i + (1<<global_depth)] = newBuckets[1];
        }
        else
        {
            /* buckets[i] = */ buckets[1 + (1<<global_depth)] = buckets[i];
        }
        
    }
    buckets = expanded;
    delete[] expanded;

    ++global_depth;
    size = newSize;
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