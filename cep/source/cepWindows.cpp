/*

 These methods will be placed into cepDataset .....soon.
 

*/


//doto_daniel: change the indices for array and vectors so they start at ZERO not ONE

void //todo_daniel: this will end being a return type .
cepDataset::doHam (double data, double startWindow, double winSize)
{
	/*
		todo_daniel: find out where this will be called from...called after doWindow or from doWindow 
		Gets a single hamming window value
		Imports:
		* data: a data element from the standard array of data [date,value,error]
		* startWindow: the date of the start of the hamming window
		* winSize: the size (in decimal years) of the window

		Exports:
		* value: the hamming value
		* weight: the weight of the value within the set
		todo_daniel: again here with two return types...maybe have them as members of the class with get and set	
		 
	*/

	weight = 0.54 - 0.46*cos(2*pi*((data(1)-startWindow)/(winSize)); //todo:daniel ask nick why 0.54 and .46...shouldn't be hard coded here.
	value  = data(2) * weight;

}//end doHam

void		//this will be a method in cepDataset -- will need to include math.h -- will most likely have a return type.
cepDataset::doWindow (double data[obs][2],double winSize, double overlap) //todo_daniel: once vector exists..fix this..wont need param data.
{
	/*
	Groups data into square windows with a pre-defined width. 
	Imports: 
	* data - matrix (observation x 3 - date, value, error) todo_daniel:vector
	* winSize - size of each window (deciaml years)
	* overlap - amount of overlap between each window

	Exports:
	* windowArray: Array of windowed data (3 * numWindows * largestWindow) todo_daniel: maybe vector here too!
	* numWindows: The number of seperate windows that were populated with data
	
	//todo_daniel: return 2 params? or maybe calculate numWindows in calling method?
	//todo_daniel: or maybe just call doHam from here sending params: startWindow and winSize

	*/

	int numSample; //number of smaples in the vector
	int nextFirstRecord; //
	int currentFirstRecord; //
	int numWindows; //the total number  of windows required
	int vecCtr; //vector counter
	double firstDate; //the first dat in the the data
	double lastDate; //the last date in the data
	double overlapWinSize; //
	
  // get timescale of data set 
  numSamples = sizeOf TheDataVector //length(data(1,:));//todo_daniel: correct notation for vector size...vector not defined yet
  firstdate = todo_daniel:vector[1][1]; //data(1,1);//todo_daniel: vector not defined yet..correct notation needed
  lastdate = todo_daniel:vector[1][numSamples];//todo_daniel: vector not defined yet..correct notation needed

  // For optimizing speed slightly 
  overlapWinSize = winSize * (1-overlap);

  // Work out number of windows 
  numWindows = ceil(((lastdate - firstdate)*(1+2*overlap))/winSize); //round up to nearest integer
	if (overlap != 0){
    numWindows -=1;
	}
  
  // Divide into windows 
  nextFirstRecord = 1;
  startWindow = todo_daniel:vector[1][1] - winSize; //totdo_daniel: vector notation
  for (int i=1; numWindows-1; i++){
	  // point vector counter to start of window 
    vecCtr = nextFirstRecord;
    currentFirstRecord = nextFirstRecord;

    // save array reference to start of window
    startWindow = startWindow + overlapWinSize;

    // populate first half of window
    while (todo_daniel:vector[1][vecCtr] < (startWindow+overlapWinSize)){
        for (int k=1; k<=3; k++){
            windowArray(k,i,vecCtr-(currentFirstRecord-1)) = todo_daniel:vector[k][vecCtr];
        }
        vecCtr += 1; // increment vector counter
		} //end while

    // mark start of next window for later use
    nextFirstRecord = vecCtr;

    // populate second half of window
    while (todo_daniel:vector[1][vecCtr] < (startWindow+winSize)){
        for (k=1; k<=3; k++){
            windowArray(k,i,vecCtr-(currentFirstRecord-1)) = todo_daniel:vector[k][vecCtr];
        }
        vecCtr += 1; // increment vector counter
		} //end while
  } // end for

  // populate final window with remaining data
  vecCtr = nextFirstRecord;
  currentFirstRecord = nextFirstRecord;
  startWindow = startWindow + winSize;
  while (vecCtr < numSamples){
    for (k=1; k<=3; k++){
      windowArray(k,numWindows,vecCtr-(currentFirstRecord-1)) = todo_daniel:vector[k][vecCtr];
    }
    vecCtr += 1;
	} //end while

}//end void cepDatset::doWindow

