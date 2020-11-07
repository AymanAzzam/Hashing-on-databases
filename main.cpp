//============================================================================
// Name        : hashskeleton.cpp
// Author      : 
// Version     :
// Copyright   : Code adapted From https://www.tutorialspoint.com/
// Description : Hashing using open addressing
//============================================================================

#include "readfile.h"

void insert(int key,int data);
int deleteItem(int key);
struct DataItem * search(int key);


int filehandle;   //handler for the database file

/* DBMS (DataBase Management System) needs to store its data in something non-volatile
 * so it stores its data into files (manteqy :)).

 * Some DBMS or even file-systems constraints the size of that file. 

 * for the efficiency of storing and retrieval, DBMS uses hashing 
 * as hashing make it very easy to store and retrieve, it introduces 
 * another type of problem which is handling conflicts when two items
 * have the same hash index

 * In this exercise we will store our database into a file and experience
 * how to retrieve, store, update and delete the data into this file 

 * This file has a constant capacity and uses external-hashing to store records,
 * however, it suffers from hashing conflicts.
 * 
 * You are required to help us resolve the hashing conflicts 

 * For simplification, consider the database has only one table 
 * which has two columns key and data (both are int)

 * Functions in this file are just wrapper functions, the actual functions are in openAddressing.cpp

*/

int main(){

//here we create a sample test to read and write to our database file

  //1. Create Database file or Open it if it already exists, check readfile.cpp
   char fname[] = "../chaining";

   filehandle = createFile(FILESIZE,fname);
  //2. Display the database file, check openAddressing.cpp
   DisplayFile(filehandle);


  //3. Add some data in the table
   insert(1, 20);    // Bucket 1,   Searched records = 1
   insert(2, 70);    // Bucket 2,   Searched records = 1
   insert(42, 80);   // Bucket 2,   Searched records = 2
   insert(4, 25);    // Bucket 4,   Searched records = 1
   insert(12, 44);   // Bucket 2-3  Searched records = 3   overflow !
   insert(14, 32);   // Bucket 4,   Searched records = 2
   insert(17, 11);   // Bucket 7,   Searched records = 1
   insert(13, 78);   // Bucket 3,   Searched records = 2
   insert(37, 97);   // Bucket 7,   Searched records = 2
   insert(11, 34);   // Bucket 1,   Searched records = 2
   insert(22, 730);  // Bucket 2-5  Searched records = 7    overflow !
   insert(46, 840);  // Bucket 6,   Searched records = 1
   insert(9, 83);    // Bucket 9,   Searched records = 1
   insert(21, 424);  // Bucket 1-5  Searched records = 10    overflow !
   insert(41, 115);  // Bucket 1-6  Searched records = 12    overflow !
   insert(71, 47);   // Bucket 1-8  Searched records = 15    overflow !
   insert(31, 92);   // Bucket 1-8  Searched records = 16    overflow !
   insert(73, 45);   // Bucket 3-9  Searched records = 14

   // Open Addressing Cases
   // Case #1
   //insert(91, 20);   // Bucket 1,   Searched records = 1
   //insert(82, 80);   // Bucket 2,   Searched records = 1
   //insert(42, 90);   // Bucket 2,   Searched records = 2
   //insert(44, 27);   // Bucket 4,   Searched records = 1
   //insert(31, 40);   // Bucket 1    Searched records = 2
   //insert(34, 36);   // Bucket 4,   Searched records = 2
   //insert(17, 81);   // Bucket 7,   Searched records = 1
   //insert(13, 28);   // Bucket 3,   Searched records = 2
   //insert(37, 67);   // Bucket 7,   Searched records = 2
   //insert(21, 54);   // Bucket 1,   Searched records = 2

   // Case #2
   //insert(91, 20);   // Bucket 1,   Searched records = 1
   //insert(85, 80);   // Bucket 5,   Searched records = 1
   //insert(41, 90);   // Bucket 1,   Searched records = 2
   //insert(10, 27);   // Bucket 0,   Searched records = 1
   //insert(30, 40);   // Bucket 0    Searched records = 2
   //insert(57, 36);   // Bucket 7,   Searched records = 1
   //insert(11, 81);   // Bucket 1-2  Searched records = 3
   //insert(13, 28);   // Bucket 3,   Searched records = 2
   //insert(37, 67);   // Bucket 7,   Searched records = 2
   //insert(27, 54);   // Bucket 7-8  Searched records = 3


   //4. Display the database file again
   DisplayFile(filehandle);

   //5. Search the database
   search(31);

   //6. delete an item from the database
   deleteItem(31);

   //7. Display the final data base
   DisplayFile(filehandle);
   // And Finally don't forget to close the file.
   close(filehandle);
   return 0;



}

/* functionality: insert the (key,data) pair into the database table
                  and print the number of comparisons it needed to find
    Input: key, data
    Output: print statement with the no. of comparisons
*/
void insert(int key,int data){
     struct DataItem item ;
     item.data = data;
     item.key = key;
     item.valid = 1;
     int result= insertItem(filehandle,item);  //TODO: implement this function in openAddressing.cpp
     printf("Insert: No. of searched records:%d\n",abs(result));
}

/* Functionality: search for a data in the table using the key

   Input:  key
   Output: the return data Item
*/
struct DataItem * search(int key)
{
  struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
     item->key = key;
     int diff = 0;
     int Offset= searchItem(filehandle,item,&diff); //this function is implemented for you in openAddressing.cpp
     printf("Search: No of records searched is %d\n",diff);
     if(Offset <0)  //If offset is negative then the key doesn't exists in the table
       printf("Item not found\n");
     else
        printf("Item found at Offset: %d,  Data: %d and key: %d\n",Offset,item->data,item->key);
  return item;
}

/* Functionality: delete a record with a certain key

   Input:  key
   Output: return 1 on success and -1 on failure
*/
int deleteItem(int key){
   struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
   item->key = key;
   int diff = 0;
   int Offset= searchItem(filehandle,item,&diff);
   printf("Delete: No of records searched is %d\n",diff);
   if(Offset >=0 )
   {
    return deleteOffset(filehandle,Offset);
   }
   return -1;
}
