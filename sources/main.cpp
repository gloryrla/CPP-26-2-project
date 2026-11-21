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
const string RESET        = "\033[0m";
const string GREEN        = "\033[32m";      // 초록 글자 (깔끔한 풀숲)
const string GRASS_TILE   = "\033[42;92m";   // 초록 배경 + 연두 글자 (타일형 풀숲)
const string YELLOW       = "\033[93m";      // 플레이어 표시
const string GRAY         = "\033[90m";      // 빈 길 표시


// 사용자 정의 함수
bool isValidXY(int px, int py, vector<vector<int>> map);
void moveUp(vector<vector<int>> map, int& px, int& py);
void moveDown(vector<vector<int>> map, int& px, int& py);
void moveLeft(vector<vector<int>> map, int& px, int& py);
void moveRight(vector<vector<int>> map, int& px, int& py);
void printMap(const vector<vector<int>>& map, int playerY, int playerX);

// g++ -std=c++17 -Ipocketmon *.cpp pocketmon/*.cpp -o main && ./main
int main() {


    //v1.map은 2찬원 배열 백터로 표현
    // 0 = 길, 1 = 벽, 2 = 포켓몬 등장하는 풀숲
    vector<vector<int>> map = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 2, 2, 2, 0, 1},
        {1, 0, 0, 2, 2, 2, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 2, 2, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}
    };

    //유저의 위치를 저장할 변수
    int user_x = 0;
    int user_y = 0;


    int py = 1, px = 1; // 플레이어 시작 좌표
    bool isMove = true;
    printMap(map, py, px);
    srand(time(0));
    User user;
    

    //게임 시작
    while(1) { //사용자에게 계속 입력 받기 위한 무한루프 
        
        //사용자의 입력을 저장할 변수
        string user_input = "";

        //최초 유저포켓몬이 하나도 없다면 포켓몬 하나 고르는 로직 실행
        if (user.getPocketmons().size() < 1) {
            cout << "지급 받을 포켓몬을 고르세요 1.이상해씨, 2.파이리, 3.꼬부기: ";
            cin >> user_input;
            if (user_input == "1") {
                Pocketmon p1 = Pocketmon(1);
                user.addPocketmon(p1);
                p1.printPocketInfo();
            } else if (user_input == "2") {
                Pocketmon p2 = Pocketmon(4);
                user.addPocketmon(p2);
                p2.printPocketInfo();
            } else {
                Pocketmon p3 = Pocketmon(7);
                user.addPocketmon(p3);
                p3.printPocketInfo();
            }
        }

        cout << "이동할 명렁어 숫자를 입력하세요 1.상,하,좌,우 2. 가방: " ;
        cin >> user_input;

        
        
        if (user_input == "상") {
            moveUp(map, px, py);
        } else if (user_input == "하") {
            moveDown(map, px, py);
        } else if (user_input == "좌") {
            moveLeft(map, px, py);
        } else if (user_input == "우") {
            moveRight(map, px, py);
        } else if (user_input == "2") {
            //1. 포켓몬 리스트
            cout << "보유한 포켓몬" << endl;
            user.printPocketmon();
        } else {
            cout << "잘못된 입력입니다.: " << endl;
        }
    
        
        // 이동 후 풀숲 판정 예시
        if (map[py][px] == GRASS) {
            if (rand() % 100 < 100) { // 30% 확률로 야생 포켓몬 등장
                cout << "야생의 포켓몬이 나타났다!\n";
                Pocketmon p;
                p.printPocketInfo();
            }
        }

    }
}



//위로 움직이는 함수
void moveUp(vector<vector<int>> map, int& px, int& py) {
    
    if (!isValidXY(px, py, map)) {
        cout << "맵을 벗어났습니다. 다시 돌아갑니다." << endl;
    }
    else {
        cout << "위로 한 칸 올라갑니다." << endl;
            // 위로 한 칸 올라가기
        py--;
        printMap(map, py, px);

    }
}

//아래로 움직이는 함수
void moveDown(vector<vector<int>> map, int& px, int& py) {

    if (!isValidXY(px, py, map)) {
        cout << "맵을 벗어났습니다. 다시 돌아갑니다." << endl;
    } 
    else {
        cout << "위로 한 칸 내려갑니다." << endl;
        py++;
        printMap(map, py, px);
    }
}

//왼쪽로 움직이는 함수
void moveLeft(vector<vector<int>> map, int& px, int& py) {
    // TODO: 왼쪽으로 이동하기


    if (!isValidXY(px, py, map)) {
        cout << "맵을 벗어났습니다. 다시 돌아갑니다." << endl;
    } 
    else {
        cout << "왼쪽으로 이동합니다." << endl;
        px--;
        printMap(map, py, px);

    }
}


//오른쪽로 움직이는 함수
void moveRight(vector<vector<int>> map, int& px, int& py) {
    // TODO: 오른쪽으로 이동하기

    if (!isValidXY(px, py, map)) {
        cout << "맵을 벗어났습니다. 다시 돌아갑니다." << endl;
    } 
    else {
        cout << "오른쪽으로 이동합니다." << endl;
        px++;
        printMap(map, py, px);

    }
}

// 이동하려는 좌표(px, py)가 맴 범위 안이고 벽이 아닌지 체크하는 함수
bool isValidXY(int px, int py, vector<vector<int>> map) {
	bool checkFlag = false;
    //유효하지 않은 맵 위치 이동시 trueq 반환
	if (py >= 0 && py < map.size() && px >= 0 && px < map[py].size()) {
		checkFlag = true;
	}

    // 이동하려는 위치가 벽이면 true 반환
    if(map[py][px] != 1) {
        checkFlag = true;
    }
    
	return checkFlag;
}



void printMap(const vector<vector<int>>& map, int py, int px) {
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (y == py && x == px) {
                cout << YELLOW << "@" << RESET << " ";        // 플레이어
            } else if (map[y][x] == 1) {
                cout << "# ";                                 // 벽/나무
            } else if (map[y][x] == 2) {
                cout << GREEN << "\"" << RESET << " ";        // 풀숲 (타일형은 GRASS_TILE + "\" " + RESET)
            } else {
                cout << GRAY << "." << RESET << " ";          // 일반 길
            }
        }
        cout << "\n";
    }
}