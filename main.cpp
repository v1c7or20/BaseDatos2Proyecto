#include <iostream>
#include "SecuentialFile.h"
#include "Record.h"


int main() {
    SecuentialFile<int,Record<int>> * fileSequential;
    fileSequential = new SecuentialFile<int ,Record<int>>("SecFile.data");

    Record<int> r1(1),r2(2),r3(3),r4(4),r5(5),r6(6),r7(7),r8(8);

    fileSequential->insertRecord(r1);
    fileSequential->insertRecord(r6);
    fileSequential->insertRecord(r2);
    fileSequential->insertRecord(r4);
    fileSequential->insertRecord(r7);
    Record<int> rp(0);
    rp = fileSequential->searchRecord(1);
    rp = fileSequential->searchRecord(4);
    rp = fileSequential->searchRecord(6);
    rp = fileSequential->searchRecord(2);
    rp = fileSequential->searchRecord(7);

    return 0;
}
