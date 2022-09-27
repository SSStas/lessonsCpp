#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <cassert>

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
    using ListIt = typename std::list<DataChain_t<T>>::iterator;
    std::list<DataChain_t<T>> cache;
    std::unordered_map<int, ListIt> hash;

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
    ListIt inputCacheValue(DataChain_t<KeyT> dataChain, F getPage) {
        for (auto it = cache.begin(); it != cache.end(); ++it) {
            if (dataChain.nextIndex <= it->nextIndex) {
                cache.insert(it, dataChain);
                return std::prev(it);
            }
        }

        cache.insert(cache.end(), dataChain);
        return std::prev(cache.end());
    }

    template <typename F>
    int getHitsBestCache(F getPage) {
        assert(maxSize_ > 0);

        int hits = 0, index = 0;

        for (auto dataIt = data.begin(); dataIt != data.end(); ++dataIt) {
            auto hit = hash.find(index);

            if (hit != hash.end()) {
                ++hits;
                cache.erase(hit->second);
                hash.erase(index);
            }
            
            if (dataIt->nextIndex == -1) {
                ++index;
                continue;
            }

            if (cache.size() + 1 > maxSize_) {
                hash.erase((std::prev(cache.end()))->nextIndex);
                cache.pop_back();
            }

            hash[dataIt->nextIndex] = inputCacheValue(*dataIt, getPage);

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
            std::cout << "value: " << it->value << "; nextIndex: " << it->nextIndex << std::endl;
        }
    }
};

}
