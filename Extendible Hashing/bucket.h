const int BUCKETSIZE = 3;

class DataItem {
    int key;
    int data;

    public:
        void setKey(int k)  {   key = k;        }
        void setData(int d) {   data = d;       }
        int getKey()        {   return key;     }
        int getData()       {   return data;    }
};

class Bucket {
    DataItem data[BUCKETSIZE];
    int id;
    int size;
    int local_depth;

    public:
        Bucket(int iid);                                
        bool insertItem(DataItem item);                 
        int deleteItem(int key);                       
        bool searchItem(int key);                       
        bool isFull();                                  
        int getLocalDepth();                            
        void incDepth();                                
        void setId(int iid);                                   
        int getId();                                    
        void printKeys();                               
        void printData();                                
        void print();                                      
};
