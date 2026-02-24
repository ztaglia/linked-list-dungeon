#include <iostream>
#include <memory>

struct Item {
    std::string name;
};

struct Node {
    Item item; // not smart ptr because the node owns the item
    std::unique_ptr<Node> next;

    Node(Item i, std::unique_ptr<Node> n) : 
        item(std::move(i)), next(std::move(n)) {} // rather than copying (=) the item bc copying is expensive af especially for strings 
        // and unique_ptrs can't be copied
};

struct Room {
    Room* north = nullptr;
    Room* south = nullptr;
    Room* east = nullptr;
    Room* west = nullptr;
    std::string description;
};

class Inventory {
    private:
        std::unique_ptr<Node> head;
        int itemCount;
    public:
        Inventory() : head(nullptr), itemCount(0) {}
        ~Inventory() = default; // smart ptr will automatically clean up
        void add(const std::string& itemName) {
            auto newNode = std::make_unique<Node>(Item{itemName}, std::move(head));
            head = std::move(newNode);
            itemCount++;
        }
        void print() {
            Node* current = head.get(); // not head pointer so list doesn't delete itself
            while (current) {
                std::cout << current->item.name << std::endl;
                current = current->next.get(); // move to next node, but don't delete current node
            }
        }
        bool remove(const std::string& itemName) {
            Node* current = head.get();
            Node* previous = nullptr;
            if (!current) { return false; }
            if (current->item.name == itemName) {
                head = std::move(current->next);
                itemCount--;
                return true;
            } else {
                previous = current;
                current = current->next.get();
            }
            while (current) {
                if (current->item.name == itemName) {
                    previous->next = std::move(current->next);
                    itemCount--;
                    return true;
                }
                previous = current;
                current = current->next.get();
            }
            return false;
        }
};

int main() {
    Inventory inv;
    
    inv.add("Sword");
    inv.add("Shield");
    inv.add("Rusty Key");
    
    std::cout << "Inventory:" << std::endl;
    inv.print();
    
    std::cout << "Removing Shield..." << std::endl;
    inv.remove("Shield");
    
    std::cout << "\nInventory:" << std::endl;
    inv.print();

    Room treasureRoom, armory, entranceHall, library;
    treasureRoom.description = "a room filled with treasure";
    armory.description = "a room filled with weapons";
    entranceHall.description = "the entrance to the dungeon";
    library.description = "a room filled with old books";

    entranceHall.north = &treasureRoom;
    entranceHall.east = &library;
    entranceHall.west = &armory;
    entranceHall.south = nullptr; // already initialized to nullptr

    treasureRoom.south = &entranceHall;
    library.west = &entranceHall;
    armory.east = &entranceHall;

    treasureRoom.north = nullptr;
    treasureRoom.east = nullptr;
    treasureRoom.west = nullptr;

    library.north = nullptr;
    library.south = nullptr;
    library.east = nullptr;

    armory.north = nullptr;
    armory.south = nullptr;
    armory.west = nullptr;

    Room* currentRoom = &entranceHall;
    std::cout << "current room's description: " << currentRoom->description << std::endl;

    char direction;
    do {
        std::cout << "enter a direction (n/s/e/w) or 'q' to quit: ";
        std::cin >> direction;
        Room* nextRoom = nullptr;
        switch (direction) {
            case 'n':
                nextRoom = currentRoom->north;
                break;
            case 's':
                nextRoom = currentRoom->south;
                break;
            case 'e':
                nextRoom = currentRoom->east;
                break;
            case 'w':
                nextRoom = currentRoom->west;
                break;
            case 'q':
                std::cout << "bye!" << std::endl;
                continue;
            default:
                std::cout << "invalid direction. try again pls." << std::endl;
                continue;
        }
        if (nextRoom) {
            currentRoom = nextRoom;
            std::cout << "current room's description: " << currentRoom-> description << std::endl;
        } else {
            std::cout << "you can't go that way!" << std::endl;
        }
    } while (direction != 'q');
    
    return 0;
}