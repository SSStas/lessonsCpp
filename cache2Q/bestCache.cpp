#include <cassert>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cerrno>
#include "bestCache.hpp"


int getPageInt(int key) { return key; }

int main() {
    int n;
    size_t m;

    std::cin >> m >> n;
    assert(std::cin.good());
    bestCache::bestCache_t<int> bc{m, n};

    for (int i = 0; i < n; ++i) {
        int q;
        std::cin >> q;
        assert(std::cin.good());
        bc.addKey(q);
    }

    std::cout << bc.getHitsBestCache(getPageInt) << std::endl;
    
    return 0;
}
