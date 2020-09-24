//
// Created by msi on 24/09/2020.
//

#include "RecordBucket.h"

template<typename Record>
Record *RecordBucket<Record>::getRecord() {
    return records;
}

template<typename Record>
RecordBucket<Record>::RecordBucket() {
    this->size = 0;
    this->nextBucket = -1;
}

template<typename Record>
int RecordBucket<Record>::getSize() {
    return size;
}

template<typename Record>
long RecordBucket<Record>::getNextBucket() {
    return nextBucket;
}

template<typename Record>
void RecordBucket<Record>::setSize(int _size) {
    this->size = _size;
}

template<typename Record>
void RecordBucket<Record>::setNextBucket(long _next) {
    this->nextBucket = _next;
}
