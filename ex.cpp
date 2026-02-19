#include <iostream>
#include <memory>

int main() {
    int* p = new int(42);
    std::cout << "value = " << *p << std::endl;
    *p = 100;
    std::cout << "value = " << *p << std::endl;
    delete p;
    p = nullptr;

    std::unique_ptr<int> up = std::make_unique<int>(42);
    *up = 100;
    std::unique_ptr<int> up2 = std::move(up);
    std::cout << "value = " << *up2 << std::endl;
    return 0;
}