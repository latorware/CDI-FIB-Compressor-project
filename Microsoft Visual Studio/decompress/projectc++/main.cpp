//has to be compiled with minimum c++ 14
#pragma warning(disable : 4996)
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <sstream>
#include <limits>
using namespace std; 



int fifteento_base10(string &number) {
    int integerresult = 0;
    int index = 1;
    for (int i = number.size() - 1; i >= 0; i--) {
        int numberr; 
        if (number[i] < 0x3A)
        {
            numberr = number[i] - 0x30; 
        }
        else
        {
            numberr = number[i] - 0x42 + 10;
        }
        integerresult += numberr * index;
        index *= 15;
    }
    return integerresult;
}



int main(int argc, char** argv) {


    char* fileIN, * fileOUT;
    if (argc == 3) { fileIN = argv[1]; fileOUT = argv[2]; }
    else { printf("Usage: ./exe fileIN fileOUT\n"); exit(0); }


    
    chrono::steady_clock::time_point begin1 = chrono::steady_clock::now();

    //Open file into RAM
    FILE *fp;
    fp = fopen(fileIN, "rb");
    if (fp == NULL) {
        printf("Error opening file");
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);  /* same as rewind(f); */

    char* string = (char*)malloc(fsize);
    fread(string, fsize, 1, fp);
    cout << "buffered file into pointer of chars" << endl; 
    fclose(fp);

    
    chrono::steady_clock::time_point end1 = chrono::steady_clock::now();
    cout << "Read and buffered in " << (chrono::duration_cast<chrono::microseconds>(end1 - begin1).count()) / 1000000.0 << "[seconds]" << std::endl;



    
    cout << "Decoding (in RAM)   ...\n";
    begin1 = chrono::steady_clock::now();


    int acabaPrimeraPart = 471985160; //a partir d'aquest nombre inclos comença segona part (la que ens informa sobre els numeros de despres dels dos punts)
    int actual = 0; 
    int actualSegonaPart = 471985160; 
    bool actualSegonaPartPrimeraMeitat = true; 
    bool fetPrimeraIteracioCalcularNumeroDespresDePunts = false;  //sempre false, excepte quan sestigui calculant nombre. Util en cas que actualSegonaPartPrimeraMeitat
    int liniesRestantsNumeroSeguent; 
    std::string liniesRestantsNumeroSeguentStringBase15 = ""; 
    std::string numeroActual; 
    std::string stringDecodejada = ""; 

    std::string numeroActualBase15 = "";
    //primer de tot agafem el primer numero despres de punts
    while (true)
    {
        char high = (string[actualSegonaPart] >> 4) & 0x0F;
        char low = string[actualSegonaPart] & 0x0F;

        if (high == 0xA)
        {
            actualSegonaPartPrimeraMeitat = true; 
            break; 
        }
        else if (low == 0xA)
        {
            
            if (high < 0x0A)
            {
                numeroActualBase15 += high + 0x30;
            }
            else
            {
                numeroActualBase15 += high + 0x41 - 10;
            }

            actualSegonaPartPrimeraMeitat = false;
            actualSegonaPart++;
            break; 
        }
        else
        {
            if (high < 0x0A)
            {
                numeroActualBase15 += high + 0x30;
            }
            else
            {
                numeroActualBase15 += high + 0x41 - 10;
            }


            if (low < 0x0A)
            {
                numeroActualBase15 += low + 0x30;
            }
            else
            {
                numeroActualBase15 += low + 0x41 - 10;
            }

            actualSegonaPart++;
        }
        
    }

    numeroActual = std::to_string(fifteento_base10(numeroActualBase15)); 
    numeroActualBase15 = ""; 


    //calculem linies en que es repetira primer numero despres dels punts
    while (true)
    {
        
        if (!fetPrimeraIteracioCalcularNumeroDespresDePunts)
        {
            fetPrimeraIteracioCalcularNumeroDespresDePunts = true; 

            if (actualSegonaPartPrimeraMeitat)
            {
                char low = string[actualSegonaPart] & 0x0F;
                if (low < 0x0A)
                {
                    liniesRestantsNumeroSeguentStringBase15 += low + 0x30;
                }
                else
                {
                    liniesRestantsNumeroSeguentStringBase15 += low + 0x41 - 10;
                }
                actualSegonaPartPrimeraMeitat = false;
                actualSegonaPart++;
            }

        }
        else
        {
            char high = (string[actualSegonaPart] >> 4) & 0x0F;
            char low = string[actualSegonaPart] & 0x0F;

            if (high == 0xA)
            {
                actualSegonaPartPrimeraMeitat = true;
                fetPrimeraIteracioCalcularNumeroDespresDePunts = false;
                break;
            }
            else if (low == 0xA)
            {

                if (high < 0x0A)
                {
                    liniesRestantsNumeroSeguentStringBase15 += high + 0x30;
                }
                else
                {
                    liniesRestantsNumeroSeguentStringBase15 += high + 0x41 - 10;
                }

                actualSegonaPartPrimeraMeitat = false;
                actualSegonaPart++;
                fetPrimeraIteracioCalcularNumeroDespresDePunts = false;
                break;
            }
            else
            {
                if (high < 0x0A)
                {
                    liniesRestantsNumeroSeguentStringBase15 += high + 0x30;
                }
                else
                {
                    liniesRestantsNumeroSeguentStringBase15 += high + 0x41 - 10;
                }


                if (low < 0x0A)
                {
                    liniesRestantsNumeroSeguentStringBase15 += low + 0x30;
                }
                else
                {
                    liniesRestantsNumeroSeguentStringBase15 += low + 0x41 - 10;
                }

                actualSegonaPart++;
            }
        }

    }
    


    liniesRestantsNumeroSeguent = fifteento_base10(liniesRestantsNumeroSeguentStringBase15); 
    liniesRestantsNumeroSeguent++; 
    liniesRestantsNumeroSeguentStringBase15 = ""; 








    
    while (actual < acabaPrimeraPart)
    {
        //llegir 40 abans comes
        for (int i = 0; i < 20; i++)
        {
            char high = (string[actual] >> 4) & 0x0F;
            char low = string[actual] & 0x0F;

            if (high < 0x0A)
            {
                stringDecodejada += high + 0x30;
            }
            else
            {
                stringDecodejada += high + 0x41 - 10;
            }


            if (low < 0x0A)
            {
                stringDecodejada += low + 0x30;
            }
            else
            {
                stringDecodejada += low + 0x41 - 10;
            }

            actual++; 
        }

        stringDecodejada += ':'; 


        //escriure numero despres de els dos punts
        if (liniesRestantsNumeroSeguent == 0)
        {
            //Calculem proxim numero
            while (actualSegonaPart < fsize)
            {

                if (!fetPrimeraIteracioCalcularNumeroDespresDePunts)
                {
                    fetPrimeraIteracioCalcularNumeroDespresDePunts = true;

                    if (actualSegonaPartPrimeraMeitat)
                    {
                        char low = string[actualSegonaPart] & 0x0F;
                        if (low < 0x0A)
                        {
                            numeroActualBase15 += low + 0x30;
                        }
                        else
                        {
                            numeroActualBase15 += low + 0x41 - 10;
                        }
                        actualSegonaPartPrimeraMeitat = false;
                        actualSegonaPart++;
                    }

                }
                else
                {
                    char high = (string[actualSegonaPart] >> 4) & 0x0F;
                    char low = string[actualSegonaPart] & 0x0F;

                    if (high == 0xA)
                    {
                        actualSegonaPartPrimeraMeitat = true;
                        fetPrimeraIteracioCalcularNumeroDespresDePunts = false;
                        break;
                    }
                    else if (low == 0xA)
                    {

                        if (high < 0x0A)
                        {
                            numeroActualBase15 += high + 0x30;
                        }
                        else
                        {
                            numeroActualBase15 += high + 0x41 - 10;
                        }

                        actualSegonaPartPrimeraMeitat = false;
                        actualSegonaPart++;
                        fetPrimeraIteracioCalcularNumeroDespresDePunts = false;
                        break;
                    }
                    else
                    {
                        if (high < 0x0A)
                        {
                            numeroActualBase15 += high + 0x30;
                        }
                        else
                        {
                            numeroActualBase15 += high + 0x41 - 10;
                        }


                        if (low < 0x0A)
                        {
                            numeroActualBase15 += low + 0x30;
                        }
                        else
                        {
                            numeroActualBase15 += low + 0x41 - 10;
                        }

                        actualSegonaPart++;
                    }
                }

            }

            numeroActual = std::to_string(fifteento_base10(numeroActualBase15));
            numeroActualBase15 = "";



            //Calculem numero de linies
            while (actualSegonaPart < fsize)
            {

                if (!fetPrimeraIteracioCalcularNumeroDespresDePunts)
                {
                    fetPrimeraIteracioCalcularNumeroDespresDePunts = true;

                    if (actualSegonaPartPrimeraMeitat)
                    {
                        char low = string[actualSegonaPart] & 0x0F;
                        if (low < 0x0A)
                        {
                            liniesRestantsNumeroSeguentStringBase15 += low + 0x30;
                        }
                        else
                        {
                            liniesRestantsNumeroSeguentStringBase15 += low + 0x41 - 10;
                        }
                        actualSegonaPartPrimeraMeitat = false;
                        actualSegonaPart++;
                    }

                }
                else
                {
                    char high = (string[actualSegonaPart] >> 4) & 0x0F;
                    char low = string[actualSegonaPart] & 0x0F;

                    if (high == 0xA)
                    {
                        actualSegonaPartPrimeraMeitat = true;
                        fetPrimeraIteracioCalcularNumeroDespresDePunts = false;
                        break;
                    }
                    else if (low == 0xA)
                    {

                        if (high < 0x0A)
                        {
                            liniesRestantsNumeroSeguentStringBase15 += high + 0x30;
                        }
                        else
                        {
                            liniesRestantsNumeroSeguentStringBase15 += high + 0x41 - 10;
                        }

                        actualSegonaPartPrimeraMeitat = false;
                        actualSegonaPart++;
                        fetPrimeraIteracioCalcularNumeroDespresDePunts = false;
                        break;
                    }
                    else
                    {
                        if (high < 0x0A)
                        {
                            liniesRestantsNumeroSeguentStringBase15 += high + 0x30;
                        }
                        else
                        {
                            liniesRestantsNumeroSeguentStringBase15 += high + 0x41 - 10;
                        }


                        if (low < 0x0A)
                        {
                            liniesRestantsNumeroSeguentStringBase15 += low + 0x30;
                        }
                        else
                        {
                            liniesRestantsNumeroSeguentStringBase15 += low + 0x41 - 10;
                        }

                        actualSegonaPart++;
                    }
                }

            }

            
            if (numeroActual == "14")
            {
                //cout << "14??" << endl; 
                liniesRestantsNumeroSeguent = std::numeric_limits<int>::max();
            }
            else
            {
                liniesRestantsNumeroSeguent = fifteento_base10(liniesRestantsNumeroSeguentStringBase15);
                liniesRestantsNumeroSeguentStringBase15 = "";

            }

            //cout << "numero: " << numeroActual << "         linies: " << liniesRestantsNumeroSeguent << endl; 


            stringDecodejada += numeroActual;
        }
        else
        {
            stringDecodejada += numeroActual; 
            liniesRestantsNumeroSeguent--; 
        }


        stringDecodejada += "\n"; 

    }



    
    end1 = chrono::steady_clock::now();
    cout << "Decoded (in RAM) in" << (chrono::duration_cast<chrono::microseconds>(end1 - begin1).count()) / 1000000.0 << "[seconds]" << std::endl;


    

    cout << "Writting decoded file\n";
    begin1 = chrono::steady_clock::now();
    FILE* fp2;
    fp2 = fopen(fileOUT, "wb");
    fwrite(stringDecodejada.c_str(), 1, stringDecodejada.size(), fp2);
    end1 = chrono::steady_clock::now();
    cout << "written in" << (chrono::duration_cast<chrono::microseconds>(end1 - begin1).count()) / 1000000.0 << "[seconds]" << std::endl;

    cout << endl; 

    cout << "size of encoded file: " << fsize << endl; 
    //cout << "size of stringdecodejada: " << stringDecodejada.size() << endl;
    fseek(fp2, 0, SEEK_END);
    long sizeDecoded = ftell(fp2);
    cout << "size of decoded file: " << sizeDecoded << endl; 
    cout << endl; 
    cout << "Ratio: " << (double)sizeDecoded / (double)fsize << endl;


    

}
    

