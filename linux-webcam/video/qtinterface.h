#include <qapplication.h>
#include <qpainter.h>
#include <qwidget.h>
#include "read-camera.h"

class drawingWindow : public QWidget {
	public:
	   drawingWindow();
	   ~drawingWindow();	
	private:
	QPainter* dataCanvas;
	virtual void paintEvent(QPaintEvent *);
	videoObject* myvideo;
};


void drawingWindow::paintEvent(QPaintEvent *) {
	int w,h;
	rgbColor mycolor;
	w = myvideo->returnPictureWidth();
	h = myvideo->returnPictureHeight();	
	dataCanvas = new QPainter;
	dataCanvas->begin(this);
	
	for (int i=0; i < h; i++) 
	  for (int a=0; a < w; a++) {
		mycolor = myvideo->returnNextRGB();
		dataCanvas->setPen(QColor(mycolor.r,mycolor.g,mycolor.b));	
		dataCanvas->drawPoint(a,i);
	}
	
	dataCanvas->end();
}

drawingWindow::drawingWindow() {
	int w,h;
	myvideo = new videoObject;			
	myvideo->takePicture();
	w = myvideo->returnPictureWidth();
	h = myvideo->returnPictureHeight();	
	resize(w,h);

}

drawingWindow::~drawingWindow() {
}
