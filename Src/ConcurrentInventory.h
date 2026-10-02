//
// Created by jenso on 01/10/2026.
//

#ifndef MYGAME_CONCURRENTINVENTORY_H
#define MYGAME_CONCURRENTINVENTORY_H
#include <string>
#include <vector>
#include <thread>
#include <mutex>

class ConcurrentInventory {
public:
    ConcurrentInventory();

    ~ConcurrentInventory();

    void AddItem(std::string itemName);

    void DisplayAllItems();

private:
    std::vector <std::string> items;

    std::mutex itemsMutex;

};



#endif //MYGAME_CONCURRENTINVENTORY_H
