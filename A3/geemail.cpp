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
#include <sqlite3.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    return 0;
};

void login(){
  string userName;
  string password;

  return;
};

void hashSaltPass(string userName, string userPass){

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
  //cout << hash << endl;
  sqlite3* db;
  char *zErrMsg = 0;
  int rc;

   rc = sqlite3_open("geemail.db", &db);

   if( rc ){
       fprintf(stderr, "Can't open database: %s", sqlite3_errmsg(db));
       sqlite3_close(db);
       return;
   };
   string sqlInsert = "insert into Users ('UserName','Password','Salt') values ('" +userName+ "','"+hash+"','"+salt+"');";
   const char * sql = sqlInsert.c_str();
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s", zErrMsg);
     sqlite3_free(zErrMsg);
   }else{
     fprintf(stdout, "User Added Please Restart Progam");
   }
   sqlite3_close(db);
};

string validateCredentials(string salt, string password){
  CryptoPP::SHA256 sha256;
  string source = salt + password;  //This will be randomly generated somehow
  string hash = "";
  for(int i=0; i<20; i++){
    CryptoPP::StringSource(source, true, new CryptoPP::HashFilter(sha256, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
  }
  //cout <<"SALT INT: " << str << endl;
  //cout << "SALT STRING: " << salt << endl;
  return hash;
};

string encryptMessage(string password, string plaintext){

      unsigned int iterations = 15000;
      char purpose = 0; // unused by Crypto++

      CryptoPP::SecByteBlock derived(32);

      CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> kdf;
      kdf.DeriveKey(derived.data(), derived.size(), purpose, (byte*)password.data(), password.size(), NULL, 0, iterations);

      string ciphertext;

      CryptoPP::EAX<CryptoPP::AES>::Encryption encryptor;
      encryptor.SetKeyWithIV(derived.data(), 16, derived.data() + 16, 16);

      CryptoPP::AuthenticatedEncryptionFilter ef(encryptor, new CryptoPP::StringSink(ciphertext));
      ef.Put((byte*)plaintext.data(), plaintext.size());
      ef.MessageEnd();

      // Encode various parameters
      CryptoPP::HexEncoder encoder;
      string key, iv, cipher;

      encoder.Detach(new CryptoPP::StringSink(key));
      encoder.Put(derived.data(), 16);
      encoder.MessageEnd();

      encoder.Detach(new CryptoPP::StringSink(iv));
      encoder.Put(derived.data() + 16, 16);
      encoder.MessageEnd();

      encoder.Detach(new CryptoPP::StringSink(cipher));
      encoder.Put((byte*)ciphertext.data(), ciphertext.size());
      encoder.MessageEnd();

      //cout << "plaintext: " << plaintext << endl;
      //cout << "key: " << key << endl;
      //cout << "iv: " << iv << endl;
      //cout << "ciphertext: " << cipher << endl;

      return cipher;
};

string decryptMessage(string password, string ciphertext){

      unsigned int iterations = 15000;
      char purpose = 0; // unused by Crypto++

      CryptoPP::SecByteBlock derived(32);

      CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> kdf;
      kdf.DeriveKey(derived.data(), derived.size(), purpose, (byte*)password.data(), password.size(), NULL, 0, iterations);

      string recovered;

      cout << "Cipher text: " << ciphertext << endl;

      string cipher_raw;

      CryptoPP::HexDecoder decoder;
      decoder.Put((byte*)ciphertext.data(), ciphertext.size());
      decoder.MessageEnd();

      long long size = decoder.MaxRetrievable();
      cipher_raw.resize(size);
      decoder.Get((byte*)cipher_raw.data(), cipher_raw.size());

      string decrypted_text;

      CryptoPP::EAX<CryptoPP::AES>::Decryption decryptor;
      decryptor.SetKeyWithIV(derived.data(), 16, derived.data() + 16, 16);

      CryptoPP::StringSource ss(cipher_raw, true, new CryptoPP::AuthenticatedDecryptionFilter(decryptor, new CryptoPP::StringSink(decrypted_text)));

      //cout << "Decrypted text: " << decrypted_text << endl;

      return decrypted_text;

};

void sendMessage(){

};

void receiveMessages(){

};

int main(){

  cout << "WELCOME TO THE CPEG676 SECURE SOFTWARE DEVELOPMENT GEE MAIL SERVER PLEASE CHOOSE FROM THE LIST OF OPTIONS!" << endl;
  cout << "YES CAPS IS CRUISE CONTROL FOR COOL :D" << endl;
  cout << "----------------------------------------------------------------------------------------------------------" << endl;
  cout << "Press 1 to Register as a new user." << endl;
  cout << "Press 2 to Login as an existing user." << endl;
  cout << "Press 3 to exit" << endl;

  int mainMenuSelection =0;

  cin >> mainMenuSelection;
  cout << endl;
  while (cin.fail()){
         cout << "Please enter an integer for the Menu Selection";
         // clear error state
         cin.clear();
         // discard 'bad' character(s)
         cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         cout << endl;
         cout << "----------------------------------------------------------------------------------------------------------" << endl;
       }

  if(mainMenuSelection==1){
    //NEWUSER
    string userName;
    string passWord;
    cout << "Please enter desired Username: ";
    cout << endl;
    cin >> userName;
    cout << endl;
    cout << "Please enter desired Password: ";
    cout << endl;
    cin >> passWord;
    cout << endl;
    hashSaltPass(userName, passWord);
    main();

  }
  else if(mainMenuSelection==2){
    //LOGIN
    string salt;
    string pass;
    string userName;
    string passWord;
    cout << "Please enter Username: ";
    cout << endl;
    cin >> userName;
    cout << endl;
    cout << "Please enter Password: ";
    cout << endl;
    cin >> passWord;
    cout << endl;

    sqlite3* db;
    char *zErrMsg = 0;
    int rc;

     rc = sqlite3_open("geemail.db", &db);

     if( rc ){
         fprintf(stderr, "Can't open database: %s", sqlite3_errmsg(db));
         sqlite3_close(db);
         return 0;
     };

    sqlite3_stmt *statement;

    string sqlSelect = "select password from users where username='"+userName+"';";
    const char * query = sqlSelect.c_str();

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int ctotal = sqlite3_column_count(statement);
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                for ( int i = 0; i < ctotal; i++ )
                {
                    pass = (char*)sqlite3_column_text(statement, i);
                    // print or format the output as you want
                }
                cout << endl;
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                //cout << "done " << endl;
                break;
            }
        }
    }

    sqlite3_stmt *statement2;

    string sqlSelect2 = "select salt from users where username='"+userName+"';";
    const char * query2 = sqlSelect2.c_str();

    if ( sqlite3_prepare(db, query2, -1, &statement2, 0 ) == SQLITE_OK )
    {
        int ctotal = sqlite3_column_count(statement2);
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement2);

            if ( res == SQLITE_ROW )
            {
                for ( int i = 0; i < ctotal; i++ )
                {
                    salt = (char*)sqlite3_column_text(statement2, i);
                    // print or format the output as you want
                }
                cout << endl;
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                //cout << "done " << endl;
                break;
            }
        }
    }

    sqlite3_close(db);

    if(validateCredentials(salt, passWord).compare(pass) != 0){
      cout << "Invalid Password for this Username.  Program exiting!" << endl;
      return 0;
    }
    else{
      cout << "Welcome " + userName + "!" << endl;
      return 0;
    }

  }
  else if(mainMenuSelection==3){
    return 0;
  }
  else{
    main();
  }


  return 0;
};
