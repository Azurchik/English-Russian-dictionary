#include "Dict.h"
#include <fstream>
#include <algorithm>

Dict::Dict(const string &filePath)
    : filePath(filePath)
    , was_changed (false)
    , abc_type (true)
{
    ifstream f_in(filePath);
    if (f_in.is_open()) {
        if (f_in.peek() == ifstream::traits_type::eof())
            return;

        int size = 0;
        bool sorted = false;

        f_in >> size;
        f_in >> sorted;
        if (sorted)
            f_in >> abc_type;

        Unit *units = new Unit[size];
        for (int i = 0; i < size; ++i) {
            f_in >> units[i].word;
            f_in >> units[i].translate;
            f_in >> units[i].counter;
        }
        f_in.close();

        if(!sorted)
            sort(units, units + size, wrd_pred);

        initializeTree(units, size);
        delete[] units;
    }
}

Dict::~Dict() {
    if (was_changed) {
        ofstream f_out (filePath, ios_base::out | ios_base::trunc);
        if (tree.empty()) {
            f_out.close();
            return;
        }

        while (!f_out.is_open()) {
            cout << "Файл не может быть открыт или создан!" << endl
                 << "Пожалуйста, введите новое имя файла(-fileName-.txt): ";
            cin >> filePath;
        }
        queue<Unit> queue;
        getAllTreeUnits(queue);

        int size = queue.size();
        Unit *units = new Unit [size];
        int i = 0;
        while (!queue.empty()) {
            units[i++] = queue.front();
            queue.pop();
        }

        f_out << size << ' ' << true << ' ';

        abc_type ? sort(units, units + size, wrd_pred) : sort(units, units + size, cnt_pred);
        f_out << abc_type;
        f_out << '\n';

        for (i = 0; i < size; ++i) {
            f_out << units[i].word << ' '
                  << units[i].translate << ' '
                  << units[i].counter << '\n';
        }
        f_out.close();
    }
}

int Dict::getCode(const string &word) {
    int sum = 0;
    for (char i : word)
        sum += static_cast<int>(i);

    return sum / static_cast<int>(word.size());
}

void Dict::initializeTree(Unit *units, int size) {
    queue<Unit> queue;
    setCentralUnits(queue, units, size);

    while (!queue.empty()) {
        addWord(queue.front());
        queue.pop();
    }
}

void Dict::setCentralUnits(queue<Unit> &queue, Unit *units, int size) {
    if (size == 1) {
        queue.push(*units);
        return;
    }

    int t_size = size / 2;
    queue.push(units[t_size]);

    Unit *unitsLess = new Unit[t_size];
    int cnt = 0;
    for (int i = 0; i < t_size; ++i)
        unitsLess[cnt++] = units[i];

    setCentralUnits(queue, unitsLess, t_size);
    delete[] unitsLess;

    t_size = size - t_size - 1;
    if (t_size) {
        Unit *unitsAbove = new Unit[t_size];
        cnt = t_size;
        for (int i = size - 1; i > size / 2; --i)
            unitsAbove[--cnt] = units[i];

        setCentralUnits(queue, unitsLess, t_size);
        delete[] unitsAbove;
    }
}

void Dict::addWord(const Unit &unit) {
    if (tree.empty()) {
        tree.emplace_back(unit);
        was_changed = true;
        return;
    }

    Node *c_node = &tree.front(); // current node
    Node *p_node = nullptr; // previous node
    int code0 = abc_type ? getCode(unit.word) : unit.counter;
    int code1 = 0;
    do {
        p_node = c_node;
        code1 = abc_type ? getCode(c_node->m_unit().word) : c_node->m_unit().counter;

        if (code0 < code1) {
            c_node = c_node->left;
        }
        else if (code0 > code1) {
            c_node = c_node->right;
        }
        else { // If collision
            bool found = false;
            for (Unit &t_unit : c_node->l_units) {
                if (t_unit.word == unit.word) {
                    cout << "\"" << unit.word
                         << "\" уже есть в словаре." << endl;
                    found = true;
                    break;
                }
            }
            if (!found)
                c_node->l_units.emplace_back(unit);

            return;
        }
    }
    while (c_node != nullptr);

    tree.emplace_back(unit);
    if (code0 < code1) {
        p_node->left = &tree.back();
    }
    else {
        p_node->right = &tree.back();
    }
    was_changed = true;
}

