#include "tlb.h"
#include <iostream>

TLB::TLB(int size): sz(size), entries(size, {-1,-1}) {}

int TLB::lookup(int page) {
    for (auto &e: entries) {
        if (e.first == page) return e.second;
    }
    return -1;
}

void TLB::insert(int page, int frame) {
    // simple FIFO replacement: shift
    for (int i = sz-1; i>0; --i) entries[i] = entries[i-1];
    entries[0] = {page, frame};
}
