#include "Dict.h"
#include<bits/stdc++.h>

void enterInt(int &data) {
    while (!(cin >> data)) {
        cout << "�� �������� ������!" << endl;
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
        cout << "1 - ��������� �����;" << endl
             << "2 - �������� �����;" << endl
             << "3 - ������� �����;" << endl
             << "4 - ��� �������;" << endl
             << "5 - ������� ��� �������;" << endl
             << "0 - ����� �� ��������." << endl;
        enterInt(answer);
        cout << endl;

        switch (answer) {
        case 1:
            {
                string word;
                cout << "������� �����, ������� ������ ��������: ";
                cin >> word;
                toLow(word);
                dict.translateWord(word);
            }
            break;
        case 2:
            {
                Unit unit;
                string buf;
                cout << "������� ����� �� ����.: ";
                cin >> buf;
                toLow(buf);
                unit.word = buf;
                cout << "������� ��� �������: ";
                cin >> buf;
                toLow(buf);
                unit.translate = buf;
                cout << "������� ��� �������: ";
                enterInt(answer);

                dict.addWord(unit);
                cout << "����� ���� ������.";
            }
            break;
        case 3:
            {
                string word;
                cout <<"������� ����� �� ����., ������� ������ �������: ";
                cin >> word;
                toLow(word);
                dict.removeWord(word);
            }
            break;
        case 4:
            if (dict.isAbc_type()) {
                cout << "������� ������������ �� ��������." << endl;
            } else {
                cout << "������� ������������ �� ��������." << endl;
            }
            break;
        case 5:
            dict.changeType();
            cout << "��� ������� ��� ������� ������." << endl;
            break;
        case 0:
            return 0;

        default:
            cout << "�� ����� ������������ ����� �������!" << endl;
        }
        cout << endl;
    }
}