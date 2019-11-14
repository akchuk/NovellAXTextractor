#include <fstream>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include <vector>
#include <istream>
#include <direct.h>

using namespace std;

//Func defs
void readFile(string, vector<string>&);
void processAXT(vector<string>);
string stripPrefix(string);
string stripDWORD(string);
string stripBinary(string);
void processDEF(vector<string>);
string findPrefix(string);
string stripDrive(string);
string stripFileName(string);
string findFileName(string);

//Global Vars
string appname;
string workingdir;
string AXTpath;

//main
int main(int argc, char *argv[])
{
    printf("KDeploy v1.0\n");
    printf("------------\n");
    printf("Purpose: To extract Novell Snapshots without Novell.\n\n");
    
    if(argc<2)
    {
              printf("WRONG NUMBER OF ARGUMENTS!");
              getch();
              return 0;
    }
    
    AXTpath = argv[1];
    
    //File.Copy("MTG.reg","OMG.wtf");
    //USE if few args
    
    string temppath = argv[0];
    workingdir = stripFileName(temppath)+"\\SSRip\\";
    mkdir(workingdir.c_str());
    
    vector<string> AXTfile;
    vector<string> DEFfile;
    //vector<string> AOTfile;
    
    //readFiles
    //test
    
    //readFile(string FileName, vector<string> array);
    //processAXT(...);
    readFile(AXTpath,AXTfile);
    processAXT(AXTfile);
    
    readFile(stripFileName(AXTpath)+"\\filedef.txt",DEFfile);
    processDEF(DEFfile);
    
    /*for(int i=0; i<AXTfile.size(); i++)
    {
            printf("NewLine: ");
            printf((AXTfile.at(i)).c_str());
            printf("\n");
    }*/
    printf("Finished!\n");
    getch();
    return 0;
}

//Process filedef.txt (Copy files, make dirs)
void processDEF(vector<string> DEFfile)
{
     mkdir((workingdir+appname).c_str());
     string lastdir;
     printf("Making folders...\n");
     for(int i=0; i<DEFfile.size(); i++)
     {
             string directory = stripFileName(stripDrive(stripPrefix(DEFfile.at(i))));
             
             if(lastdir!=directory)
                 system(("mkdir \""+(workingdir+appname+"\\"+directory)+"\"").c_str());
             
             lastdir = directory;
             //printf((workingdir+appname+"\\"+directory).c_str());
             //printf("\n");
             //need stip file name
     }
     printf("Copying files...\n");
     for(int i=0; i<DEFfile.size(); i++)
     {
             
             string directory = stripFileName(stripDrive(stripPrefix(DEFfile.at(i))));
             string findir = workingdir+appname+"\\"+directory;
             string orgname = stripFileName(AXTpath)+"\\"+findPrefix(DEFfile.at(i));
             string finname = findir+"\\"+findFileName(DEFfile.at(i));
             
             if(DEFfile.at(i).length()>2)
             {
             /*printf(orgname.c_str());
             printf("\n");*/
             printf(finname.c_str());
             printf("\n");
             system(("copy \""+orgname+"\" \""+finname+"\"").c_str());
             }
     }
}

