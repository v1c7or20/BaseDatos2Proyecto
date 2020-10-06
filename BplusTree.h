//
// Created by msi on 29/09/2020.
//

#ifndef BASEDATOS2PROYECTO_BPLUSTREE_H
#define BASEDATOS2PROYECTO_BPLUSTREE_H

#include <fstream>
#include <vector>
#include <string>


template <typename keyType, typename Record>
class node{
private:
    std::vector<keyType> * keys;
    std::vector<node<keyType,Record>> * nodes;
    bool isLeaf = false;

public:
    std::vector<node<keyType,Record>> getNodes(){
        return nodes;
    }

    std::vector<keyType> getKeys(){
        return keys;
    }
    void insertRecord(Record recordToInsert){
        if (not isLeaf){
            if (recordToInsert->getKey() < keys->at(0)){

            }
        }
    }


};

template <typename keyType, typename Record>
class bPlusThree{
private:
    node<keyType,Record> * root;
    std::string filename;
public:
    void insertRecord(Record recordToInsert){
        root->insertRecord(recordToInsert);
    }
};

#endif //BASEDATOS2PROYECTO_BPLUSTREE_H
