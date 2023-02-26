#include "balancedBinTree.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>


void programCommands(BalancedBinaryTree<int>& tree, BalancedBinaryTree<int>& otherTree, std::map<std::string, int>& mapping) {
    std::string cmd = "";
    std::vector<int> vect;
    int length, num;

    std::cin >> cmd;

    auto it = mapping.find(cmd);

    if (it == mapping.end()) {
        std::cout << "Command failed!" << std::endl; 
        return;
    }

    switch (mapping[cmd]) {
    case 3:
        std::cin >> cmd;
        tree.add(std::stoi(cmd));
        break;
    case 4:
        tree = otherTree;
        break;
    case 5:
        std::cin >> cmd;
        tree.remove(std::stoi(cmd));
        break;
    case 6:
        std::cin >> cmd;
        length = std::stoi(cmd);
        for (int i = 0; i < length; ++i) {
            std::cin >> cmd;
            vect.push_back(std::stoi(cmd));
        }
        tree.addElements(vect);
        break;
    case 7:
        tree.clean();
        break;
    case 8:
        tree.rebalance();
        break;    
    case 9:
        if (tree.getMin(num)) {
            std::cout << "min value: " << num << std::endl;
        } else {
            std::cout << "There is no values" << std::endl;
        }
        break;
    case 10:
        if (tree.getMax(num)) {
            std::cout << "max value: " << num << std::endl;
        } else {
            std::cout << "There is no values" << std::endl;
        }
        break;
    case 11:
        tree.print();
        break;
    case 12:
        tree.show();
        break;
    case 13:
        tree.joinAnd(otherTree);
        break;
    case 14:
        tree.joinOr(otherTree);
        break;         
    default:
        std::cout << "Command failed!" << std::endl; 
        break;
    }
}

int main() {
    BalancedBinaryTree<int> tree1, tree2;
    std::string cmd = "";
    bool isProgramContinue = true;
    std::map<std::string, int> mapping;

    mapping["exit"] = 0;
    mapping["tree1"] = 1;
    mapping["tree2"] = 2;
    mapping["add"] = 3;
    mapping["operator="] = 4;
    mapping["remove"] = 5;
    mapping["addElements"] = 6;
    mapping["clean"] = 7;
    mapping["rebalance"] = 8;
    mapping["getMin"] = 9;
    mapping["getMax"] = 10;
    mapping["print"] = 11;
    mapping["show"] = 12;
    mapping["joinAnd"] = 13;
    mapping["joinOr"] = 14;

    while(isProgramContinue) {
        std::cin >> cmd;

        auto it = mapping.find(cmd);

        if (it == mapping.end()) {
            std::cout << "Command failed!" << std::endl; 
            continue;
        }

        switch (mapping.at(cmd)) {
        case 0:
            isProgramContinue = false;
            break;
        case 1:
            programCommands(tree1, tree2, mapping);
            break;
        case 2:
            programCommands(tree2, tree1, mapping);
            break;
        default:
            std::cout << "Command failed!" << std::endl; 
            break;
        }
    }

    return 0;
}