void processAXT(vector<string> AXTfile)
{
    //TODO - CHECK IF SIZE ZERO
    
    
    
    /*printf("Type: ");
    printf(header.c_str());
    printf("\n");
    
    printf("Size: %i\n",AXTfile.size());*/
    /*string appname;
    for(int i=0; i<AXTfile.size(); i++)
    {
            if(AXTfile.at(i)=="[Application Name]")
                appname = AXTfile.at(i+1);
    }
    printf(appname.c_str());
    printf("\n");
    printf((stripPrefix(appname)).c_str());*/
    
    ofstream regfile((workingdir+"\\RegistryData.reg").c_str());
    if(!regfile.is_open())
        return;
    regfile<<"Windows Registry Editor Version 5.00"<<endl<<endl;
    
    string rvc_Type;
    string rvc_Flag;
    string rvc_Key;
    string rvc_Name;
    string rvc_Length;
    
    string rvc_Value;
    
    for(int i=0; i<AXTfile.size(); i++)
    {
            if(AXTfile.at(i)=="[Application Name]")
            {
                appname = stripPrefix(AXTfile.at(i+1));
            }
            else if(AXTfile.at(i)=="[Registry Value Create]")
            {
                int p=1;
                rvc_Type = stripPrefix(AXTfile.at(i+p));
                p++;
                rvc_Flag = stripPrefix(AXTfile.at(i+p));
                p++;
                rvc_Key = stripPrefix(AXTfile.at(i+p));
                p++;
                rvc_Name = stripPrefix(AXTfile.at(i+p));
                p++;
                if(rvc_Type=="Binary")
                {
                    rvc_Length = stripPrefix(AXTfile.at(i+p));
                    p++;
                }
                rvc_Value = stripPrefix(AXTfile.at(i+p));
                
                regfile<<"["<<rvc_Key.c_str()<<"]"<<endl;
                if(rvc_Type=="String")
                    regfile<<"\""<<rvc_Name.c_str()<<"\"=\""<<rvc_Value.c_str()<<"\""<<endl;
                else if(rvc_Type=="Default")
                    regfile<<"@=\""<<rvc_Value.c_str()<<"\""<<endl;
                else if(rvc_Type=="DWORD")
                    regfile<<"\""<<rvc_Name.c_str()<<"\"=dword:"<<stripDWORD(rvc_Value).c_str()<<endl;
                else if(rvc_Type=="Binary")
                {
                     regfile<<"\""<<rvc_Name.c_str()<<"\"=hex:"<<stripBinary(rvc_Value).c_str()<<"\\"<<endl;
                     int tempp = p+1;
                     while(AXTfile.at(i+tempp).length()>1)
                     {
                         regfile<<"  "<<stripBinary(stripPrefix(AXTfile.at(i+tempp)))<<"\\"<<endl;
                         tempp++;
                     }
                }
            }
    }
    
    regfile.close();
    
    //return;
    
    
    
    /*ofstream regfile("MTG.reg");
    
    if (regfile.is_open())
    {
        //Header
        regfile<<"Windows Registry Editor Version 5.00"<<endl;
        
        //for(int i=0; i<AXTfile.size(); i++)
        //        regfile<<AXTfile.at(i)<<endl;
        regfile.close();
    }*/
    
}

string stripPrefix(string fullstring)
{
       if(fullstring.length()<2)
           return fullstring;
       
       int pos = 0;
       while(pos<fullstring.length() && fullstring[pos] != '=')
           pos++;
       
       return fullstring.substr(pos+1,fullstring.length()-(pos+1));
}

string findPrefix(string fullstring)
{
       if(fullstring.length()<2)
           return fullstring;
       
       int pos = 0;
       while(pos<fullstring.length() && fullstring[pos] != '=')
           pos++;
       
       return fullstring.substr(0,pos);
}

string findFileName(string fullstring)
{
       if(fullstring.length()<2)
           return fullstring;
       
       int pos = 0;
       int backslash;
       while(pos<fullstring.length())
       {
           if(fullstring[pos] == '\\')
               backslash = pos;
           pos++;
       }
       
       return fullstring.substr(backslash+1,fullstring.length()-(backslash+1));
}

string stripFileName(string fullstring)
{
       if(fullstring.length()<2)
           return fullstring;
       
       int pos = 0;
       int backslash;
       while(pos<fullstring.length())
       {
           if(fullstring[pos] == '\\')
               backslash = pos;
           pos++;
       }
       
       return fullstring.substr(0,backslash);
}

string stripDrive(string fullstring)
{
       if(fullstring.length()<2)
           return fullstring;
       
       int pos = 0;
       while(pos<fullstring.length()-1 && fullstring.substr(pos,2) != ":\\")
           pos++;
       
       return fullstring.substr(pos+2,fullstring.length()-(pos+1));
}

string stripDWORD(string fullstring)
{
       if(fullstring.length()!=10)
           return fullstring;
       
       return fullstring.substr(2,8);
}

string stripBinary(string fullstring)
{
       if(fullstring.length()<2)
           return fullstring;
       
       string newstring;
       
       for(int i=0; i<fullstring.length(); i++)
       {
           if(fullstring.substr(i,1)==" ")
               newstring+=",";
           else
               newstring+=fullstring.substr(i,1);
       }
       
       return newstring;
}

void readFile(string filename, vector<string>& file)
{
    char buffer;
    string input;
    ifstream filestream(filename.c_str());
    
    if(!filestream.is_open())
    {
        printf("Error opening file.");
        getch();
        exit(1);
    }
    
    while(!filestream.eof())
    {
        getline(filestream,input);
        file.push_back(input);
        /*printf(input.c_str());
        printf("\n");*/
    }
    
    filestream.close();
}
/*
void readFiles()
{
    //string regEntries[
    fstream file_op("c:\\test_file.txt",ios::in);
    while(!file_op.eof()) 
    {
          //file_reader.getline(str,2000);
          //cout <<str;
    }
    file_op.close();
}
*/
