#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include "bucket.h"

class Directory {
    Bucket** buckets;
    int global_depth;
    int size;

    public:
        Directory();        //Done
        bool insertItem(DataItem item);
        int deleteItem(int key);  //we maybe need to merge buckets or decrement the global depth
        bool searchItem(int key); //Done
        int getGlobalDepth(); //Done
        int getSize();
        Bucket** getBuckets();
        void expand(Bucket** newBuckets);
        void shrink();
        void printKeys(); //Done
        void printData(); //Done
        void print(); //Done
        ~Directory(); //Done
};

#endif