/*
 These methods will be placed into cepDataset .....soon.
*/

cepDataset
cepDataset::doHam (double datRow[3], double startWindow, double winSize)
{
	/*	Imports:
		* datRow: a data row from the standard array of data [date,value,error]
		* startWindow: the date of the start of the hamming window
		* winSize: the size (in decimal years) of the window

		Exports:
		* hamValue: the hamming value
		* hamWeight: the weight of the value within the set
	*/
	double hamWeight=0;
	double hamValue=0;

	hamWeight = 0.54 - 0.46*cos(2*pi*((datRow[0]-startWindow)/(winSize)); //todo:daniel ask nick why 0.54 and .46...shouldn't be hard coded here.
	hamValue  = datRow[1] * weight;
	cepDataset ham =  new cepDataset(hamValue, hamWeight);
	return ham;
	//totdo:daniel- need to ensure a constructor to deal with this returning business above.

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
	int dataVectorRow; //vector counter
	int col, row; //loop counters
	double firstDate; //the first dat in the the data
	double lastDate; //the last date in the data
	double overlapWinSize; //
	cep_datarow windowRow;
	vector<cep_datarow> windowData;//this is windowsArray
	vector<cep_datarow> dataCopy//need a copy of dataCopy
	
	dataCopy = m_data;
	
  // get timescale of data set 
  numSamples = dataCopy.size() 
  firstdate = dataCopy.pop_first(); //data(1,1);//todo_daniel: vector not defined yet..correct notation needed
  lastdate = dataCopy.pop_back() //[numSamples][0];
	//todo_daniel: these pops could cause a problems since they're REMOVING the elements.
	
  // For optimizing speed slightly 
  overlapWinSize = winSize * (1-overlap);

  // Work out number of windows 
  numWindows = ceil(((lastdate - firstdate)*(1+2*overlap))/winSize); //round up to nearest integer
	if (overlap != 0){
    numWindows -=1;//totdo:daniel - may not need this line.
	}
  
  // Divide into windows 
  nextFirstRecord = 0;
  startWindow = dataCopy[0][0] - winSize; 
  for (row=0; numWindows; i++){
	// point vector counter to start of window 
    dataVectorRow = nextFirstRecord;
    currentFirstRecord = nextFirstRecord;

    // save array reference to start of window
    startWindow = startWindow + overlapWinSize;

    // populate first half of window
    while (dataCopy[dataVectorRow][0] < (startWindow+overlapWinSize))
	{
        for (col=0; col<3; col++)
		{
				//windowArray(k,i,vecCtr-(currentFirstRecord)) = dataCopy[vecCtr][k];
				windowData[row][col][vecCtr-currentFirstRecord] = dataCopy[dataVectorRow][col];
        }
        dataVectorRow += 1; // increment vector row counter
		} //end while

		// mark start of next window for later use
		nextFirstRecord = dataVectorRow;

		// populate second half of window
		while dataCopy[dataVectorRow][0] < (startWindow+winSize))
		{
			for (col=0; col<3; col++)
			{
				//windowArray(k,i,vecCtr-(currentFirstRecord)) = dataCopy[vecCtr][k];
				windowData[row][col][dataVectorRow-currentFirstRecord] = dataCopy[dataVectorRow][col];
			}
			dataVectorRow += 1; // increment vector counter
	} //end while
  } // end for


  // populate final window with remaining data
  dataVectorRow = nextFirstRecord; 
  currentFirstRecord = nextFirstRecord;
  startWindow = startWindow + winSize;
  while (dataVectorRow < numSamples)
  {
    for (col=0; col<3; col++)
	{
		//windowArray(k,numWindows,vecCtr-(currentFirstRecord)) = dataCopy[vecCtr][k];
		windowData[numWindows][col][dataVectorRow-currentFirstRecord] = dataCopy[dataVectorRow][col];
    }
    dataVectorRow += 1;
  } //end while

	//create cep_windowData populated withthe windowData vector and numWindows for return to the calling place
	cepDatset cep_windowData = new cepDataset(windowData, numWindows);
	return cep_windowData;
	
}//end void cepDatset::doWindow

