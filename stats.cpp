#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "stats.h"
#include <utility>
#include <vector>
using namespace std;
using namespace cs221util;

  stats::stats(PNG & im){

    for(unsigned int i = 0; i< im.width(); i++){
      vector<long> tempR;
      vector<long> tempG;
      vector<long> tempB;
      vector<long> tempSR;
      vector<long> tempSG;
      vector<long> tempSB;
      for(unsigned int j = 0; j< im.height(); j++){
        tempR.push_back(-1);
        tempG.push_back(-1);
        tempB.push_back(-1);
        tempSR.push_back(-1);
        tempSG.push_back(-1);
        tempSB.push_back(-1);
      }
      this->sumRed.push_back(tempR);
      this->sumGreen.push_back(tempG);
      this->sumBlue.push_back(tempB);
      this->sumsqRed.push_back(tempSR);
      this->sumsqGreen.push_back(tempSG);
      this->sumsqBlue.push_back(tempSB);
    }

    this->sumRed[0][0] = im.getPixel(0,0)->r;
    this->sumGreen[0][0] = im.getPixel(0,0)->g;
    this->sumBlue[0][0] = im.getPixel(0,0)->b;
    this->sumsqRed[0][0] = im.getPixel(0,0)->r * im.getPixel(0,0)->r;
    this->sumsqGreen[0][0] = im.getPixel(0,0)->g * im.getPixel(0,0)->g;
    this->sumsqBlue[0][0] = im.getPixel(0,0)->b * im.getPixel(0,0)->b;

    for(int x=0; x<im.width(); x++){
      for(int y =0;y<im.height();y++){
        if(x ==0 && y ==0) continue;
        if(y == 0) {
          this->sumRed[x][y] = this->sumRed[x-1][y] + im.getPixel(x,y)->r;
          this->sumGreen[x][y] = this->sumGreen[x-1][y] + im.getPixel(x,y)->g;
          this->sumBlue[x][y] = this->sumBlue[x-1][y] + im.getPixel(x,y)->b;
          this->sumsqRed[x][y] = this->sumsqRed[x-1][y] + im.getPixel(x,y)->r * im.getPixel(x,y)->r;
          this->sumsqGreen[x][y] = this->sumsqGreen[x-1][y] + im.getPixel(x,y)->g * im.getPixel(x,y)->g;
          this->sumsqBlue[x][y] = this->sumsqBlue[x-1][y] + im.getPixel(x,y)->b * im.getPixel(x,y)->b;
        }
        else if ( x == 0){
          this->sumRed[x][y] = this->sumRed[x][y-1] + im.getPixel(x,y)->r;
          this->sumGreen[x][y] = this->sumGreen[x][y-1] + im.getPixel(x,y)->g;
          this->sumBlue[x][y] = this->sumBlue[x][y-1] + im.getPixel(x,y)->b;
          this->sumsqRed[x][y] = this->sumsqRed[x][y-1] + im.getPixel(x,y)->r * im.getPixel(x,y)->r;
          this->sumsqGreen[x][y] = this->sumsqGreen[x][y-1] + im.getPixel(x,y)->g * im.getPixel(x,y)->g;
          this->sumsqBlue[x][y] = this->sumsqBlue[x][y-1] + im.getPixel(x,y)->b * im.getPixel(x,y)->b;

        } else {
          this->sumRed[x][y] = this->sumRed[x-1][y] + this->sumRed[x][y-1] - this->sumRed[x-1][y-1]+ im.getPixel(x,y)->r;
          this->sumGreen[x][y] = this->sumGreen[x-1][y] +this->sumGreen[x][y-1]- this->sumGreen[x-1][y-1]+ im.getPixel(x,y)->g;
          this->sumBlue[x][y] = this->sumBlue[x-1][y] + this->sumBlue[x][y-1]- this->sumBlue[x-1][y-1]+im.getPixel(x,y)->b;
          this->sumsqRed[x][y] = this->sumsqRed[x-1][y] + this->sumsqRed[x][y-1]- this->sumsqRed[x-1][y-1]+im.getPixel(x,y)->r * im.getPixel(x,y)->r;
          this->sumsqGreen[x][y] = this->sumsqGreen[x-1][y] +this->sumsqGreen[x][y-1]- this->sumsqGreen[x-1][y-1]+ im.getPixel(x,y)->g * im.getPixel(x,y)->g;
          this->sumsqBlue[x][y] = this->sumsqBlue[x-1][y] + this->sumsqBlue[x][y-1]- this->sumsqBlue[x-1][y-1]+im.getPixel(x,y)->b * im.getPixel(x,y)->b;

        }

      }
    }


  }

  long stats::getScore(pair<int,int> ul, pair<int,int> lr){
    long r = getSumSq('r',ul,lr)-getSum('r',ul,lr)*getSum('r',ul,lr)/rectArea(ul,lr);
    long g = getSumSq('g',ul,lr)-getSum('g',ul,lr)*getSum('g',ul,lr)/rectArea(ul,lr);
    long b = getSumSq('b',ul,lr)-getSum('b',ul,lr)*getSum('b',ul,lr)/rectArea(ul,lr);
    return r+g+b;
  }

	// given a rectangle, return the average color value over the
	// rectangle as a pixel.
	/* Each color component of the pixel is the average value of that
	* component over the rectangle.
	* @param ul is (x,y) of the upper left corner of the rectangle
	* @param lr is (x,y) of the lower right corner of the rectangle */
	RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    // if(rectArea(ul,lr) == 0)
    //   printf("ul: %d %d | lr %d %d\n",ul.first, ul.second,lr.first,lr.second);
    if(ul == lr){
      RGBAPixel ep(getSum('r',ul,lr),getSum('g',ul,lr),getSum('b',ul,lr));
      return ep;
    }
    RGBAPixel Rp(getSum('r',ul,lr)/rectArea(ul,lr),getSum('g',ul,lr)/rectArea(ul,lr),getSum('b',ul,lr)/rectArea(ul,lr));
    return Rp;
  }

	// given a rectangle, return the number of pixels in the rectangle
	/* @param ul is (x,y) of the upper left corner of the rectangle
	* @param lr is (x,y) of the lower right corner of the rectangle */
	long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    if (ul == lr) return 1;
    return (get<0>(lr)-get<0>(ul)+1)*(get<1>(lr)-get<1>(ul)+1);
  }

  /* returns the sums of all pixel values across all color channels.
  * useful in computing the score of a rectangle
  * PA3 function
  * @param channel is one of r, g, or b
  * @param ul is (x,y) of the upper left corner of the rectangle
  * @param lr is (x,y) of the lower right corner of the rectangle */
  long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
  //  printf("in getsum ul: %d %d | lr: %d %d\n",ul.first, ul.second, lr.first, lr.second);
    vector<vector<long>> *v;
    if(channel =='r') v = &sumRed;
    else if (channel == 'g') v = &sumGreen;
    else if (channel == 'b') v = &sumBlue;
    if (ul == lr){
      if(ul.first == 0 && ul.second == 0) return (*v)[lr.first][lr.second];
      else if (ul.first == 0) return (*v)[lr.first][lr.second] - (*v)[lr.first][lr.second -1];
      else if(ul.second ==0) return (*v)[lr.first][0] - (*v)[lr.first -1][0];
      else {
        pair<int,int> p(lr.first-1, lr.second-1);
        return abs((*v)[get<0>(p)][get<1>(p)] - (*v)[get<0>(p)][get<1>(ul)] - (*v)[get<0>(ul)][get<1>(p)] +(*v)[get<0>(ul)][get<1>(ul)]);
      }
    } else if(ul.first == 0 && ul.second == 0) return (*v)[lr.first][lr.second];
      else if (ul.first == 0) return (*v)[lr.first][lr.second] - (*v)[lr.first][ul.second - 1];
      else if (ul.second == 0) return (*v)[lr.first][lr.second] - (*v)[ul.first -1][lr.second];
    else{
      pair<int,int> p (ul.first-1,ul.second-1);
      return abs((*v)[get<0>(lr)][get<1>(lr)] - (*v)[get<0>(lr)][get<1>(p)] - (*v)[get<0>(p)][get<1>(lr)] +(*v)[get<0>(p)][get<1>(p)]);
    }

  }

  /* returns the sums of squares of all pixel values across all color channels.
  * useful in computing the score of a rectangle
  * PA3 function
  * @param channel is one of r, g, or b
  * @param ul is (x,y) of the upper left corner of the rectangle
  * @param lr is (x,y) of the lower right corner of the rectangle */
  long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
