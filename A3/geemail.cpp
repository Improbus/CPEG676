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
#include <vector>
#include <termios.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    return 0;
};

void HideStdinKeystrokes(){
    termios tty;

    tcgetattr(STDIN_FILENO, &tty);

    /* we want to disable echo test commit*/
    tty.c_lflag &= ~ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
};

void ShowStdinKeystrokes(){
   termios tty;

    tcgetattr(STDIN_FILENO, &tty);

    /* we want to reenable echo */
    tty.c_lflag |= ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
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
  struct timespec tim, tim2;
  tim.tv_sec = 1;
  tim.tv_nsec = 500000000L;
  nanosleep(&tim , &tim2);

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
     cout << endl;
     cout << "----------------------------------------------------------------------------------------------------------" << endl;
   }
   sqlite3_close(db);
};

string validateCredentials(string salt, string password){
  try{
  CryptoPP::SHA256 sha256;
  string source = salt + password;  //This will be randomly generated somehow
  string hash = "";
  for(int i=0; i<20; i++){
    CryptoPP::StringSource(source, true, new CryptoPP::HashFilter(sha256, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));

  }
    return hash;
  }
  catch(const CryptoPP::Exception& e){
    //cerr << e.what() << endl;
    cout << "Incorrect Password Program Exiting" << endl;
    exit(1);
  }
  //cout <<"SALT INT: " << str << endl;
  //cout << "SALT STRING: " << salt << endl;

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
    try{
      CryptoPP::SecByteBlock derived(32);

      CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> kdf;
      kdf.DeriveKey(derived.data(), derived.size(), purpose, (byte*)password.data(), password.size(), NULL, 0, iterations);

      string recovered;

      //cout << "Cipher text: " << ciphertext << endl;

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
      return decrypted_text;
    }
    catch(const CryptoPP::Exception& e){
      //cerr << e.what() << endl;
      cout << "Incorrect Password to Decrypt Message Program Exiting" << endl;
      exit(1);
    }
      //cout << "Decrypted text: " << decrypted_text << endl;



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
    HideStdinKeystrokes();
    cout << "Please enter desired Password: ";
    cout << endl;
    cin >> passWord;
    ShowStdinKeystrokes();
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
    HideStdinKeystrokes();
    cout << "Please enter Password: ";
    cout << endl;
    cin >> passWord;
    ShowStdinKeystrokes();
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
      cout << "Invalid Password for this Username.  Program Returning to Main Menu!" << endl;
      cout << endl;
      cout << "----------------------------------------------------------------------------------------------------------" << endl;
      main();
    }
    else{
      cout << "Welcome " + userName + "!" << endl;
      cout << "Please Make a Selection below." << endl;
      cout << "1. Send Message." << endl;
      cout << "2. Check Messages." << endl;
      cout << "3. Logout" << endl;

      int userSelect;

      cout << endl;
      cin >> userSelect;
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

      if(userSelect==1){
        string receiver;
        string message;
        string uniquePassword;

        cout << "Please Enter the recipients user name: " << endl;
        cin >> receiver;
        cout << endl;
        cout << "Please Enter the message: " << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        getline(cin, message);
        cout << endl;
        cout << "Please Enter the password you have shared together. NOTE: THIS WILL NOT BE SAVED YOU AND YOUR RECEIVER MUST KNOW THIS!!!!! " << endl;
        HideStdinKeystrokes();
        cin >> uniquePassword;
        ShowStdinKeystrokes();
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

         string sqlInsert = "insert into Messages ('FromUser','ToUser','MessageCipherText') values ('" +userName+ "','"+receiver+"','"+encryptMessage(uniquePassword, message)+"');";

         const char * sql = sqlInsert.c_str();

         rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

         if( rc != SQLITE_OK ){
         fprintf(stderr, "SQL error: %s", zErrMsg);
           sqlite3_free(zErrMsg);
         }
         else{
           fprintf(stdout, "Message Sent");
           cout << endl;
         }

         sqlite3_close(db);

         return 0;

      }
      else if(userSelect==2){
        cout << "Retreiving Your Messages." << endl;
        vector<string> messages;
        vector<string> senders;
        string mess;
        string sentfrom;

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

        string sqlSelect = "select MessageCipherText from Messages where ToUser='"+userName+"';";
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
                        mess = (char*)sqlite3_column_text(statement, i);
                        messages.push_back(mess);
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

        string sqlSelect2 = "select FromUser from Messages where ToUser='"+userName+"';";
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
                        sentfrom = (char*)sqlite3_column_text(statement2, i);
                        senders.push_back(sentfrom);
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
        if(senders.size() == 0){
          cout << "You Have No Messages.  Returning to Main menu." << endl;
          main();
        }
        for (int i = 0; i < senders.size(); i++){
          cout << "You have a Message from " + senders[i] << endl;
          cout << "Do you want to Decrypt it?" << endl;
          int choice;
          cout << "1. Decrypt Message" << endl;
          cout << "2. Skip Message" << endl;
          cin.clear();
          cin.ignore(1000, '\n');
          cin >> choice;
          cout << endl;

          if(choice == 1){
              string decryptPass;
              cout << "Enter the shared Decryption Password you have with: " + senders[i] << endl;
              HideStdinKeystrokes();
              cin >> decryptPass;
              ShowStdinKeystrokes();
              cout << endl;
              string descryptedmessage = decryptMessage(decryptPass, messages[i]);

              cout << "Your Decrypted Message from " +senders[i]+ " is:" << endl;
              cout << descryptedmessage << endl;
          }
          else if(choice == 2){
            cout << "Moving on to next message in the queue" << endl;
          }
          else{
            cout << "Invalid Selection.  Returning to main menu." << endl;
            main();
          }
        }
        return 0;
      }
      else if(userSelect==3){
        main();
      }
      else{
        cout << "Invalid Selection.  Returning to main menu." << endl;
        main();
      }

      return 0;
    }

  }
  else if(mainMenuSelection==3){
    cout << "Program exiting!" << endl;
    return 0;
  }
  else{
    main();
  }

  return 0;
};
