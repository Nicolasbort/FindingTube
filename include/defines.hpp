#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>
#include <chrono>

#define ARENA false

#if ARENA

    // Limiares da cor azul ( Imagem HSV )
    #define MINBLUE         70
    #define MAXBLUE         155

    #define MINSATBLUE      90
    #define MAXSATBLUE      140

    #define MINVALBLUE      65
    #define MAXVALBLUE      135

    // Limiares da cor amarela ( Imagem HSV )
    #define MINYELLOW       60
    #define MAXYELLOW       140

    #define MINSATYELLOW    165
    #define MAXSATYELLOW    245

    #define MINVALYELLOW    215
    #define MAXVALYELLOW    255

#else

    // Limiares da cor azul ( Imagem HSV )
    #define MINBLUE         60
    #define MAXBLUE         130

    #define MINSATBLUE      60
    #define MAXSATBLUE      160

    #define MINVALBLUE      60
    #define MAXVALBLUE      130

    // Limiares da cor amarela ( Imagem HSV )
    #define MINYELLOW       7
    #define MAXYELLOW       60

    #define MINSATYELLOW    90
    #define MAXSATYELLOW    210

    #define MINVALYELLOW    130
    #define MAXVALYELLOW    225

#endif


// Limiares da cor vermelha ( Imagem BGR )
#define MAXRED 120
#define MINRED 56

#define MAXSATRED 100
#define MINSATRED 60

#define MAXVALRED 220
#define MINVALRED 176


// Limiares da cor laranja ( Imagem HSV )
#define MINORANGE 5
#define MAXORANGE 25

#define MINSATORANGE 120
#define MAXSATORANGE 240

#define MINVALORANGE 50
#define MAXVALORANGE 235


// Limiares da cor branca ( Imagem BGR )
#define MINWHITE 120
#define MAXWHITE 225

#define MINSATWHITE 95
#define MAXSATWHITE 210

#define MINVALWHITE 90
#define MAXVALWHITE 210


// Limiares da cor preta ( 1º canal LAB e 3º canal HSV)
#define MINBLACK 0
#define MAXBLACK 35

#define MINSATBLACK 0
#define MAXSATBLACK 40

#define MINVALBLACK 0
#define MAXVALBLACK 45





#define GAUSSIANFILTER      3
#define KERNELSIZE          7


#define COLOR_RED   Scalar(0, 0, 255)
#define COLOR_GREEN Scalar(0, 255, 0)
#define COLOR_BLUE  Scalar(255, 0, 0)
#define COLOR_BLACK Scalar(0,  0,  0)


#define BLUE_ID 48
#define YELLOW_ID 49
#define BLACK_ID 2


#define ESC         27
#define RETURN      10
#define SPACE       32


// Porcentagem utilizada para verificar se é quadrado ou nao
#define ERROR 2.9f


// Utilizado para mostrar o tempo de execução <chrono>
struct Timer
{
    std::chrono::_V2::system_clock::time_point start, end;
    std::chrono::duration<float> duration;
    
    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;

        std::cout << "timing: " << duration.count() << "s FPS: "<< 1/duration.count() << "\n";
    }
};