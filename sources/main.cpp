#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
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
// 몬스터볼 12x12: 공백은 투명
const string BALL_TILE[12] = {
    "    KKKK    ", "  KKRRRRKK  ", " KRRWWRRRRK ", " KRWWRRRRRK ",
    "KRRRRRRRRRRK", "KKKKKWWKKKKK", "KWWWKWWKWWWK", "KWWWWKKWWWWK",
    " KWWWWWWWWK ", " KWWWWWWWWK ", "  KKWWWWKK  ", "    KKKK    "};

// 픽셀 글자 -> "R;G;B"
string pixelColor(char c)
{
    switch (c)
    {
    case 'p':
        return "215;190;130"; // 길
    case 's':
        return "195;168;110"; // 길 위 자갈
    case 'd':
        return "40;110;60"; // 나무 밑 바닥
    case 'D':
        return "15;80;40"; // 나무 테두리
    case 'G':
        return "30;140;60"; // 나뭇잎
    case 'L':
        return "80;180;90"; // 밝은 나뭇잎
    case 'T':
        return "110;75;40"; // 나무 줄기
    case 'g':
        return "90;180;80"; // 풀숲 바닥
    case 'b':
        return "50;140;60"; // 풀잎
    case 'l':
        return "140;215;110"; // 밝은 풀잎
    case 'R':
        return "220;50;50"; // 모자
    case 'k':
        return "245;200;160"; // 피부
    case 'B':
        return "50;90;200"; // 옷
    case 'W':
        return "240;240;240"; // 몬스터볼 흰색
    default:
        return "30;30;30"; // 'K' 검정
    }
}

// 사용자 정의 함수
bool isValidXY(int px, int py, vector<vector<int>> map);
void moveUp(vector<vector<int>> map, int &px, int &py);
void moveDown(vector<vector<int>> map, int &px, int &py);
void moveLeft(vector<vector<int>> map, int &px, int &py);
void moveRight(vector<vector<int>> map, int &px, int &py);
void printMap(const vector<vector<int>> &map, int playerY, int playerX);
void pocketmonBattle(User &user, Pocketmon &p1, vector<Skill> skills, Pocketmon &toPocketmon);
void checkGrass(User &user, vector<vector<int>> map, int px, int py);
void drawBattle(Pocketmon &me, Pocketmon &enemy, const string &msg, bool typing = false, Pocketmon *hidden = nullptr);

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
            // 풀숲인지 확인
            checkGrass(user, map, px, py);
        }
        else if (user_input == "하")
        {
            moveDown(map, px, py);
            // 풀숲인지 확인
            checkGrass(user, map, px, py);
        }
        else if (user_input == "좌")
        {
            moveLeft(map, px, py);
            // 풀숲인지 확인
            checkGrass(user, map, px, py);
        }
        else if (user_input == "우")
        {
            moveRight(map, px, py);
            // 풀숲인지 확인
            checkGrass(user, map, px, py);
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

// 플레이어 칸이면 플레이어 픽셀, 투명(공백)이면 밑의 타일 픽셀
char tilePixel(int tile, bool isPlayer, int row, int col)
{
    char c = ' ';
    if (isPlayer)
        c = PLAYER_TILE[row][col];
    if (c == ' ')
        c = TILES[tile][row][col];
    return c;
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
                bool isPlayer = (y == py && x == px);
                for (int tx = 0; tx < TILE_SIZE; tx++)
                {
                    cout << "\033[38;2;" << pixelColor(tilePixel(map[y][x], isPlayer, ty, tx)) << "m"
                         << "\033[48;2;" << pixelColor(tilePixel(map[y][x], isPlayer, ty + 1, tx)) << "m▀";
                }
            }
            cout << RESET << "\n";
        }
    }
}

