/*** ---------------------- includes ----------------- ***/
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include<fcntl.h>
#include <stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iomanip>
#include<time.h>
#include <pwd.h>
#include <grp.h>
#include <bits/stdc++.h>
using namespace std;

#define CTRL_KEY(k) ((k) & 0x1f)
#define clr() printf("\033[H\033[J")  
#define clearLine() printf("\033[K")


void sig_handler(int sig);
void moveCursor(int x,int y);
void errorcheck(string a);
void listDir(string dirName);
void countnoofFiles(string dirName);
void die(string s) ;
void disableRawMode();
void enableRawMode() ;
char editorReadKey() ;
int getWindowSize(int &rows, int &cols) ;
void editorDrawRows() ;
void editorRefreshScreenAtExit() ;
void editorRefreshScreen() ;
void cursorup();
void cursordown();
void home();
void onelevelup();
void leftbutton();
void rightbutton();
void enter();                                                                                       
void editorProcessKeypress() ;
void create_file();
void create_dir();
void Searchrecur(string basePath, string &ans, string match);
void delete_file();
void delete_dir(string basePath);
void dele();
void copy_file(string file, string path);
void copy_dir_recur(string fromdir, string todir);
void copyall();
void moveall();
void Systemcalls(string query);
void initEditor(string dirname) ;
string giveInput();
bool checkPath();
void gotoCommandMode();
void Normalmode();




vector<dirent*> vt(1000);
vector<string> commandParts;
stack<string> leftst; 
stack<string> rightst;
vector<string> History;

int maxcontentrows = 18;
int maxwindow = maxcontentrows;
size_t cwdsize=2048;
char cwd[2048];

string fg="0";
int modebit = 0;
int commlinicurX = maxcontentrows + 15;
int commlinicurY = 17;
int commstart = 17;
int exestaX = maxcontentrows + 27;
int exestrt = 22;
int ihis=-1;  
string username="marmik";

struct editorParameters {
  int cx, cy;
  int maxcontent;
  int screenrows;
  int screencols;
  int topfileind;
  int bottomfileind;
  string rootdir;
  string cwdir;
};
struct editorParameters E;

struct termios orig_termios;

// **************************************** Body ***********************************************

void sig_handler(int sig)
{
    if(SIGWINCH==sig)
    {
        editorRefreshScreen();
        if(modebit==1)
        {
          moveCursor(commlinicurX,1);
          clearLine();
          cout<<" Commandline $ ";

          cout<<"\n\n Execution Status : ";
          moveCursor(commlinicurX,commstart);
          commlinicurY=commstart;
        }
    }

}

void moveCursor(int x,int y) {
  cout<<"\033["<<x<<";"<<y<<"H";
  fflush(stdout);
}

void errorcheck(string a)
{
  moveCursor(E.screenrows-2,1);
  cout<<a;
  moveCursor(E.cx,E.cy);
}

// ************************************************ List Dirs **********************************************

