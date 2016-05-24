// Name: Ben Wilfong
// File Name: Vigenere.cpp
// Due Date: 2/16/16
// Program Description:
/*  
 * This file contains all of the function definitions
 * for the Vigenere decryption object. Upon creation with
 * some given ciphertext as an argument, the object will
 * filter the text and obtain only alphabetical characters.
 * It will then parse the filtered ciphertext and record
 * the frequency data for each character. Using this data
 * we calculate the index of coincidence, which we then use
 * to estimate the period of the key for the given ciphertext.
 * In this particular version of the program, the estimated 
 * period is always taken to be the ceiling of the number
 * returned from the length formula. After the period is
 * estimated, it then attempts to crack the key using it
 * combined with a frequency analysis of the intervals 
 * (equal to the estimated period). Finally, it uses
 * the guessed key to decrypt the ciphertext and show
 * the user what was found. If it is not correct,
 * the decryption algorithm was made public so the main
 * program can ask the users to try again or quit. 
 */

#include "Vigenere.h"

Vigenere::Vigenere(string cipherText)
{
     this->keyGuess = "";
     this->plainText = "";
     this->cipherText = cipherText;
     this->trimCipherText(); // get rid of non-alphabetic characters
     this->cipherTextLength = this->cipherText.length(); // need length of only the chars
     this->indexOfCoincidence = 0.0;
     this->populateFrequencyArray(this->letterFrequency, this->cipherText);
     this->calculateIOC();
     this->estimateKeyLength();
     this->displayInfo(); // show guessed information before decryption
     this->guessKey();
     this->decrypt(keyGuess);
}

/**********************************************************************************************/

void Vigenere::trimCipherText()
{
     string temp = "";

     for (int i = 0; i < this->cipherText.length(); i++)
     {
          if (this->cipherText.at(i) >= 'A' && this->cipherText.at(i) <= 'Z')
               temp = temp + this->cipherText.at(i); // take only alphabetic characters
     }

     this->cipherText = temp;
}

/**********************************************************************************************/

void Vigenere::populateFrequencyArray(int* frequencyArray, string cipherText)
{
     int whatLetterIsThis = 0; // # indicates what letter we are processing
                               // as we will be parsing through a string
                               // comprised of characters

     for (int i = 0; i < cipherText.length(); i++)
     {
          whatLetterIsThis = cipherText.at(i) - 65; // convert ascii to corresponding position in alph.
          frequencyArray[whatLetterIsThis]++; // increment the respective count in the array
     }
}

/**********************************************************************************************/

void Vigenere::calculateIOC()
{
     double numeratorSum = 0.0; // var for upper summation formula

     for(int i = 0; i < 26; i++) // sum up all the frequencies
          numeratorSum = numeratorSum + (this->letterFrequency[i] * 
                                             (this->letterFrequency[i] - 1));

//     cout << numeratorSum << cipherTextLength << endl; // debugging

     this->indexOfCoincidence = numeratorSum / (cipherTextLength * (cipherTextLength - 1));
}

/**********************************************************************************************/

void Vigenere::estimateKeyLength()
{
     this->precisionEstimate = 0.0; // Used to show what we got, then how we used it (ceiling)

     precisionEstimate = (0.027 * cipherTextLength) /
                              ((cipherTextLength - 1) * indexOfCoincidence -
                                   (0.038 * cipherTextLength) + 0.065);

     // this is Friedman's formula for estimating the period of Vigenere ciphertext

     this->estimatedPeriod = ceil(precisionEstimate); // take the ceiling as the period
}

/**********************************************************************************************/

void Vigenere::displayInfo() // this is all just console output formatting
{
     cout << "Letter Frequency: " << endl;

     for (int i = 0; i < 13; i++)
          printf("%c: %02d    %c: %02d\n", (char)(i + 65), this->letterFrequency[i],
                    (char)(i + 65 + 13), this->letterFrequency[i + 13]);

                    // show letter frequency in formatted way

     
     cout << "Index of Coincidence: " << this->indexOfCoincidence << endl;

     cout << "Estimated period: " << this->precisionEstimate << " =====> " 
          << this->estimatedPeriod << " (using ceiling...)" << endl;
}

/**********************************************************************************************/

