#include <iostream>
#include <algorithm>

#include "list.h"
#include "allocator.h"
#include "square.h"

int main() {
    container::list<square<double>, allocator::my_allocator<square<double>, 500>> list;
    int command, pos;

    while(true) {
        std::cout << std::endl;
        std::cout << "0 - Quit" << std::endl;
        std::cout << "1 - Add element to list (push front / by index)" << std::endl;
        std::cout << "2 - Delete element from list (pop front / erase by index / erase by iterator)" << std::endl;
        std::cout << "3 - Print all elements" << std::endl;
        std::cout << "4 - Count_if example" << std::endl;
        std::cout << "5 - Print element by [index]" << std::endl << std::endl;
        std::cin >> command;

        if (command == 0) {
            break;

        } else if(command == 1) {
            std::cout << "Enter coordinates" << std::endl;
            square<double> square(std::cin);

            std::cout << "1 - PushFront" << std::endl;
            std::cout << "2 - Insert by index" << std::endl;
            std::cin >> command;
            if(command == 1) {
                list.push(square);
                continue;
            } else if(command == 2) {
                std::cout << "Enter index" << std::endl;
                std::cin >> pos;
                list.insert(pos, square);
                continue;
            } else {
                std::cout << "Wrong command" << std::endl;
                std::cin >> command;
                continue;
            }

        } else if(command == 2) {
            std::cout << "1 - Erase by index" << std::endl;
            std::cout << "2 - Erase by iterator" << std::endl;
            std::cout << "3 - Pop front" << std::endl;
            std::cin >> command;
            if (command == 1) {
                std::cout << "Enter index" << std::endl;
                std::cin >> pos;
                list.erase(pos);
                continue;
            } else if (command == 2) {
                std::cout << "Enter index" << std::endl;
                std::cin >> pos;
                auto temp = list.begin();
                for(int i = 0; i < pos; ++i) {
                    ++temp;
                }
                list.erase(temp);
                continue;

            } else if (command == 3) {
                try {
                    list.popFront();
                } catch(std::exception& e) {
                    std::cout << e.what() << std::endl;
                    continue;
                }
            } else {
                std::cout << "Wrong command" << std::endl;
                std::cin >> command;
                continue;
            }
        } else if(command == 3) {
            for(const auto& item : list) {
                item.print(std::cout);
                std::cout << "Center: [" << item.center() << "]" << std::endl;
                std::cout << "Area: " << item.area() << std::endl;
                continue;
            }

        } else if(command == 4) {
            std::cout << "Enter required area" << std::endl;
            std::cin >> pos;
            std::cout << "Number of squares with area less than " << pos << " equals ";
            std::cout << std::count_if(list.begin(), list.end(), [pos](square<double> square) {return square.area() < pos;}) << std::endl;
            continue;

        } else if (command == 5) {
            std::cout << "Enter index to print for" << std::endl;
            std::cin >> pos;
            try {
                list[pos].print(std::cout);
                std::cout << "Center: [" << list[pos].center() << "]" << std::endl;
                std::cout << "Area: " << list[pos].area() << std::endl;
            } catch(std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
            continue;

        } else {
            std::cout << "Wrong command" << std::endl;
            continue;
        }
    }

    return 0;
}