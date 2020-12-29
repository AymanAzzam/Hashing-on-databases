#include "directory.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int readFile(int size, char * name, DataItem*& items,int &items_size);
int createFile(int size,char * name);
int saveFile(Bucket** buckets, int buckets_num, char * name);

void case1Insert(Directory& dir);
void case2Insert(Directory& dir);
void case2Delete(Directory& dir);


#define MAXFILESIZE 100*sizeof(DataItem)


int main()
{
    int fd, items_size;
    char test[] = "test";
    Directory dir;
    DataItem d;
    DataItem* items;

    fd = readFile(MAXFILESIZE,test,items,items_size);
    
    for(int i=0; i< items_size; i++)
        dir.insertItem(items[i]);
    
    case2Insert(dir);

//    case1Insert(dir);

    //case1Delete(dir);

    saveFile(dir.getBuckets(),dir.getSize(),test);
}

void case1Insert(Directory& dir)
{
    DataItem d;

    d.setData(15);
    //1
    d.setKey(5);
    dir.insertItem(d);
    dir.print();
    //2
    d.setKey(4);
    dir.insertItem(d);
    dir.print();
    //3
    d.setKey(6);
    dir.insertItem(d);
    dir.print();
    //4
    d.setKey(40);
    dir.insertItem(d);
    dir.print();
    //5
    d.setKey(3);
    dir.insertItem(d);
    dir.print();
    //6
    d.setKey(50);
    dir.insertItem(d);
    dir.print();
    //7
    d.setKey(60);
    dir.insertItem(d);
    dir.print();
}

void case2Insert(Directory& dir)
{
    DataItem d;

    d.setData(15);
    //1
    d.setKey(500);
    dir.insertItem(d);
    dir.print();

    //2
    d.setKey(713);
    dir.insertItem(d);
    dir.print();
  
}

void case1Delete(Directory& dir)
{
    dir.deleteItem(50);
    dir.print();

    dir.deleteItem(3);
    dir.print();

    dir.deleteItem(6);    
    dir.print();

    dir.deleteItem(5);    
    dir.print();

    dir.deleteItem(40);    
    dir.print();

    dir.deleteItem(4);    
    dir.print();

    dir.deleteItem(60);    
    dir.print();
}

int createFile(int size,char * name)
{

    int fd;
	int result;
    struct stat sb;

    if (stat(name, &sb) == -1) {
         printf("file doesn't exist, create it\n");
    fd = open(name, O_RDWR | O_CREAT , (mode_t)0600);
    if (fd == -1) {
	perror("Error opening file for writing");
        return 1;
    }

    /* Stretch the file size.
     * Note that the bytes are not actually written,
     * and cause no IO activity.
     * Also, the diskspace is not even used
     * on filesystems that support sparse files.
     * (you can verify this with 'du' command)
     */
    result = lseek(fd, size-1, SEEK_SET);
    if (result == -1) {
	close(fd);
	perror("Error calling lseek() to 'stretch' the file");
        return 1;
    }

    /* write just one byte at the end */
    result = write(fd, "", 1);
    if (result < 0) {
		close(fd);
		perror("Error writing a byte at the end of the file");
			return -1;
    	}
    }
    else  {//file exists
    	//| O_EXCL
    	    fd = open(name, O_RDWR  , (mode_t)0600);
    	    if (fd == -1) {
    		perror("Error opening file for writing");
    	        return -1;
    	    }
    }
    return fd;
}

int saveFile(Bucket** buckets, int buckets_num, char * name)
{
    int fd,size = 0, result, Offset=0;

    for(int i=0; i<buckets_num; i++)
        if(i == buckets[i]->getId())
            size += buckets[i]->getSize();

    fd = createFile(size* sizeof(DataItem)+sizeof(int), name);

    result = pwrite(fd,&size,sizeof(int), Offset);
    Offset += sizeof(int);
    if(result <= 0) { 	  return -1;	}	// an error happened in the pwrite

    for(int i=0; i<buckets_num; i++)
        if(i == buckets[i]->getId())
        {
            DataItem data; 
            for(int j=0; j<buckets[i]->getSize(); j++)
            {
                data = buckets[i]->getData(j);
                result = pwrite(fd,&data,sizeof(DataItem), Offset);
                //cout<<data.getKey()<<endl;
                Offset += sizeof(DataItem);
                if(result <= 0) { 	  return -1;	}	// an error happened in the pwrite
            }
        }
        
    return fd;
}

int readFile(int size, char * name, DataItem*& items,int &items_size)
{
    int fd, Offset = 0, result;

    fd = createFile(size,name);

    result = pread(fd,&items_size,sizeof(int), Offset);
    Offset += sizeof(int);
    if(result <= 0) { 	  return -1;	}	// an error happened in the pwrite

    items = new DataItem [items_size];

    for(int i=0;i<items_size; i++)
    {
        DataItem d;
        result = pread(fd,&d,sizeof(DataItem), Offset);
        items[i] = d;
        Offset += sizeof(DataItem);
        //cout<<d.getKey()<<endl;
        if(result <= 0) { 	  return -1;	}	// an error happened in the pwrite
    }
    return fd;
}
