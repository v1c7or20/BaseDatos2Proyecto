//
// Created by msi on 30/09/2020.
//

#ifndef BASEDATOS2PROYECTO_RECORD_H
#define BASEDATOS2PROYECTO_RECORD_H

template <typename keyType>
class Record{
private:
    keyType key;
    long next = -1;
    long nextErased;
public:
    keyType getKey(){
        return key;
    }

    long getNext(){
        return next;
    }

    long getNextErased(){
        return nextErased;
    }

    void setNext(long _next){
        next = _next;
    }

    void setNextErased(long _nextErased){
        nextErased = _nextErased;
    }

    Record(keyType _key){
        key = _key;
    }
    Record(void ){

    }
};

#endif //BASEDATOS2PROYECTO_RECORD_H
