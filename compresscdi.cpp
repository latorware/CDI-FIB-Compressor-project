//has to be compiled with minimum c++ 14
#pragma warning(disable : 4996)
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <map>
#include <queue>
#include <sstream>
using namespace std; 


string base15 = "0123456789BCDEF";
string to_base15(int number) {
    string result = "";

    if (number == 0)
    {
        return "0";
    }


    while (number > 0) {
        result = base15[number % 15] + result;
        number /= 15;
    }
    return result;
}



void printHexValuesChars(string a)
{
    //print hex values of each character
    for (int i = 0; i < a.size(); i++)
    {
        //printf("%x ", a[i]);
        printf("\\%02hhx", (unsigned char)a[i]);
    }
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




    //Juntar chars i treure punts
    //La part abans dels punts cada caracter va de 0-f, 0x0-0xF, i dos caracter sajunten en un byte, un es part esquerre i laltre la part dreta
    //sempre hi ha 40 caracters (nombre parell) abans dels punts
    //els punts no els enmagatzemem
    //La part després dels punts cada caracter va de 0-9, 0x0-0x9, i 0xA es el breakline. Tambe sajunten dos caracters en un. 
    
    cout << "Encoding (in RAM)   ...\n";

    begin1 = chrono::steady_clock::now(); 
    std::string strReduida = ""; 
    std::string strReduidaSegonaPart = ""; //Informacio sobre els numeros de despres dels dos punts de cada linia
    //int linia = 0; 
    std::string anteriorNumeroDespresPunts = ""; 
    bool comaGranPosada = true; 
    int liniesSeguides = 0; 
    bool firstNumero = true; 

    for (int i = 0; i < fsize; i++) {
            do {
                //estem a part dabans de punts
                    char charEsquerre; 
                    char charDreta; 

                    //part esquerre del char
                    if (string[i] < 0x3A)
                    {
                        //part esquerre representa un numero (per tant anira de 0x0 a 0x9)
                        charEsquerre = string[i] - 0x30;
                    }
                    else 
                    {
                        //part esquerre representa una lletra (per tant anira de 0xA a 0xF)
                        charEsquerre = string[i] - 0x41 + 10;
                    }

                    //part dreta del char
                    if (string[i+1] < 0x3A)
                    {
                        //part dreta representa un numero (per tant anira de 0x0 a 0x9)
                        charDreta = string[i+1] - 0x30;
                    }
                    else 
                    {
                        //part dreta representa una lletra (per tant anira de 0xA a 0xF)
                        charDreta = string[i+1] - 0x41 + 10;
                    }

                    //concatenar els dos caracters
                    //guardar el char concatenat
                    strReduida += (char)((charEsquerre << 4) | charDreta);

                
                i += 2; 
                //al ser 40 caracters abans de punts (parell), si no hi ha més caracters que no sigui punts, sortir del bucle
            } while (string[i] != ':');

            i++; //ens saltem els ':'

            std::string numeroDespresPunts = ""; 

            //iterate through breakline
            while (string[i] != '\n')
            {
                numeroDespresPunts += string[i]; 
                i++; 
            }

            if (anteriorNumeroDespresPunts != numeroDespresPunts)
            {
                
                //cout << numeroDespresPunts << "         " << linia << endl;
                anteriorNumeroDespresPunts = numeroDespresPunts;
                //cout << atoi(numeroDespresPunts.c_str()) << endl;


                //convertir numero despres de punts a base 15 (0-9, B-F)
                std::string numeroDespresPuntsBase15 = to_base15(atoi(numeroDespresPunts.c_str())); 
                //cout << numeroDespresPuntsBase15 << endl; 
                //cout << numeroDespresPuntsBase15.size() << endl; 
                //cout << endl; 



                //convertir linies seguides a base 15 (0-9, B-F)
                std::string liniesSeguidesBase15 = to_base15(liniesSeguides); 
                //cout << liniesSeguidesBase15 << endl;
                //cout << liniesSeguidesBase15.size() << endl;



                //std::string liniesSeguidesString = std::to_string(liniesSeguides);



                bool coma_posada = false; //coma 0x0B

                int j = 0;

                if (!comaGranPosada) //0x0A
                {

                    if (liniesSeguidesBase15[0] < 0x3A)
                    {
                        //printf("\\%02hhx", (unsigned char)((char)(((char)(0x0A) << 4) | (char)(liniesSeguidesBase15[0] - 0x30))));
                        strReduidaSegonaPart += (char)(((char)(0x0A) << 4) | (char)(liniesSeguidesBase15[0] - 0x30));
                    }
                    else
                    {
                        //printf("\\%02hhx", (unsigned char)((char)(((char)(0x0A) << 4) | (char)(liniesSeguidesBase15[0] - 0x41 + 10))));
                        strReduidaSegonaPart += (char)(((char)(0x0A) << 4) | (char)(liniesSeguidesBase15[0] - 0x41 + 10));
                    }

                    j++;
                }

                comaGranPosada = false;

                if (!firstNumero)
                {
                    //reduim cada digit en part esquerre i dreta per a cada char
                    while (j < liniesSeguidesBase15.size())
                    {
                        //Mirem si el digit de despres es numero o no
                        if (j + 1 < liniesSeguidesBase15.size())
                        {

                            char charEsquerre;
                            char charDreta;

                            //part esquerre del char
                            if (liniesSeguidesBase15[j] < 0x3A)
                            {
                                //part esquerre representa un numero (per tant anira de 0x0 a 0x9)
                                charEsquerre = liniesSeguidesBase15[j] - 0x30;
                            }
                            else
                            {
                                //part esquerre representa una lletra (per tant anira de 0xA a 0xF)
                                charEsquerre = liniesSeguidesBase15[j] - 0x41 + 10;
                            }

                            //part dreta del char
                            if (liniesSeguidesBase15[j + 1] < 0x3A)
                            {
                                //part dreta representa un numero (per tant anira de 0x0 a 0x9)
                                charDreta = liniesSeguidesBase15[j + 1] - 0x30;
                            }
                            else
                            {
                                //part dreta representa una lletra (per tant anira de 0xA a 0xF)
                                charDreta = liniesSeguidesBase15[j + 1] - 0x41 + 10;
                            }

                            //concatenar els dos caracters
                            //guardar el char concatenat
                            //printf("\\%02hhx", (unsigned char)((char)((charEsquerre << 4) | charDreta)));
                            strReduidaSegonaPart += (char)((charEsquerre << 4) | charDreta);


                            j++;
                        }
                        else
                        {

                            //Part esquerre, digit, part dreta, coma (representats per A)
                            if (liniesSeguidesBase15[j] < 0x3A)
                            {
                                //printf("\\%02hhx", (unsigned char)((char)(((char)(liniesSeguidesBase15[j] - 0x30) << 4) | (char)(0x0A))));
                                strReduidaSegonaPart += (char)(((char)(liniesSeguidesBase15[j] - 0x30) << 4) | (char)(0x0A));
                            }
                            else
                            {
                                //printf("\\%02hhx", (unsigned char)((char)(((char)(liniesSeguidesBase15[j] - 0x41 + 10) << 4) | (char)(0x0A))));
                                strReduidaSegonaPart += (char)(((char)(liniesSeguidesBase15[j] - 0x41 + 10) << 4) | (char)(0x0A));
                            }

                            coma_posada = true;
                        }
                        j++;
                    }
                }
                else
                {
                    coma_posada = true; 
                    firstNumero = false; 
                }

                //cout << endl; 
                int k = 0;

                if (!coma_posada) //no coma 0x0B
                {



                    if (numeroDespresPuntsBase15[0] < 0x3A)
                    {
                        //printf("\\%02hhx", (unsigned char)((char)(((char)(0x0A) << 4) | (char)(numeroDespresPuntsBase15[0] - 0x30))));
                        strReduidaSegonaPart += (char)(((char)(0x0A) << 4) | (char)(numeroDespresPuntsBase15[0] - 0x30));
                    }
                    else
                    {
                        //printf("\\%02hhx", (unsigned char)((char)(((char)(0x0A) << 4) | (char)(numeroDespresPuntsBase15[0] - 0x41 + 10))));
                        strReduidaSegonaPart += (char)(((char)(0x0A) << 4) | (char)(numeroDespresPuntsBase15[0] - 0x41 + 10));
                    }

                    k++;

                }

                while (k < numeroDespresPuntsBase15.size())
                {
                    //Mirem si el digit de despres es numero o no
                    if (k + 1 < numeroDespresPuntsBase15.size())
                    {

                        char charEsquerre;
                        char charDreta;

                        //part esquerre del char
                        if (numeroDespresPuntsBase15[k] < 0x3A)
                        {
                            //part esquerre representa un numero (per tant anira de 0x0 a 0x9)
                            charEsquerre = numeroDespresPuntsBase15[k] - 0x30;
                        }
                        else
                        {
                            //part esquerre representa una lletra (per tant anira de 0xA a 0xF)
                            charEsquerre = numeroDespresPuntsBase15[k] - 0x41 + 10;
                        }

                        //part dreta del char
                        if (numeroDespresPuntsBase15[k + 1] < 0x3A)
                        {
                            //part dreta representa un numero (per tant anira de 0x0 a 0x9)
                            charDreta = numeroDespresPuntsBase15[k + 1] - 0x30;
                        }
                        else
                        {
                            //part dreta representa una lletra (per tant anira de 0xA a 0xF)
                            charDreta = numeroDespresPuntsBase15[k + 1] - 0x41 + 10;
                        }

                        //concatenar els dos caracters
                        //guardar el char concatenat
                        //printf("\\%02hhx", (unsigned char)((char)((charEsquerre << 4) | charDreta)));
                        strReduidaSegonaPart += (char)((charEsquerre << 4) | charDreta);
                        k++;
                    }
                    else
                    {




                        //Part esquerre, digit, part dreta, coma (representats per A)
                        if (numeroDespresPuntsBase15[k] < 0x3A)
                        {
                            //printf("\\%02hhx", (unsigned char)((char)(((char)(numeroDespresPuntsBase15[k] - 0x30) << 4) | (char)(0x0A))));
                            strReduidaSegonaPart += (char)(((char)(numeroDespresPuntsBase15[k] - 0x30) << 4) | (char)(0x0A));
                        }
                        else
                        {
                            //printf("\\%02hhx", (unsigned char)((char)(((char)(numeroDespresPuntsBase15[k] - 0x41 + 10) << 4) | (char)(0x0A))));
                            strReduidaSegonaPart += (char)(((char)(numeroDespresPuntsBase15[k] - 0x41 + 10) << 4) | (char)(0x0A));
                        }

                    
                        comaGranPosada = true;
                    }



                    k++;
                }

                //cout << liniesSeguides << endl; 
                liniesSeguides = 0; 
                //cout << endl; 

            }
            else
            {
                liniesSeguides++; 
            }

            //linia++; 

        
    }
    
    end1 = chrono::steady_clock::now();
    cout << "Encoded (in RAM) in" << (chrono::duration_cast<chrono::microseconds>(end1 - begin1).count()) / 1000000.0 << "[seconds]" << std::endl;

    //printHexValuesChars(strReduidaSegonaPart); 

    free(string);
    string = NULL;


    strReduida += strReduidaSegonaPart;
    strReduidaSegonaPart = ""; 
    std::string().swap(strReduidaSegonaPart); //intentar alliberar memoria de strReduidaSegonaPart


    //cout << "Tamany de strReduida en bytes: " << strReduida.size() << endl;

    
    cout << "Writting encoding to file\n";
    begin1 = chrono::steady_clock::now(); 
    FILE *fp2;
    fp2 = fopen(fileOUT, "wb");
    fwrite(strReduida.c_str(), 1, strReduida.size(), fp2);
    end1 = chrono::steady_clock::now();
    cout << "written in" << (chrono::duration_cast<chrono::microseconds>(end1 - begin1).count()) / 1000000.0 << "[seconds]" << std::endl;

    cout << endl; 

    cout << "size of original file: " << fsize << endl;

    fseek(fp2, 0, SEEK_END);
    long sizeEncoded = ftell(fp2);
    cout << "size of encoded file: " << sizeEncoded << endl;
    fclose(fp2);


    cout << endl; 

    cout << "Ratio: " << (double)fsize / (double)sizeEncoded << endl;
    




}
    

