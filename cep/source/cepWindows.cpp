/*

 These methods will be placed into cepDataset .....soon.
 Vector not yet iMPLEMENTED FOR WINDOWaRRAY.	or data.

*/


//doto_daniel: change the indices for array and vectors so they start at ZERO not ONE

void //todo_daniel: this will end being a return type .
cepDataset::doHam (double startWindow, double winSize)
{
	/*	Imports:
		* startWindow: the date of the start of the hamming window
		* winSize: the size (in decimal years) of the window

		Exports:
		* value: the hamming value
		* weight: the weight of the value within the set
		todo_daniel: again here with two return types...maybe have them as members of the class with get and set	
		
		Uses m_data: a data element from the standard array of data [date,value,error]
	*/
	double weight=0;
	double value=0;
	weight = 0.54 - 0.46*cos(2*pi*((data(1)-startWindow)/(winSize)); //todo:daniel ask nick why 0.54 and .46...shouldn't be hard coded here.
	value  = data(2) * weight;
	cepDataset ham =  new cepDataset();
	return ham;

}//end doHam

void		//this will be a method in cepDataset -- will need to include math.h -- will most likely have a return type.
cepDataset::doWindow (double winSize, double overlap) //todo_daniel: once vector exists..fix this..wont need param data.
{
	/*
	Groups data into square windows with a pre-defined width. 
	Imports: 
	* winSize - size of each window (deciaml years)
	* overlap - amount of overlap between each window

	Exports:
	* windowArray: Array of windowed data (3 X numWindows X largestWindow) todo_daniel: maybe vector here too!
	* numWindows: The number of seperate windows that were populated with data
	*/

	int numSamples; //number of smaples in the vector
	int nextFirstRecord; //start of next window
	int currentFirstRecord; //start of current window
	int numWindows; //the total number  of windows required
	int vecCtr; //vector counter
	double firstDate; //the first dat in the the data
	double lastDate; //the last date in the data
	double overlapWinSize; //
	cep_datarow windowRow;
	vector<cep_datarow> windowData; //this is windowArray

	
  // get timescale of data set 
  numSamples = m_data.size() 
  firstdate = m_data.pop_first(); //data(1,1);//todo_daniel: vector not defined yet..correct notation needed
  lastdate = m_data.pop_back() //[numSamples][0];
	//todo_daniel: these pops could cause a problems since they're REMOVING the elements.
	

  // For optimizing speed slightly 
  overlapWinSize = winSize * (1-overlap);

  // Work out number of windows 
  numWindows = ceil(((lastdate - firstdate)*(1+2*overlap))/winSize); //round up to nearest integer
	if (overlap != 0){
    numWindows -=1;
	}
  
  // Divide into windows 
  nextFirstRecord = 0;
  startWindow = m_data[0][0] - winSize; 
  for (int i=0; numWindows; i++){
	// point vector counter to start of window 
    vecCtr = nextFirstRecord;
    currentFirstRecord = nextFirstRecord;

    // save array reference to start of window
    startWindow = startWindow + overlapWinSize;

    // populate first half of window
    while (m_data[vecCtr][0] < (startWindow+overlapWinSize)){
        for (int k=0; k<3; k++){

						//windowArray(k,i,vecCtr-(currentFirstRecord)) = m_data[vecCtr][k];
						//totdo:daniel - an iterator here? to point to appropriate vector location?
						//todo:daniel -these indeces k, i etc have to be change to proper format
        }
        vecCtr += 1; // increment vector counter
		} //end while

    // mark start of next window for later use
    nextFirstRecord = vecCtr;

    // populate second half of window
    while m_data[vecCtr][0] < (startWindow+winSize)){
        for (k=0; k<3; k++){
            //windowArray(k,i,vecCtr-(currentFirstRecord)) = m_data[vecCtr][k];
						//totdo:daniel - an iterator here? to point to appropriate vector location?
						//todo:daniel -these indeces k, i etc have to be change to proper format					
        }
        vecCtr += 1; // increment vector counter
		} //end while
  } // end for


  // populate final window with remaining data
  vecCtr = nextFirstRecord; 
  currentFirstRecord = nextFirstRecord;
  startWindow = startWindow + winSize;
  while (vecCtr < numSamples){
    for (k=0; k<3; k++){
      //windowArray(k,numWindows,vecCtr-(currentFirstRecord)) = m_data[vecCtr][k];
			//totdo:daniel - an iterator here? to point to appropriate vector location?
    }
    vecCtr += 1;
  } //end while

  cepDatset windowData = new cepDataset(windowArray, numWindows);
  return windowData;

}//end void cepDatset::doWindow

