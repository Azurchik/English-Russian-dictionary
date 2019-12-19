#include "Dict.h"
#include<bits/stdc++.h>

void enterInt(int &data) {
    while (!(cin >> data)) {
        cout << "Не числовой формат!" << endl;
        cin.clear();
        fflush(stdin);
    }
}

void toLow(string &str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

int main() {
    system("chcp 1251");
    system("title Dictionary");
    setlocale(LC_ALL, "ru");
    Dict dict("dict.txt");
    for(;;) {
        int answer = 0;
        cout << "1 - перевести слово;" << endl
             << "2 - добавить слово;" << endl
             << "3 - удалить слово;" << endl
             << "4 - тип словаря;" << endl
             << "5 - сменить тип словаря;" << endl
             << "0 - выход из програмы." << endl;
        enterInt(answer);
        cout << endl;

        switch (answer) {
        case 1:
            {
                string word;
                cout << "Введите слово, которое хотите превести: ";
                cin >> word;
                toLow(word);
                dict.translateWord(word);
            }
            break;
        case 2:
            {
                Unit unit;
                string buf;
                cout << "Введите слово на Англ.: ";
                cin >> buf;
                toLow(buf);
                unit.word = buf;
                cout << "Введите его перевод: ";
                cin >> buf;
                toLow(buf);
                unit.translate = buf;
                cout << "Введите его счётчик: ";
                enterInt(answer);

                dict.addWord(unit);
                cout << "Слова были доданы.";
            }
            break;
        case 3:
            {
                string word;
                cout <<"Введите слово на англ., которое хотите удалить: ";
                cin >> word;
                toLow(word);
                dict.removeWord(word);
            }
            break;
        case 4:
            if (dict.isAbc_type()) {
                cout << "Словарь отсортирован по алфавиту." << endl;
            } else {
                cout << "Словарь отсоритрован по счётчику." << endl;
            }
            break;
        case 5:
            dict.changeType();
            cout << "Тип словаря был успешно изменён." << endl;
            break;
        case 0:
            return 0;

        default:
            cout << "Вы ввели неправильный номер команды!" << endl;
        }
        cout << endl;
    }
}