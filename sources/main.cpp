#include <iostream>
#include "pocketmon.h"
#include "user.h"
using namespace std;

const int mapX = 5;
const int mapY = 5;

const int ROAD = 0;
const int WALL = 1;
const int GRASS = 2;

// ANSI 색상 코드 상수
const string RESET = "\033[0m";

// 픽셀 타일: 글자 하나 = 픽셀 하나 (색은 pixelColor 참고), 한 칸은 8x8 픽셀
const int TILE_SIZE = 8;
// 맵 값(ROAD, WALL, GRASS)을 그대로 인덱스로 사용
const string TILES[3][TILE_SIZE] = {
    {"pppppppp", "ppppppsp", "pspppppp", "pppppppp", // 길
     "ppppsppp", "pppppppp", "ppsppppp", "pppppppp"},
    {"ddDDDDdd", "dDGLLGDd", "DGLLGGGD", "DGLGGGGD", // 나무(벽)
     "DGGGGGGD", "dDGGGGDd", "ddDTTDdd", "dddTTddd"},
    {"gggggggg", "gbgggbgg", "gblgbblg", "gggggggg", // 풀숲
     "gggbgggg", "glbbgbgg", "ggggbblg", "gggggggg"}};
// 플레이어: 공백은 투명 (밑에 깔린 타일이 보임)
const string PLAYER_TILE[TILE_SIZE] = {
    "  RRRR  ", " RRRRRR ", " KkkkkK ", " kKkkKk ",
    "  BBBB  ", " kBBBBk ", "  BBBB  ", "  K  K  "};

// 픽셀 글자 -> "R;G;B"
string pixelColor(char c)
{
    switch (c)
    {
    case 'p': return "215;190;130"; // 길
    case 's': return "195;168;110"; // 길 위 자갈
    case 'd': return "40;110;60";   // 나무 밑 바닥
    case 'D': return "15;80;40";    // 나무 테두리
    case 'G': return "30;140;60";   // 나뭇잎
    case 'L': return "80;180;90";   // 밝은 나뭇잎
    case 'T': return "110;75;40";   // 나무 줄기
    case 'g': return "90;180;80";   // 풀숲 바닥
    case 'b': return "50;140;60";   // 풀잎
    case 'l': return "140;215;110"; // 밝은 풀잎
    case 'R': return "220;50;50";   // 모자
    case 'k': return "245;200;160"; // 피부
    case 'B': return "50;90;200";   // 옷
    default: return "30;30;30";     // 'K' 검정
    }
}

// 사용자 정의 함수
bool isValidXY(int px, int py, vector<vector<int>> map);
void moveUp(vector<vector<int>> map, int &px, int &py);
void moveDown(vector<vector<int>> map, int &px, int &py);
void moveLeft(vector<vector<int>> map, int &px, int &py);
void moveRight(vector<vector<int>> map, int &px, int &py);
void printMap(const vector<vector<int>> &map, int playerY, int playerX);
void pocketmonBattle(Pocketmon &p1, vector<Skill> skills, Pocketmon &toPocketmon);

// g++ -std=c++17 -Ipocketmon *.cpp pocketmon/*.cpp -o main && ./main
int main()
{

    // v1.map은 2찬원 배열 백터로 표현
    //  0 = 길, 1 = 벽, 2 = 포켓몬 등장하는 풀숲
    vector<vector<int>> map = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 2, 2, 2, 0, 1},
        {1, 0, 0, 2, 2, 2, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 2, 2, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}};

    // 유저의 위치를 저장할 변수
    int user_x = 0;
    int user_y = 0;

    int py = 1, px = 1; // 플레이어 시작 좌표
    bool isMove = true;
    printMap(map, py, px);
    srand(time(0));
    User user;

    // 게임 시작
    while (1)
    { // 사용자에게 계속 입력 받기 위한 무한루프

        // 사용자의 입력을 저장할 변수
        string user_input = "";

        // 최초 유저포켓몬이 하나도 없다면 포켓몬 하나 고르는 로직 실행
        if (user.getPocketmons().size() < 1)
        {
            cout << "지급 받을 포켓몬을 고르세요 1.이상해씨, 2.파이리, 3.꼬부기: ";
            cin >> user_input;
            if (user_input == "1")
            {
                Pocketmon p1 = Pocketmon(1);
                user.addPocketmon(p1);
                p1.printPocketInfo();
            }
            else if (user_input == "2")
            {
                Pocketmon p2 = Pocketmon(4);
                user.addPocketmon(p2);
                p2.printPocketInfo();
            }
            else
            {
                Pocketmon p3 = Pocketmon(7);
                user.addPocketmon(p3);
                p3.printPocketInfo();
            }
        }

        cout << "이동할 명렁어 숫자를 입력하세요 1.상,하,좌,우 2. 가방: ";
        cin >> user_input;

        if (user_input == "상")
        {
            moveUp(map, px, py);
        }
        else if (user_input == "하")
        {
            moveDown(map, px, py);
        }
        else if (user_input == "좌")
        {
            moveLeft(map, px, py);
        }
        else if (user_input == "우")
        {
            moveRight(map, px, py);
        }
        else if (user_input == "2")
        {
            // 1. 포켓몬 리스트
            cout << "보유한 포켓몬" << endl;
            user.printPocketmon();
        }
        else
        {
            cout << "잘못된 입력입니다.: " << endl;
        }

        // 이동 후 풀숲 판정 예시
        if (map[py][px] == GRASS)
        {
            if (rand() % 100 < 100)
            { // 30% 확률로 야생 포켓몬 등장
                cout << "야생의 포켓몬이 나타났다!\n";
                Pocketmon p;
                p.printPocketInfo();
                pocketmonBattle(user.getPocketmons()[0], user.getPocketmons()[0].getSkills(), p);
            }
        }
    }
}