// 몬스터볼 그리기: 투명 픽셀은 배경 그대로 (indent칸 만큼 오른쪽으로)
void printBall(int indent = 0)
{
    for (int y = 0; y < 12; y += 2)
    {
        cout << string(indent, ' ');
        for (int x = 0; x < 12; x++)
        {
            char top = BALL_TILE[y][x], bot = BALL_TILE[y + 1][x];
            if (top == ' ' && bot == ' ')
                cout << RESET << " ";
            else if (top == ' ') // 아래만 색: ▄ 글자색으로
                cout << RESET << "\033[38;2;" << pixelColor(bot) << "m▄";
            else if (bot == ' ') // 위만 색: ▀ 글자색으로
                cout << RESET << "\033[38;2;" << pixelColor(top) << "m▀";
            else
                cout << "\033[38;2;" << pixelColor(top) << "m\033[48;2;" << pixelColor(bot) << "m▀";
        }
        cout << RESET << "\n";
    }
}

// ms 밀리초 동안 멈춘다
void pause(int ms)
{
    this_thread::sleep_for(chrono::milliseconds(ms));
}

// 메시지를 한 글자씩 띄우고 읽을 시간을 준다
void show(Pocketmon &p1, Pocketmon &toPocketmon, const string &msg)
{
    drawBattle(p1, toPocketmon, msg, true);
    pause(700);
}

// 공격 한 번: 기술 선언 → 피격 깜빡임 → HP가 줄어듦 → 결과
// 맞은 쪽이 아직 살아있으면 true
bool attack(Pocketmon &p1, Pocketmon &toPocketmon, const string &who, Skill s, Pocketmon &target)
{
    string msg = who + "의 " + s.name + "!";
    show(p1, toPocketmon, msg);

    for (int i = 0; i < 3; i++) // 맞은 쪽 그림을 숨겼다 보였다
    {
        drawBattle(p1, toPocketmon, msg, false, &target);
        pause(80);
        drawBattle(p1, toPocketmon, msg);
        pause(80);
    }

    for (int left = s.power; left > 0 && target.getHp() > 0; left -= 2) // HP를 2씩 나눠 깎기
    {
        target.DemagePocketmon({s.name, min(2, left)}, true);
        drawBattle(p1, toPocketmon, msg);
        pause(40);
    }

    show(p1, toPocketmon, target.getName() + "에게 " + to_string(s.power) + " 데미지!");
    return target.getHp() > 0;
}

void pocketmonBattle(User &user, Pocketmon &p1, vector<Skill> skills, Pocketmon &toPocketmon)
{
    string user_input = "";
    show(p1, toPocketmon, "야생의 " + toPocketmon.getName() + "이(가) 나타났다!");

    while (true)
    {
        drawBattle(p1, toPocketmon, p1.getName() + "은(는) 무엇을 할까?", true);
        cout << "1. 싸운다 2. 몬스터볼 3. 도망친다: ";
        cin >> user_input;

        if (user_input == "3")
        {
            show(p1, toPocketmon, "무사히 도망쳤다!");
            return;
        }

        if (user_input == "2")
        {
            // 포켓몬 포획
            for (int i = 0; i < 3; i++) // 볼 3번 깜빡임: 그림 → 멈춤 → 지움 → 멈춤
            {
                drawBattle(p1, toPocketmon, "몬스터볼을 던졌다!", false, &toPocketmon); // 적 그림 숨김
                printBall(10);
                pause(400);
                drawBattle(p1, toPocketmon, "몬스터볼을 던졌다!", false, &toPocketmon); // 볼 없이 다시 그림
                pause(200);
            }

            if (toPocketmon.tryCatch()) {
                show(p1, toPocketmon, "신난다! " + toPocketmon.getName() + "을(를) 붙잡았다!");
                user.addPocketmon(toPocketmon); 
                return;
            } 

            
        }

        if (user_input == "1") {
            drawBattle(p1, toPocketmon, "어떤 기술을 쓸까?");
            p1.printSkills();
            cout << "사용할 스킬 번호를 입력하세요: ";
            cin >> user_input;

            int idx = user_input[0] - '1';
            if (user_input.size() != 1 || idx < 0 || idx >= skills.size())
                continue; // 잘못된 번호면 메뉴로

            // 내 턴
            if (!attack(p1, toPocketmon, p1.getName(), skills[idx], toPocketmon))
            {
                show(p1, toPocketmon, "야생의 " + toPocketmon.getName() + "을(를) 쓰러트렸다!");
                return;
            }

            // 적 턴: 적 자신의 스킬 중 하나
            vector<Skill> enemySkills = toPocketmon.getSkills();
            Skill s = enemySkills[rand() % enemySkills.size()];
            if (!attack(p1, toPocketmon, "야생 " + toPocketmon.getName(), s, p1))
            {
                show(p1, toPocketmon, p1.getName() + "이(가) 쓰러졌다...");
                return;
            }
        }
    }
}

