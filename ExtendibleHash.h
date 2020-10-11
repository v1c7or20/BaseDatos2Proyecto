#ifndef BASEDATOS2PROYECTO_EXTENDIBLEHASHTREE_H
#define BASEDATOS2PROYECTO_EXTENDIBLEHASHTREE_H

#include <unordered_map>
#include <fstream>
#include <math.h>
#include "Bucket.h"

template<typename Record, typename keyType>
class ExtendibleHash {
private:
    std::string file;
    int globalDepth;
    std::pair<int, int> * tableInd;
    std::pair<int, long> * tablePos;
    int nroBuckets = 2;

public:
    ExtendibleHash(int globalDepthC, std::string fileC){
        globalDepth = globalDepthC;
        int totalColumns = pow(2,globalDepth);
        tableInd = new std::pair<int,int>[totalColumns];
        tablePos = new std::pair<int,long>[totalColumns];
        file = fileC;
        std::ofstream write;
        Bucket<Record, keyType> record;
        write.open(fileC, std::ios::app | std::ios::binary);
        write.write((char *) &record, sizeof(Record));
        write.write((char *) &record, sizeof(Record));
        for (int i = 0; i < totalColumns; i++){
            tableInd[i].first = i;
            tableInd[i].second = 1;
            tablePos[i].first = i;
            tablePos[i].second = i%2;
        }
    }

    std::pair<int, int> * getTable(){
        return tableInd;
    }

    std::pair<int,long> * getTablePos(){
        return tablePos;
    }

    int getGlobalDepth(){
        return globalDepth;
    }

    void insertRecord(Record recordToInsert){
        std::ifstream reader;
        Bucket<Record,keyType> bucket;
        reader.open(file, std::ios::in);
        int totalColumns = pow(2,globalDepth);
        int hashed = recordToInsert.getKey() % totalColumns;
        long pos = tablePos[hashed].second;
        reader.seekg(sizeof(Bucket<Record, keyType>) * pos);
        reader.read((char *) &bucket, sizeof(Bucket<Record, keyType>));
        int localDepth = tableInd[hashed].second;
        if(bucket.getSize() < 5 or tableInd[hashed].second == globalDepth){
            bucket.add_data(recordToInsert,file,nroBuckets);
            std::ofstream writeO;
            writeO.open(file, std::ios::in | std::ios::out | std::ios::binary);
            writeO.seekp(pos*sizeof(Bucket<Record,keyType >),std::ios::beg);
            writeO.write((char *) &bucket, sizeof(Bucket<Record,keyType >));
            writeO.close();
        } else if (tableInd[hashed].second < globalDepth){
            Bucket<Record, keyType> first,second;
            int toChange = 0;
            nroBuckets++;
            for (int i =0; i < totalColumns; i++){
                if (tablePos[i].second  == pos){
                    toChange++;
                    tableInd[i].second = localDepth + 1;
                    if (toChange%2 == 1){
                        tablePos[i].second = nroBuckets-1;
                    }
                }
            }
            Record * array = bucket.getdata();
            for (int r = 0; r < bucket.getSize(); ++r) {
                int hashedNr = array[r].getKey()%totalColumns;
                if (tablePos[hashedNr].second == pos){
                    first.add_data(array[r],file);
                } else{
                    second.add_data(array[r],file);
                }
            }
            first.add_data(recordToInsert,file);
            std::ofstream write;
            write.open(file, std::ios::app | std::ios::binary);
            write.write((char *) &second, sizeof(Bucket<Record,keyType >));
            std::ofstream writeO;
            writeO.open(file, std::ios::binary | std::ios::in | std::ios::out);
            writeO.seekp(pos*sizeof(Bucket<Record,keyType >),std::ios::beg);
            writeO.write((char *) &first,sizeof(Bucket<Record,keyType >));
            write.close();
            write.close();
        }
        reader.close();
    }

    Record * searchRecord(keyType key){
        int totalColumns = pow(2,globalDepth);
        int hashed = key % totalColumns;
        int pos = tablePos[hashed].second;

    }
};

#endif //BASEDATOS2PROYECTO_EXTENDIBLEHASHTREE_H
