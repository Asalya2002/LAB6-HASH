// LAB6-HASH.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>

class HashTable {
public:
    struct Element {
        std::string key;
        int value;

        Element(const std::string& k, int v) : key(k), value(v) {}
    };

    // Конструкторы и деструктор
    HashTable();
    HashTable(const HashTable& other);
    ~HashTable();

    // Методы
    void insert(const std::string& key, int value);  // Добавление элемента
    void remove(const std::string& key);  // Удаление элемента
    bool contains(const std::string& key);  // Проверка наличия элемента
    void printTable();  // Вывод содержимого таблицы
    void changeHashFunction(int newC, int newD);  // Замена хеш-функции
    void resize(int newSize);  // Изменение размера хеш-таблицы
    HashTable& operator=(const HashTable& other);  // Оператор присваивания
    int& operator[](const std::string& key);  // Получение ссылки на значение по ключу

private:
    static const int N = 100;
    std::vector<std::list<Element>> table;
    std::hash<std::string> hasher;
    int c;
    int d;

    // Вспомогательные методы
    size_t hash(const std::string& key, int i);  // Хеш-функция
    size_t findIndex(const std::string& key);  // Поиск индекса по ключу
};

// Реализация методов класса HashTable

// Конструктор по умолчанию
HashTable::HashTable() : table(N) {
    c = 1 % 5; // Остаток от деления 1 на 5
    d = 1 % 7; // Остаток от деления 1 на 7
}

// Конструктор копирования
HashTable::HashTable(const HashTable& other) : table(other.table), c(other.c), d(other.d) {}

// Деструктор
HashTable::~HashTable() {
    table.clear();
}

// Добавление элемента с заданным ключом в таблицу
void HashTable::insert(const std::string& key, int value) {
    int i = 0;
    size_t index = hash(key, i);

    while (!table[index].empty()) {
        ++i;
        index = hash(key, i);
    }

    table[index].push_back(Element(key, value));
}

// Удаление элемента из таблицы по ключу
void HashTable::remove(const std::string& key) {
    size_t index = findIndex(key);
    if (index != -1) {
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->key == key) {
                table[index].erase(it);
                break;
            }
        }
    }
}

// Проверка наличия элемента с заданным ключом
bool HashTable::contains(const std::string& key) {
    size_t index = findIndex(key);
    return index != -1;
}

// Вывод содержимого таблицы в консоль
void HashTable::printTable() {
    for (size_t i = 0; i < N; ++i) {
        if (!table[i].empty()) {
            std::cout << "Hash: " << i << std::endl;
            for (const auto& element : table[i]) {
                std::cout << element.key << ": " << element.value << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

// Замена хеш-функции
void HashTable::changeHashFunction(int newC, int newD) {
    c = newC;
    d = newD;

    // Пересчитываем хеши для всех элементов в таблице
    std::vector<std::list<Element>> newTable(N);
    for (size_t i = 0; i < N; ++i) {
        for (const auto& element : table[i]) {
            size_t newIndex = hash(element.key, 0);
            newTable[newIndex].push_back(element);
        }
    }
    table = newTable;
}

// Изменение размера хеш-таблицы
void HashTable::resize(int newSize) {
    table.resize(newSize);
}

// Оператор присваивания
HashTable& HashTable::operator=(const HashTable& other) {
    if (this != &other) {
        table = other.table;
        c = other.c;
        d = other.d;
    }
    return *this;
}

// Получение ссылки на значение по ключу
int& HashTable::operator[](const std::string& key) {
    size_t index = findIndex(key);
    if (index != -1) {
        for (auto& element : table[index]) {
            if (element.key == key) {
                return element.value;
            }
        }
        // Если ключ не найден, добавляем новый элемент со значением по умолчанию
        table[index].push_back(Element(key, 0));
        return table[index].back().value;
    }
    else {
        size_t newIndex = hash(key, 0);
        table[newIndex].push_back(Element(key, 0));
        return table[newIndex].back().value;
    }
}

// Хеш-функция
size_t HashTable::hash(const std::string& key, int i) {
    return (hasher(key) + c * i + d * i * i) % N;
}

// Поиск индекса по ключу
size_t HashTable::findIndex(const std::string& key) {
    size_t i = 0;
    size_t index = hash(key, i);

    while (i < N && !table[index].empty()) {
        for (const auto& element : table[index]) {
            if (element.key == key) {
                return index;
            }
        }
        ++i;
        index = hash(key, i);
    }

    return -1; // Если элемент не найден
}

int main() {
    HashTable ht;

    ht.insert("apple", 5);
    ht.insert("banana", 10);


    ht.printTable();
    if (ht.contains("apple")) {
        std::cout << "Apple is in the table." << std::endl;
    }

    ht["apple"] = 7;

    std::cout << "Value of apple: " << ht["apple"] << std::endl;

    return 0;
}

