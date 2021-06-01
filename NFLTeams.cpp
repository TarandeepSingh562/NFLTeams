#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

class NFL_Team
{
private:
    string name;
    string conference;
    string division;
    unsigned short wins;
    unsigned short losses;
    unsigned short ties;
public:
    void setName(string);
    void setConference(string);
    void setDivision(string);
    void setWins(unsigned short);
    void setLosses(unsigned short);
    void setTies(unsigned short);
    string getName() const;
    string getConference() const;
    string getDivision() const;
    unsigned short getWins() const;
    unsigned short getLosses() const;
    unsigned short getTies() const;
    
};

class NFL_teams {
public:
    NFL_Team* data;
   NFL_teams(int amount) {
       _size = amount;
       data = new NFL_Team[amount];
      }
   ~NFL_teams() { delete[] data;}
    int Size() { return _size;}
private:
  int _size;
};

void NFL_Team::setName(string n)
{
    name = n;
}
void NFL_Team::setConference(string c)
{
    conference = c;
}

void NFL_Team::setDivision(string d)
{
    division = d;
}

void NFL_Team::setWins(unsigned short w)
{
    wins = w;
}

void NFL_Team::setLosses(unsigned short l)
{
    losses = l;
}

void NFL_Team::setTies(unsigned short t)
{
    ties = t;
}

string NFL_Team::getName() const
{
    return name;
}

string NFL_Team::getConference() const
{
    return conference;
}

string NFL_Team::getDivision() const
{
    return division;
}

unsigned short NFL_Team::getWins() const
{
    return wins;
}

unsigned short NFL_Team::getLosses() const
{
    return losses;
}

unsigned short NFL_Team::getTies() const
{
    return ties;
}


const int NumTeams = 32;

void getNFL_Teams(NFL_Team teams[], string filename);
void processGames(NFL_Team teams[], string filename);
void swap(NFL_Team& a, NFL_Team& b);
void sortForStandings(NFL_Team teams[]);
void printStandings(NFL_Team teams[]);

int main(){
    string ScoresFile = "scores.txt";
    string TeamsFile = "teams.txt";
    int pct;
    NFL_Team teams[NumTeams];
    getNFL_Teams(teams, TeamsFile);
    processGames(teams, ScoresFile);
    sortForStandings(teams);
    printStandings(teams);

    return 0;
}

void getNFL_Teams(NFL_Team teams[], string filename){
string line, conference_flag, division_flag;
int i = 0;
ifstream infile("teams.txt");
    if(!infile){
        cout << "Error in opening teams file";
        exit(1);
    }
    if(infile.is_open()){
    while(getline(infile,line)){
        if(line.empty() || line == " "){
            continue;
        }
        else if(line == "American Football Conference" || line == "National Football Conference"){
            conference_flag = line;
            continue;
        }
        else if(line == "AFC East" || line == "NFC East"){
            division_flag = "East Division";
            continue;
        }
        else if(line == "AFC North" || line == "NFC North"){
            division_flag = "North division";
            continue;
        }
        else if(line == "AFC South" || line == "NFC South"){
            division_flag = "South Division";
            continue;
        }
        else if(line == "AFC West" || line == "NFC West"){
            division_flag = "West Division";
            continue;
        }
        teams[i].setConference(conference_flag);
        teams[i].setDivision(division_flag);
        teams[i].setName(line);
        teams[i].setWins(0);
        teams[i].setLosses(0);
        teams[i].setTies(0);  
        i++;
        }
    }
infile.close();
}

void processGames(NFL_Team teams[], string filename){
    string bufferstring;
    string *winteam = new string[105];
    string *lostteam = new string[105];
    int *ptsw = new int[105];
    int *ptsl = new int[105];
    int i = 0;
    ifstream infile1("scores.txt");
    if(!infile1){
        cout << "Error in opening scores file";
        exit(1);
    }
    if(infile1.is_open()){
        while(getline(infile1, bufferstring)){
            if (bufferstring.find("Date") != string::npos){
                continue;
            }
            int pos = bufferstring.find(' ');
            bufferstring.erase(0,pos+1);
            if (bufferstring[0] == ' ') bufferstring.erase(0,1);
            pos = bufferstring.find("  ");
            winteam[i].append(bufferstring, 0, pos);
            bufferstring.erase(0,pos);
            while(bufferstring[0] < 'A' || bufferstring[0] > 'z'){
            bufferstring.erase(0,1);
            }
            pos = bufferstring.find("  ");
            lostteam[i].append(bufferstring,0, pos);
            pos = bufferstring.find("box");
            bufferstring.erase(0, pos+1);
            pos = bufferstring.find("  ");
            bufferstring.erase(0, pos+3);
            ptsw[i]=stoi(bufferstring);
            pos = bufferstring.find("  ");
            bufferstring.erase(0,pos+1);
            ptsl[i]=stoi(bufferstring);
            ++i;
        }
        for(int k = 0; k < 105; k++){
            for(int i = 0; i < NumTeams; i++){
                if(teams[i].getName() == winteam[k]){
                    teams[i].setWins(teams[i].getWins() + 1);
                }
                if(teams[i].getName() == lostteam[k]){
                    teams[i].setLosses(teams[i].getLosses() + 1);
                }
                if(teams[i].getName() == winteam[k] || teams[i].getName() == lostteam[k]){
                    if(ptsw[k] == ptsl[k]){
                        teams[i].setTies(teams[i].getTies() + 1);
                            if(teams[i].getName() == winteam[k]){
                                teams[i].setWins(teams[i].getWins() - 1);
                            }
                            else if(teams[i].getName() == lostteam[k]){
                                teams[i].setLosses(teams[i].getLosses() - 1);
                            }
                    }
                }
            }
        }
    }
    infile1.close();
    delete[] winteam;
    delete[] lostteam;
    delete[] ptsw;
    delete[] ptsl;
}


void swap(NFL_Team& a, NFL_Team& b){
  NFL_Team t = a;
  a = b;
  b = t;
}

void sortForStandings(NFL_Team teams[]){
  for(int i=0; i<8;i++){
    for(int j =4*i; j < 4*(i+1); j++){
        for(int k = j+1; k < 4*(i+1); k++){
            double x = (teams[j].getWins()+0.5*teams[j].getTies())/(teams[j].getWins()+teams[j].getLosses()+teams[j].getTies());
            double y = (teams[k].getWins()+0.5*teams[k].getTies())/(teams[k].getWins()+teams[k].getLosses()+teams[k].getTies());
            if(x < y){
                swap(teams[k], teams[j]);
            }
        }
    }
  }
}

void printStandings(NFL_Team teams[]){
    double pct;
    for(int i=0; i < NumTeams; i++){
    if(i % 16 == 0){
        cout << "\n" << teams[i].getConference() << "\n";
        }
    if(i % 4 ==0){
        cout << "\n" << left << setw(26) << teams[i].getDivision() << "W" << right << setw(6) << "L" << right << setw(6) << "T" 
         << right << setw(8) << "Pct" << "\n";
        }
    pct = ((teams[i].getWins() + 0.5 * teams[i].getTies()) / (teams[i].getWins() + teams[i].getLosses() + teams[i].getTies()));
    cout << left << setw(26) << teams[i].getName()  <<  right << setw(1) <<  teams[i].getWins() << right << setw(6) << teams[i].getLosses() << right << setw(6) << teams[i].getTies() 
    << right << setw(8) << fixed << setprecision(3) << pct << "\n";
    }
}
