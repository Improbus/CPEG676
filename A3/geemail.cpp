#include <iostream>
#include <stdio.h>
#include "stdlib.h"
#include <unistd.h>
#include <time.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hmac.h>
#include <cryptopp/secblock.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/iterhash.h>
#include <cryptopp/eax.h>
#include <cryptopp/gcm.h>
#include <string>
#include <sstream>
#include <fstream>

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
  ofstream myfile;
  myfile.open ("creds.txt");
  myfile << "daniel" << "," << salt << "," << hash;
  myfile.close();
};

void validateCredentials(string salt, string password){
  CryptoPP::SHA256 sha256;
  string source = salt + password;  //This will be randomly generated somehow
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

string encryptMessage(){
  // KDF parameters
      string password = "Super secret password";
      unsigned int iterations = 15000;
      char purpose = 0; // unused by Crypto++

      // 32 bytes of derived material. Used to key the cipher.
      //   16 bytes are for the key, and 16 bytes are for the iv.
      CryptoPP::SecByteBlock derived(32);

      // KDF function
      CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> kdf;
      kdf.DeriveKey(derived.data(), derived.size(), purpose, (byte*)password.data(), password.size(), NULL, 0, iterations);

      // Encrypt a secret message
      string plaintext = "Attack at dawn", ciphertext, recovered;

      // Key the cipher
      CryptoPP::EAX<CryptoPP::AES>::Encryption encryptor;
      encryptor.SetKeyWithIV(derived.data(), 16, derived.data() + 16, 16);

      CryptoPP::StringSource(plaintext, true, new CryptoPP::AuthenticatedEncryptionFilter(encryptor, new CryptoPP::HexEncoder(new CryptoPP::StringSink(ciphertext))));

    //  CryptoPP::AuthenticatedEncryptionFilter ef(encryptor, new CryptoPP::StringSink(ciphertext));
    //      ef.Put((byte*)plaintext.data(), plaintext.size());
  //        ef.MessageEnd();

    //   Key the cipher
  //    CryptoPP::EAX<CryptoPP::AES>::Decryption decryptor;
  //      decryptor.SetKeyWithIV(derived.data(), 16, derived.data() + 16, 16);

  ////      CryptoPP::AuthenticatedDecryptionFilter df(decryptor, new CryptoPP::StringSink(recovered));
  //      df.Put((byte*)ciphertext.data(), ciphertext.size());
  //        df.MessageEnd();

      // Done with encryption and decryption

      // Encode various parameters
      //CryptoPP::HexEncoder encoder;
      //string key, iv, cipher;

    //  encoder.Detach(new CryptoPP::StringSink(key));
    //  encoder.Put(derived.data(), 16);
    //  encoder.MessageEnd();

      //encoder.Detach(new CryptoPP::StringSink(iv));
      //encoder.Put(derived.data() + 16, 16);
      //encoder.MessageEnd();

      //encoder.Detach(new CryptoPP::StringSink(cipher));
      //encoder.Put((byte*)ciphertext.data(), ciphertext.size());
      //encoder.MessageEnd();

      // Print stuff
      //cout << "plaintext: " << plaintext << endl;
      //cout << "key: " << key << endl;
    //  cout << "iv: " << iv << endl;
  //    cout << "ciphertext: " << cipher << endl;
//      cout << "recovered: " << recovered << endl;


};

void decryptMessage(string ciphertext){
  // KDF parameters
      string password = "Super secret password";
      unsigned int iterations = 15000;
      char purpose = 0; // unused by Crypto++

      // 32 bytes of derived material. Used to key the cipher.
      //   16 bytes are for the key, and 16 bytes are for the iv.
      CryptoPP::SecByteBlock derived(32);

      // KDF function
      CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> kdf;
      kdf.DeriveKey(derived.data(), derived.size(), purpose, (byte*)password.data(), password.size(), NULL, 0, iterations);

      // Encrypt a secret message
      string recovered;
      //cout << "enter ciphertext for checking " << endl;
      //cin >> ciphertext;
      //cout << endl;

      // Key the cipher
      CryptoPP::EAX<CryptoPP::AES>::Decryption decryptor;
      decryptor.SetKeyWithIV(derived.data(), 16, derived.data() + 16, 16);


      CryptoPP::StringSource( ciphertext, true, new CryptoPP::AuthenticatedDecryptionFilter(decryptor, new CryptoPP::HexEncoder(new CryptoPP::StringSink(recovered))));

    //  CryptoPP::AuthenticatedDecryptionFilter df(decryptor, new CryptoPP::StringSink(recovered));
  //    df.Put((byte*)ciphertext.data(), ciphertext.size());
  //    df.MessageEnd();

      // Encode various parameters
//      CryptoPP::HexEncoder encoder;
//      string key, iv, cipher;

//      encoder.Detach(new CryptoPP::StringSink(key));
//      encoder.Put(derived.data(), 16);
//      encoder.MessageEnd();

//      encoder.Detach(new CryptoPP::StringSink(iv));
//      encoder.Put(derived.data() + 16, 16);
//      encoder.MessageEnd();

      //encoder.Detach(new CryptoPP::StringSink(cipher));
    //  encoder.Put((byte*)ciphertext.data(), ciphertext.size());
  //    encoder.MessageEnd();

      // Print stuff
    //  cout << "plaintext: " << plaintext << endl;
    //  cout << "key: " << key << endl;
    //  cout << "iv: " << iv << endl;
    //  cout << "ciphertext: " << cipher << endl;
  //    cout << "recovered: " << recovered << endl;

};

void receiveMessages(){

};

int main(){

  decryptMessage(encryptMessage());

  string userPass;
  cout << "ENTER A PASSWORD!!!!!!!" << endl;
  cin >> userPass;
  cout << endl;

  hashSaltPass(userPass);

  cout << endl;
  cout << endl;
  cout << endl;
  cout << endl;

  string salttotest;
  string passtotest;

  cout << "ENTER A SALT" << endl;
  cin >> salttotest;
  cout << "ENTER A PASSWORD!!!!!!!" << endl;
  cin >> passtotest;
  cout << endl;

  validateCredentials(salttotest, passtotest);


  return 0;
};
