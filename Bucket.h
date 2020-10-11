//
// Created by msi on 8/10/2020.
//

#ifndef BASEDATOS2PROYECTO_BUCKET_H
#define BASEDATOS2PROYECTO_BUCKET_H

#include <algorithm>
#include <string>
#include <fstream>

template <typename data>
bool comparerData(data *data1, data *data2){
    return data1->getKey() < data2->getKey();
}

template <typename data, typename keyType>
class Bucket{
private:
    data dataArray[5];
    int sizeArray = 0;
    long nextBucket = -1;
public:

    Bucket(){

    }
    data * getdata(){
        return dataArray;
    }

    int getSize(){
        return sizeArray;
    }

    void add_data(data dataRecord, std::string file){
        if (sizeArray == 5){
            if (nextBucket == -1){
                Bucket<data, keyType> newBucket;
                newBucket.add_data(dataRecord,file);
                std::ofstream writer;
                writer.open(file,std::ios::app | std::ios::binary);
                writer.write((char *) &newBucket,sizeof(Bucket<data,keyType>));
                writer.close();

            } else{
                Bucket<data,keyType> nextBucketB;
                std::ifstream reader;
                reader.open(file,std::ios::in);
                reader.seekg(nextBucket * sizeof(Bucket<data,keyType>),std::ios::beg);
                reader.read((char *) &nextBucketB, sizeof(Bucket<data,keyType>));
                nextBucketB.add_data(dataRecord,file);

                std::ofstream writer;
                writer.open(file,std::ios::app | std::ios::binary);
                writer.write((char *) &nextBucketB,sizeof(Bucket<data,keyType>));
                writer.close();
            }
        } else{
            dataArray[sizeArray] = dataRecord;
        }
        sizeArray++;
    }

    data * getRecord(keyType key,std::string file){
        for (auto & dato : dataArray){
            data * data1 = new data();
            if (dato.getKey() == key){
                *data1 = dato;
                return data1;
            }
        }
        if (nextBucket != -1){
            Bucket<data,keyType> nextBucketB;
            std::ifstream reader;
            reader.open(file,std::ios::in);
            reader.seekg(nextBucket * sizeof(Bucket<data,keyType>),std::ios::beg);
            reader.read((char *) &nextBucketB, sizeof(Bucket<data,keyType>));
            reader.close();
            return nextBucketB->getRecord(key);
        }
        return nullptr;
    }
};



#endif //BASEDATOS2PROYECTO_BUCKET_H
