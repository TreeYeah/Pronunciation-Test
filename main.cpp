#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

using namespace std;

#define WARNING 79
#define EMPHASIZE 159
#define TIP 249
#define INSTRUCT 241
#define CHOOSE 63
#define LOCK 31
#define CORRECT 242
#define INCORRECT 252
#define TONE 243

#define HistoryIncorrectWidth 22

bool ToneAssist = false;
int wordsN;
int completedN;
int testsN;
string name;
unsigned long long timing;
int correctN;
int delayTime; //ms

void result(bool);

map <string,int> wordIndex;

struct wordData
{
    string word;
    string choice[4];
    short myChoice;
    short which;
    short correct;
    short randomIndex;
}word[256];

struct testData
{
    string name;

    int wordsN;
    int correctN;
    int timing;

    int year;
    int month;
    int day;
    int hour;
    int min;

    vector <string> incorrect;
}test[256];

struct TimeData
{
    int Year,Mon,Day,Hour,Min,Sec,WDay;
};

TimeData getTime()
{
    TimeData t{};
    time_t rawtime;
    struct tm*timeinfo;
    time(&rawtime);
    timeinfo=localtime(&rawtime);
    time_t tick=mktime(timeinfo);
    t.Year = timeinfo->tm_year+1900;
    t.Mon = timeinfo->tm_mon+1;
    t.Day = timeinfo->tm_mday;
    t.Hour = timeinfo->tm_hour;
    t.Min = timeinfo->tm_min;
    t.Sec = timeinfo->tm_sec;
    t.WDay = timeinfo->tm_wday;

    return t;
}

int getRand(int mini,int maxi)
{
    return rand() % (maxi - mini + 1) + mini;
}


void color(int a)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

void go(float x,float y)  //����ƶ�������X��ʾ�����꣬Y��ʾ�����ꡣ
{
    COORD coord;         //ʹ��ͷ�ļ��Դ�������ṹ
    coord.X = x*2;            //���ｫint����ֵ����short,�����������漰������ֵ�����ᳬ��short��Χ
    coord.Y = y;
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);  //��ñ�׼������
    SetConsoleCursorPosition(a,coord);         //�Ա�׼����ľ��Ϊ�������ÿ���̨�������
}

short getTone(string p)
{
    if(p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos)
    {
        return 1;
    }
    if(p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos)
    {
        return 2;
    }
    if(p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos)
    {
        return 3;
    }
    if(p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos || p.find("��") != string::npos)
    {
        return 4;
    }
    return 0;
}

bool cmpRand(wordData a, wordData b)
{
    return a.randomIndex < b.randomIndex;
}

bool cmpPinyin(string a, string b)
{
    return wordIndex[a] < wordIndex[b];
}

void readWords()
{
    ifstream infile("Words.txt");
    if(!infile)
    {
        color(WARNING);
        go(1,1);
        printf("��ȡ��Ŀʧ�ܣ�\n");
        system("pause");
        return;
    }

    infile >> wordsN;
    completedN = wordsN;
    char w = ' ';

    unsigned seed = time(nullptr);
    srand(seed);//�������������

    for(int i = 0; i < wordsN; i++)
    {
        word[i].myChoice = -1;
        infile >> w;
        word[i].which = w - '0';
        if(word[i].which <= 0 || word[i].which > 9)
        {
            color(WARNING);
            printf("��ȡ��Ŀ����\n");
            system("pause");
            break;
        }


        infile >> word[i].word;

        wordIndex[word[i].word] = i;
        word[i].randomIndex = getRand(1,32767);

        for(auto & j : word[i].choice)
            infile >> j;
        word[i].correct = 0;

        //����
        for(int j = 1; j <= 4; j++)
        {
            int target = getRand(0,3);
            swap(word[i].choice[target], word[i].choice[word[i].correct]);
            word[i].correct = target;
        }
    }

    sort(word, word + wordsN, cmpRand);
}

