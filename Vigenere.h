// Name: Ben Wilfong
// File Name: Vigenere.h
// Due Date: 2/16/16
// Project Description:

/*
 * This is simply a header file for the Vigenere class.
 * The only things made public are a getter function 
 * to obtain the decrypted plaintext (so we can write
 * it to a file but not modify it), the decryption function
 * in case the user wants to use a key other than the one
 * the program guesses, and the constructor, for obvious
 * reasons. 
 */

#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cmath>

using namespace std;

class Vigenere
{
     private:
          void trimCipherText();
          void populateFrequencyArray(int*, string);
          void calculateIOC();
          void estimateKeyLength();
          void displayInfo();
          void guessKey();
          char getMostFrequent(int*);
          int letterFrequency[26] = {0};
          int cipherTextLength;
          double indexOfCoincidence;
          int estimatedPeriod;
          float precisionEstimate;
          string cipherText;
          string plainText;
          string keyGuess;

     public:
          Vigenere(string);
          void decrypt(string);
          string getPlainText();


};

#endif
