/*
 * Name - Sajal Gupta
 * UIN - 662790230
 * Project 01 - filter spam emails
*/
#include <iostream>
#include <fstream>
#include <string>

#include "ourvector.h"

using namespace std;


int getNumberOfEntries(string filename){                      //function to count number of lines in a loaded spam list
    ifstream inFS;
    string s,t;
    int count = 0;
    inFS.open(filename);
    if (!inFS.is_open()){
        cout<<"**Error, unable to open "<<filename<<endl;
        return 1;
    }
    inFS>>s;
    while(!inFS.eof()){
        getline(inFS, t);
        count++;
    }//end of while loop
    inFS.close();
    return count-1;
}//end of getNumberOfEntries

ourvector<string>  loadSpamLists(string filename){            //function to put the contents of a spam list into a vector
    ourvector<string> newSpamList;
    string middleList;
    ifstream inFS;
    inFS.open(filename);
    if (!inFS.is_open()){
        cout<<"**Error, unable to open '"<<filename<<"'"<<endl<<endl;
        return newSpamList;
    }
    inFS>>middleList;
    while (!inFS.eof()){
        newSpamList.push_back(middleList);
        inFS>>middleList;
    }//end of while loop
    inFS.close(); 
    cout << "Loading '"<<filename<<"'"<<endl;
    cout<<"# of spam entries: "<<getNumberOfEntries(filename)<<endl<<endl;
    return newSpamList;
 
    }//end of loadingSpamLists


void parseEmailAddress(string email, 
                       string& username, 
                       string& domain){                      // function to seperate the username and domain of a given email
    unsigned int i=0;
    int x=0;
    for ( i = 0; i < email.size(); i++){
        if (email[i]=='@'){
            x =i;
            break;
            
        }
    }
    username = email.substr(0,x);
    domain = email.substr(x+1, email.size());
}//end of parseEmailAdrress

bool binarySearch (string email, ourvector<string>& loadedList){
    int low = 0;
    int mid = 0;
    string str;
    int high = loadedList.size()-1;
    while (low<=high){
        mid = low +((high-low)/2);
        str = loadedList[mid];
        if (email == str){
            return true;
        }
        else if (email  < str){
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }//end of while loop
    return false;
}//end of binary search

void filterEmails(string outputFilename,                     //function to filter non spam emails from an email list
                  string filename2, 
                  string username, 
                  string domain, 
                  ourvector<string>& loadedList){
    string msgID, emailAddress, subject, newEmail, newEmail2;
    int nonSpamCount = 0;
    int emailCount = 0;
    
    ofstream outfile(outputFilename);
    if (!outfile.good()){
        cout<<"Unable to open the output file"<<endl;
        return;
    }
    ifstream inFS;
    inFS.open(filename2);
    if (!inFS.is_open()){
        cout<<"**Error, unable to open '"<<filename2<<"'"<<endl<<endl;
        return;
    }
    inFS>>msgID;
    inFS>>emailAddress;
    getline(inFS,subject);
    while (!inFS.eof()){
        parseEmailAddress(emailAddress,username,domain);
        newEmail = domain + ":" + username;
        newEmail2 = domain + ":*";
        if (binarySearch(newEmail, loadedList)||binarySearch(newEmail2, loadedList)){
            
        }
        else {
            nonSpamCount++;
            outfile<<msgID<<" ";
            outfile<<emailAddress;
            outfile<<subject;
            outfile<<endl;
        }
        inFS>>msgID;
        inFS>>emailAddress;
        getline(inFS, subject);
        emailCount++;
    }
    inFS.close();
    outfile.close();
    cout<<"# emails processed: "<<emailCount<<endl<<"# non-spam emails: "<<nonSpamCount<<endl<<endl;
}//end of filterEmails

void checkSpam(string email, 
               ourvector<string>& loadedList, 
               string &username, 
               string &domain){
    string newEmail, newEmail2;
    parseEmailAddress(email,username,domain);
    newEmail = domain + ":" + username;
    newEmail2 = domain + ":*";
    if (binarySearch(newEmail, loadedList)||binarySearch(newEmail2, loadedList)){
        cout<<email<<" is spam"<<endl<<endl;
    }
    else {
        cout<< email << " is not spam"<<endl<<endl;
    }
}
int main()                                                   
{
    string userInput, filename, email, username, domain, newEmail, newEmail2, filename2, outputFilename; 
    ourvector<string> V;
    cout <<"** Welcome to spam filtering app **"<<endl<<endl;
    while (true){
        cout<<"Enter command or # to exit> ";
        cin >> userInput;
        if (userInput == "#"){
            return 0;
        }
        if (userInput == "load"){
            cin>>filename;
            V.clear();
            V = loadSpamLists(filename);
            

        
    }//end of while loop(load)
    else if (userInput == "display"){
        for (string s:V){
            cout<<s<<endl;
        }
        cout<<endl;
    }
    
    else if (userInput == "check"){
        cin >> email;
        checkSpam(email, V, username, domain);


    }//end of else if statement (check)
    else if (userInput == "filter"){
        cin>>filename2;
        cin>>outputFilename;
        filterEmails(outputFilename, filename2, username, domain, V);
        
    }//end of else if statement(filter)
    
    
    else {
        cout<<"**invalid command"<<endl<<endl;
    }

    
    }//end of while statement
    
    return 0;
}