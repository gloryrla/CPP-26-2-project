#include <iostream>
#include <fstream>
#include <string>
#include "pocketmon.h"
using namespace std;

const string NAMES[151] = {
    "이상해씨","이상해풀","이상해꽃","파이리","리자드","리자몽","꼬부기","어니부기","거북왕","캐터피",
    "단데기","버터플","뿔충이","딱충이","독침붕","구구","피죤","피죤투","꼬렛","레트라",
    "깨비참","깨비드릴조","아보","아보크","피카츄","라이츄","모래두지","고지","니드런암컷","니드리나",
    "니드퀸","니드런수컷","니드리노","니드킹","삐삐","픽시","식스테일","나인테일","푸린","푸크린",
    "주뱃","골뱃","뚜벅쵸","냄새꼬","라플레시아","파라스","파라섹트","콘팡","도나리","디그다",
    "닥트리오","나옹","페르시온","고라파덕","골덕","망키","성원숭","가디","윈디","발챙이",
    "슈륙챙이","강챙이","캐이시","윤겔라","후딘","알통몬","근육몬","괴력몬","모다피","우츠동",
    "우츠보트","왕눈해","독파리","꼬마돌","데구리","딱구리","포니타","날쌩마","야돈","야도란",
    "코일","레어코일","파오리","두두","두트리오","쥬쥬","쥬레곤","질퍽이","질뻐기","셀러",
    "파르셀","고오스","고우스트","팬텀","롱스톤","슬리프","슬리퍼","크랩","킹크랩","찌리리공",
    "붐볼","아라리","나시","탕구리","텅구리","시라소몬","홍수몬","내루미","또가스","또도가스",
    "뿔카노","코뿌리","럭키","덩쿠리","캥카","쏘드라","시드라","콘치","왕콘치","별가사리",
    "아쿠스타","마임맨","스라크","루주라","에레브","마그마","쁘사이저","켄타로스","잉어킹","갸라도스",
    "라프라스","메타몽","이브이","샤미드","쥬피썬더","부스터","폴리곤","암나이트","암스타","투구",
    "투구푸스","프테라","잠만보","프리져","썬더","파이어","미뇽","신뇽","망나뇽","뮤츠",
    "뮤"
};

// 포켓몬 타입 종류 및 타입 인덱싱
enum Type { NORMAL, FIRE, WATER, GRASS, ELECTRIC, ICE, FIGHTING, POISON, GROUND, PSYCHIC, BUG, ROCK, GHOST, DRAGON };

// NAMES와  TYPES 같은 인덱싱 해서 포켓몬 별 타입 지정
const Type TYPES[151] = {
    GRASS,GRASS,GRASS,FIRE,FIRE,FIRE,WATER,WATER,WATER,BUG,
    BUG,BUG,BUG,BUG,BUG,NORMAL,NORMAL,NORMAL,NORMAL,NORMAL,
    NORMAL,NORMAL,POISON,POISON,ELECTRIC,ELECTRIC,GROUND,GROUND,POISON,POISON,
    POISON,POISON,POISON,POISON,NORMAL,NORMAL,FIRE,FIRE,NORMAL,NORMAL,
    POISON,POISON,GRASS,GRASS,GRASS,BUG,BUG,BUG,BUG,GROUND,
    GROUND,NORMAL,NORMAL,WATER,WATER,FIGHTING,FIGHTING,FIRE,FIRE,WATER,
    WATER,WATER,PSYCHIC,PSYCHIC,PSYCHIC,FIGHTING,FIGHTING,FIGHTING,GRASS,GRASS,
    GRASS,WATER,WATER,ROCK,ROCK,ROCK,FIRE,FIRE,WATER,WATER,
    ELECTRIC,ELECTRIC,NORMAL,NORMAL,NORMAL,WATER,WATER,POISON,POISON,WATER,
    WATER,GHOST,GHOST,GHOST,ROCK,PSYCHIC,PSYCHIC,WATER,WATER,ELECTRIC,
    ELECTRIC,GRASS,GRASS,GROUND,GROUND,FIGHTING,FIGHTING,NORMAL,POISON,POISON,
    GROUND,GROUND,NORMAL,GRASS,NORMAL,WATER,WATER,WATER,WATER,WATER,
    WATER,PSYCHIC,BUG,ICE,ELECTRIC,FIRE,BUG,NORMAL,WATER,WATER,
    WATER,NORMAL,NORMAL,WATER,ELECTRIC,FIRE,NORMAL,ROCK,ROCK,ROCK,
    ROCK,ROCK,NORMAL,ICE,ELECTRIC,FIRE,DRAGON,DRAGON,DRAGON,PSYCHIC,
    PSYCHIC
};

