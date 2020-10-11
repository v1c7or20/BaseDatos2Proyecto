#include <iostream>
#include "SecuentialFile.h"
#include "ISAM.h"
#include "Record.h"
#include "ExtendibleHash.h"
#include <chrono>

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_>
                (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

int main() {
    Record<int> rp(0);
    /*
    SecuentialFile<int,Record<int>> * fileSequential;
    fileSequential = new SecuentialFile<int ,Record<int>>("SecFile.data");

    ISAM<int,Record<int>> * isam;
    isam = new ISAM<int,Record<int>>("ISAM.data");

    Timer timer;

    for (int i = 0; i < 1000; ++i) {
        Record<int> recordSeq(i);
        fileSequential->insertRecord(recordSeq);
    }

    double t = timer.elapsed();
    std::cout << t <<std::endl;

    timer.reset();

    for (int i = 0; i < 50; ++i) {
        rp = fileSequential->searchRecord(i);
    }

    double t2 = timer.elapsed();
    std::cout << t2 <<std::endl;

    timer.reset();

    for (int i = 0; i < 1000; ++i) {
        Record<int> recordISAM(i);
        isam->insertRecord(recordISAM);
    }

    double t3 = timer.elapsed();
    std::cout << t3 <<std::endl;

    timer.reset();
    for (int i = 0; i < 26; ++i) {
        rp = isam->searchRecord(i);
    }
    double t4 = timer.elapsed();
    std::cout << t4 <<std::endl;
    */

    ExtendibleHash<Record<int>,int> * extendibleHash;
    extendibleHash = new ExtendibleHash<Record<int>, int>(3,"Exte");

    Timer timer;
    for (int i = 0; i < 1000; ++i) {
        Record<int> recordExt(i);
        extendibleHash->insertRecord(recordExt);
    }
    double t5 = timer.elapsed();
    std::cout<<t5<<std::endl;
    return 0;
}