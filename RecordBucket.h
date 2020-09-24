//
// Created by msi on 24/09/2020.
//

#ifndef BASEDATOS2PROYECTO_RECORDBUCKET_H
#define BASEDATOS2PROYECTO_RECORDBUCKET_H

const int BUCKET_SIZE = 5;

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
