#include "lbpFeature.h"
#include "multiImg.h"
#include <stdio.h>
using namespace cv;
using namespace std;
int main(int argc, const char* argv[] ){
    multiImg pos,neg;
    lbpFeatureEvaluator evaluator;
    int i,j;
    int counter[256];
    double p1[256],p2[256];
    int posSize,negSize;
    cout << "pos : " << pos.load("list_.txt",false,false) << endl;
    cout << "neg : " << neg.load("list_neg_.txt",false,false) << endl;

    memset( counter, 0, sizeof(counter) );
    posSize = pos.size();
    for( i = 0; i<posSize; i++){
        evaluator.loadImage( *pos.read(i) );
        evaluator.countHistogram();
        for( j=0; j<8;j++){

            counter[ (int)(evaluator.feature.sortList[j]) ]++;
        }
    }
    for( i=0; i<256; i++){
        p1[i] = (double)counter[i] / (double)posSize;
    }

    memset( counter, 0, sizeof(counter) );
    negSize = neg.size();
    for( i = 0; i<negSize; i++){
        evaluator.loadImage( *neg.read(i) );
        evaluator.countHistogram();
        for( j=0; j<25;j++){
            counter[ evaluator.feature.sortList[j] ]++;
        }
    }
    for( i=0; i<256; i++){
        p2[i] = (double)counter[i] / (double)negSize;
    }

    for( i=0; i<256; i++){
        printf(" %3d : %3.2f - %3.2f = %3.2f\n", i, p1[i], p2[i], p1[i] - p2[i]);
    }

return 0;
}