// 위로 움직이는 함수
void moveUp(vector<vector<int>> map, int &px, int &py)
{

    if (!isValidXY(px, py - 1, map))
    {
        cout << "맵을 벗어났습니다. 다시 돌아갑니다." << endl;
    }
    else
    {
        cout << "위로 한 칸 올라갑니다." << endl;
        // 위로 한 칸 올라가기
        py--;
        printMap(map, py, px);
    }
}

// 아래로 움직이는 함수
void moveDown(vector<vector<int>> map, int &px, int &py)
{

    if (!isValidXY(px, py + 1, map))
    {
        cout << "맵을 벗어났습니다. 다시 돌아갑니다." << endl;
    }
    else
    {
        cout << "위로 한 칸 내려갑니다." << endl;
        py++;
        printMap(map, py, px);
    }
}

// 왼쪽로 움직이는 함수
void moveLeft(vector<vector<int>> map, int &px, int &py)
{
    // TODO: 왼쪽으로 이동하기

    if (!isValidXY(px - 1, py, map))
    {
        cout << "맵을 벗어났습니다. 다시 돌아갑니다." << endl;
    }
    else
    {
        cout << "왼쪽으로 이동합니다." << endl;
        px--;
        printMap(map, py, px);
    }
}

// 오른쪽로 움직이는 함수
void moveRight(vector<vector<int>> map, int &px, int &py)
{
    // TODO: 오른쪽으로 이동하기

    if (!isValidXY(px + 1, py, map))
    {
        cout << "맵을 벗어났습니다. 다시 돌아갑니다." << endl;
    }
    else
    {
        cout << "오른쪽으로 이동합니다." << endl;
        px++;
        printMap(map, py, px);
    }
}

// 이동하려는 좌표(px, py)가 맴 범위 안이고 벽이 아닌지 체크하는 함수
bool isValidXY(int px, int py, vector<vector<int>> map)
{
    return py >= 0 && py < map.size() && px >= 0 && px < map[py].size() && map[py][px] != WALL;
}

void printMap(const vector<vector<int>> &map, int py, int px)
{
    // 포켓몬 그림과 같은 방식: 반블록 ▀ 한 글자에 위 픽셀(글자색) + 아래 픽셀(배경색)
    for (int y = 0; y < map.size(); y++)
    {
        for (int ty = 0; ty < TILE_SIZE; ty += 2)
        {
            for (int x = 0; x < map[y].size(); x++)
            {
                // 플레이어 칸이면 플레이어 픽셀, 투명(공백)이면 밑의 타일 픽셀
                auto pixel = [&](int row, int col)
                {
                    char c = (y == py && x == px) ? PLAYER_TILE[row][col] : ' ';
                    return c == ' ' ? TILES[map[y][x]][row][col] : c;
                };
                for (int tx = 0; tx < TILE_SIZE; tx++)
                {
                    cout << "\033[38;2;" << pixelColor(pixel(ty, tx)) << "m"
                         << "\033[48;2;" << pixelColor(pixel(ty + 1, tx)) << "m▀";
                }
            }
            cout << RESET << "\n";
        }
    }
}

void pocketmonBattle(Pocketmon &p1, vector<Skill> skills, Pocketmon &toPocketmon)
{
    bool flag = true;

    // 사용자의 입력을 저장할 변수
    string user_input = "";
    int turn = 0;

    while (flag)
    {
        int r = rand() % 2;

        cout << "1. 싸운다 2. 도망친다";
        cin >> user_input;

        if (user_input == "1")
        {
            p1.printPocketInfo();
            p1.printSkills();

            cout << "사용할 스킬 번호를 입력하세요: ";
            cin >> user_input;

            if (user_input == "1")
            {   
                cout << p1.getName();
                flag = toPocketmon.DemagePocketmon(skills[0], flag);
                if (flag) {
                    cout << toPocketmon.getName();
                    flag = p1.DemagePocketmon(skills[r], flag);
                }
            }
            else if (user_input == "2")
            {
                cout << p1.getName();
                flag = toPocketmon.DemagePocketmon(skills[1], flag);
                if(flag) {
                    cout << toPocketmon.getName();
                    flag = p1.DemagePocketmon(skills[r], flag);
                }
            }
        }
        else
        {
            cout << "무사히 도망쳤다" << endl;
            flag = false;
        }
    }
}