#include <iostream>
#include "SecuentialFile.h"
#include "ISAM.h"
#include "Record.h"
#include "ExtendibleHash.h"

int main() {
    ExtendibleHashTree<Record<int>,int> extendibleHashTree(3,"Exte.data");
    for (int i = 0; i < 46; ++i) {
        Record<int32_t> recordISAM(i*2);
        extendibleHashTree.insertRecord(recordISAM);
    }


    SecuentialFile<int,Record<int>> * fileSequential;
    fileSequential = new SecuentialFile<int ,Record<int>>("SecFile.data");

    Record<int32_t> rp(0);

    for (int i = 0; i < 46; ++i) {
        Record<int32_t> recordISAM(i*2);
        fileSequential->insertRecord(recordISAM);
    }

    rp = fileSequential->searchRecord(1);
    rp = fileSequential->searchRecord(4);
    rp = fileSequential->searchRecord(6);
    rp = fileSequential->searchRecord(2);
    rp = fileSequential->searchRecord(7);

    ISAM<int,Record<int>> * isam;
    isam = new ISAM<int,Record<int>>("ISAM.data");

    for (int i = 0; i < 26; ++i) {
        Record<int> recordISAM(i);
        isam->insertRecord(recordISAM);
    }

    rp = isam->searchRecord(14);

    return 0;
}