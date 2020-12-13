#include "bucket.h"

class Directory {
    Bucket** buckets;
    int global_depth;

    public:
        Directory();
        bool insertItem(DataItem item);
        bool deleteItem(int key);  //we maybe need to merge buckets or decrement the global depth
        bool searchItem(int key);
        int getGlobalDepth(); 
        void incDepth();
        void decDepth();
        void printKeys();
        void printData();
        void print();
        ~Directory();
};