Для корректного запуска программы, следуйте этой инструкции:

**1. Подготовка входного файла:**

- Создайте текстовый файл с именем "input.txt".
- Откройте файл и введите данные, представляющие дерево узлов. Каждая строка должна содержать один узел с его идентификатором, родительским идентификатором, именем и значением.
- Структура узлов должна отражать иерархию дерева. Дочерние узлы должны быть вложены внутрь родительских узлов с использованием фигурных скобок {}.

**2. Запуск программы:**

- Скомпилируйте и соберите программу, используя компилятор C++ и файлы исходного кода (Node.hpp, Node.cpp и main.cpp).
- Убедитесь, что скомпилированный исполняемый файл находится в том же каталоге, где находятся файлы "input.txt" и "output.txt".
- Откройте командную строку (терминал) и перейдите в каталог с исполняемым файлом программы и файлами "input.txt" и "output.txt".
- Запустите программу, введя имя исполняемого файла.
- Пример запуска в командной строке (Linux/macOS):
./program

- Пример запуска в командной строке (Windows):
program.exe

**3. Проверка результатов:**

- После выполнения программы проверьте содержимое файла "output.txt".
- Файл "output.txt" должен содержать строку с отформатированной структурой дерева узлов.
- Пример содержимого файла "output.txt" (на основе примера входных данных выше):

```
< id: 1; parentid: null; name: Node1; value: >
  < id: 2; parentid: 1; name: Node2; value: >
  < id: 3; parentid: 1; name: Node3; value: >
    < id: 4; parentid: 3; name: Node4; value: >
```
