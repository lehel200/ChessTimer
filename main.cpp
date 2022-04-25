#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <functional>

int playTime, bonusSeconds;

void setOptions(){
    std::cout << "Jatekido felenkent percben megadva [szam]: ";
    std::cin >> playTime;
    std::cout << "Bonusz masodperc [szam]: ";
    std::cin >> bonusSeconds;
}

void printOptions(){
    std::cout << "\t\t\t\tJatekido: " << playTime << " perc, bonusz: " << bonusSeconds << " mp!\n";
    std::cout << "Valtas: SPACE\t\t\tKilepes: ESC\n";
}

void start(){
    std::cout << "Az ora inditashoz nyomd meg az entert!\n";
    std::cin.get();
    std::cin.get();
}

class ESC{};

class Finish{
    const std::string name;
public:
    Finish(const std::string& n) : name(n) {}

    std::string WhoIsTheLoser(){ return name;}
};

class Timer{
    int min , sec, ms;
public:
    Timer(int min_ = 0, int sec_ = 0, int ms_ = 0)
        : min(min_)
        , sec(sec_)
        , ms(ms_)
    {}

    void reduceMs() { ms--; }
    void reduceSec(){ sec--;}
    void reduceMin(){ min--;}

    int getMs()  const{ return ms; }
    int getSec() const{ return sec;}
    int getMin() const{ return min;}

    void addMs(int n = 1) { ms += n; }
    void addSec(int n = 1){ sec += n;}
    void addMin(int n = 1){ min += n;}

    void resetMs() { ms = 59; }
    void resetSec(){ sec = 59;}
    void resetAll(){ min = 0; sec = 0; ms = 0; }

    void print() const { std::cout << getMin() << ":" << getSec() << ":" << getMs() << std::endl; }

    void clock()
    {
        if (getMin() < 0){      //ha lejár az idő
            throw int{};
        }

        reduceMs();

        if(getMs() < 0){
            reduceSec();
            resetMs();
        }
        if (getSec() < 0)
        {
            reduceMin();
            resetSec();
        }
        Sleep(4);
    }

    void CheckClock(){

        while(true){
            if(getMs() > 59){
                addSec();
                ms -= 60;
            }
            if (getSec() > 59)
            {
                addMin();
                sec -= 60;
            } else break;
        }
    }

    bool WaitForExit()
    {
        if (kbhit())
        {
            char ch = getch();
            switch (ch)
            {
                case 32:               // press SPACE to switch
                    return false;
                case 27:
                    throw ESC{};       // press ESC to exit
            }
        }
        return true;
    }
};

class Player{
    const std::string name;
    const int bonusSec;
public:
    Timer timer;
public:

    Player(const std::string& n, int bonus = 5, Timer t = Timer(5,0,0))
        : name(n)
        , bonusSec(bonus)
        , timer(t)
        {}

    void print(const std::string text = "") const {
        std::cout << name << " " << text <<std::endl;
        timer.print();
    }

    void StartClock(Player& p){
        while (timer.WaitForExit())
        {
            printEveryThing(p);   
            try{
                timer.clock();
            }catch(int){
                throw Finish{name};
            }
        }
        timer.addSec(bonusSec);
        timer.CheckClock();
        printAllPlayers(p);

    }

private:
    void ScreenRefresh(){ system("cls"); }

    void printAllPlayers(Player& p){ 
        ScreenRefresh();
        print();
        p.print();
    }

    void printEveryThing(Player& p){
        printAllPlayers(p);
        std::cout << std::endl;
        printOptions();
    }

};

Player CreatePlayer(){
    std::string playerName;

    std::cout << "Add meg a neved [szoveg]: ";
    std::cin >> playerName;

    return Player{playerName,bonusSeconds,Timer(playTime)};
}

int main()
{
    setOptions();

    Player p1 = CreatePlayer();
    Player p2 = CreatePlayer();

    start();

    try{
        while(true){
            p1.StartClock(p2);
            p2.StartClock(p1);
        }
    }catch(Finish f){
        std::cout << f.WhoIsTheLoser() << " vesztett!\n";
    }catch(ESC e){}

    return 0;
}

