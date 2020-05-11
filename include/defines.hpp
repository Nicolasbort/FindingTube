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
    #define MINBLUE         40
    #define MAXBLUE         125

    #define MINSATBLUE      50
    #define MAXSATBLUE      115

    #define MINVALBLUE      70
    #define MAXVALBLUE      130

    // Limiares da cor amarela ( Imagem HSV )
    #define MINYELLOW       7
    #define MAXYELLOW       50

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


#define GAUSSIANFILTER      3
#define KERNELSIZE          7


#define COLOR_RED   Scalar(0, 0, 255)
#define COLOR_GREEN Scalar(0, 255, 0)
#define COLOR_BLUE  Scalar(255, 0, 0)

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