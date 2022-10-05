#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <cassert>
#include <map>

namespace bestCache {

template <typename T>
struct DataChain_t {
    T value;
    int nextIndex;
};

template <typename T, typename KeyT = int>
struct bestCache_t {
    size_t maxSize_;
    int dataMaxSize_;

    // list of all input elements
    using DataListIt = typename std::list<DataChain_t<KeyT>>::iterator;
    std::list<DataChain_t<KeyT>> data;
    std::unordered_map<KeyT, DataListIt> dataHash;

    // best cache
    std::multimap<int, T> cache;

    bestCache_t(size_t maxSize, int dataMaxSize): maxSize_(maxSize), dataMaxSize_(dataMaxSize) {}

    void addKey(KeyT key) {
        assert(dataMaxSize_ > 0);

        auto elemIt = dataHash.find(key);
        data.push_back({key, -1});
        
        if (elemIt != dataHash.end()) {
            elemIt->second->nextIndex = data.size() - 1;
        }

        dataHash[key] = std::prev(data.end());
    }

    template <typename F>
    int getHitsBestCache(F getPage) {
        assert(maxSize_ > 0);

        int hits = 0, index = 0;

        for (auto dataIt = data.begin(); dataIt != data.end(); ++dataIt) {
            
            if (cache.size() >= 1 && cache.begin()->first == index) {
                ++hits;
                cache.erase(cache.begin());
            }
            
            if (dataIt->nextIndex == -1) {
                ++index;
                continue;
            }
            
            if (cache.size() + 1 > maxSize_) {
                cache.erase(std::prev(cache.end()));
            }
            
            cache.insert(std::pair<int,T>(dataIt->nextIndex, getPage(dataIt->value)));
            
            ++index;
        }

        return hits;
    }

    void printData() const {
        for (auto it = data.begin(); it != data.end(); ++it) {
            std::cout << "value: " << it->value << "; nextIndex: " << it->nextIndex << std::endl;
        }
    }

    void printCache() const {
        for (auto it = cache.begin(); it != cache.end(); ++it) {
            std::cout << "value: " << it->second << "; nextIndex: " << it->first << std::endl;
        }
    }
};

}
