#include "memorypool.h"
#include <stdlib.h>
#include <string.h>
#include <QDebug>

// 构造函数 - 常量成员变量必须在初始化列表中初始化
MemoryPool::MemoryPool() : DEFAULT_BLOCK_SIZE(1024), MAX_FREE_BLOCKS(100) {}

// 获取单例实例
MemoryPool& MemoryPool::getInstance() {
    static MemoryPool instance;
    return instance;
}

// 分配PDU对象
PDU* MemoryPool::allocatePDU(uint uiMsgLen) {
    uint uiPDULen = sizeof(PDU) + uiMsgLen;
    QMutexLocker locker(&mutex);

    // 遍历空闲链表，查找足够大的内存块
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->blockSize >= uiPDULen) {
            PDU* pdu = it->pdu;
            freeList.erase(it);

            // 初始化PDU
            memset(pdu, 0, uiPDULen);
            pdu->uiPDULen = uiPDULen;
            pdu->uiMsgLen = uiMsgLen;
            return pdu;
        }
    }

    // 如果没有找到合适的内存块，则分配新的
    PDU* pdu = (PDU*)malloc(uiPDULen);
    if (pdu == NULL) {
        qCritical() << "内存分配失败：无法为PDU分配内存";  // 使用Qt的日志系统输出错误
        return NULL;  // 返回NULL而不是直接退出程序
    }
    memset(pdu, 0, uiPDULen);
    pdu->uiPDULen = uiPDULen;
    pdu->uiMsgLen = uiMsgLen;
    return pdu;
}

// 释放PDU对象到内存池
void MemoryPool::deallocatePDU(PDU* pdu) {
    if (pdu == NULL) {
        return;
    }

    QMutexLocker locker(&mutex);

    // 如果空闲块数量超过最大值，则直接释放内存
    if (freeList.size() >= MAX_FREE_BLOCKS) {
        free(pdu);
        return;
    }

    // 将内存块添加到空闲链表
    MemoryBlock block;
    block.pdu = pdu;
    block.blockSize = pdu->uiPDULen;
    freeList.push_back(block);
}

// 清理内存池（释放所有空闲块）
void MemoryPool::clear() {
    QMutexLocker locker(&mutex);
    for (auto& block : freeList) {
        free(block.pdu);
    }
    freeList.clear();
}

// 获取当前空闲块数量
size_t MemoryPool::getFreeBlockCount() {
    QMutexLocker locker(&mutex);
    return freeList.size();
}

// 析构函数
MemoryPool::~MemoryPool() {
    clear();
}