void listDir(string dirName)
{

       DIR* dir;

       struct dirent *dirEntry;
       struct stat inode;

       string mtime;
       char name[1000];

       dir = opendir(dirName.c_str());

        if (dir == 0) {

              perror ("Eroare deschidere fisier");

              exit(1);

        }
      int ct = 1;
      int i=E.topfileind;
      // int winheight = min(numofrows-i+1, maxwindow);
      int tpmaxwindow = maxwindow;
      while ((dirEntry=readdir(dir)) != 0 ) 
      {

        if(ct<E.topfileind)
        {
          ct++;
          continue;
        }

        if(!(--tpmaxwindow))
        {
          break;
        }
          sprintf(name,"%s/%s",dirName.c_str(),dirEntry->d_name); 
          lstat (name, &inode);
          cout<<left;

          cout<<setw(3)<<ct;
        ct++;
          long long filesi = inode.st_size;
          string si;

          if(filesi > 1024*1024*1024) filesi = filesi/(1024*1024*1024), si = to_string(filesi) + " GB";   
          else if(filesi > 1024*1024) filesi = filesi/(1024*1024), si = to_string(filesi) + " MB";
          else if(filesi > 1024) filesi = filesi/(1024), si = to_string(filesi) + " KB";
          else  si = to_string(filesi) + " B";   



          
          
          // cout<<setw(20)<<S_IRUSR;

          string permi="";
          int fgdir =0;
          if (S_ISDIR(inode.st_mode)) fgdir=1, permi += 'd';
        else if (S_ISREG(inode.st_mode))  permi += '-';
        // else if (S_ISLNK(inode.st_mode)) cout<<setw(10)<<"lnk";

        if ( inode.st_mode & S_IRUSR ) permi += 'r'; else permi += '-';  /* 3 bits for user  */
        if ( inode.st_mode & S_IWUSR ) permi += 'w'; else permi += '-';
        if ( inode.st_mode & S_IXUSR ) permi += 'x'; else permi += '-';
        permi += ' ';
        if ( inode.st_mode & S_IRGRP ) permi += 'r'; else permi += '-';  /* 3 bits for group */
        if ( inode.st_mode & S_IWGRP ) permi += 'w'; else permi += '-';
        if ( inode.st_mode & S_IXGRP ) permi += 'x'; else permi += '-';
        permi += ' ';
        if ( inode.st_mode & S_IROTH ) permi += 'r'; else permi += '-';    /* 3 bits for other */
        if ( inode.st_mode & S_IWOTH ) permi += 'w'; else permi += '-';
        if ( inode.st_mode & S_IXOTH ) permi += 'x'; else permi += '-';

        cout<<setw(15)<<permi;

        struct passwd *pw = getpwuid(inode.st_uid);
          struct group  *gr = getgrgid(inode.st_gid);

          cout<<setw(8)<<pw->pw_name;
          cout<<setw(8)<<gr->gr_name;

          cout<<setw(14)<<si;
          mtime = strtok(ctime(&inode.st_mtime), "\n");
       
          cout<<setw(27)<<mtime;
          string tpfilename = dirEntry->d_name;

          if(tpfilename.size()>10) tpfilename = tpfilename.substr(0,9)+"...";
          
          if(!fgdir)cout<<"\033[1;36m"<<tpfilename<<"\033[0m";
          else cout<<"\033[1;33m"<<tpfilename<<"\033[0m";
          if(E.screenrows > 0)cout<<"\n";

        }

        

        // moveCursor(11,1);

        cout<<"\n\n";
        cout<<"Current File Name :=>  "<<"\033[1;32m"<<vt[E.topfileind + E.cx-1]->d_name<<"\033[0m"<<"\n\n";
        // cout<<E.maxcontent<<" "<<E.cx<<" "<<E.topfileind<<"\n";
        cout<<left;
        cout<<setw(20)<<"Home Path "<<setw(10)<<":=>  "<<"\033[1;33m"<<E.rootdir<<"\033[0m"<<"\n";
        cout<<setw(20)<<"Current Path "<<setw(10)<<":=>  "<<"\033[1;34m"<<E.cwdir<<"\033[0m"<<"\n\n";
        
        string whichmod;
        if(modebit == 0) whichmod = "Normal Mode (Press ':' for Command Mode)";
        else whichmod = "Command Mode (Press 'Ese' for Normal Mode)";

        cout<<setw(20)<<"Mode "<<setw(10)<<":=>  "<<"\033[1;36m"<<whichmod<<"\033[0m"<<"\n\n";
        for(int i=0;i<E.screencols;i++) cout<<"\033[1;32m"<<"*"<<"\033[0m";
        // moveCursor(E.cx,E.cy);

        // moveCursor(max(14,(E.screenrows+11)/2),1);
        
  // moveCursor(E.cx,E.cy);


}



void countnoofFiles(string dirName)
{
        DIR* dir;

       struct dirent *dirEntry;
       struct stat inode;

       string mtime;
       char name[1000];

       dir = opendir(dirName.c_str());

        if (dir == 0) {

              perror ("Error Opening Dir");
              exit(1);

        }
      int ct = 0;
      int i=E.topfileind;
      // int winheight = min(numofrows-i+1, maxwindow);
      vt.clear();
      while ((dirEntry=readdir(dir)) != 0 ) 
      {
          ct++;
          sprintf(name,"%s/%s",dirName.c_str(),dirEntry->d_name); 
          lstat (name, &inode);
          vt[ct]=dirEntry;
          // vt.push_back(dirEntry);

      }
      E.maxcontent = ct;

}



// ************************************************* Normal Mode Buttons ********************************


void cursorup()
{
  if(E.cx>1)E.cx--;

  else if(E.cx == 1)
  {
    E.topfileind = max(1,E.topfileind - 1);
  }
  clr();
  moveCursor(E.cx,E.cy);
  
}

