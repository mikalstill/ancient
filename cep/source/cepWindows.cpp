/* 

    Implementation for CEP windowing

	At this stage this is still largely nick's matlab code
 
    Groups data into square windows with a pre-defined width. 
	Imports: 
	* data - matrix (observations x 2)
	* winSize - size of each window (deciaml years)
	* overlap - amount of overlap between each window

	Exports:
	* m_windowArray: Array of windowed data (3 * numWindows * largestWindow)
	* m_numWindows: The number of seperate windows that were populated with data
	
	//todo_daniel: return 2 params? or maybe calculate numWindows in calling method?

*/
cepWindows::cepWindows (double data[obs][2],double winSize, double overlap):
{
  m_data[][] = data[][]; // todo_daniel: proper copying
			//maybe use memcpy..or just refer to data itself..might be better
  m_winSize = winSize;
  m_overlap = overlap;
  m_windowArray (-1);
  m_numWindows (-1);
}

void
cepWindows::doWindow ()
{

  // get timescale of data set 
  numSamples = length(data(1,:));//totdo_daniel: find the size of an array without stepping through
  firstdate = m_data[1][1]; //data(1,1);
  lastdate = data[1],[numSamples];

  // For optimizing speed slightly 
  overlapWinSize = winSize * (1-overlap);

  // Work out number of windows 
  value = (((lastdate - firstdate)*(1+2*overlap))/winSize);
  numWindows = ceil(value)
  if (overlap ~= 0)
    numWindows = numWindows - 1
  end

  // Divide into windows 
  nextFirstRecord = 1;
  startWindow = data(1,1) - winSize;
  for i=1:numWindows-1
    // point array counter to start of window 
    j = nextFirstRecord;
    currentFirstRecord = nextFirstRecord;
    // save array reference to start of window
    startWindow = startWindow + overlapWinSize;
    // populate first half of window
    while data(1,j) < startWindow+overlapWinSize
        for k=1:3
            windowArray(k,i,j-(currentFirstRecord-1)) = data(k,j);
        end
        // increment array counter
        j = j + 1;
    end //while

    // mark start of next window for later use
    nextFirstRecord = j;
    // populate second half of window
    while data(1,j) < startWindow+winSize
        for k=1:3
            windowArray(k,i,j-(currentFirstRecord-1)) = data(k,j);
        end
        // increment array counter
        j = j + 1;
    end //while
  end //for

  // populate final window with remaining data
  j = nextFirstRecord;
  currentFirstRecord = nextFirstRecord;
  startWindow = startWindow + winSize;
  while j < numSamples
    for k=1:3
      windowArray(k,numWindows,j-(currentFirstRecord-1)) = data(k,j);
    end
    j = j + 1;
  end //while

}