//    printf("in getsumsp ul: %d %d | lr: %d %d\n",ul.first, ul.second, lr.first, lr.second);
    vector<vector<long>> *v;
    if(channel =='r') v = &sumsqRed;
    else if (channel == 'g') v = &sumsqGreen;
    else if (channel == 'b') v = &sumsqBlue;
    if (ul == lr){
      if(ul.first == 0 && ul.second == 0) return (*v)[lr.first][lr.second];
      else if (ul.first == 0) return (*v)[lr.first][lr.second] - (*v)[lr.first][lr.second -1];
      else if(ul.second ==0) return (*v)[lr.first][0] - (*v)[lr.first -1][0];
      else {
        pair<int,int> p(lr.first-1, lr.second-1);
        return abs((*v)[get<0>(p)][get<1>(p)] - (*v)[get<0>(p)][get<1>(ul)] - (*v)[get<0>(ul)][get<1>(p)] +(*v)[get<0>(ul)][get<1>(ul)]);
      }
    } else if(ul.first == 0 && ul.second == 0) return (*v)[lr.first][lr.second];
      else if (ul.first == 0) return (*v)[lr.first][lr.second] - (*v)[lr.first][ul.second - 1];
      else if (ul.second == 0) return (*v)[lr.first][lr.second] - (*v)[ul.first -1][lr.second];
    else{
      pair<int,int> p (ul.first-1,ul.second-1);
      return abs((*v)[get<0>(lr)][get<1>(lr)] - (*v)[get<0>(lr)][get<1>(p)] - (*v)[get<0>(p)][get<1>(lr)] +(*v)[get<0>(p)][get<1>(p)]);
    }
  }
