#include<opencv2/opencv.hpp>
#include<opencv2/viz.hpp>
#include<iostream>

using namespace std;
using namespace cv;

static void help(){
    cout
    << "--------------------------------------------------------------------------" << endl
    << "This program shows how to launch a 3D visualization window. You can stop event loop to continue executing. "
    << "You can access the same window via its name. You can run event loop for a given period of time. " << endl
    << "Usage:"                                                                     << endl
    << "./launching_viz"                                                            << endl
    << endl;
}

int main(){
    help();
    viz::Viz3d myWindow("Demo");

    myWindow.spin();
    cout << "first event loop is over!\n" << endl;

    viz::Viz3d sameWindow = viz::getWindowByName("Demo");
    sameWindow.spinOnce(1, true);

    while(!sameWindow.wasStopped()){
        sameWindow.spinOnce(1, true);
    }

    cout << "Last event loop is over\n" << endl;
    return 0;
}
