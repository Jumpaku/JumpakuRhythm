#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        cout << "usage: DisplayImage.out <Image_Path>\n";
        return -1;
    }
    Mat image;
    image = imread( argv[1], 1 );
    if ( !image.data )
    {
        cout << "No image data \n";
        return -1;
    }
    for (size_t i = 0; i < image.cols; i++)
    {
        for (size_t j = 0; j < image.rows; j++)
        {
            cout << static_cast<int>(image.data[i*image.rows + j]);
        }
        cout << "\n";
    }
    
    return 0;
}