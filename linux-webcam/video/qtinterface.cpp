#include "qtinterface.h"

int main (int argc, char **argv) {
unsigned char *mydata;
QApplication app(argc, argv);

drawingWindow *myapp = new drawingWindow;

//videoObject* myvideo = new videoObject;
//myvideo->takePicture();
//mydata = myvideo->dumpPicture();
//myapp->loadData(mydata, 10000);
myapp->show();

app.setMainWidget(myapp);
return(app.exec());
}
