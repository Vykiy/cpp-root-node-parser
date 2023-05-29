#include <iostream>
#include <fstream>

#include "Node.hpp"

int main() {
    // Открытие входного файла для чтения
    std::ifstream ifile("input.txt");

    if (ifile.fail()) {
        std::cerr << "Невозможно открыть файл input.txt" << std::endl;
        return 1;
    }

    // Открытие выходного файла для записи
    std::ofstream ofile("output.txt");

    if (ofile.fail()) {
        std::cerr << "Невозможно открыть файл output.txt" << std::endl;
        return 1;
    }

    try {
        std::string buffer;
        // Чтение содержимого файла в строку
        std::getline(ifile, buffer, '\000');

        ifile.close();

        // Парсинг строки и создание дерева узлов
        Node::sptr rootNode = Node::parse(buffer);

        // Преобразование дерева в строку с отступами
        std::string outputString = to_string(rootNode);

        ofile << outputString;
        ofile.close();
    }
    catch (const std::system_error &e) {
        std::cerr << e.code().message() << std::endl;
        return 1;
    }

    return 0;
}
