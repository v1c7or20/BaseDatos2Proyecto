//
// Created by msi on 24/09/2020.
//


#ifndef BASEDATOS2PROYECTO_RECORDBUCKET_H
#define BASEDATOS2PROYECTO_RECORDBUCKET_H

const int BUCKET_SIZE = 128;

template <typename Record>
class RecordBucket {
private:
    Record records[BUCKET_SIZE];
    int size;
    long nextBucket;
public:
    Record* getRecord();

    RecordBucket();

    int getSize();

    long getNextBucket();

    void setSize(int _size);

    void setNextBucket(long _next);
};


#endif //BASEDATOS2PROYECTO_RECORDBUCKET_H

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
