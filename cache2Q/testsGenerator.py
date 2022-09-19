import random


def getSizes(allSize):
    sizeFifoIn, sizeFifoOut, sizeLru2 = (allSize // 5 * 1), (allSize // 5 * 2), (allSize // 5 * 2)
    extra = allSize % 5
    if (extra >= 1): sizeFifoOut += 1
    if (extra >= 2): sizeLru2 += 1
    if (extra >= 3): sizeFifoIn += 1
    if (extra >= 4): sizeFifoOut += 1
    return (sizeFifoIn, sizeFifoOut, sizeLru2)


def findElement(arr, value):
    try:
        return arr.index(value)
    except ValueError:
        return -1


def runTest(arr, size):
    fifoIn, fifoOut, lru2 = [], [], []
    sizeFifoIn, sizeFifoOut, sizeLru2 = size
    hits = 0

    for value in arr:
        indexFifoIn = findElement(fifoIn, value)
        indexFifoOut = findElement(fifoOut, value)
        indexLru2 = findElement(lru2, value)

        if (indexFifoOut != -1):
            lru2.insert(0, fifoOut.pop(indexFifoOut))
            if (len(lru2) > sizeLru2):
                lru2.pop()
            hits += 1
        elif (indexLru2 != -1):
            lru2.insert(0, lru2.pop(indexLru2))
            hits += 1
        elif (indexFifoIn != -1):
            hits += 1
        else:
            fifoIn.insert(0, value)
            if (len(fifoIn) > sizeFifoIn):
                fifoOut.insert(0, fifoIn.pop())
            if (len(fifoOut) > sizeFifoOut):
                fifoOut.pop()
    
    return hits


def generateData(count, a = -30, b = 30):
    return [random.randint(a, b) for _ in range(0, count)]


def writeToFile(fileName, size, count, arr, res):
    with open("tests/" + str(fileName), 'w') as f:
        f.write(str(size) + " " + str(count) + "\n")
        f.write(" ".join([str(i) for i in arr]) + "\n")
        f.write(str(res))


def getGeneratedData(startAt = 1, length = 10):
    for i in range(startAt, startAt + length):
        size = random.randint(1, 300)
        count = random.randint(10, 100000)
        arr = generateData(count)
        res = runTest(arr, getSizes(size))
        
        writeToFile("test{}.txt".format(i), size, count, arr, res)
        print("test: {:02}; size: {:3}; count: {:6}; result: {:6}".format(i, size, count, res))


def getUnitTests(startAt = 1):
    #   Test Block
    #   count;  sizeCache;  res;        arr;
    arr = [
        [5,         4,        0,    [1, 2, 3, 4, 5]],
        [5,         4,        4,    [0, 0, 0, 0, 0]],
        [3,         1,        1,    [1, 0, 0]],
        [5,         3,        0,    [1, 2, 3, 1, 2]],
        [10,        6,        4,    [1, 2, 2, 3, 4, 2, 1, 5, 7, 4]],
    ]

    for i in range(0, len(arr)):
        writeToFile("test{}.txt".format(i + startAt), arr[i][1], arr[i][0], arr[i][3], arr[i][2])
        print("test: {:02}; size: {:3}; count: {:6}; result: {:6}".format(i + startAt, arr[i][1], arr[i][0], arr[i][2]))

    return startAt + len(arr)


def main():
    testNumber = getUnitTests()
    getGeneratedData(testNumber, 25)


if (__name__ == '__main__'):
    main()