//타입별 포켓몬 스킬
const Skill TYPE_SKILLS[][2] = {
    {{"몸통박치기", 10}, {"할퀴기", 12}}, //노멀
    {{"불꽃세례", 15}, {"화염방사", 25}}, //불
    {{"물대포", 15}, {"파도타기", 25}},  //물
    {{"덩굴채찍", 15}, {"솔라빔", 25}},  //풀
    {{"전기쇼크", 15}, {"10만볼트", 25}},  //전기
    {{"얼음뭉치", 15}, {"냉동빔", 25}},  //얼음
    {{"태권당수", 15}, {"지옥의바퀴", 25}},  //격투
    {{"독침", 15}, {"오물폭탄", 25}},  //독
    {{"진흙뿌리기", 15}, {"지진", 25}}, //땅
    {{"염동력", 15}, {"사이코키네시스", 25}}, //에스퍼
    {{"벌레먹음", 15}, {"시저크로스", 25}}, //벌레
    {{"돌떨구기", 15}, {"스톤샤워", 25}}, //바위
    {{"핥기", 15}, {"섀도볼", 25}}, //고스트
    {{"용의분노", 15}, {"역린", 25}}, //드래곤
};

// 1. 기본 생성자: 랜덤 id를 뽑아서 아래 생성자를 호출
Pocketmon::Pocketmon() : Pocketmon(rand() % 151 + 1) {
}

// 2. id를 받는 생성자: 실제 초기화 로직을 한 곳에서만 관리
Pocketmon::Pocketmon(int id) {
    this->id = id;          // <-- 멤버 변수 id에 저장 (필수!)
    maxHp = 100 + level * 5;
    crrHp = maxHp;
    name = NAMES[id - 1];   // 배열은 0부터라서 -1
    Type type = TYPES[id - 1]; // 이 포켓몬의 타입
    skills.push_back(TYPE_SKILLS[type][0]);
    skills.push_back(TYPE_SKILLS[type][1]);
    level = 1;
    exp = 0;
    ifstream fin("../pocketmonImages/" + to_string(id) + "_" + name + ".txt");
    string line;
    while (getline(fin, line)) {
        art += line + "\n";
    }

}



void Pocketmon::printPocketInfo() {
    cout << name << endl;
    printHpBar();
    cout << art;
}

void Pocketmon::printHpBar() {
    int width = 20;
    if (crrHp < 0) crrHp = 0;
    int filled = crrHp * width / maxHp;

    // 50% 초과 초록, 20~50% 노랑, 20% 미만 빨강
    int pct = crrHp * 100 / maxHp;
    string color;
    if (pct > 50)
        color = "\033[32m";
    else if (pct >= 20)
        color = "\033[33m";
    else
        color = "\033[31m";

    cout << "HP [" << color;


    for (int i = 0; i < width; i++)
        cout << (i < filled ? "█" : "░");
        
    cout << "\033[0m] " << crrHp << "/" << maxHp << endl;
}


void Pocketmon::printSkills() {
    for(int i = 0; i < skills.size(); i++) {
        cout << i + 1 << ". " << skills[i].name << " 위력: " << skills[i].power << endl;
    }
}

vector<Skill> Pocketmon::getSkills() {
    return skills;
}

int Pocketmon::getHp() {
    return crrHp;
}

bool Pocketmon::DemagePocketmon(Skill skill, bool flag) {
    // 출력은 배틀 화면(drawBattle)이 담당, 여기선 HP만 깎는다
    crrHp -= skill.power + level;

    if (crrHp <= 0) {
        crrHp = 0;
        flag = false;
    }

    return flag;
}

string Pocketmon::getName() {
    return name;
};

//Hp가 적을 수록 잘잡힘
bool Pocketmon::tryCatch() {
    int chance = (maxHp - crrHp) * 100 / maxHp + 10;
    // return rand() % 100 < chance; 
    return true; //테스트용

}

//경험치를 얻고 레벨업 했는지 t/f 반환
bool Pocketmon::gainExp(int amount) {
    exp += amount;
    //레밸업 안했다면 
    if (exp < level * 10) return false;
    // 레밸업 했다면 
    exp -= level * 10; // 레밸 업 후 남은 경험치 추가
    level++; // 레밸업
    maxHp += 10; // 최대최력 추가 
    crrHp = maxHp;
    return true;
}

int Pocketmon::getLevel() {
    return level;
}

void Pocketmon::setLevel(int amount) {
    level = amount;
    // 랜덤 값이 0보다 작으면 1로 초기화
    if (amount < 0) {
        level = 1;
    }
}