void Dict::removeWord(const string &wr) {
    if (tree.empty()) {
        cout << "Словарь пуст!" << endl;
        return;
    }
    bool found = false;
    Node* node = nullptr;

    if (abc_type) {
        node = deepSearch(wr);
    }
    else {
        queue<Node*> queue;
        queue.push(&tree.front());
        node = wideSearch(wr, queue);
    }
    if (node != nullptr) {
        if (node->l_units.size() > 1) { // Delete Units
            for (Unit &unit : node->l_units) {
                if (unit.word == wr) {
                    node->l_units.remove(unit);
                    found = true;
                    was_changed = true;
                    break;
                }
            }
        } else { // Delete Node and reinitialize Tree
            found = true;
            was_changed = true;
            tree.remove(*node);

            queue<Unit> queue;
            getAllTreeUnits(queue);

            int size = queue.size();
            Unit *units = new Unit[size];
            int i = 0;
            while (!queue.empty()) {
                units[i++] = queue.front();
                queue.pop();
            }
            tree.clear();

            abc_type ? sort(units, units + size, wrd_pred) : sort(units, units + size, cnt_pred);

            initializeTree(units, size);
            delete[] units;
        }
    }
    if (!found)
        cout << "В словаре нет слова \"" << wr << "\"" << endl;
}

void Dict::getAllTreeUnits(queue<Unit> &queue) {
    for (Node &node : tree) {
        for (Unit &unit : node.l_units)
            queue.push(unit);
    }
}

void Dict::translateWord(const string &wr) {
    if (tree.empty()) {
        cout << "Словарь пуст!" << endl;
        return;
    }
    bool found = false;
    Node* node = nullptr;

    if (abc_type) { // Deep Search
        node = deepSearch(wr);
    }
    else { // Wide Search
        queue<Node*> queue;
        queue.push(&tree.front());
        node = wideSearch(wr, queue);
    }

    if (node != nullptr) {
        for (Unit &unit : node->l_units) {
            if (unit.word == wr) {
                cout << wr << " - " << unit.translate << endl;
                unit.counter++;
                found = true;
            }
        }
    }
    if (!found)
        cout << "В словаре нет слова \"" << wr << "\"" << endl;
}

Node* Dict::deepSearch(const string &word) {
    if (tree.empty())
        return nullptr;

    int code0 = getCode(word);
    int code1 = 0;
    Node *node = &tree.front();

    do {
        code1 = getCode(node->m_unit().word);

        if (code0 < code1) {
            node = node->left;
        }
        else if (code0 > code1) {
            node = node->right;
        }
        else {
            return node;
        }
    }
    while (node != nullptr);

    return nullptr;
}

Node* Dict::wideSearch(const string &word, queue<Node *> &queue) {
    if (queue.empty())
        return nullptr;

    int size = queue.size();
    for (int i = 0; i < size; ++i) {
        Node *node = queue.front();
        if (getCode(node->m_unit().word) == getCode(word))
            return node;

        if (node->left != nullptr)
            queue.push(node->left);

        if (node->right != nullptr)
            queue.push(node->right);

        queue.pop();
    }
    return wideSearch(word, queue);
}

void Dict::changeType() {
    queue<Unit> queue;
    getAllTreeUnits(queue);

    int size = queue.size();
    Unit *units = new Unit [size];
    int i = 0;
    while (!queue.empty()) {
        units[i++] = queue.front();
        queue.pop();
    }
    tree.clear();

    abc_type ? sort(units, units + size, wrd_pred) : sort(units, units + size, cnt_pred);

    initializeTree(units, size);
    abc_type = !abc_type;
}

bool Dict::isAbc_type() {
    return abc_type;
}

bool Dict::wrd_pred(const Unit &a, const Unit &b) {
    return getCode(a.word) < getCode(b.word);
}

bool Dict::cnt_pred(const Unit &a, const Unit &b) {
    return a.counter < b.counter;
}