void startingAnimation()
{
    CONSOLE_CURSOR_INFO cursor_info={1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
    system("color F0");

    color(151);
    int spacing = 7;
    for(int i = 0; i < 50 ; i+=2)
    {
        for(int j = 0 ;j < 16; j++)
        {
            go(i, j);
            printf("  ");
        }
        Sleep(spacing);
    }

    color(249);
    for(int i = 49; i >= 0 ; i-=2)
    {
        for(int j = 0 ;j < 16; j++)
        {
            go(i, j);
            printf("  ");
        }
        Sleep(spacing);
    }

    color(151);
    for(int i = 0; i < 16 ; i++)
    {
        for(int j = 0 ;j < 50; j+=2)
        {
            go(j, i);
            printf("  ");
        }
        Sleep(spacing * 2);
    }

    color(249);
    for(int i = 15; i >= 0 ; i--)
    {
        for(int j = 0 ;j < 50; j+=2)
        {
            go(j, i);
            printf("  ");
        }
        Sleep(spacing * 2);
    }

    Sleep(256);
}

void start()
{
    SetConsoleTitle("���ﳣ���ִʶ�����׼�Բ���");
    system("mode con cols=50 lines=16");

    startingAnimation();

    readWords();

    color(EMPHASIZE);
        go(5,6);
        printf("  ���ﳣ���ִʶ�����׼�Բ���  ");
        go(5,5);
        printf("                              ");
        go(5,7);
        printf("                              ");

    CONSOLE_CURSOR_INFO cursor_info={1,1};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

    Sleep(256);

    color(INSTRUCT);
        go(7,9);
        for(int p = 0; p < strlen("����������֣�"); p += 2)
        {
            cout << "����������֣�"[p] << "����������֣�"[p+1];
            Sleep(64);
        }
        cin >> name;

    CONSOLE_CURSOR_INFO cursor_info2={1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info2);

    system("cls");

    color(EMPHASIZE);
        go(5,2);
        printf("  ���ﳣ���ִʶ�����׼�Բ���  ");
        go(5,1);
        printf("                              ");
        go(5,3);
        printf("                              ");

    color(TIP);
        go(4,5);
        printf("[1][2][3][4]ѡ��");
        go(4,6);
        printf("[-][=]������һ��/��һ��");
        go(4,7);
        printf("һ����%d����Ŀ��Ԥ��%d���ӿ������",wordsN,(wordsN * 3 + 59) / 60);
        go(4,8);
        printf("����[Esc]��ǰ����");

    color(INSTRUCT);
        go(3,10);
        printf("��������ʱ�ᱻ��¼��������[Space]��ͣ");
        go(6,11);
        printf("��[T]�ɿ���������ʶ����ϵͳ");
        go(6.5,12);
        printf("�밴���Լ��ĵ�һ��Ӧ����");
        go(9.5,13);
        printf("���������ʼ");

    if(getch() == 'T')
    {
        ToneAssist = 1 - ToneAssist;
    }
}

void printTheChoice(int ch,int wordT,int COLOR)
{
    go(9,7 + ch);
    color(COLOR);
    if(ch == 0)
        cout << "�� " << word[wordT].choice[0];
    if(ch == 1)
        cout << "�� " << word[wordT].choice[1];
    if(ch == 2)
        cout << "�� " << word[wordT].choice[2];
    if(ch == 3)
        cout << "�� " << word[wordT].choice[3];

    if(!ToneAssist)
        return;

    color(TONE);
    printf("%d",getTone(word[wordT].choice[ch]));
}

void printWord(int t)
{
    for(int i = 0; i < word[t].word.length(); i += 2)
    {
        if(i / 2 + 1 == word[t].which)
            color(EMPHASIZE);
        else
            color(INSTRUCT);
        cout << word[t].word[i] << word[t].word[i+1];
    }
}

void testing()
{
    system("cls");

    timing = time(nullptr);

    for(int t = 0; t < wordsN; ++t)
    {
        SetConsoleTitle(("���ﳣ���ִʶ�����׼�Բ��ԣ�" + to_string(t + 1) + " / " + to_string(wordsN) + "��").c_str());

        go(9.5, 5);
        printWord(t);

        int key = 0;
        while( ( key != '1' && key != '2' && key != '3' && key != '4' && key != '-' && key != '=' && key != '+' && key != 27 && key != 'T' || kbhit() ) && key != 32 )
        {
            printTheChoice(0,t,INSTRUCT);
            printTheChoice(1,t,INSTRUCT);
            printTheChoice(2,t,INSTRUCT);
            printTheChoice(3,t,INSTRUCT);

            if(word[t].myChoice >= 0)
            {
                printTheChoice(word[t].myChoice,t,CHOOSE);
            }

            key = getch();
            if(key >= '1' && key <= '4')
            {
                word[t].myChoice = key - '1';

                go(9, 7 + key - '1');
                printTheChoice(key - '1',t,CHOOSE);

                while(GetKeyState(key) < 0)
                    Sleep(50);

                if(!kbhit())
                {
                    printTheChoice(key - '1',t,LOCK);
                    Sleep(300);
                    delayTime += 350;
                }
            }

            if(key == '-')
            {
                if(t > 0)
                    t -=2;
                else
                    t = -1;
            }
            if(key == 27)
            {
                if(t)
                {
                    completedN = t;
                }
            }
            if(key == 'T')
            {
                ToneAssist = 1 - ToneAssist;
                t--;
            }
            if(key == 32)
            {
                int reduce = 0;

                SetConsoleTitle(("���ﳣ���ִʶ�����׼�Բ��ԣ�" + to_string(t + 1) + " / " + to_string(wordsN) + "��������ͣ�У���[Space]������").c_str());
                system("cls");
                color(EMPHASIZE);
                    go(0,0);
                    printf("                                                  "
                           "                                                  "
                           "                                                  "
                           "                                                  "
                           "                                                  "
                           "                                                  "
                           "            ���ﳣ���ִʶ�����׼�Բ���            "
                           "                                                  "
                           "                                                  "
                           "                                                  "
                           "                                                  "
                           "                                                  "
                           "                                                  ");

                color(TIP);
                    go(7, 14);
                    printf("��ͣ�У���[Space]������");

                while(!kbhit() || kbhit() && getch() != 32)
                {
                    reduce++;
                    Sleep(1000);
                }
                t--;
                key = 32;
                timing += reduce;
            }
        }

        if(completedN != wordsN)
            break;

        if(t == completedN)
        {
            system("cls");
            system("color F0");
            go(9.5, 5);
            printf("��[-]���ؼ�飬�������ύ��");
            key = getch();
            if(key == '-')
                t--;
        }

        system("cls");
        system("color F0");

        CONSOLE_CURSOR_INFO cursor_info={1,0};
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//���ع��
    }
    timing = time(nullptr) - timing - (delayTime + 500) / 1000;
}

void save()
{
    ifstream infile("User_data.txt");
    test[0] = {name, completedN, correctN, (int)timing, getTime().Year, getTime().Mon, getTime().Day, getTime().Hour, getTime().Min,};
    for(int i = 0; i < completedN; i++)
        if(word[i].correct != word[i].myChoice)
            test[0].incorrect.push_back(word[i].word);

    sort(test[0].incorrect.begin(), test[0].incorrect.end(), cmpPinyin);

    int i = 0;
    while(infile >> test[++i].name)
    {
        infile >> test[i].wordsN >> test[i].correctN >> test[i].timing >> test[i].year >> test[i].month >> test[i].day >> test[i].hour >> test[i].min;
        for(int j = 0; j < test[i].wordsN - test[i].correctN; j++)
        {
            string s;
            infile >> s;
            test[i].incorrect.push_back(s);
        }
    }

    testsN = i;

    ofstream outfile("User_data.txt");

    for(int j = 0; j < i; j++)
    {
        outfile << test[j].name << " " << test[j].wordsN << " " << test[j].correctN  << " " << test[j].timing  << " " << test[j].year  << " " << test[j].month  << " " << test[j].day  << " " << test[j].hour  << " " << test[j].min << " ";
        for(auto & k : test[j].incorrect)
        {
            outfile << k << " ";
        }
        outfile << endl;
    }
}

void history()
{
    system("cls");
    system("color F0");
    SetConsoleTitle("��ʷ���ݣ���[Esc]���أ�");

    CONSOLE_CURSOR_INFO cursor_info={1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

    int  line = 3;

    system(("mode con cols=" + to_string(62 + HistoryIncorrectWidth * 2) + " lines=" + to_string(testsN * 12 + line)).c_str());

    color(EMPHASIZE);
    go(0,line - 2);
    for(int i = 1; i <= 31 + HistoryIncorrectWidth; i++)
        printf("  ");
    go(2,line - 2);
    printf("����");
    go(8,line - 2);
    printf("��ȷ����");
    go(13,line - 2);
    printf("��ȷ��");
    go(17,line - 2);
    printf("�ٶ�");
    go(21,line - 2);
    printf("�ύʱ��");
    go(30,line - 2);
    printf("����");

    color(INSTRUCT);

    for(int i = 0; i < testsN; i++)
    {
        go(2,line);
        cout << test[i].name;

        go(8,line);
        cout << test[i].correctN << "/" << test[i].wordsN;

        go(13,line);
        printf("%.2lf%%",(double)test[i].correctN / test[i].wordsN * 100);

        go(17,line);
        printf("%.1fs/��", (float) test[i].timing / test[i].wordsN);

        go(21,line);
        printf("%.2d-%.2d-%.2d %.2d:%.2d",test[i].year,test[i].month,test[i].day,test[i].hour,test[i].min);

        int width = 0;
        for(int j = 0; j < test[i].incorrect.size(); j++)
        {
            width += test[i].incorrect[j].length() / 2 + 1;
            if(width > HistoryIncorrectWidth)
            {
                width = test[i].incorrect[j].length() / 2 + 1;
                line++;
            }
            go(30 + width - test[i].incorrect[j].length() / 2 - 1, line);
            cout << test[i].incorrect[j] << "  ";
        }
        line += 2;
    }

    int key = getch();
    while(key != 27)
    {
        key = getch();
    }

    result(false);
}

void result(bool saveOrNot)
{
    system("cls");
    system("color F0");
    SetConsoleTitle("���Խ�������[Enter]���֣���[Tab]�鿴��ʷ���ݡ�");

    CONSOLE_CURSOR_INFO cursor_info={1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

    vector <int>incorrectT;

    for(int i = 0; i < completedN; i++)
        if(word[i].correct == word[i].myChoice)
        {
            if(saveOrNot)
                correctN++;
        }
        else
            incorrectT.push_back(i);

    if(saveOrNot)
    {
        save();
    }


    system(("mode con cols=53 lines=" + to_string(14 + completedN - correctN)).c_str());

    go(3,3);
    color(EMPHASIZE);
    cout << "����" ;
    color(INSTRUCT);
    cout << name;

    go(3,4);
    color(EMPHASIZE);
    cout << "�������" ;
    color(INSTRUCT);
    cout << correctN << "����" << completedN << "��";

    go(3,5);
    color(EMPHASIZE);
    cout << "��ȷ��" ;
    color(INSTRUCT);
    cout << ((double)correctN / completedN) * 100 << "%";

    go(3,6);
    color(EMPHASIZE);
    cout << "��ʱ";
    color(INSTRUCT);
    cout << " ";
    if(timing + (delayTime + 500) / 1000 >= 60)
        cout << (timing + (delayTime + 500) / 1000) / 60 << "min ";
    cout << (timing + (delayTime + 500) / 1000) % 60 << "s";
    printf(" (%.1fs/��)", (float) timing / completedN);

    go(3,8);
    color(EMPHASIZE);
    cout << "�������\n";
    color(INSTRUCT);
    cout << "        ";

    bool flag = true;

    for(int i = 0; i < completedN; i++)
        if(word[i].correct != word[i].myChoice)
        {
            flag = false;
            printWord(i);

            if (word[i].myChoice >= 0)
            {
                color(INCORRECT);
                cout << " (" << word[i].choice[word[i].myChoice] << ")";
            }
            color(CORRECT);

            cout << "(" << word[i].choice[word[i].correct] << ")\n        ";
        }
    if(flag)
        printf("�ޣ�\n");


    color(TIP);
    cout << "\n" << "    ��[-][=][Enter]���֣���[Tab]�鿴��ʷ���ݡ�";

    int key = 0,p = 0;

    if(0 != incorrectT.size())
    {
        go(1,9);
        cout << "��ѯ>";
        while(key != '0' && p != incorrectT.size())
        {
            key = getch();
            if(key == 13)
            {
                system(("start https://hanyu.baidu.com/s?wd=" + word[incorrectT[p]].word.substr(word[incorrectT[p]].which * 2 - 2, 2) + "^&ptype=zici").c_str());
            }
            if(key == '-' && p > 0)
            {
                p--;
                go(1,10 + p);
                cout << "     ";
                go(1,9 + p);
                cout << "��ѯ>";
            }
            if((key == '=' || key == '+') && p < incorrectT.size() - 1)
            {
                p++;
                go(1,8 + p);
                cout << "     ";
                go(1,9 + p);
                cout << "��ѯ>";
            }
            if(key == 9)
            {
                history();
                return;
            }
        }
    }
    else
    {
        while(key != '0')
        {
            key = getch();
            if(key == 9)
            {
                history();
                return;
            }
        }
    }
}

int main()
{
    printf("Data Reading...");
    start();
    testing();
    result(true);

    return 0;
}