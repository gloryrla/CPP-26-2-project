#include <iostream>
#include "pocketmon.h"
#include "user.h"
using namespace std;

int main() {
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
                user.addPocketmon(Pocketmon(2));
            } else {
                user.addPocketmon(Pocketmon(3));        
            }

        cout << "실행한 명렁어 숫자를 입력하세요 (1. 싸운다, 2. 도망친다): " ;
        cin >> user_input;
        
        if (user_input == "1") {
            //싸운다 기능
        } else if (user_input == "2") {
            //도망치는 기능
        }
    }
}
}
