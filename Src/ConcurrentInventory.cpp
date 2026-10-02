//
// Created by jenso on 01/10/2026.
//

#include "ConcurrentInventory.h"

#include <iostream>

ConcurrentInventory::ConcurrentInventory() {

}

ConcurrentInventory::~ConcurrentInventory() {

}

void ConcurrentInventory::AddItem(std::string itemName) {
    std::lock_guard<std::mutex> lock(itemsMutex);

    items.push_back(itemName);
}

void ConcurrentInventory::DisplayAllItems() {
    std::lock_guard<std::mutex> lock(itemsMutex);

    for (auto item: items) {
        std::cout << item << std::endl;
    }
}
