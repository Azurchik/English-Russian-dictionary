#include <iostream>
#include <list>
#include <queue>

using namespace std;

struct Unit {
    bool empty() {
        return word.empty()
        && translate.empty()
        && counter == 0;
    }
    friend bool operator== (const Unit &left, const Unit &right) {
        return left.word == right.word
               && left.translate == right.translate
               && left.counter == right.counter;
    }

    string word = "";
    string translate = "";
    int counter = 0;
};

struct Node {
    Node () {
        left  = nullptr;
        right = nullptr;
    }

    explicit Node (const Unit &unit) {
        left  = nullptr;
        right = nullptr;
        l_units.push_back(unit);
    }

    Unit m_unit() {
        if (l_units.empty())
            return Unit();

        return l_units.front();
    }
    friend bool operator== (const Node &left, const Node &right) {
        return left.l_units == right.l_units
               && left.left == right.left
               && left.right == right.right;
    }

    list<Unit> l_units;
    Node *left;
    Node *right;
};

class Dict {
public:
    explicit Dict(const string &filePath);
    ~Dict();

    void addWord(const Unit &unit);
    void removeWord(const string &wr);
    void translateWord(const string &wr);
    static int getCode(const string &word);
    void changeType();
    bool isAbc_type();

private:
    static void sortUnitsByWrd(Unit *units, int size);
    static void sortUnitsByCnt(Unit *units, int size);
    void getAllTreeUnits(queue<Unit> &queue);

    void initializeTree(Unit *units, int size);
    Node* deepSearch(const string &word);

    // Recursion methods
    static void setCentralUnits(queue<Unit> &queue, Unit *units, int size);
    static Node* wideSearch(const string &word, queue<Node*> &queue);
private:
    string filePath;
    bool was_changed;
    bool abc_type;
    list<Node> tree;
};

