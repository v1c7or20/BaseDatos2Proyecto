//
// Created by msi on 29/09/2020.
//

#ifndef BASEDATOS2PROYECTO_SECUENTIALFILE_H
#define BASEDATOS2PROYECTO_SECUENTIALFILE_H

#include <iostream>
#include <string>
#include <fstream>


template <typename keyType, typename Record>
class SecuentialFile{
private:
    std::string fileName;
    long nroData;
public:
    SecuentialFile(std::string _filename){
        fileName = std::move(_filename);
        long first = 0, second = -1;
        nroData = getNroData();
        std::ofstream writer;
        writer.open(this->fileName, std::ios::in | std::ios::out | std::ios::binary);

        writer.seekp(0 * sizeof(long), std::ios::beg);
        writer.write((char *) &first, sizeof(long));
        writer.seekp(1 * sizeof(long), std::ios::beg);
        writer.write((char *) &second, sizeof(long));

    }

    void insertRecord(Record recordToInsert){
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
                writer.close();
                append.close();
                reader.close();
            }
        }
    }

    Record searchRecord(keyType keyToLook){

        std::ifstream reader;
        reader.open(this->fileName, std::ios::in);

        long positionBefore;
        reader.seekg(0,std::ios::beg);
        reader.read((char *) &positionBefore, sizeof(long));

        Record foundRecord(0);
        reader.seekg((positionBefore * sizeof(Record)) + (2 * sizeof(long )), std::ios::beg);
        reader.read((char *) &foundRecord, sizeof(Record));

        while (foundRecord.getNext() != -1){
            if (foundRecord.getKey() == keyToLook){
                return foundRecord;
            }
            reader.seekg((foundRecord.getNext() * sizeof(Record)) + (2 * sizeof(long )),std::ios::beg);
            reader.read((char *) &foundRecord, sizeof(Record));
        }
        return foundRecord;
    }

    void deleteRecord(keyType key) {
        std::fstream inFile;
        Record obj, objBefore;
        inFile.open(this->fileName, std::ios::in | std::ios::out | std::ios::binary);
        nroData--;

        if (inFile.is_open()) {
            long delPointer , delBefore, delPointBeg;
            std::ifstream reader;

            reader.open(this->fileName, std::ios::in);
            reader.seekg(0 , std::ios::beg);
            reader.read((char *) &delPointer, sizeof(long ));

            reader.seekg(2 * sizeof(long) + delPointer * sizeof(Record),std::ios::beg);
            reader.read((char *) &obj, sizeof(Record ));

            while (obj.getNext() != -1){
                if (obj.getKey() == key){
                    break;
                }
                objBefore = obj;
                delBefore = delPointer;
                delPointer = obj.getNext();

                reader.seekg(obj.getNext() * sizeof(Record) + 2 * sizeof(long ),std::ios::beg);
                reader.read((char *) &obj, sizeof(Record));

            }
            reader.seekg(sizeof(long ) , std::ios::beg);
            reader.read((char *) &delPointBeg, sizeof(long ));


            objBefore.setNext(obj.getNext());
            obj.setNext(-1);
            obj.setNextErased(delPointBeg);

            inFile.seekg(sizeof(long ), std::ios::beg);
            inFile.write((char *) &delPointer, sizeof(long ));

            inFile.seekg(2 * sizeof(long) + delPointer * sizeof(Record),std::ios::beg);
            inFile.write((char *) &obj, sizeof(Record));

            inFile.seekg(2 * sizeof(long) + delBefore * sizeof(Record),std::ios::beg);
            inFile.write((char *) &objBefore, sizeof(Record));

        }else
            std::cout << "Could not open the file.\n";
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

#endif //BASEDATOS2PROYECTO_SECUENTIALFILE_H
