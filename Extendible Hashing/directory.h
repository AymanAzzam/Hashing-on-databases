#include "bucket.h"

class Directory {
    Bucket** buckets;
    int global_depth;
    int size;

    public:
        Directory();        //Done
        bool insertItem(DataItem item);
        bool deleteItem(int key);  //we maybe need to merge buckets or decrement the global depth
        bool searchItem(int key); //Done
        int getGlobalDepth(); //Done
        void expand(Bucket** newBuckets);
        void shrink();
        void printKeys(); //Done
        void printData(); //Done
        void print(); //Done
        ~Directory(); //Done
};