// 화면에 보이는 폭: 한글(3바이트 UTF-8)은 2칸, 그 외는 1칸
int displayWidth(const string &s)
{
    int w = 0;
    for (unsigned char c : s)
    {
        if ((c & 0xC0) == 0x80) // UTF-8 이어지는 바이트는 건너뜀
            continue;
        if (c >= 0xE0)
            w += 2;
        else
            w += 1;
    }
    return w;
}

// 배틀 화면: 매번 화면을 지우고 같은 배치로 다시 그린다
// typing: 메시지를 한 글자씩 출력 / hidden: 그림을 숨길 포켓몬 (피격 깜빡임용)
void drawBattle(Pocketmon &me, Pocketmon &enemy, const string &msg, bool typing, Pocketmon *hidden)
{
    const int BOX = 44; // 메시지 박스 안쪽 폭
    string line;
    for (int i = 0; i < BOX; i++)
        line += "─";

    cout << "\033[2J\033[3J\033[H"; // 화면 + 스크롤 기록 지우고 커서를 맨 위로
    cout << enemy.getName() << "  ";
    cout << "Level. " << enemy.getLevel() << endl;
    enemy.printHpBar();
    enemy.printArt(&enemy == hidden);
    me.printArt(&me == hidden);
    cout << me.getName() << "  ";
    cout << "Level. " << enemy.getLevel() << endl;
    me.printHpBar();

    string shown = msg; // 메시지 박스에서 처음 그릴 글자

    if (typing) // 타이핑이면 빈 박스부터 그림
        shown = "";
    cout << "┌" << line << "┐\n"
         << "│ " << shown << string(max(0, BOX - 1 - displayWidth(shown)), ' ') << "│\n"
         << "└" << line << "┘" << endl;

    if (typing) // 빈 박스 출력 후 글자 하나씩 출력
    {
        cout << "\033[2A\033[3G"; // 커서를 박스 안(2줄 위, 3번째 칸)으로
        for (int i = 0; i < msg.size(); i++)
        {
            cout << msg[i];
            // 한글은 3바이트라, 글자 하나가 끝났을 때만 화면에 내보내고 쉰다
            if (i + 1 == msg.size() || (msg[i + 1] & 0xC0) != 0x80)
            {
                cout << flush;
                pause(25); // 글자 한글자 출력 후 pause
            }
        }
        cout << "\033[2B\r" << flush; // 커서를 박스 아래로 되돌림
    }
}

void checkGrass(User &user, vector<vector<int>> map, int px, int py)
{
    // 이동 후 풀숲 판정 예시
    if (map[py][px] == GRASS)
    {
        if (rand() % 100 < 100)
        { // 30% 확률로 야생 포켓몬 등장
            Pocketmon p;
            int userPocketmonLv = user.getPocketmons()[0].getLevel();
            p.setLevel(userPocketmonLv - 2 + rand() % 5);
            pocketmonBattle(user, user.getPocketmons()[0], user.getPocketmons()[0].getSkills(), p);
            printMap(map, py, px); // 배틀 화면이 맵을 지웠으니 다시 그림
        }
    }
}
