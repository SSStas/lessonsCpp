#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <cassert>


namespace cache2q {

enum QueueName { FIFO_IN, FIFO_OUT, LRU2 };

template <typename KeyT, typename T>
struct Chain_t {
    QueueName name;
    KeyT dataKey;
    T data;
};

struct CacheSizes_t {
    size_t maxSize = 0;
    size_t currentSize = 0;

    void setSize(size_t allSize, int a, int b, int extra) {
        maxSize = allSize / b * a + extra;  
        currentSize = 0;
    }

    bool isFull() const { return (maxSize == currentSize); }
    void print() const { std::cout << "maxSize: " << maxSize << std::endl << "currentSize: " << currentSize << std::endl; }
};

template <typename T, typename KeyT = int>
struct Cache2q_t {
    CacheSizes_t fifoInSize;
    CacheSizes_t fifoOutSize;
    CacheSizes_t lru2Size;
    
    using ListIt = typename std::list<Chain_t<KeyT, T>>::iterator;
    ListIt fifoOutBegin;
    ListIt lru2Begin;

    std::list<Chain_t<KeyT, T>> cache;
    std::unordered_map<KeyT, ListIt> hash;

    Cache2q_t(size_t maxSize) {
        assert(maxSize > 0);

        int extraFifioIn = 0, extraFifoOut = 0, extraLru2 = 0;

        switch (maxSize % 5) {
            case 4: extraFifoOut++;
            case 3: extraFifioIn++;
            case 2: extraLru2++;
            case 1: extraFifoOut++;
            default: break;
        }

        fifoInSize.setSize(maxSize, 1, 5, extraFifioIn); 
        fifoOutSize.setSize(maxSize, 2, 5, extraFifoOut); 
        lru2Size.setSize(maxSize, 2, 5, extraLru2); 

        fifoOutBegin = cache.end();
        lru2Begin = cache.end();
    }

    template <typename F>
    bool appendTo2Q(KeyT key, F getPage) {
        assert(fifoOutSize.maxSize > 0);

        auto hit = hash.find(key);

        // not found
        if (hit == hash.end()) {
            
            if (fifoInSize.isFull() && fifoOutSize.isFull()) {
                fifoOutSize.currentSize--;
                hash.erase(std::prev(lru2Begin)->dataKey);
                cache.erase(std::prev(lru2Begin));
            }

            cache.push_front({(fifoInSize.maxSize > 0) ? FIFO_IN : FIFO_OUT, key, getPage(key)});
            hash[key] = cache.begin();

            if (!fifoInSize.isFull()) {
                fifoInSize.currentSize++;
            } else if (!fifoOutSize.isFull()) {
                fifoOutSize.currentSize++;
                if (fifoOutSize.currentSize == 1) {
                    fifoOutBegin = std::prev(lru2Begin);
                } else {
                    fifoOutBegin--;
                }
                fifoOutBegin->name = FIFO_OUT;
            }
            
            return false;
        }   

        
        if (lru2Size.maxSize == 0) 
            return true;

        auto eltit = hit->second;

        if (eltit->name == FIFO_OUT) {
            fifoOutSize.currentSize--;
            eltit->name = LRU2;

            if (lru2Size.isFull()) {
                hash.erase(std::prev(cache.end())->dataKey);
                cache.pop_back();
                if (lru2Size.maxSize == 1) {
                    lru2Begin = cache.end();
                }
            } else {
                lru2Size.currentSize++;
            }

            fifoOutBegin = (eltit != fifoOutBegin) ? fifoOutBegin : 
                ((!fifoOutSize.currentSize) ? cache.end() : std::next(fifoOutBegin));
        }

        if (eltit != lru2Begin && eltit->name != FIFO_IN) {
            if (lru2Begin == cache.end()) {
                cache.splice(cache.end(), cache, eltit, std::next(eltit));
                lru2Begin = eltit;
            } else {
                cache.splice(lru2Begin, cache, eltit, std::next(eltit));
                lru2Begin--;
            }
        }
        
        return true;
    }

    void print() const {
        fifoInSize.print();
        fifoOutSize.print();
        lru2Size.print();
    }

    void printCache() const {
        for (auto it = cache.begin(); it != cache.end(); ++it) {
            std::cout << it->data << " (" << 
                ((it->name == FIFO_IN) ? "FifoIn" : ((it->name == FIFO_OUT) ? "FifoOut" : "Lru2")) << ")" << std::endl;
        }
        std::cout << std::endl;
        if (fifoOutBegin == cache.end())
            std::cout << "fifoOutBegin: null" <<  std::endl;
        else
            std::cout << "fifoOutBegin: " << 
            ((fifoOutBegin->name == FIFO_IN) ? "FifoIn" : ((fifoOutBegin->name == FIFO_OUT) ? "FifoOut" : "Lru2")) << " " << 
            fifoOutBegin->data <<  std::endl;
 
        if (lru2Begin == cache.end())
            std::cout << "lru2Begin: null" << std::endl;
        else
            std::cout << "lru2Begin: " << ((lru2Begin->name == FIFO_IN) ? "FifoIn" : ((lru2Begin->name == FIFO_OUT) ? "FifoOut" : "Lru2")) << 
            " " << lru2Begin->data << std::endl;
    }
};

}
