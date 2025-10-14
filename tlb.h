#pragma once
#include <vector>

class TLB {
public:
    TLB(int size);
    int lookup(int page); // returns frame or -1
    void insert(int page, int frame);
private:
    int sz;
    std::vector<std::pair<int,int>> entries; // (page, frame), -1 page means empty
};
