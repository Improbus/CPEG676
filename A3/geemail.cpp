#include <iostream>
#include <stdio.h>
#include "stdlib.h"
#include <unistd.h>
#include <time.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <string>
#include <sstream>

using namespace std;


void makeUser(){
  string userName;
  string password;

  return;
};

void login(){
  string userName;
  string password;

  return;
};

void hashSaltPass(string userPass){
  setvbuf(stdout, NULL, _IONBF, 0);

  unsigned int randval;
  FILE *f;

  f = fopen("/dev/urandom", "r");
  fread(&randval, sizeof(randval), 1, f);
  fclose(f);

  //std::cout << "RANDVAL TO SALT: " << randval << '\n';

  fflush(stdout);
  usleep(1);

  stringstream ss;
  ss << randval;
  string str = ss.str();

  std::string salt = str;
  CryptoPP::SHA256 sha256;
  string source = salt + userPass;  //This will be randomly generated somehow
  string hash = "";
  for(int i=0; i<20; i++){
    CryptoPP::StringSource(source, true, new CryptoPP::HashFilter(sha256, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
  }
  //cout <<"SALT INT: " << str << endl;
  //cout << "SALT STRING: " << salt << endl;
  cout << hash << endl;

};

void sendMessage(){

};

void encryptMessage(){

};

void decryptMessage(){

};

void receiveMessages(){

};

int main(){
  string userPass;
  cout << "ENTER A PASSWORD!!!!!!!" << endl;
  cin >> userPass;
  cout << endl;

  hashSaltPass(userPass);
  return 0;
};
