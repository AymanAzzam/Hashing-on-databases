#include "readfile.h"
#include <string>

/* First hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */
int firstHashCode(int key){
    int  p = 13;    // The prime number to be used
    std::string keyStr = std::to_string(key);
    int value = keyStr[0];
    for(int i = 1; i < keyStr.size(); ++i)
    {
        value = (value * p) + keyStr[i];
    }
    return value % MBUCKETS;
}

/* Second hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */
int secondHashCode(int key)
{
    return key % MBUCKETS;
}


/* Functionality insert the data item into the correct position
 *          1. use the first hash function to determine which bucket to insert into
 *          2. search for the first empty space within the bucket
 *          	2.1. if it has empty space
 *          	           insert the item
 *          3. if no empty spaces found withing the bucket
 *              repeat steps 1 and 2 using a different hash function
 *          	3.1 if no empty spaces found within the new bucket
 *          	          use OpenAddressing to insert the record
 *          4. return the number of records accessed (searched)
 *
 * Input:  fd: filehandler which contains the db
 *         item: the dataitem which should be inserted into the database
 *
 * Output: No. of records searched
 */
int insertItem(int fd, DataItem item){
    // The number of records accessed to be returned
    int numAccessed = 0;
    // First hash function
    int hashIndex = firstHashCode(item.key);
    // A dummy record to read the data into
    DataItem record;
    // The offset of the data to be read
    int offset = hashIndex * sizeof(Bucket);
    // Searching for an empty place in the first bucket
    for(int i = 0; i < RECORDSPERBUCKET; ++i)
    {
        int result = pread(fd, &record, sizeof(DataItem), offset);
        if (result <= 0)
        {
            printf("An error occured while trying to read a recored...");
            return -1;
        }
        ++numAccessed;
        if (record.valid == 0)  // An empty recocrd was found
        {
            result = pwrite(fd, &item, sizeof(DataItem), offset);
            if (result <= 0)
            {
                printf("An error occured while trying to write to a recored...");
                return -1;
            }
            return numAccessed;
        }
        else    // This was not an empty record
        {
            offset = (offset + sizeof(DataItem)) % int(FILESIZE);
        }   
    }
    // No empty records were found
        // Use another hash function then open addressing
    //resetting the search initial parameters
    hashIndex = secondHashCode(item.key);
    offset = hashIndex * sizeof(Bucket);
    // Searching for an empty place starting from the second bucket
    for(int i = 0; i < MBUCKETS * RECORDSPERBUCKET; ++i)
    {
        int result = pread(fd, &record, sizeof(DataItem), offset);
        if (result <= 0)
        {
            printf("An error occured while trying to read a recored...");
            return -1;
        }
        ++numAccessed;
        if (record.valid == 0)  // An empty recocrd was found
        {
            result = pwrite(fd, &item, sizeof(DataItem), offset);
            if (result <= 0)
            {
                printf("An error occured while trying to write to a record...");
                return -1;
            }
            return numAccessed;
        }
        else    // This was not an empty record
        {
            offset = (offset + sizeof(DataItem)) % int(FILESIZE);
        } 
    }
    // The file is full
    return numAccessed;
}


/* Functionality: using a key, it searches for the data item
 *          1. use the first hash function to determine which bucket to search into
 *          2. if not found, use the second hash function to determine the new bucket
 *          3. search for the element starting from this second bucket untill this item is found or all records are searched
 *          3. return the data and the number of records accessed (searched)
 *
 * Input:  fd: filehandler which contains the db
 *         item: the dataitem which contains the key you will search for
 *               the dataitem is modified with the data when found
 *         count: No. of record searched
 *
 * Output: the offset (position) in the file where we found the item
 */

int searchItem(int fd, DataItem *item, int *count)
{
    *count = 0;
    int hashIndex = firstHashCode(item->key);
    DataItem record;
    int offset = hashIndex * sizeof(Bucket);
    
    for(int i = 0; i < RECORDSPERBUCKET; ++i)
    {
        int result = pread(fd, &record, sizeof(DataItem), offset);
        if (result <= 0)
        {
            printf("An error occured while trying to read a bucket...");
            return -1;
        }
        ++(*count);
        if (record.valid == 1 && record.key == item->key)  // A valid record with the same key was found
        {
            item->data = record.data;
            return offset;
        }
        else
        {
            offset = (offset + sizeof(DataItem)) % int(FILESIZE);
        }
    }
    // The bucket from the first hash function did not result in a match
    
    hashIndex = secondHashCode(item->key);
    offset = hashIndex * sizeof(Bucket);

    for(int i = 0; i < MBUCKETS * RECORDSPERBUCKET; ++i)
    {
        int result = pread(fd, &record, sizeof(DataItem), offset);
        if (result <= 0)
        {
            printf("An error occured while trying to read a bucket...");
            return -1;
        }
        ++(*count);
        if (record.valid == 1 && record.key == item->key)  // A valid record with the same key was found
        {
            item->data = record.data;
            return offset;
        }
        else
        {
            offset = (offset + sizeof(DataItem)) % int(FILESIZE);
        }
    }
    // The item is not in the file

    return -1;
}


/* Functionality: Display all the file contents
 *
 * Input:  fd: filehandler which contains the db
 *
 * Output: no. of non-empty records
 */
int DisplayFile(int fd){

	struct DataItem data;
	int count = 0;
	int Offset = 0;
	for(Offset =0; Offset< FILESIZE;Offset += sizeof(DataItem))
	{
		ssize_t result = pread(fd,&data,sizeof(DataItem), Offset);
		if(result < 0)
		{ 	  perror("some error occurred in pread");
			  return -1;
		} else if (result == 0 || data.valid == 0 ) { //empty space found or end of file
			printf("Bucket: %d, Offset %d:~\n",Offset/int(BUCKETSIZE),Offset);
		} else {
			pread(fd,&data,sizeof(DataItem), Offset);
			printf("Bucket: %d, Offset: %d, Data: %d, key: %d\n",Offset/int(BUCKETSIZE),Offset,data.data,data.key);
					 count++;
		}
	}
	return count;
}


/* Functionality: Delete item at certain offset
 *
 * Input:  fd: filehandler which contains the db
 *         Offset: place where it should delete
 *
 * Hint: you could only set the valid key and write just and integer instead of the whole record
 */
int deleteOffset(int fd, int Offset)
{
	struct DataItem dummyItem;
	dummyItem.valid = 0;
	dummyItem.key = -1;
	dummyItem.data = 0;
	int result = pwrite(fd,&dummyItem,sizeof(DataItem), Offset);
	return result;
}

