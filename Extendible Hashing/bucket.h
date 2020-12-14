int hashing(int key, int n);

const int BUCKETSIZE = 3;

class DataItem {
    int key;
    int data;

    public:
        void setKey(int k)  {   key = k;        }
        void setData(int d) {   data = d;       }
        int getKey()        {   return key;     }
        int getData()       {   return data;    }
        void print()        {   cout << "(" << key << "," << data << ")";  }
};

class Bucket {
    DataItem data[BUCKETSIZE];
    int id;
    int size;
    int local_depth;

    public:
        Bucket(int iid, int ld = 1);                                
        bool insertItem(DataItem item);                 
        int deleteItem(int key);                       
        bool searchItem(int key);                       
        bool isFull();                                  
        int getLocalDepth();                            
        Bucket** split();                                
        void setId(int iid);                                   
        int getId();
        int getSize();                                    
        void printKeys();                               
        void printData();                                
        void print();                                      
};
