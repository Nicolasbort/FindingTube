#include "../include/defines.hpp"

#include <math.h>


int ARR_MINORANGE[3] = {MINORANGE, MINSATORANGE, MINVALORANGE};
int ARR_MAXORANGE[3] = {MAXORANGE, MAXSATORANGE, MAXVALORANGE};

int ARR_MINRED[3] = {MINRED, MINSATRED, MINVALRED};
int ARR_MAXRED[3] = {MAXRED, MAXSATRED, MAXVALRED};


using namespace cv;
using namespace std;

#define DEBBUG_COLOR_RED false

bool isSquare( Rect rectangle )
{
    float proporcao;

    if ( rectangle.x >= rectangle.y )
        proporcao = (float)rectangle.x / (float)rectangle.y;
    else
        proporcao = (float)rectangle.y / (float)rectangle.x;
    

    if ( proporcao <= ERROR )
        return true;
    else
        //cout << "\nProporcao: " << proporcao << "  No: " << rectangle.x << ", " << rectangle.y << "\n\n";
        return false;
}


class FindSensor
{
public:

    Mat mainImage_C3, imageHSV_C3, image_C1;
    Mat kernel;
    Mat tubo, marcador_ruim;

    Rect badmark;

    int centerX, centerY;

    // Variaveis do findContours
    vector<vector<Point>> contours;


    FindSensor()
    {
        this->kernel = Mat::ones(KERNELSIZE, KERNELSIZE, CV_8U);
    }


    void camParam(Mat img)
    {
        this->centerX = img.rows / 2;
        this->centerY = img.cols / 2;
    }


    void setImage(Mat img)
    {
        this->camParam(img);

        this->mainImage_C3 = img;
    }


    void processImage()
    {
        Mat bitwise_tubo, bitwise_badmark;

        GaussianBlur(this->mainImage_C3, this->mainImage_C3, Size(GAUSSIANFILTER, GAUSSIANFILTER), 0);

        // Converte de BGR para HSV (MAL DESEMPENHO)
        cvtColor(this->mainImage_C3, this->imageHSV_C3, COLOR_BGR2HSV);

        tubo = imlimiares(imageHSV_C3, ARR_MINORANGE, ARR_MAXORANGE);
        bitwise_and(mainImage_C3, mainImage_C3, bitwise_tubo, tubo);

		morphologyEx(bitwise_tubo, bitwise_tubo, MORPH_CLOSE, kernel, Point(-1,-1), 1);

        marcador_ruim = imlimiares(bitwise_tubo, ARR_MINRED, ARR_MAXRED);

#if DEBBUG_COLOR_RED
        imshow("badmark", marcador_ruim);
#endif
    }


    Mat imlimiares(Mat img, int hsvMin[3], int hsvMax[3])
    {
        Mat grayImage, grayfilled;

        inRange(img, Scalar(hsvMin[0], hsvMin[1], hsvMin[2]), Scalar(hsvMax[0], hsvMax[1], hsvMax[2]), grayImage);

        grayfilled = imfill(grayImage);

        return grayfilled;
    }


    Mat imfill(Mat img)
    {
        vector<vector<Point>> aux_contours;

        morphologyEx(img, img, MORPH_CLOSE, kernel, Point(-1, -1), 3);

        findContours(img, aux_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	    vector<vector<Point>> hull( aux_contours.size() );

        for (size_t i = 0; i<aux_contours.size(); i++)
        {
            convexHull( aux_contours[i], hull[i] );
        }

        for (size_t i = 0; i<aux_contours.size(); i++)
        {
            drawContours(img, hull, i, 255, -1);
        }

        return img;
    }


    bool findSensor()
    { 
        bool success = false;

        this->processImage();

        findContours(this->marcador_ruim, this->contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        for (int i=0; i<this->contours.size(); i++)
        {
            Rect currentrect = boundingRect( this->contours[i] );

            if ( currentrect.area() <  40)
            {
                continue;
            }
            else
            {
                //cout << currentrect.size() << "\n";
                if ( isSquare( currentrect ) )
                {
                    badmark = currentrect;
                    success = true;
                }
            }
            

        }

        return success;
    }

    void drawSensor()
    {
        rectangle(mainImage_C3, badmark, COLOR_BLUE , -1);
    }


    void show()
    {
        //imshow("this->image", this->mainImage_C3);
        imshow("this->image", this->mainImage_C3);
    }
};


int clickX, clickY;

void getClickPosition(int event, int x, int y, int flags, void* data)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        clickX = x;
        clickY = y;
    } 
    else
    {
        clickX = 0;
        clickY = 0;
    }
    
}




int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Rodar o código > ./NomeDoArquivo \"NomeDoVideo\"\n";
        return -1;
    }

    char* file_name = argv[1];

    bool VIDEO = true;



    if (VIDEO)
    {
        FindSensor tubo;

        VideoCapture cap(file_name);

        int contImwrite = 0;

        if (!cap.isOpened())
        {
            std::cerr << "Falha ao abrir o video\n";
            return -1;
        }
        
        Mat frame;

        Vec3b color;

        while (true)
        {
            cap >> frame;

            if (frame.empty())
                break;


            tubo.setImage(frame);

#if DEBBUG_COLOR_RED

            tubo.processImage();

#else
            if ( tubo.findSensor() )
            {
                tubo.drawSensor();
                putText( tubo.mainImage_C3, "BAD", Point(tubo.badmark.x, tubo.badmark.y), FONT_HERSHEY_SIMPLEX, 1, COLOR_BLUE, 2 );
            }

#endif

            tubo.show();
  
            int key = waitKey(10); 

            // Pressionar espaço para salvar o frame atual
            if (key == 32)
            {
                imwrite("tube_"+to_string(contImwrite)+".jpeg", tubo.mainImage_C3);
                contImwrite++;
            }
        }
    }
    else
    {
        FindSensor tubo;

        Mat mainImg = imread(file_name);

        if (mainImg.empty())
        {
            std::cout << "Image empty!\n";
            return -1;
        }

        tubo.setImage(mainImg);
        tubo.processImage();

        /*if (tubo.findBiggestRect())
            tubo.drawBiggestRect();*/

        tubo.show();

        int key = waitKey(); 

        // Pressionar espaço para salvar o frame atual
        if (key == 32)
            imwrite("frame.jpg", tubo.imageHSV_C3);
    }
}