void Vigenere::guessKey()
{
     string keySection = "";
          // This var will be used to build a string of 
          // ciphertext based on the period of the key.
          // For instance, if the key has an estimated period 
          // of 6, we will build a string using position
          // 1,7,13,19,... and so on to then find what
          // the most frequent character was used on that
          // interval, which we will take to mean as 'e'
          // in plaintext. The same will be done for
          // the intervals 2,8,14,20,... and 3,9,15,21
          // and so on until we have done this as many
          // times as the estimated period.

     char keyChar; // used to append onto key string

     int **frequencyIntervals = new int*[estimatedPeriod]; // create as many arrays
                                                           // as there is intervals (the period)
                                                           // Dynamically, of course.

     for(int i = 0; i < estimatedPeriod; ++i)  // dynamically allocate arrays
          frequencyIntervals[i] = new int[26]; // for each interval of letters

     for(int i = 0; i < estimatedPeriod; i++) // set vals to 0
     {
          for(int j = 0; j < 26; j++)
               frequencyIntervals[i][j] = 0;
     }

     for(int i = 0; i < estimatedPeriod; i++)
     {
          for (int j = i; j < this->cipherTextLength; j = j + estimatedPeriod)
               keySection.push_back(cipherText.at(j));

//          cout << "Interval #" << i + 1 << ": " << keySection << endl; // debugging

          this->populateFrequencyArray(frequencyIntervals[i], keySection);
               // pass the interval to the count function to get letter frequencies

/*          for (int x = 0; x < 13; x++)
               printf("%c: %02d    %c: %02d\n", (char)(x + 65), frequencyIntervals[i][x],
                         (char)(x + 65 + 13), frequencyIntervals[i][x + 13]); // debugging
*/
          keyChar = getMostFrequent(frequencyIntervals[i]); // find the most frequently used char
                                                            // in that particular interval

//          cout << "Most frequent of line " << i + 1 << ": " 
//               << keyChar << endl; // debugging

          keyChar = keyChar - 'E' + 65; // take the most frequently used char to be 'E', the 
                                        // statistically most frequently used letter in the
                                        // english language.

          if(keyChar < 65) // just some stuff to wrap around if we go out of bounds
          {
               keyChar = keyChar - 65 + 'Z' + 1;
               keyGuess.push_back(keyChar); // append guessed letter to key
          }

          else // if no wrapping, append to key
               keyGuess.push_back(keyChar);

          keySection = ""; // flush out keySection
     }

     cout << "Estimated Key: " << keyGuess << endl;     
     
     for(int i = 0; i < estimatedPeriod; ++i)
          delete [] frequencyIntervals[i]; // deallocate

     delete [] frequencyIntervals; // deallocate

}

/**********************************************************************************************/

void Vigenere::decrypt(string key)
{
     this->plainText = ""; // flush out plaintext from previous attempts

     int keyPosition = 0;
     char plainChar;
     
     for (int i = 0; i < this->cipherTextLength; i++)
     {
          plainChar = cipherText.at(i) - key.at(keyPosition) + 'A';
               // find the difference, then convert back to char
          
          if (plainChar < 'A')
          {
               plainChar = plainChar - 65 + 'Z' + 1;
               this->plainText.push_back(plainChar);
          }

          else
               this->plainText.push_back(plainChar);

          keyPosition = (keyPosition + 1) % key.length();
     }
        
     cout << "Decrypted Ciphertext:" << endl;

     
     for(int i = 0; i < 400 && i < plainText.length(); i++)
          cout << plainText.at(i); // print a few lines of plaintext

     cout << "\nKey Used: " << key << endl;
}

/**********************************************************************************************/

char Vigenere::getMostFrequent(int* intervalArray)
{
     int max = 0;
     int pos = 0;

     for (int i = 0; i < 26; i++) // loop to get biggest count in array
     {
          if (intervalArray[i] >= max)
          {
               max = intervalArray[i];
               pos = i; // save place, which denotes what letter was the most
          }
     }

     return (char)(pos + 65); // return the position of the array as a character
}

/**********************************************************************************************/

string Vigenere::getPlainText()
{
     return this->plainText;
          // We'll need to get the plaintext so it can be written to a file.
          // However, nothing should SET the plaintext except the decryption algorithm
          // so plainText remains private with a "getter" function. 
}




