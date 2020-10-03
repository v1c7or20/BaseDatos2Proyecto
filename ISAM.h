//
// Created by msi on 29/09/2020.
//

#ifndef BASEDATOS2PROYECTO_ISAM_H
#define BASEDATOS2PROYECTO_ISAM_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

template <typename keyType, typename Record>
class ISAM{
private:
    std::string fileName;
    std::string fileNameIndex0;
    std::string fileNameIndex1;
    long nroData = 0;
    const int nroInd = 16;
public:
    ISAM(std::string _file){
        fileName = _file;
        fileNameIndex0 = _file + ".index0";
        fileNameIndex1 = _file + ".index1";

        long first = 0, second = -1;
        nroData = 0;
        std::ofstream writer;
        writer.open(this->fileName, std::ios::in | std::ios::out | std::ios::binary);

        writer.seekp(0 * sizeof(long), std::ios::beg);
        writer.write((char *) &first, sizeof(long));
        writer.seekp(1 * sizeof(long), std::ios::beg);
        writer.write((char *) &second, sizeof(long));

    }

    void insertRecord(Record recordToInsert) {
        Record recordBefore(0);          //Record before recordToInsert
        long positionBefore;            //Position of record before recordToInsert

        long erasedPointer;             //Pointer to records that were deleted
        bool usedErasedSpace = false;   //Used to identify were to write te record

        long nroDataLocal = nroData;
        nroData++;

        Record recordNext(0);
        //Use only to read
        std::ifstream reader;
        reader.open(this->fileName, std::ios::in);
        //Write or overwrite records
        std::ofstream writer;
        writer.open(this->fileName, std::ios::in | std::ios::out | std::ios::binary);
        //Append data at the last part of the file
        std::ofstream append;
        append.open(this->fileName, std::ios::app | std::ios::binary);

        //Get the erased pointer from file
        reader.seekg(sizeof(long ), std::ios::beg);
        reader.read((char *) &erasedPointer, sizeof(long ));
        reader.seekg(0,std::ios::beg);
        reader.read((char *) &positionBefore, sizeof(long));

        if (nroDataLocal == 0 and erasedPointer == -1){
            append.write((char *) &recordToInsert, sizeof(Record));
            writer.seekp(std::ios::beg);
            writer.write((char *) &nroDataLocal, sizeof(long));
        } else if (nroDataLocal == 1 and erasedPointer == -1){
            reader.seekg( 2 * sizeof(long), std::ios::beg);
            reader.read((char *) (&recordBefore), sizeof(Record));
            if (recordBefore.getKey() < recordToInsert.getKey()) {
                recordBefore.setNext(nroDataLocal);
                recordToInsert.setNext(-1);
                nroDataLocal = 0;
            } else{

                recordBefore.setNext(-1);
                recordToInsert.setNext(0);
            }
            append.write((char *) &recordToInsert, sizeof(Record));
            writer.seekp(2 * sizeof(long), std::ios::beg);
            writer.write((char *) &recordBefore, sizeof(Record));
            writer.seekp(0 * sizeof(long), std::ios::beg);
            writer.write((char *) &nroDataLocal, sizeof(long ));

        } else{
            //Get the last position of deleted record
            if (erasedPointer != -1){
                Record  * iterErased;
                reader.seekg(erasedPointer * sizeof(Record) + 2 * sizeof(long), std::ios::beg);
                reader.read((char *) iterErased, sizeof(Record));
                writer.seekp(1 * sizeof(long), std::ios::beg);
                long nextErased = iterErased->getNextErased();
                writer.write((char *) &nextErased, sizeof(long));
                usedErasedSpace = true;
            }

            reader.seekg(0,std::ios::beg);
            reader.read((char *) &positionBefore, sizeof(long));
            reader.seekg(positionBefore * sizeof(Record) + 2 * sizeof(long), std::ios::beg);
            reader.read((char *) &recordBefore, sizeof(Record));

            if (recordToInsert.getKey() < recordBefore.getKey()){

                recordToInsert.setNext(positionBefore);
                writer.seekp(0 * sizeof(long), std::ios::beg);
                writer.write((char *) &nroDataLocal, sizeof(long));
            } else{
                do{
                    long nextPosition = recordBefore.getNext();
                    reader.seekg(nextPosition * sizeof(Record) + 2 * sizeof(long ), std::ios::beg);
                    reader.read((char *) &recordNext, sizeof(Record));
                    if (recordBefore.getKey() < recordToInsert.getKey() and recordToInsert.getKey() < recordNext.getKey()){

                        recordToInsert.setNext(recordBefore.getNext());
                        recordBefore.setNext(nroDataLocal);
                        writer.seekp(positionBefore * sizeof(Record) + 2 * sizeof(long), std::ios::beg);
                        writer.write((char *) &recordBefore, sizeof(Record));
                        break;
                    }
                    recordBefore = recordNext;
                    positionBefore = nextPosition;
                }while (recordNext.getNext() != -1);
            }
            if (recordNext.getKey() < recordToInsert.getKey()){

                recordToInsert.setNext(-1);
                recordNext.setNext(nroDataLocal);
                writer.seekp(positionBefore * sizeof(Record) + 2 * sizeof(long), std::ios::beg);
                writer.write((char *) &recordNext, sizeof(Record));
            }

            if (usedErasedSpace){
                writer.seekp(erasedPointer * sizeof(Record) + 2 * sizeof(long), std::ios::beg);
                writer.write((char *) &recordToInsert, sizeof(Record));
            } else{
                append.write((char *) &recordToInsert, sizeof(Record));
            }
        }
        reviewIndex();
    }

