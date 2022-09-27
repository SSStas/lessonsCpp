#include <cassert>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cerrno>
#include "cache2Q.hpp"
#include "bestCache.hpp"


int getPageInt(int key) { return key; }

int startInt2Q() {
    int hits = 0;
    int n;
    size_t m;

    std::cin >> m >> n;
    assert(std::cin.good());
    cache2q::Cache2q_t<int> c{m};

    for (int i = 0; i < n; ++i) {
        int q;
        std::cin >> q;
        assert(std::cin.good());
        if (c.appendTo2Q(q, getPageInt))
            hits += 1;
    }

    return hits;
}

void checkTestInt2Q(int testNumber, std::string fileName) {
    int n, hits = 0, testHits = -1;
    size_t m;
    std::ifstream file; 

    file.open("tests/" + fileName);

    if (file.is_open()) {
        file >> m >> n;
        assert(file.good());
        cache2q::Cache2q_t<int> c{m};
        bestCache::bestCache_t<int> bc{m, n};

        for (int i = 0; i < n; ++i) {
            int q;
            file >> q;
            assert(file.good());

            bc.addKey(q);
            if (c.appendTo2Q(q, getPageInt))
                hits += 1;
        }

        file >> testHits;

        file.close();
        
        std::cout << "Test number: " << testNumber << "; " << ("tests/" + fileName) << " " << 
            ((hits == testHits) ? "OK" : ("ERR (answer: " + std::to_string(testHits) + ")")) << std::endl;
        
        if (hits == testHits) {
            std::cout << "2Qcache: " << hits << "; Best cache: " << bc.getHitsBestCache(getPageInt) << std::endl << std::endl;
        }
    } else {
        perror("File open failed");
    }

    return;
}


int main(int argc, char **argv) {
    int countTests = 30;
    
    if (argc == 2 && (!strcmp(argv[1], "-test") || !strcmp(argv[1], "--test") || !strcmp(argv[1], "-t"))) {
        for (int i = 1; i <= countTests; ++i) {
            checkTestInt2Q(i, "test" + std::to_string(i) + ".txt");
        }
    } else if (argc == 1) {
        std::cout << startInt2Q() << std::endl;
    } else {
        std::cout << "Arguments error" << std::endl;
    }
    
    return 0;
}