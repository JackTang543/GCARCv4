#include "sLib_Reg.h"

//time 2024.08.11 第一版


/**
 * @brief 修改寄存器中的特定位段，只在需要时更新。
 * 这个函数首先计算掩码中最低位的位置，然后将输入数据左移对齐到这个位置。
 * 之后，它会计算需要更新的位，并只更新这些位，其他位保持不变。
 *
 * @param reg_addr 寄存器的地址，通常是一个指向uint8_t的指针。
 * @param mask 指定要修改的位，例如0b00111000表示修改第4到第6位。
 * @param data 要写入寄存器位段的数据，数据需要是从最低位开始并只包含目标位段。
 */
void sLib_ModifyReg(uint8_t* reg_addr, uint8_t mask, uint8_t data){
    uint8_t pos = 0;
    uint8_t mask_original = mask;
    // 计算掩码最低位的位置
    while ((mask & 0x01) == 0) {
        mask >>= 1;
        pos++;
    }
    // 将数据左移，对齐到掩码指定的位
    uint8_t aligned_data = (data << pos) & mask_original;
    // 读取当前寄存器值
    uint8_t current_value = *reg_addr;
    // 计算需要变更的位
    uint8_t changes = (current_value & mask_original) ^ aligned_data;
    // 只更新变化的位，不影响其他位
    uint8_t new_value = (current_value & ~changes) | (aligned_data & changes);
    // 写回修改后的值到寄存器
    *reg_addr = new_value;
}

