// Name: Ben Wilfong
// File Name: p1_decryption.cpp
// Program Due Date: 2/16/16
// Program Description:
/* 
 * This program is designed to accept ciphertext that has
 * been knowingly encrypted using a Vigenere cipher.
 * The program will display the frequency of each letter
 * used in the ciphertext and use each letter's frequency 
 * to calculate the index of coincidence, which will be used
 * to make an estimation on the length of the key. Once the
 * length of the key has been estimated, that length will
 * be used in a frequency analysis attack in an attempt
 * to reveal the plaintext of the message.
 *
 * If our guess is incorrect, the user will be prompted
 * to try to change the key, or quit if the decryption
 * is deemed futile. The user may also write the decrypted
 * results to a file. 
 *
 * This file serves as a driver program that gets the input
 * from the user, including the ciphertext (either input
 * by hand on the console or through a file with command
 * line arguments) and the user's choice of putting in 
 * new keys, quitting, or writing the results to a file.
 * There is some moderate error handling in place, but
 * I don't expect it to be immune to all bugs. 
 */

#include <iostream>
#include <fstream>
#include "Vigenere.h"

void writeToFile(string,string);

using namespace std;

int main(int argc, char** argv)
{
     string input = "",
            buffer = "",
            cipherText = "";

     if (argc == 2) // command line arguments for file
     {
          ifstream inputFile;
          inputFile.open(argv[1]); // open file passed by commandline

          if (!inputFile) // file open failure
          {
               cout << "File open failure. Please try again." << endl;
               return 0;
          }

          else
               while(!inputFile.eof()){
                    getline(inputFile, buffer);
                    cipherText = cipherText + buffer + "\n";
                    buffer = ""; 
               }

               cout << "Ciphertext Imported: " << endl << cipherText << endl;

          inputFile.close();          

     }

     else if (argc > 2)
     {
          cout << "Invalid command line arguments. You may run the program "
               << "regularly or with a file name after the program name only. "
               << endl
               << "Example: ./decryption OR ./decryption myFile.txt" << endl;

          return 0;
     }

     else
     {
          cout << "Please enter any Vigenere ciphertext you would like to decrypt.\n"
               << "If you would like to import ciphertext from a text file, restart"
               << " the program and add your file name as a command line argument.\n"
               << "Example: ./decryption myFile.txt" << endl;

          getline(cin,cipherText);
     }

     Vigenere v(cipherText);

     cout << "Was this correct? If not, you may enter in a new key to try, or type"
          << " /q to quit. You may also use /f [YOURFILE.txt] to write the results"
          << " to a file of your choice." << endl;

     getline(cin,input);

     while(true)
     // while the user hasn't chosen to quit or print the results to file
     {
          if(input.compare("/q") == 0)
               return 0; // user chooses to quit

          else if(input.substr(0,2).compare("/f") == 0) // user chooses to write to file
          {
               if(input.length() > 3) // " there was not a proper file name following
               {
                    cout << "Writing results to file..." << endl;// write to file
                    writeToFile(v.getPlainText(), input.substr(3, input.length() - 3));
                         // pass the portion of the input that was the file name and the plaintext
                    return 0;
               }

               else
               {
                    cout << "Bad file name. Please reenter '/f yourFileName.txt' or '/q' to quit."
                         << endl;

                    getline(cin,input);
               }
          }

          else
          {
               transform(input.begin(), input.end(), input.begin(), ::toupper);

               v.decrypt(input);

               cout << "Was this correct? If not, you may enter in a new key to try, or type"
                    << " /q to quit. You may also use /f [YOURFILE.txt] to write the results"
                    << " to a file of your choice." << endl;

               getline(cin,input);
          }
     }
}

void writeToFile(string plainText, string fileName)
{
     ofstream outputFile;
     outputFile.open(fileName.c_str());

     if(!outputFile)
     {
          cout << "File open failed. Please try again." << endl;
          return;
     }

     else
          outputFile << plainText;

     cout << "File write complete." << endl;
     
     outputFile.close();
}

