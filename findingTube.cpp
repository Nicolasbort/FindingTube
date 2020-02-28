#include <opencv2/opencv.hpp>
#include <iostream>

#define MINVAL 100
#define MAXVAL 220

#define MINSAT 50
#define MAXSAT 255

#define MINORANGE 0
#define MAXORANGE 20

#define GAUSSIANFILTER 3
#define KERNELSIZE 7
 

int ARR_MINORANGE[3] = {MINORANGE, MINSAT, MINVAL};
int ARR_MAXORANGE[3] = {MAXORANGE, MAXSAT, MAXVAL};

int ARR_MINTESTE[3] = {MINTESTE, MINSAT, MINVAL};
int ARR_MAXTESTE[3] = {MAXTESTE, MAXSAT, MAXVAL};


using namespace cv;


class FindingTube
{

public:

    Mat mainImage_C3, imageHSV_C3, image_C1;
    Mat kernel;

    int centerX, centerY;

    // Variaveis do findContours
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;

    // Variaveis da funcao findBiggestRest
    std::vector<Point> lastContour;
    int biggestContourIdx;
    float biggestContourArea;
    RotatedRect biggestArea;

    FindingTube()
    {
        this->kernel = Mat::ones(KERNELSIZE, KERNELSIZE, CV_8U);

        this->biggestContourIdx = -1;
        this->biggestContourArea = 0;
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
        // Converte de BGR para HSV
        //cvtColor(this->mainImage_C3, this->imageHSV_C3, COLOR_BGR2HSV);

        // Aplicar o blur
        GaussianBlur(this->mainImage_C3, this->mainImage_C3, Size(GAUSSIANFILTER, GAUSSIANFILTER), 0);

        // Pega o range das cores e transforma a imagem em um canal
        this->image_C1 = this->imlimiares(this->mainImage_C3, ARR_MINORANGE, ARR_MAXORANGE);

        // Remove algumas falhas na imagem
		morphologyEx(this->image_C1, this->image_C1, MORPH_CLOSE, this->kernel, Point(-1,-1), 3);

        findContours(this->image_C1, this->contours, this->hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    }


    Mat imlimiares(Mat img, int hsvMin[3], int hsvMax[3])
    {
        Mat grayImage;

        inRange(img, Scalar(hsvMin[0], hsvMin[1], hsvMin[2]), Scalar(hsvMax[0], hsvMax[1], hsvMax[2]), grayImage);

        return grayImage;
    }


    bool findBiggestRect()
    {
        for( int i = 0; i< this->contours.size(); i++ )
        {
            float ctArea;

            if (this->contours[i].size() < 200 && !(this->lastContour.empty()))
            {
                ctArea= contourArea(this->lastContour);
            }
            else
            {
                this->lastContour = this->contours[i];
            
                ctArea= contourArea(this->contours[i]);
            }

            
            if(ctArea > this->biggestContourArea)
            {
                this->biggestContourArea = ctArea;
                this->biggestContourIdx = i;
            }

            if ( ctArea > 1000)
                return true;

            return false;
        }   
    }


    void drawBiggestRect()
    {
        if (this->contours.size() > 0)
        {
            this->biggestArea = minAreaRect(this->contours[this->biggestContourIdx]);

            Point2f corners[4];
            this->biggestArea.points(corners);

            line(this->mainImage_C3, corners[0], corners[1], Scalar(255,0 ,255), 3);
            line(this->mainImage_C3, corners[1], corners[2], Scalar(255,0 ,255), 3);
            line(this->mainImage_C3, corners[2], corners[3], Scalar(255,0 ,255), 3);
            line(this->mainImage_C3, corners[3], corners[0], Scalar(255,0 ,255), 3);
        }
    }


    void show()
    {
        imshow("this->image", this->mainImage_C3);
    }
};


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
        FindingTube find;

        VideoCapture cap(file_name);

        if (!cap.isOpened())
        {
            std::cerr << "Falha ao abrir o video\n";
            return -1;
        }
        
        Mat frame;

        while (true)
        {
            cap >> frame;

            if (frame.empty())
                break;

            find.setImage(frame);
            find.processImage();

            if ( find.findBiggestRect() )
                find.drawBiggestRect();

            find.show();

            
            int key = waitKey(30); 

            // Pressionar espaço para salvar o frame atual
            if (key == 32)
                imwrite("frame2.jpg", find.mainImage_C3);
        }
    }
    else
    {
        FindingTube find;

        Mat mainImg = imread(file_name);

        if (mainImg.empty())
        {
            std::cout << "Image empty!\n";
            return -1;
        }

        find.setImage(mainImg);
        find.processImage();

        if (find.findBiggestRect())
            find.drawBiggestRect();

        find.show();

        int key = waitKey(); 

        // Pressionar espaço para salvar o frame atual
        if (key == 32)
            imwrite("frame.jpg", find.mainImage_C3);
    }
}