    void reviewIndex(){
        int nroIndex = getNroDataFile(this->fileNameIndex1);
        if ((int) (nroData / nroInd != 0) and nroData == (nroIndex+1) * nroInd) {
            int nroPoint = nroInd / 2, totalPointer = (nroData / nroInd) * 2;
            std::vector<long> positions;

            for (int data = 1; data <= totalPointer; ++data) {
                long toInsert =  nroPoint * data - 1;
                positions.push_back(toInsert);
            }

            std::ifstream reader;
            reader.open(this->fileName, std::ios::in);

            std::ofstream writer;
            writer.open(this->fileNameIndex0, std::ios::in | std::ios::out | std::ios::binary);

            long positionBefore;
            reader.seekg(0,std::ios::beg);
            reader.read((char *) &positionBefore, sizeof(long));

            Record foundRecord(0);
            reader.seekg((positionBefore * sizeof(Record)) + (2 * sizeof(long )), std::ios::beg);
            reader.read((char *) &foundRecord, sizeof(Record));

            int pos = 0;
            for (int datos = 0; datos < nroData; ++datos) {
                auto it = std::find(positions.begin(),positions.end(),datos);
                if (it != positions.end()){
                    std::pair<keyType,long> toEnter(foundRecord.getKey(),positionBefore);
                    writer.seekp((pos * sizeof(std::pair<keyType,long>)));
                    writer.write((char *) &toEnter.first, sizeof(keyType));
                    writer.write((char *) &toEnter.second, sizeof(long ));
                    pos++;
                }
                positionBefore = foundRecord.getNext();
                reader.seekg((foundRecord.getNext() * sizeof(Record)) + (2 * sizeof(long )),std::ios::beg);
                reader.read((char *) &foundRecord, sizeof(Record));
            }
        }
        long nroDataIndex0 = getNroDataFile(this->fileNameIndex0);
        if (nroDataIndex0 / nroInd != 0){
            int nroPoint = nroInd / 2, totalPointer = (nroData / nroInd) * 2;
            std::vector<long> positions;

            for (int data = 0; data < totalPointer; ++data) {
                long toInsert =  nroPoint * data - 1;
                positions.push_back(toInsert);
            }

            std::ifstream reader;
            reader.open(this->fileNameIndex0, std::ios::in);

            std::ofstream writer;
            writer.open(this->fileNameIndex1, std::ios::in | std::ios::out | std::ios::binary);

            long positionBefore;
            reader.seekg(0,std::ios::beg);
            reader.read((char *) &positionBefore, sizeof(long));

            Record foundRecord(0);
            reader.seekg((positionBefore * sizeof(Record)) + (2 * sizeof(long )), std::ios::beg);
            reader.read((char *) &foundRecord, sizeof(Record));

            int pos = 0;
            for (int datos = 0; datos < nroData; ++datos) {
                auto it = std::find(positions.begin(),positions.end(),datos);
                if (it != positions.end()){
                    std::pair<keyType,long>toEnter(foundRecord.getKey(),positionBefore);
                    writer.seekp((pos * sizeof(Record)));
                    writer.write((char *) &toEnter.first, sizeof(keyType));
                    writer.write((char *) &toEnter.second, sizeof(long ));
                    pos++;
                }
                positionBefore = foundRecord.getNext();
                reader.seekg((foundRecord.getNext() * sizeof(Record)) + (2 * sizeof(long )),std::ios::beg);
                reader.read((char *) &foundRecord, sizeof(Record));
            }
        }
    }

