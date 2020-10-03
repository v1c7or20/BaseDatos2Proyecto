#include <iostream>
#include "SecuentialFile.h"
#include "ISAM.h"
#include "Record.h"


int main() {
    SecuentialFile<int,Record<int>> * fileSequential;
    fileSequential = new SecuentialFile<int ,Record<int>>("SecFile.data");


    Record<int> rp(0);

    for (int i = 0; i < 26; ++i) {
        Record<int> recordISAM(i);
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
