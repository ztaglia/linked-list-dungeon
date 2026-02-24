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
    Inventory items;
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

    armory.items.add("Sword");
    armory.items.add("Shield");
    library.items.add("Dusty Book");
    treasureRoom.items.add("Gold Crown");

    std::string command;
    do {
        std::cout << "enter a direction (n/s/e/w), a stationary action (look/take {item}/inventory) or 'q' to quit: ";
        std::getline(std::cin, command);
        Room* nextRoom = nullptr;
        if (command == "n") {
            nextRoom = currentRoom->north;
        } else if (command == "s") {
            nextRoom = currentRoom->south;
        } else if (command == "e") {
            nextRoom = currentRoom->east;
        } else if (command == "w") {
            nextRoom = currentRoom->west;
        } else if (command == "look") {
            std::cout << "current room's description: " << currentRoom->description << std::endl;
            std::cout << "items in the room: " << std::endl;
            currentRoom->items.print();
            continue;
        } else if (command == "inventory") {
            std::cout << "your inventory: " << std::endl;
            inv.print();
            continue;
        } else if (command.rfind("take ", 0) == 0) { // command starts with "take ", rfind returns 0 if "take " is found at the start of the string
            std::string itemName = command.substr(5); // get item name after "take "
            inv.add(itemName);
            if (currentRoom->items.remove(itemName)) { // this checks if the item was successfully removed from the room's inventory, which means it was in the room
                std::cout << "you took the " << itemName << "!" << std::endl;
            } else {
                std::cout << "there's no " << itemName << " in this room!" << std::endl;
            }
            continue;
        } else if (command == "q") {
            std::cout << "bye!" << std::endl;
            continue;
        } else {
            std::cout << "invalid direction. try again pls." << std::endl;
            continue;
        }
        if (nextRoom) {
            currentRoom = nextRoom;
            std::cout << "current room's description: " << currentRoom-> description << std::endl;
        } else {
            std::cout << "you can't go that way!" << std::endl;
        }
    } while (command != 'q');
    
    return 0;
}