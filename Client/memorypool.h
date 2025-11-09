#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include "protocol.h"
#include <QMutex>
#include <list>

// 内存块结构，用于内存池中的空闲块链表
struct MemoryBlock {
    PDU* pdu;          // 实际的PDU对象指针
    size_t blockSize;  // 内存块的大小
};

class MemoryPool {
private:
    std::list<MemoryBlock> freeList;  // 空闲内存块链表
    QMutex mutex;                     // 用于线程同步的互斥锁
    const size_t DEFAULT_BLOCK_SIZE;  // 默认内存块大小
    const size_t MAX_FREE_BLOCKS;     // 内存池中保留的最大空闲块数量

    // 私有构造函数，实现单例模式
    MemoryPool();

    // 禁止拷贝构造和赋值操作
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

public:
    // 获取单例实例
    static MemoryPool& getInstance();

    // 分配PDU对象
    PDU* allocatePDU(uint uiMsgLen);

    // 释放PDU对象到内存池
    void deallocatePDU(PDU* pdu);

    // 清理内存池（释放所有空闲块）
    void clear();

    // 获取当前空闲块数量
    size_t getFreeBlockCount();

    // 析构函数
    ~MemoryPool();
};

#endif // MEMORYPOOL_H
