# include "opencv/cv.h"
# include "opencv/highgui.h"
# include <stdio.h>
# include <stdlib.h>

int main(int argc, char **argv){

        IplImage* newImg, *grayImg, *smoothImg, *sampImg, *sharpImg;
        int  i =0, j= 0;
        CvScalar pixel = {}, pixel1 = {}, pixel2 = {}, pixel3 = {};
        double pixval;

        newImg = cvLoadImage("southpark.jpg",1);
        grayImg = cvCreateImage(cvSize(newImg->width, newImg->height), IPL_DEPTH_8U, 1);
        smoothImg = cvCreateImage(cvSize(newImg->width, newImg->height), IPL_DEPTH_8U, 3);
        sampImg = cvCreateImage(cvSize(newImg->width, newImg->height), IPL_DEPTH_8U, 3);
        sharpImg = cvCreateImage(cvSize(newImg->width, newImg->height), IPL_DEPTH_8U, 3);

        cvSmooth(newImg,smoothImg,CV_GAUSSIAN,9,0,0,0);


        printf("newImg height:%d \n", newImg->height);
        printf("newImg width:%d \n", newImg->width);

        cvCvtColor(newImg, grayImg, CV_BGR2GRAY);

        cvNamedWindow("original",1);
        cvNamedWindow("grayscale",1);
        cvNamedWindow("gaussian",1);
        cvNamedWindow("sample",1);
        cvNamedWindow("sharp",1);



        for(i = 0; i < newImg->width; i++)
                for(j = 0; j < newImg->height; j++){
                        pixel = cvGet2D(newImg, j, i);
                        pixel1 = cvGet2D(newImg, j, i);
                        pixel1.val[1] = pixel1.val[2] = 0;
                        pixel2 = cvGet2D(smoothImg, j, i);

                        pixel3.val[0] = pixel.val[0]- pixel2.val[0];
                        pixel3.val[1] = pixel.val[1]- pixel2.val[1];
                        pixel3.val[2] = pixel.val[2]- pixel2.val[2];


                        cvSet2D(sharpImg,j,i,pixel3);
                        cvSet2D(sampImg,j,i,pixel1);
                        printf("Pixel[%d][%d]: ",i,j);
                        printf(" B:%d G:%d R:%d \n ",(int)pixel.val[0],(int)pixel.val[1],(int)pixel.val[2]);
                }

        printf("\n-------------------------------------------------------------------------");
        for(i = 0; i < grayImg->width; i++){
                for(j = 0; j < grayImg->height; j++){
                        pixel = cvGet2D(grayImg, j, i);
                        printf("Pixel[%d][%d]: ",i,j);
                        printf("Brightness: %d \n",(int)pixel.val[0]);
                }
        }
        printf("\n-------------------------------------------------------------------------");

        for(i = 0; i < smoothImg->width; i++){
                for(j = 0; j < smoothImg->height; j++){
                        pixel = cvGet2D(smoothImg, j, i);
                        printf("Pixel[%d][%d]: ",i,j);
                        printf(" B:%d G:%d R:%d \n ",(int)pixel.val[0],(int)pixel.val[1],(int)pixel.val[2]);
                }
                printf("\n");
        }

        cvShowImage("original", newImg);
        cvShowImage("grayscale", grayImg);
        cvShowImage("gaussian", smoothImg);
        cvShowImage("sample", sampImg);
        cvShowImage("sharp", sharpImg);

        cvWaitKey(0);

        cvDestroyWindow("original");
        cvDestroyWindow("grayscale");
        cvDestroyWindow("gaussian");
        cvDestroyWindow("sample");
        cvDestroyWindow("sharp");

        cvReleaseImage(&newImg);
        cvReleaseImage(&grayImg);
        cvReleaseImage(&smoothImg);
        cvReleaseImage(&sampImg);
        cvReleaseImage(&sharpImg);
        
        return 0;
}