void cursordown()
{
 if(E.cx == maxwindow-1 && E.topfileind  < E.maxcontent-(maxwindow-2)) 
  {E.topfileind++;}
  
  // if(E.maxcontent - E.topfileind  <= maxwindow) E.topfileind--;
  if(E.cx < min(E.maxcontent-E.topfileind+1, maxwindow-1))E.cx++;
clr();
  moveCursor(E.cx,E.cy);
}

void home(){
  if(E.cwdir==E.rootdir){
    // printAlertLine("You're already present in the home directory");
    return;
  } 
  leftst.push(E.cwdir);
  initEditor(E.rootdir);
  return;
}

void onelevelup()
{
  if(E.rootdir == E.cwdir )
  {
    // print alert;
    return;
  }
  else
  {
    leftst.push(E.cwdir);
    initEditor(E.cwdir + "/..");
    return;
  }
  return;
}

void leftbutton()
{
  if(leftst.size()==0)return;
  
  string prevdir = leftst.top();
  leftst.pop();
  rightst.push(E.cwdir);
  initEditor(prevdir);
  return;
}

void rightbutton()
{
  if(rightst.size()==0)return;
  
  string prevdir = rightst.top();
  rightst.pop();
  leftst.push(E.cwdir);
  initEditor(prevdir);
  return;
}


void enter(){
  struct stat fileInfo;
  string filen = vt[E.topfileind + E.cx-1]->d_name;
  string fileName1 = E.cwdir + "/"+ filen;
  // string fileName = string(getcwd((char*)fileName1.c_str(),cwdsize));;
  string fileName = fileName1;
  lstat(filen.c_str(),&fileInfo);
  fg = fileName;

  if(S_ISDIR(fileInfo.st_mode))
  {
      string tp1 = vt[E.topfileind + E.cx-1]->d_name;
      if(E.rootdir == E.cwdir && tp1 == ".." ) {errorcheck(tp1); return;}
      else if(vt[E.topfileind + E.cx-1]->d_name == ".") return;
      leftst.push(E.cwdir);
      initEditor(fileName);
  }
  else
  {
      pid_t pid=fork();
      if(pid==0)
      {
          // printAlertLine("File opened in default editor                   ");
          execl("/usr/bin/xdg-open","xdg-open",filen.c_str(),NULL);
          exit(1);
      }
  }
  return;
}




// ************************************ System Call Functions ***********************************************

