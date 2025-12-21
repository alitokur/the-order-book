#pragma once

#include "types.h"
#include <iostream>
#include <vector>


/// TODO: add usage documentation
class Pool
{
public:

    Level* get_level(Order* order){
        if(free_objects.empty()){
            create_block();
        }
        Level* slot = free_objects.back();
        free_objects.pop_back();
        return new(slot) Level(order);
    }

    void release_level(Level* level){
        if(!level){
            return;
        }
        level->~Level();
        free_objects.push_back(level);
    }

    ~Pool(){
        for(auto& b : blocks){
            free(b);
        }
    }

private:
    std::vector<void*> blocks;
    std::vector<Level*> free_objects;
    static const std::size_t BLOCK_SIZE = 64*1024;

    void create_block(){
        void* memory;
        int rc = posix_memalign(&memory, 4096, BLOCK_SIZE);
        if (rc != 0){
            ///TODO: add error handling
            std::cout << "ERROR" << std::endl;
        }
        blocks.push_back(memory);

        auto* block = static_cast<Level*>(memory);
        auto count = BLOCK_SIZE/sizeof(Level);
        if(count == 0){
            /// TODO: add error handling
            std::cout << "Another ERROR" << std::endl;
        }

        for(std::size_t i = 0; i<count; i++){
            free_objects.push_back(&block[i]);
        }
    }
};
