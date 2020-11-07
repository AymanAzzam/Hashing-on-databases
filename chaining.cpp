//
// Created by aymanElakwah on 11/7/20.
//

#include "readfile.h"
#include <math.h>

#define OVERFLOW_BUCKETS int(floor(0.2 * MBUCKETS))

/* Hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */
int hashCode(int key) {
    return key % (MBUCKETS - OVERFLOW_BUCKETS);
}

ssize_t readBucket(int fd, int index, ChainingBucket &chainingBucket) {
    return pread(fd, &chainingBucket, sizeof(ChainingBucket), index * sizeof(ChainingBucket));
}

ssize_t readRecord(int fd, int bucketIndex, int recordIndex, DataItem &record) {
    return pread(fd, &record, sizeof(DataItem), bucketIndex * sizeof(ChainingBucket) + recordIndex * sizeof(DataItem));
}

ssize_t writeRecord(int fd, int bucketIndex, int recordIndex, DataItem record) {
    return pwrite(fd, &record, sizeof(DataItem), bucketIndex * sizeof(ChainingBucket) + recordIndex * sizeof(DataItem));
}

ssize_t readOverflowIndex(int fd, int bucketIndex, int &overflowIndex) {
    size_t offset = bucketIndex * sizeof(ChainingBucket) + RECORDSPERBUCKET * sizeof(DataItem);
    return pread(fd, &overflowIndex, sizeof(int), offset);
}

ssize_t writeOverflowIndex(int fd, int bucketIndex, int overflowIndex) {
    return pwrite(fd, &overflowIndex, sizeof(int),
                  bucketIndex * sizeof(ChainingBucket) + RECORDSPERBUCKET * sizeof(DataItem));
}

int insertInOverflow(int fd, int originalBucketIndex, const DataItem &item) {

}

int emptyOverflowIndex(int fd, int &accessCount) {
    for (int bucketIndex = MBUCKETS - OVERFLOW_BUCKETS; bucketIndex < MBUCKETS; ++bucketIndex) {
        int overflow_index;
        readOverflowIndex(fd, bucketIndex, overflow_index);
        if (overflow_index == 0)
            return bucketIndex;
        accessCount++;
    }
    return -1;
}

int insertIntoBucket(int fd, int bucketIndex, int &accessCount, const DataItem &item) {
    ChainingBucket chainingBucket;
    ssize_t success = readBucket(fd, bucketIndex, chainingBucket);
    if (success <= 0) {
        perror("Can't read a bucket");
        return -1;
    }
    for (int recordIndex = 0; recordIndex < RECORDSPERBUCKET; ++recordIndex) {
        if (chainingBucket.dataItems[recordIndex].valid == 1) {
            accessCount++;
        } else {
            success = writeRecord(fd, bucketIndex, recordIndex, item);
            if (success <= 0) {
                perror("Can't write a record");
                return -1;
            }
            return 0;
        }
    }
    if (chainingBucket.overflow_index <= 0) {
        int overflowIndex = emptyOverflowIndex(fd, accessCount);
        if (overflowIndex == -1) {
            perror("Overflow section is full");
            return -1;
        }
        writeRecord(fd, overflowIndex, 0, item);
        // write -1 in the overflow index of the overflow bucket is an indication that this bucket is occupied,
        // it will never be zero again, it will either point to another overflow bucket or it will remain -1
        writeOverflowIndex(fd, overflowIndex, -1);
        writeOverflowIndex(fd, bucketIndex, overflowIndex);
        if (success <= 0) return -1;
    } else if (insertIntoBucket(fd, chainingBucket.overflow_index, accessCount, item) == -1) {
        perror("Can't insert the record");
    }
    return 0;
}

int insertItem(int fd, DataItem item) {
    int accessCount = 0;
    int hashIndex = hashCode(item.key);
    insertIntoBucket(fd, hashIndex, accessCount, item);
    return accessCount;

}

int searchInBucket(int fd, int bucketIndex, const DataItem &record, int &accessCount) {
    ChainingBucket bucket;
    readBucket(fd, bucketIndex, bucket);
    for (int recordIndex = 0; recordIndex < RECORDSPERBUCKET; ++recordIndex) {
        const DataItem &currentRecord = bucket.dataItems[recordIndex];
        accessCount++;
        if (currentRecord.valid == 1 && currentRecord.key == record.key)
            return bucketIndex * sizeof(ChainingBucket) + recordIndex * sizeof(DataItem);
    }
    if (bucket.overflow_index > 0)
        return searchInBucket(fd, bucket.overflow_index, record, accessCount);
    return -1;
}


/* Functionality: using a key, it searches for the data item
 *          1. use the hash function to determine which bucket to search into
 *          2. search for the element starting from this bucket and till it find it.
 *          3. return the number of records accessed (searched)
 *
 * Input:  fd: filehandler which contains the db
 *         item: the dataitem which contains the key you will search for
 *               the dataitem is modified with the data when found
 *         count: No. of record searched
 *
 * Output: the in the file where we found the item
 */

int searchItem(int fd, struct DataItem *item, int *count) {
    int hashIndex = hashCode(item->key);
    return searchInBucket(fd, hashIndex, *item, *count);
}


/* Functionality: Display all the file contents
 *
 * Input:  fd: filehandler which contains the db
 *
 * Output: no. of non-empty records
 */
int DisplayFile(int fd) {
    int count = 0;
    ChainingBucket bucket;
    for (int bucketIndex = 0; bucketIndex < MBUCKETS; ++bucketIndex) {
        readBucket(fd, bucketIndex, bucket);
        for (int recordIndex = 0; recordIndex < RECORDSPERBUCKET; ++recordIndex) {
            int offset = bucketIndex * sizeof(ChainingBucket) + recordIndex * sizeof(DataItem);
            const DataItem &data = bucket.dataItems[recordIndex];
            if (data.valid == 0) { //empty space found or end of file
                printf("Bucket: %d, Offset: %d ~\n", bucketIndex, offset);
            } else {
                printf("Bucket: %d, Offset: %d, Data: %d, Key: %d\n", bucketIndex, offset, data.data, data.key);
                count++;
            }
        }
        printf("Overflow Index: %d\n", bucket.overflow_index);
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
int deleteOffset(int fd, int Offset) {
    int valid = 0;
    return pwrite(fd, &valid, sizeof(int), Offset);
}