void create_file(){
 
  for(int i=1; i<commandParts.size()-1; i++){
    open((commandParts[commandParts.size()-1] + '/' + commandParts[i]).c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
  }
}

void create_dir(){

  for(int i=1; i<commandParts.size()-1; i++){
    mkdir((commandParts[commandParts.size()-1] + '/' + commandParts[i]).c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
  }
}

void Searchrecur(string basePath, string &ans, string match)
{
    string path;
    struct dirent *dp;
    DIR *dir = opendir(basePath.c_str());

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        string curdirname = dp->d_name;
        if (curdirname != "." && curdirname != "..")
        {
            
            // cout<<curdirname<<"\n";

            // Construct new path from our base path
            path = basePath + "/" + curdirname;
            if(curdirname == match) ans += path + " ";
            Searchrecur(path, ans, match);
        }
    }

    closedir(dir);
}


void delete_file()
{
  int sta1 = unlink(commandParts[1].c_str());
  if(sta1 == 0)
  {
      moveCursor(exestaX,exestrt);
      cout<<"\033[1;34m"<<"File is Successfully Deleted "<<"\033[0m";
  }
  else
  {
    moveCursor(exestaX,exestrt);
    cout<<"\033[1;32m"<<"Deletion is Unsuccessful"<<"\033[0m";
  }
}

void delete_dir(string basePath){

  struct dirent *dp;
  struct stat fileInfo;

  DIR *dir = opendir(basePath.c_str());

    // Unable to open directory stream
  if (!dir) return;
  
  while((dp = readdir(dir))){
    
    string curdirname = string(dp->d_name);
    string path = basePath + "/" + curdirname;
    lstat(path.c_str(),&fileInfo);
    
    
    if(S_ISDIR(fileInfo.st_mode))
    {
      
      if(curdirname != "." && curdirname != "..")
      {
          delete_dir(path);
          rmdir(path.c_str());
      }

    }
    else
    {
     int tp = unlink(path.c_str());
    }
  }
  closedir(dir);
}

void dele()
{
   if(commandParts[1]==cwd){
      moveCursor(exestaX,exestrt);
      cout<<"\033[1;32m"<<"Deletion is not Possible "<<"\033[0m";
              
     }
  else{
      moveCursor(30,4);
      string anr = commandParts[1];
      cout<<commandParts[1]<<"\n";
      delete_dir(anr);
      rmdir(anr.c_str());
      moveCursor(exestaX,exestrt);
      cout<<"\033[1;32m"<<"Directory is Successfully Deleted "<<"\033[0m";
  }
}


void copy_file(string file, string path)
{
  struct stat fileInfo;
  lstat(file.c_str(),&fileInfo);
  char b[2048];
  int fin = open(file.c_str(),O_RDONLY);
  int fout = open((path).c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
  int nread;
  while((  nread = read(fin,b,sizeof(b)))>0){
    write(fout,b,nread);
  }
  chmod(path.c_str(),fileInfo.st_mode);
}

void copy_dir_recur(string fromdir, string todir)
{
  struct dirent *dp;
  struct stat fileInfo;

  DIR *dir = opendir(fromdir.c_str());

    // Unable to open directory stream
  if (!dir) return;
  
  while((dp = readdir(dir))){
    
    string curdirname = string(dp->d_name);
    string path = fromdir + "/" + curdirname;
    lstat(path.c_str(),&fileInfo);
    
    
    if(S_ISDIR(fileInfo.st_mode))
    {
      
      if(curdirname != "." && curdirname != "..")
      {
        

          mkdir((todir + '/' + curdirname).c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
          chmod((todir + '/' + curdirname).c_str(),fileInfo.st_mode );
          copy_dir_recur(path , todir + '/' + curdirname);
      }

    }
    else
    {
     copy_file(path, todir+"/"+curdirname);
    }
  }
  closedir(dir);
}


void copyall()
{
  int si = commandParts.size();
  string destination = commandParts[si-1];
  struct stat fileInfo;
  for(int i=1; i<si-1; i++){
    string path = E.cwdir + '/' + commandParts[i];
    lstat(path.c_str(), &fileInfo);
    if(S_ISDIR(fileInfo.st_mode)){
      string fromdir = E.cwdir + "/" + commandParts[i];
      string todir = commandParts[commandParts.size()-1] + "/" + commandParts[i];

      mkdir(todir.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
      chmod(todir.c_str(),fileInfo.st_mode );
      copy_dir_recur(fromdir,todir);
    }
    else
    {
      int si = commandParts.size();
      string path = commandParts[si-1] +'/'+ commandParts[i];
      copy_file(commandParts[i],path);
    }
  }
}



void moveall(){
  int len = commandParts.size();

  struct stat fileInfo;

  for(int i=1; i<len-1; i++){
    string loc = E.cwdir + '/' + commandParts[i];
    lstat(loc.c_str(), &fileInfo);
    if(S_ISDIR(fileInfo.st_mode)){
        string path = commandParts[commandParts.size()-1] + "/"+ commandParts[i];
        mkdir(path.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
        chmod(path.c_str(),fileInfo.st_mode );
        string curpath = E.cwdir + '/' + commandParts[i];
        copy_dir_recur(curpath ,path);
        delete_dir(commandParts[i]);
        rmdir(commandParts[i].c_str()); 
    }
    else{
      copyall();
      string path = E.cwdir + "/" + commandParts[i];
      unlink(path.c_str());
    }
  }

}

// ********************************************* Command Mode Preprocess **********************************


void Systemcalls(string query){
  
  string commlist[] = {"copy", "move", "rename", "create_file", "create_dir", "delete_file", "delete_dir", "goto", "search"};
  int num=11;
  for(int i=0;i<9;i++)
  {
    if(commlist[i]==query)
    {
      num = i;
      break;
    }
  }

  switch(num)
  {
      case 0:
          
          clr();
          
          copyall();
          initEditor(commandParts[commandParts.size()-1]);
          moveCursor(1,1);
          listDir(E.cwdir);
          moveCursor(exestaX,exestrt);

          cout<<"\033[1;34m"<<"File/s is/are Successfully Copied to Destination "<<"\033[0m";
          
          break;
      case 1:
          clr();
          moveall();
          initEditor(commandParts[commandParts.size()-1]);
          moveCursor(1,1);
          listDir(E.cwdir);
          moveCursor(exestaX,exestrt);

          cout<<"\033[1;34m"<<"File/s is/are Successfully Moved to Destination "<<"\033[0m";

          break;
      case 2:
          clr();
          rename(commandParts[1].c_str(),commandParts[2].c_str());
          countnoofFiles(E.cwdir);
          moveCursor(1,1);
          listDir(E.cwdir);
          moveCursor(exestaX,exestrt);

          cout<<"\033[1;34m"<<"Successfully File Renamed"<<"\033[0m";
          break;


      case 3:             // create_file
          clr();
          
          create_file();
          initEditor(commandParts[commandParts.size()-1]);
          moveCursor(1,1);
          listDir(E.cwdir);
          moveCursor(exestaX,exestrt);

          cout<<"\033[1;34m"<<"New File/s is/are created Successfully "<<"\033[0m";
          break;


      case 4:           // Create Dir

          clr();
          
          create_dir();
          initEditor(commandParts[commandParts.size()-1]);
          moveCursor(1,1);
          listDir(E.cwdir);
          moveCursor(exestaX,exestrt);

          cout<<"\033[1;34m"<<"New Dir/s is/are created Successfully "<<"\033[0m";
          break;


      case 5:          // delete FIle
          delete_file();
          break;


      case 6:         // delete dir
          dele();
          break;

      case 7:                // goto
          clr();
          initEditor(commandParts[1]);

          moveCursor(1,1);
          listDir(E.cwdir);
          moveCursor(exestaX,exestrt);
          cout<<"\033[1;32m"<<"Successfully Reached to Desired Folder"<<"\033[0m";
          break;


      case 8:               // search
          
          string ans="";
          Searchrecur(E.cwdir,ans,commandParts[1]);
          moveCursor(exestaX,exestrt);
          if(ans=="")
          {
            cout<<"\033[1;31m"<<"False : File Not Found"<<"\033[0m";
          }
          else
          {
            cout<<"\033[1;32m"<<"True : File Found"<<"\033[0m";
          }

          break;

     
  }

  // else if(query == "search"){
  //   bool f = search();
  //   if(f) printStatusLine("File/Folder found!                               ");
  //   else printStatusLine("File/Folder not found!                            ");
  // } 

  // else{
  //   printStatusLine("Invalid Command!                                        ");
  // }

  return;
}

string giveInput(){
  char ch; 
  string command; 
  
  while(1)
  {
    ch = cin.get();
    
    if((int)ch==10)
    { 
      History.push_back(command); 
      ihis = History.size()-1;

      return command;
    }
    else if((int)ch == 45 || (int)ch == 43)
    {

        int Historylen =  History.size();
        if(History.size() == 0) continue;
        if((int)ch == 45) ihis = max(ihis-1,0);
        else ihis = min(ihis+1, Historylen-1);

        
        string tpcomm = History[ihis];
        moveCursor(commlinicurX,1);
        clearLine();

        cout<<" Commandline $ ";
        moveCursor(commlinicurX,commstart);
        commlinicurY = commstart+tpcomm.size();
        command = tpcomm;
        cout<<tpcomm;
        moveCursor(commlinicurX, commstart+tpcomm.size());

    }
    else if((int)ch==127)
    {
        if(commlinicurY==commstart) continue;
        
        moveCursor(commlinicurX,1);
        clearLine();
        ihis = History.size()-1;
        cout<<" Commandline $ ";
        moveCursor(commlinicurX,commstart);
        command.pop_back();
        --commlinicurY;
        
        cout<<command;
        moveCursor(commlinicurX, commstart+command.size());
    }
    else if((int)ch==27) return "gotoNormalmode";
    else
    {
        command.push_back(ch);
        ihis = History.size()-1;
        cout<<ch;
        moveCursor(commlinicurX, ++commlinicurY);
    }
    
  }
  return "kuchbhi";
}

bool checkPath(){
  string path = commandParts[commandParts.size()-1];
  if(path[0]=='/') path = path;
  else if(path=="..") path = E.cwdir + "/..";
  else if(path==".") path = E.cwdir;
  else if(path[0]=='.') path = E.cwdir + path.substr(1);
  else if(path[0]=='~') path.replace(0,1, E.rootdir);
  else path = E.cwdir + '/' + path ;
  commandParts[commandParts.size()-1] = path;
  return true;
}

void gotoCommandMode()
{
  countnoofFiles(E.cwdir);
  moveCursor(1,1);
  listDir(E.cwdir);
  commandParts.clear();

  while(1)
  {
      moveCursor(commlinicurX,1);
      clearLine();
      cout<<" Commandline $ ";

      cout<<"\n\n Execution Status : ";
      moveCursor(commlinicurX,commstart);
      commlinicurY=commstart;
      string inpcommand = giveInput();
      if(inpcommand == "gotoNormalmode") return;
      istringstream ss(inpcommand);
      string word; 
      while (ss >> word) 
      {
          commandParts.push_back(word);
      }


      bool chkp = false;

       string commlist[] = {"copy", "move", "rename", "create_file", "create_dir", "delete_file", "delete_dir", "goto", "search"};
      
        for(int i=0;i<9;i++)
        {
          if(commandParts[0]==commlist[i])
          {
            chkp = true;
            break;
          }
        }

       
      if(chkp && commandParts[0]!="rename" && commandParts[0]!="search") chkp = checkPath();  

      moveCursor(exestaX,1);
      clearLine();
      cout<<" Execution Status : ";

      if(!chkp)
      {
        moveCursor(exestaX,exestrt);
        cout<<"\033[1;31m"<<"Invalid Command"<<"\033[0m";
      }

      if(chkp) Systemcalls(commandParts[0]);


      commandParts.clear();

  }

}





// Error Handling
void die(string s) {
  perror(s.c_str());
  exit(1);
}

// Exit from Normal/Raw Mode
void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");

}

// Enter into Normal/Raw Mode
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(IXON);  // disable ctrl+s and ctrl+Q
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int getWindowSize(int &rows, int &cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    cols = ws.ws_col;
    rows = ws.ws_row;
    return 0;
  }
}


void editorRefreshScreenAtExit() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  write(STDOUT_FILENO, "\x1b[H", 3);
  clr();
}

void editorRefreshScreen() {
  
  clr();
  moveCursor(1,1);
  getWindowSize(E.screenrows, E.screencols);
  if(E.screenrows<=30) maxwindow = E.screenrows - maxcontentrows;  
  else maxwindow = maxcontentrows;
  
  listDir(E.cwdir);
  moveCursor(E.cx,E.cy);

}


void editorProcessKeypress() {
  moveCursor(E.cx,89);
  cout<<"\033[1;41m"<<" <-- "<<"\033[0m";
  moveCursor(E.cx,E.cy);
  char c = cin.get();

  switch(c){
        
        case 65:cursorup();break;

        case 66:cursordown();break;

        case 107:
        if(E.cx == 1)cursorup();
        break;
                
        case 108:
        if(E.cx == maxwindow-1) cursordown();
        break;

        case 10:enter();break;

        case 127:onelevelup();break;

        case 104:home();break;

        case 68:leftbutton();break;

        case 67:rightbutton();break;

        case ':': modebit=1;  gotoCommandMode(); modebit=0; countnoofFiles(E.cwdir); break;

        case 113: editorRefreshScreenAtExit(); exit(0);
        
        default:break;
      
    }


  
}

void Normalmode()
{
  char c;
  signal(SIGWINCH, sig_handler);
  while (1) {

    editorRefreshScreen();
    editorProcessKeypress();
    
  }
  return;
}


void initEditor(string dirname) {
  E.cx = 1;
  E.cy = 1;
  E.topfileind=1;
  moveCursor(E.cx,E.cy);
  // char * tp1 = tp;
  // E.cwdir = string(getcwd((char*)dirname.c_str(),cwdsize));
  chdir(dirname.c_str());
  getcwd(cwd,cwdsize);
  E.cwdir = cwd;
  countnoofFiles(E.cwdir);
  if (getWindowSize(E.screenrows, E.screencols) == -1) die("getWindowSize");
}

int main() {
  maxcontentrows++;
  maxwindow++;
  clr();
  enableRawMode();
  string startdir = ".";
  chdir(startdir.c_str());
  getcwd(cwd,cwdsize);
  E.rootdir = cwd;
  E.cwdir = E.rootdir;

  initEditor(E.rootdir);

  Normalmode();  
  
  
  disableRawMode();
  return 0;
}


