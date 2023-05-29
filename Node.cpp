#include "Node.hpp"

using namespace std;

// Инициализация статической переменной id_giver
int Node::id_giver = 0;

// Функция преобразует узел и его дочерние узлы в строку с отступами, чтобы лучше отображать структуру дерева
std::string to_string(const Node::sptr &node, int tabs) {
    std::string str;

    if (node == nullptr)
        return str;

    // Добавление отступов перед строкой
    for (auto i = 0; i < tabs; i++)
        str += "  ";

    if (node->isList() > 0) {
        std::string childrenid;

        // Формирование строки с идентификаторами дочерних узлов
        for (auto const &i: node->getChildren()) {
            childrenid += std::to_string(i->getId()) + " ";
        }

        // Формирование строки с информацией о текущем узле (идентификатор, родительский идентификатор, имя и значения дочерних узлов)
        str +=
                "< id: " + std::to_string(node->getId()) + "; " +
                "parentid: " + (node->getParent() == nullptr ? "null" : std::to_string(node->getParent()->getId())) +
                "; " +
                "name: " + node->getName() + "; " +
                "value: " + childrenid + ">\n";

        // Рекурсивный вызов функции для каждого дочернего узла
        for (auto const &i: node->getChildren()) {
            str += to_string(i, tabs + 1);
        }

        return str;
    } else if (node->isList() < 0) {
        // Формирование строки с информацией о текущем узле (идентификатор, родительский идентификатор, имя и значение)
        str +=
                "< id: " + std::to_string(node->getId()) + "; " +
                "parentid: " + (node->getParent() == nullptr ? "null" : std::to_string(node->getParent()->getId())) +
                "; " +
                "name: " + node->getName() + "; " +
                "value: " + node->getValue() + " >\n";
        return str;
    } else
        throw std::invalid_argument("Тип узла неопределен");
}

// Метод parse принимает строку входных данных и создает дерево узлов на основе этой строки
Node::sptr Node::parse(const string &inputStr) {

    // Определение регулярных выражений для лексем
    map<string, string> sLexems =
            {
                    {"megalexem", "(\\\"{1}[\\w ]*\\\"{1})|([a-zA-Z_]+[a-zA-Z_0-9]*)|(=)|([{}])|(\\d+)"},
                    {"varName",   "([a-zA-Z_]+[a-zA-Z_0-9]*)"},
                    {"varValue",  "(\\\"{1}[\\w ]*\\\"{1})"},
                    {"charEqual", "(=)"},
                    {"charBrace", "([{}])"},
                    {"digit",     "(\\d+)"}
            };

    map<string, regex> rLexems;

    // Компиляция регулярных выражений
    for (auto const &i: sLexems)
        rLexems.try_emplace(i.first, i.second);

    // Инициализация переменных
    Node::sptr root = nullptr;
    Node::sptr parent = nullptr;
    Node::sptr node = nullptr;

    auto equalTockens = 0;
    auto braceTockens = 0;

    string status = "begin";
    auto tempstring = inputStr;
    bool error = false;

    // Поиск лексем во входной строке
    for (smatch outerMatch; regex_search(tempstring, outerMatch, rLexems["megalexem"]);) {
        if (regex_match(outerMatch.str(), rLexems["digit"])) {
            error = true;
        } else if (regex_match(outerMatch.str(), rLexems["varValue"])) {
            if (status == "charEqual") {
                node->setValue(outerMatch.str());

                equalTockens--;
                status = "varValue";
            } else {
                error = true;
            }
        } else if (regex_match(outerMatch.str(), rLexems["varName"])) {
            if (status == "begin" || status == "varValue" || status == "charBrace") {
                node = Node::create(outerMatch.str());
                if (parent != nullptr)
                    parent->addChild(node);

                if (status == "begin")
                    root = node;

                status = "varName";
            } else {
                error = true;
            }
        } else if (regex_match(outerMatch.str(), rLexems["charEqual"])) {
            if (status == "varName") {
                status = "charEqual";
                equalTockens++;
            } else {
                error = true;
            }
        } else if (regex_match(outerMatch.str(), rLexems["charBrace"])) {
            if (status == "charEqual" || status == "varValue" || "charBrace") {
                if (outerMatch.str() == "{") {
                    parent = node;
                    node = nullptr;

                    equalTockens--;
                    braceTockens++;
                }
                if (outerMatch.str() == "}") {
                    parent = parent->getParent();
                    node = nullptr;
                    braceTockens--;
                }

                status = "charBrace";
            }
        } else {
            error = true;
        }

        if (equalTockens < 0 || equalTockens > 1)
            error = true;
        if (braceTockens < 0)
            error = true;

        tempstring = tempstring.substr(tempstring.find_first_of(outerMatch.str()) + outerMatch.str().length());
    }

    if (error == true) {
        throw std::invalid_argument("Неверный формат данных");
    } else {
        return root;
    }
}
