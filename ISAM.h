//
// Created by msi on 29/09/2020.
//

#ifndef BASEDATOS2PROYECTO_ISAM_H
#define BASEDATOS2PROYECTO_ISAM_H

#include <fstream>
#include <string>
#include <iostream>

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

    }
    void insertRecord(Record recordToInsert) {
        Record recordBefore(0);          //Record before recordToInsert
        long positionBefore;            //Position of record before recordToInsert

        long erasedPointer;             //Pointer to records that were deleted
        bool usedErasedSpace = false;   //Used to identify were to write te record

        long nroDataLocal = nroData++;

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
    }

    void reviewIndex(){

    }

    long getNroData(){
        int numRecords = 0;
        std::fstream inFile;
        inFile.open(this->fileName, std::ios::in | std::ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, std::ios::end);
            long bytes = inFile.tellg() ;
            bytes = bytes - (2 * sizeof(long));
            numRecords = (bytes) / sizeof(Record);
            inFile.close();
        } else {
            std::cout << "Could not open the file.\n";
        }
        return numRecords;
    }
};

#endif //BASEDATOS2PROYECTO_ISAM_H
