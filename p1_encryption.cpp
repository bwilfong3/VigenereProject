// Name: Ben Wilfong
// Due Date: February 16, 2016
// File Name: p1_encryption.cpp
// Program Description:
/* 
 *  This program will encrypt a given plaintext using
 *  a Vigenere cipher paired with a key. The user
 *  may enter plaintext by hand on the console or can
 *  import the plaintext from a file using command line 
 *  arguments. The user will have the option to write the 
 *  results to a file name of their choice upon completion.
 * 
 *  DEV's NOTE: Unlike the decryption program, all algorithms
 *              are contained within this single file. You need
 *              only to compile this file alone to use the encryption
 *              algorithm. Also, it is recommended for testing that you
 *              use both the encryption and decryption programs in the 
 *              same files, so you can encrypt and create a new file, 
 *              then try to crack your code with the other program 
 *              immediately afterward.
 */

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

string encrypt(string, string);
void outputCipherText(string);
void writeToFile(string,string);

int main(int argc, char** argv)
{
     string plainText, cipherText,
            buffer, keyword, fileName, choice;

     if(argc == 1) // get console input for plaintext
     {
          cout << "Please enter the plaintext you would like to encrypt.\n"
               << "If you would like to import plaintext from a file, restart\n"
               << "the program and use:\n ./encrypt myFileName.txt\n"
               << endl;

          getline(cin, plainText);
     }

     else if(argc == 2) // get plaintext from file
     {
          cout << "Obtaining plaintext from " << argv[1] << endl;

          ifstream inputFile;
          inputFile.open(argv[1]);

          if(!inputFile)
          {
               cout << "File open failure. Please try again." << endl;
               return 0;
          }

          while(!inputFile.eof()) // do importing
          {
               getline(inputFile, buffer);
               plainText = plainText + buffer + "\n";
               buffer = ""; 
          }
     }

     else // bad command line args.
     {
          cout << "Invalid command line arguments. Please try again" << endl;
          return 0;
     }

     cout << "Please enter a keyword to use for encryption." << endl;

     getline(cin, keyword); // get key

     cout << "Encrypting..." << endl;

     cipherText = encrypt(plainText, keyword); // encrypt plaintext with key

     outputCipherText(cipherText); // show ciphertext in AAAAA BBBBB ABCDE format

     cout << "Would you like to write these results to a file?\n" 
          << "Enter Y/n" << endl;

     getline(cin, choice);

     if (choice == "y" || choice == "Y")
     {
          cout << "Please enter a file name." << endl;
          getline(cin, fileName);
          writeToFile(cipherText, fileName);
     }    
}

string encrypt(string plainText, string key)
{
     int keyPos = 0, keyLength = key.size();
     string cipherText = "";

     transform(plainText.begin(), plainText.end(), plainText.begin(), ::toupper);
     transform(key.begin(), key.end(), key.begin(), ::tolower);

     for (int i = 0; i < plainText.size(); i++)
     {    
          if (plainText.at(i) >= 'A' && plainText.at(i) <= 'Z') // is this a valid character?
          {
               plainText.at(i) = plainText.at(i) + (key.at(keyPos) - 'a'); // modify plaintext by key and position

               if (plainText.at(i) > 90) // need to go back to the beginning of alphabet
                    plainText.at(i) = plainText.at(i) - 'Z' + 'A' - 1; // find the difference and add that to the start

               cipherText = cipherText + plainText.at(i);        

               keyPos = (keyPos + 1) % keyLength; // move along on the key
          }
     }

     return cipherText;
}

void outputCipherText(string cipherText)
{
     cout << "Cipher Text: " << endl;

     for (int i = 0; i < cipherText.size(); i++)
     {
          cout << cipherText.at(i);

          if((i + 1) % 5 == 0)
               cout << " ";

          if((i + 1) % 30 == 0)
               cout << "\n";
     }

     cout << endl;
}

void writeToFile(string cipherText, string fileName)
{
     ofstream outputFile;
     outputFile.open(fileName.c_str());

     if(!outputFile)
     {
          cout << "File open failed. Please try again." << endl;
          return;
     }

     else
          outputFile << cipherText;

     cout << "File write complete." << endl;
     
     outputFile.close();
}

