#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <map>
#include <memory>
#include <list>
#include <string>
#include <regex>

class Node {
public:
    using sptr = std::shared_ptr<Node>;
    using wptr = std::weak_ptr<Node>;

    ~Node() = default;

    // Создает новый экземпляр узла с заданным именем
    static Node::sptr create(const std::string &name) {
        auto ptr = Node::sptr(new Node(name));
        ptr->_self = ptr;
        return ptr;
    }

    static Node::sptr parse(const std::string &inputStr);

    // Возвращает указатель на текущий узел
    Node::sptr getPtr() const {
        return this->_self.lock();
    }

    // Метод getChildren предназначен для использования с узлами, которые являются списками (т.е., _isList больше 0),
    // и возвращает список _children, содержащий дочерние узлы текущего узла.
    std::list<Node::sptr> getChildren() const {
        if (this->isList() > 0)
            return this->_children;
        else
            throw std::invalid_argument(
                    "Метод getChildren не может быть вызван для узла, который не является списком или является неопределенным");
    }

    // Метод addChild предназначен для использования с узлами, которые являются списками (т.е., _isList больше или равно 0),
    // и добавляет указанный child в список _children текущего узла.
    void addChild(const Node::sptr &child) {
        if (this->isList() == 0)
            this->_isList = 1;
        if (this->isList() > 0) {
            child->setParent(this->_self.lock());
            this->_children.push_back(child);
        } else
            throw std::invalid_argument("Метод addChild не может быть вызван для узла, который не является списком");
    }

    // 0 - Неопределенный тип узла,
    // 1 - Узел является списком, содержит дочерние узлы и представляет собой контейнер, который может содержать другие узлы,
    // -1 - Узел не является списком, представляет конкретное значение или листовой узел, который не имеет дочерних узлов.
    int isList() const {
        return this->_isList;
    }

    // Если узел является списком, то метод setValue не может быть применен, поскольку списки не имеют конкретных значений, а содержат только дочерние узлы.
    void setValue(const std::string_view &value) {
        if (this->isList() == 0)
            this->_isList = -1;
        if (this->isList() < 0)
            this->_value = value;
        else
            throw std::invalid_argument("Метод setValue не может быть вызван для узла, который является списком");
    }

    // Метод getValue предназначен для использования с узлами, которые не являются списками (т.е., _isList меньше или равно 0), и возвращает значение _value узла.
    const std::string &getValue() const {
        if (this->isList() <= 0)
            return this->_value;
        else
            throw std::invalid_argument("Метод getValue не может быть вызван для узла, который является списком");
    }

    // Устанавливает родительский узел
    void setParent(const Node::sptr &newParent) {
        this->_parent = newParent;
    }

    // Возвращает родительский узел
    Node::sptr getParent() const {
        return this->_parent.lock();
    }

    // Возвращает идентификатор узла
    int getId() const {
        return this->_id;
    }

    // Возвращает имя узла
    const std::string &getName() const {
        return this->_name;
    }

protected:
    Node(const std::string &name) :
            _id(id_giver++),
            _isList(0),
            _value(""),
            _name(name) {}

private:
    static int id_giver;

    const int _id;
    // 0 - Неопределенный тип узла,
    // 1 - Узел является списком, содержит дочерние узлы и представляет собой контейнер, который может содержать другие узлы,
    // -1 - Узел не является списком, представляет конкретное значение или листовой узел, который не имеет дочерних узлов.
    int _isList;
    wptr _parent;
    wptr _self;
    std::list<Node::sptr> _children;
    std::string _value;
    const std::string _name;
};

// Преобразует узел и его дочерние узлы в строку
std::string to_string(const Node::sptr &node, int tabs = 0);

#endif