    long getFromIndex0(keyType key){
        std::pair<keyType,long> pointerToCompare;
        std::ifstream reader;
        reader.open(this->fileNameIndex0, std::ios::in);

        long start = getFromIndex1(key);

        long data = getNroDataFile(this->fileNameIndex0);

        if (data == 0){
            return 0;
        } else{
            for (int iter = start; iter < data; ++iter) {
                reader.seekg(iter * (sizeof(keyType) + sizeof(long)), std::ios::beg);
                reader.read((char *) &(pointerToCompare.first), sizeof(keyType));
                reader.read((char *) &(pointerToCompare.second), sizeof(long));

                if (key < pointerToCompare.first){
                    return pointerToCompare.second-1;
                }
            }
            if (pointerToCompare.first < key){
                return pointerToCompare.second;
            }
            return 0;
        }
    }

    long getFromIndex1(keyType key){
        std::pair<keyType,long> pointerToCompare;
        std::ifstream reader;
        reader.open(this->fileNameIndex1, std::ios::in);

        long data = getNroDataFile(this->fileNameIndex1);

        if (data == 0){
            return 0;
        } else{
            for (int iter = 0; iter < data; ++iter) {
                reader.seekg(iter * (sizeof(keyType) + sizeof(long )), std::ios::beg);
                reader.read((char *) &pointerToCompare.first, sizeof(keyType));
                reader.read((char *) &pointerToCompare.second, sizeof(long));
                if (key < pointerToCompare.first){
                    return pointerToCompare.second;
                }
            }
            return 0;
        }
    }

    long getNroDataFile(std::string _file){
        int numRecords = 0;
        std::fstream inFile;

        inFile.open(_file, std::ios::in | std::ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, std::ios::end);
            long bytes = inFile.tellg() ;
            numRecords = (bytes) / (sizeof(keyType)+sizeof(long ));

            inFile.close();
        } else {
            std::cout << "Could not open the file.\n";
        }
        return numRecords;
    }

    Record searchRecord(keyType keyToLook){

        std::ifstream reader;
        reader.open(this->fileName, std::ios::in);

        long positionBefore = getFromIndex0(keyToLook);

        Record foundRecord(0);
        reader.seekg((positionBefore * sizeof(Record)) + (2 * sizeof(long )), std::ios::beg);
        reader.read((char *) &foundRecord, sizeof(Record));

        while (foundRecord.getNext() != -1 or foundRecord.getKey() == keyToLook){
            if (foundRecord.getKey() == keyToLook){
                return foundRecord;
            }
            reader.seekg((foundRecord.getNext() * sizeof(Record)) + (2 * sizeof(long )),std::ios::beg);
            reader.read((char *) &foundRecord, sizeof(Record));
        }
    }
};

#endif //BASEDATOS2PROYECTO_ISAM_H
