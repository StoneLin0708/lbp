#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "multiImg.h"

using namespace std;
using namespace cv;
void help(void){
    cout << " help info:" << endl
         << "   [out folder] [sample.txt] [width] [hight]" <<endl;

}
struct mouseEvent{
    int event;
    int x;
    int y;
};
struct mouseEvent mouse;
static void onMouse( int event, int x, int y, int , void* ){
    mouse.event = event;
    mouse.x = x;
    mouse.y = y;
}
int main( int argc, const char* argv[] )
{
    cout << " sampleCutter Ver 1.1" << endl;
    //argc = 5;
    char outPath[50] = "imageOut//";
    char listPath[50] = "images.txt";
    char swidth[50] = "64";
    char sheight[50] = "64";

    if(argc != 5){
        help();
        return 1;
    }
    else{
        strcpy( outPath, argv[1] );
        strcpy( listPath, argv[2] );
        strcpy( swidth, argv[3] );
        strcpy( sheight, argv[4] );
    }
    int width = atoi( swidth );
    int height = atoi( sheight );
    if( width < 0 || height < 0){
        cout << "width or hight error" << endl;
        return 2;
    }
    //loading images
    multiImg images;
    cout << " load : " << images.load(listPath) << "images" << endl;
    //
    Mat image,display,display_resize,cut,display_cut;
    Rect area;
    int number = 0;
    char in,name[30];
    bool save = false;
    Point2d pn/*, pDisplay = Point2d(0,0)*/;
    double slope = 1;
    double delta = 1.05;
    double resizeRate = 0;
    const char title[20] = "sample cutter";
    const char title_cut[20] = "cut image x4";
    const int display_w = 600/*,display_h = 480*/;
    const int step = 3;
    namedWindow(title/*, WINDOW_NORMAL*/);
    //resizeWindow(title,display_w,display_h);
	setMouseCallback(title, onMouse , 0);

    while(  images.read(number)  ){

        image = *(images.read(number) );
        display = image.clone();
        pn = Point2d(0,0);
        if(slope*width+1 >= display.cols || slope*height+1 >= display.rows ) slope = 1;
        resizeRate = (double)(display_w) / (double)(display.cols);

        while(!save){

            //imshow("t1",image);
            //imshow("t2",image_resize);
            //waitKey(0);

            display = image.clone();

            rectangle(display, pn, Point2d( pn.x+width*slope, pn.y+height*slope ) , Scalar(0), 3, 8, 0);

            cut = image( Rect( Point2d( pn.x, pn.y ),
                               Point2d( pn.x+width*slope, pn.y+height*slope ) ) );

            resize(cut,cut,Size( width, height ) );
            resize(cut,display_cut,Size( cut.cols*4, cut.rows*4 ) );

            resize(display,display_resize,Size( display_w, (int)( (double)display.rows*resizeRate ) ) );
            imshow(title, display_resize);
            imshow(title_cut,display_cut);

            switch(mouse.event)
            {
            case EVENT_MOUSEMOVE:

                break;
            case EVENT_LBUTTONDOWN:
                if( mouse.x/resizeRate+width*slope <= display.cols && mouse.y/resizeRate+height*slope <= display.rows){
                    pn.x = mouse.x/resizeRate;
                    pn.y = mouse.y/resizeRate;
                }
                break;
            /*case EVENT_LBUTTONUP:

                p1.x = event.x;
                p1.y = event.y;
                selecting = false;
                break;*/
            }

            in = waitKey(10);

            switch (in){
            case  'w' :
                if( pn.y-step >= 0 && pn.y-step+height*slope <= image.rows )
                    pn.y-=step;
                break;
            case  's' :
                if( pn.y+step >= 0 && pn.y+step+height*slope <= image.rows )
                    pn.y+=step;
                break;
            case  'a' :
                if( pn.x-step >= 0 && pn.x-step+width*slope <= image.cols )
                    pn.x-=step;
                break;
            case  'd' :
                if( pn.x+step >= 0 && pn.x+step+width*slope <= image.cols )
                    pn.x+=step;
                break;
            case  't':
                if( (int)(width*slope*delta) < display.cols-pn.x &&
                    (int)(height*slope*delta < display.rows-pn.y ) )
                        slope*=delta;
                break;
            case  'g':
                slope/=delta;
                if(slope < 1) slope = 1;
                break;
            case  'o':
                save = true;
                break;
            case  8:

                return 3;
                break;
            }

        }
        //imageOut = image( Rect( x,y,width,hight) );
        sprintf(name,"%s%03d.jpg",outPath,number);
        cout << name << endl;
        //imshow(name,cut);
        imwrite(name,cut);
        //waitKey(0);
        number ++ ;
        save = false;
    }

    return 0;
}
