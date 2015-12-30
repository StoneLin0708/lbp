#include <iostream>
#include <fstream>
#include <iomanip>
//#include <conio.h>
#include <limits>
#include <string.h>
#define MAX_PATH 200
using namespace std;

#include <termios.h>
#include <stdio.h>

static struct termios oldt, newt;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &oldt); /* grab old terminal i/o settings */
  newt = oldt; /* make new settings same as old settings */
  newt.c_lflag &= ~ICANON; /* disable buffered i/o */
  newt.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &newt); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &oldt);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}

void help(void){
    cout << " help info" << endl
         << " listFileGenerator.exe file {g/c} " << endl;
}

bool generator(const char *path){
    ofstream fout;
    char filepath[MAX_PATH] ,name[MAX_PATH] ,in,fill = '0', parameters[50];
    int number,numMax;
    bool out = false;
    cout << path << endl;
    fout.open( path/*, ios::out*/);
    if(!fout){
        cout << " fail to open file : " << path <<endl;
        return false;
    }
    while(!out){
        cout << " [path]+[number]+[file name]+[ parameters]" << endl
             << " [path] : ";
        cin  >> filepath;
        cout << " [number max] : ";
        cin  >> numMax;
        cout << " [name] : ";
        cin  >> name;
        cout << " [parameters] : ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin .get( parameters, 50);
        cout << " output like this :" <<endl <<endl
             << " "<< filepath << "001" << name << " " << parameters <<endl
             << " "<< filepath << "002" << name << " " << parameters <<endl
             << " ..." << endl
             << " "<< filepath << setfill(fill) << setw(3) << numMax << name << " " << parameters <<endl << endl
             << " backspace to do again" << endl
             << " any key to generate" << endl
             << " esc to cancel and leave" << endl;
        in = getch();
        switch(in){
        case 8:
            out = false;
            break;
        case 27:
            fout.close();
            return false;
            break;
        default:
            out = true;
            break;
        }
    }
    for(number = 1; number <=numMax;number++){
        fout << filepath << setfill(fill) << setw(3) << number << name << " " << parameters <<endl;
    }
    cout << "done!" << endl;
    fout.close();
    return true;
}

bool check(const char *listPath){
    ifstream flist;
    ifstream ftest;
    char path[MAX_PATH];
    int success = 0,fail = 0,p0,p1,p2,p3,p4;
    flist.open( listPath, ios::in);
    if(!flist){
        cout << " fail to open file : " << path <<endl;
        return false;
    }
    while( !flist.eof() ){
        flist >> path >> p0 >> p1 >> p2 >> p3 >> p4;
        cout << " open file : " << path;
        ftest.open( path, ios::in);
        if(ftest){
            success++;
            cout << " ...success" << endl;
        }
        else{
            fail++;
            cout << " ...fail" << endl;
        }
        ftest.close();
    }
    cout << " success : " << success <<endl
         << " fail : " << fail <<endl;
    flist.close();
    return true;
}

int main(int argc,const char *argv[])
{
    char path[MAX_PATH];
    cout << "list file generator v1.0" << endl;
    if(argc != 3){
        help();
        return 0;
    }
    else{
        strcpy( path, argv[1] );
        cout << path << endl;
        if( !strcmp( argv[2], "g") ){
            generator( argv[1]/*path*//*"fafa.list"*/ );
        }
        else if( !strcmp( argv[2], "c") ){
            check( argv[1] );
        }
        else{
            help();
            return 0;
        }
    }

    return 0;
}
