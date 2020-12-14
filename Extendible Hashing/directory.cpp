#include "directory.h"
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
    for(int i=0; i<size; i++)   delete buckets[i];

    delete[] buckets;
}

bool Directory::insertItem(DataItem item)
{
    int hash = hashing(item.getKey(), global_depth);
    Bucket* oldBucket = buckets[hash];

    if (oldBucket->isFull())    // overflow -> split bucket
    {
        cout << "Splitting bucket " << hash << endl;

        Bucket** newBuckets = oldBucket->split();

        if (oldBucket->getLocalDepth() == global_depth)     // expand directory
        {
            cout << "Expanding directories" << endl;

            expand(newBuckets);
            hash = hashing(item.getKey(), global_depth);   //update hash value to that after expansion
        }
        else        // directory expansion not required
        {
            cout << "Not expanding directories" << endl;

            buckets[newBuckets[0]->getId()] = newBuckets[0];
            buckets[newBuckets[1]->getId()] = newBuckets[1];
        }
        
        delete[] newBuckets;
    }
    // splitting bucket not required or handled
    if (buckets[hash]->insertItem(item))
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

// expand (double) the directories and update their pointers to buckets
void Directory::expand(Bucket** newBuckets)
{
    int newSize = (1<<(global_depth+1));
    Bucket** expanded = new Bucket*[newSize];

    for (int i = 0; i < size; ++i)
    {
        if (i == newBuckets[0]->getId())
        {
            expanded[i] = newBuckets[0];
            expanded[i + (1<<global_depth)] = newBuckets[1];
        }
        else
        {
            expanded[i] = expanded[i + (1<<global_depth)] = buckets[i];
        }
    }
    swap(buckets, expanded);
    delete[] expanded;

    ++global_depth;
    size = newSize;
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
        // if(buckets[i] != nullptr)
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