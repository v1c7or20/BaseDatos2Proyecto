#include <iostream>
#include "SecuentialFile.h"
#include "Record.h"


int main() {
    SecuentialFile<std::string,Record<std::string>> * fileSequential;
    fileSequential = new SecuentialFile<std::string ,Record<std::string>>("SecFile.data");

    Record<std::string> r1("key1"),r2("key2"),r3("key3"),r4("key4"),r5("key5"),r6("key6"),r7("key7"),r8("key8");

    fileSequential->insertRecord(r1);
    fileSequential->insertRecord(r6);
    fileSequential->insertRecord(r2);
    fileSequential->insertRecord(r4);
    fileSequential->insertRecord(r7);
    Record<std::string> rp(" ");
    rp = fileSequential->searchRecord("key1");
    rp = fileSequential->searchRecord("key4");
    rp = fileSequential->searchRecord("key6");
    rp = fileSequential->searchRecord("key2");
    rp = fileSequential->searchRecord("key7");

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
