function result = gpsgui(option, breaks, edits)
% GPSGUI Function to control gui plots
%
% Get the current graphics handles
% Define constants we need
CompName = ['North';'East ';'Up   '];
%comphndl = findobj(gcbf, 'Tag','PopupMenu1');
sigmahndl = findobj(gcbf, 'Tag','EditText1'); 
sigmacut = str2double(get(sigmahndl,'String')); 
outlierhndl = findobj(gcbf, 'Tag','PopupMenu1');
outlierval = get(outlierhndl,'Value');
outlierlist = get(outlierhndl,'String');  
outliercut = outlierlist(outlierval);
if outliercut == 'all'
  outliercut = '';
else
  outliercut = str2num(outliercut);
end
sitehndl = findobj(gcbf, 'Tag','Listbox1');
siten = get(sitehndl,'Value');
sites = get(sitehndl,'UserData');
name = sites(siten,:);
%
% Generate the file name
%fname = strcat('mb_',name,'.dat',num2str(comp,'%1d'));
fname = [strcat('mb_',name,'.dat1'); strcat('mb_',name,'.dat2'); strcat('mb_',name,'.dat3')];

switch(option)
	case 'Load'  ;
       clear DataN DataE DataU
       for i = 1:3
       h(i) = findobj(gcf, 'Tag',strcat('Axis',num2str(i,'%1d')));
		[fid, message] = fopen(fname(i,:),'r');
		if fid == -1
			disp(message)
			ok = 0;
			return
		end	
		% Now we will try to read the file.  These
		% files have three header lines and then data
		%
		% Read the header lines
		H1 = fgetl(fid); 
       H2 = fgetl(fid); 
       H3 = fgetl(fid);
		% The rest of the file is numeric with three values per lines and we
		% can read all at once
		[rData,tot] = fscanf(fid,' %f %f %f');
		% Close the data file at this point
		fclose(fid); 
		% Now reshape the Data into 3:tot/3 array
		num = tot/3;
		rData = reshape(rData,3,num);
       if i == 1
         DataN(1,:) = rData(1,:);  
	  	  DataN(2,:) = rData(2,:);
		  DataN(3,:) = rData(3,:);
         axes(h(i)); 
%	  	  set(gcbf,'UserData',DataN);
%	  	  DataN(2,:) = (DataN(2,:)-mean(DataN(2,:)))*1000 ; % Convert to mm
	  	  DataN(2,:) = DataN(2,:)*1000 ; % Convert to mm
		  DataN(3,:) = DataN(3,:)*1000 ; % Convert to mm
		  % Remove the mean and plot 
		  hold off
		  hp = plot(DataN(1,:),DataN(2,:)-mean(DataN(2,:)),'bo'); % Save the graphics handle
		  hold on
		  errorbar(DataN(1,:),DataN(2,:)-mean(DataN(2,:)),DataN(3,:),'ro'); 
		  set(hp,'MarkerFaceColor',[.8 .8 0]);
		  set(hp','Tag','PlottedDataN');
         set(gca,'Tag',strcat('Axis',num2str(i,'%1d')));
	  	  set(gca,'UserData',DataN);
  	      ylabel('Position (mm)');
         Comptext = CompName(i,:);
         PlotTitle = sprintf('Data %s %s',strrep(name,'_','\_'),Comptext);
		  title(PlotTitle);
       elseif i == 2 
         DataE(1,:) = rData(1,:);  
	  	  DataE(2,:) = rData(2,:);
		  DataE(3,:) = rData(3,:);
         axes(h(i)) 
%	  	  set(gcbf,'UserData',DataE);
%	  	  DataE(2,:) = (DataE(2,:)-mean(DataE(2,:)))*1000 ; % Convert to mm
	  	  DataE(2,:) = DataE(2,:)*1000 ; % Convert to mm
		  DataE(3,:) = DataE(3,:)*1000 ; % Convert to mm
		  % Remove the mean and plot 
		  hold off
		  hp = plot(DataE(1,:),DataE(2,:)-mean(DataE(2,:)),'bo'); % Save the graphics handle
		  hold on
		  errorbar(DataE(1,:),DataE(2,:)-mean(DataE(2,:)),DataE(3,:),'ro'); 
		  set(hp,'MarkerFaceColor',[.8 .8 0]);
		  set(hp','Tag','PlottedDataE');
         set(gca,'Tag',strcat('Axis',num2str(i,'%1d')));
	  	  set(gca,'UserData',DataE);
		  ylabel('Position (mm)');
         Comptext = CompName(i,:);
         PlotTitle = sprintf('Data %s %s',strrep(name,'_','\_'),Comptext);
		  title(PlotTitle);
       elseif i == 3
         DataU(1,:) = rData(1,:);  
	  	  DataU(2,:) = rData(2,:);
		  DataU(3,:) = rData(3,:);
         axes(h(i)) 
%	  	  set(gcbf,'UserData',DataU);
%	  	  DataU(2,:) = (DataU(2,:)-mean(DataU(2,:)))*1000 ; % Convert to mm
	  	  DataU(2,:) = DataU(2,:)*1000 ; % Convert to mm
		  DataU(3,:) = DataU(3,:)*1000 ; % Convert to mm
		  % Remove the mean and plot 
		  hold off
		  hp = plot(DataU(1,:),DataU(2,:)-mean(DataU(2,:)),'bo'); % Save the graphics handle
		  hold on
		  errorbar(DataU(1,:),DataU(2,:)-mean(DataU(2,:)),DataU(3,:),'ro'); 
		  set(hp,'MarkerFaceColor',[.8 .8 0]);
		  set(hp','Tag','PlottedDataU');
         set(gca,'Tag',strcat('Axis',num2str(i,'%1d')));
	  	  set(gca,'UserData',DataU);
		  ylabel('Position (mm)');
         Comptext = CompName(i,:);
         PlotTitle = sprintf('Data %s %s',strrep(name,'_','\_'),Comptext);
		  title(PlotTitle);
       end
       end
		% Initialize the Breaks and Edit values
		Initbe
	case 'Append' 
       h = findobj(gcf, 'Tag','Axis1');
		DataN = get(h,'UserData');
       h = findobj(gcf, 'Tag','Axis2');
		DataE = get(h,'UserData');
       h = findobj(gcf, 'Tag','Axis3');
		DataU = get(h,'UserData');
       for i = 1:3
       h(i) = findobj(gcf, 'Tag',strcat('Axis',num2str(i,'%1d')));
		[fid, message] = fopen(fname(i,:),'r');
		if fid == -1
			disp(message)
			ok = 0;
			return
		end	
		% Now we will try to read the file.  These
		% files have three header lines and then data
		%
		% Read the header lines
		H1 = fgetl(fid);
		H2 = fgetl(fid);
		H3 = fgetl(fid);
		% The rest of the file is numeric with three values per lines and we
		% can read all at once
		[rData,tot] = fscanf(fid,' %f %f %f');
		% Close the data file at this point
		fclose(fid);
		% Now reshape the Data into 3:tot/3 array
		num = tot/3;
		rData = reshape(rData,3,num);
       if i == 1
	  	  rData(2,:) = rData(2,:)*1000 ; % Convert to mm
		  rData(3,:) = rData(3,:)*1000 ; % Convert to mm
	 	  DataN = [DataN, rData];
         axes(h(i)); 
%	  	  set(gcbf,'UserData',DataN);
		  % Remove the mean and plot 
		  hold off
		  hp = plot(DataN(1,:),DataN(2,:)-mean(DataN(2,:)),'bo'); % Save the graphics handle
		  hold on
		  errorbar(DataN(1,:),DataN(2,:)-mean(DataN(2,:)),DataN(3,:),'ro'); 
		  set(hp,'MarkerFaceColor',[.8 .8 0]);
		  set(hp','Tag','PlottedDataN');
         set(gca,'Tag',strcat('Axis',num2str(i,'%1d')));
	  	  set(gca,'UserData',DataN);
  	      ylabel('Position (mm)');
         Comptext = CompName(i,:);
         PlotTitle = sprintf('Data %s %s',strrep(name,'_','\_'),Comptext);
		  title(PlotTitle);
       elseif i == 2 
	  	  rData(2,:) = rData(2,:)*1000 ; % Convert to mm
		  rData(3,:) = rData(3,:)*1000 ; % Convert to mm
	 	  DataE = [DataE, rData];
         axes(h(i)) 
%	  	  set(gcbf,'UserData',DataE);
		  % Remove the mean and plot 
		  hold off
		  hp = plot(DataE(1,:),DataE(2,:)-mean(DataE(2,:)),'bo'); % Save the graphics handle
		  hold on
		  errorbar(DataE(1,:),DataE(2,:)-mean(DataE(2,:)),DataE(3,:),'ro'); 
		  set(hp,'MarkerFaceColor',[.8 .8 0]);
		  set(hp','Tag','PlottedDataE');
         set(gca,'Tag',strcat('Axis',num2str(i,'%1d')));
	  	  set(gca,'UserData',DataE);
		  ylabel('Position (mm)');
         Comptext = CompName(i,:);
         PlotTitle = sprintf('Data %s %s',strrep(name,'_','\_'),Comptext);
		  title(PlotTitle);
       elseif i == 3
	  	  rData(2,:) = rData(2,:)*1000 ; % Convert to mm
		  rData(3,:) = rData(3,:)*1000 ; % Convert to mm
	 	  DataU = [DataU, rData];
         axes(h(i)) 
%	  	  set(gcbf,'UserData',DataU);
		  % Remove the mean and plot 
		  hold off
		  hp = plot(DataU(1,:),DataU(2,:)-mean(DataU(2,:)),'bo'); % Save the graphics handle
		  hold on
		  errorbar(DataU(1,:),DataU(2,:)-mean(DataU(2,:)),DataU(3,:),'ro'); 
		  set(hp,'MarkerFaceColor',[.8 .8 0]);
		  set(hp','Tag','PlottedDataU');
         set(gca,'Tag',strcat('Axis',num2str(i,'%1d')));
	  	  set(gca,'UserData',DataU);
		  ylabel('Position (mm)');
         Comptext = CompName(i,:);
         PlotTitle = sprintf('Data %s %s',strrep(name,'_','\_'),Comptext);
		  title(PlotTitle);
       end
       end
	case 'Break'
%		Get the times at which we should add breaks in the
%		times series 
		breaks = get(gcbo,'UserData');
		button = 3;
		while button == 3;
			[x, y, button] = ginput(1);
			breaks = [breaks; x];
			plot([x x],ylim,'y');
		end	
		set(gcbo,'UserData',breaks);
	case 'Edit'
		edits = get(gcbo,'UserData');
       h = findobj(gcf, 'Tag','Axis1');
		DataN = get(h,'UserData');
       h = findobj(gcf, 'Tag','Axis2');
		DataE = get(h,'UserData');
       h = findobj(gcf, 'Tag','Axis3');
		DataU = get(h,'UserData');
%		Data = get(gcbf,'UserData'); 

		hn = findobj(gcbf, 'Tag','PlottedDataN');
		he = findobj(gcbf, 'Tag','PlottedDataE');
		hu = findobj(gcbf, 'Tag','PlottedDataU'); 
%		Get the data actually plotted on the screen		
		xdataN = get(hn,'Xdata') ;
		ydataN = get(hn,'Ydata') ;		
		ysizeN = ylim;
		ytolN = abs(ysizeN(2)-ysizeN(1))/100;
		xdataE = get(he,'Xdata') ;
		ydataE = get(he,'Ydata') ;		
		ysizeE = ylim;
		ytolE = abs(ysizeE(2)-ysizeE(1))/100;
		xdataU = get(hu,'Xdata') ;
		ydataU = get(hu,'Ydata') ;		
		ysizeU = ylim;
		ytolU = abs(ysizeU(2)-ysizeU(1))/100;

		xsize = xlim;
		xtol = abs(xsize(2)-xsize(1))/500;

		button = 3;
		
		while button == 3;
			[x, y, button] = ginput(1);  
           h = gca;
           ax = get(h,'Tag');
			for i = 1:length(xdataN)
				if (ax == 'Axis1' & abs(xdataN(i)-x) < xtol & abs(ydataN(i)-y) < ytolN) | ...
                  (ax == 'Axis2' & abs(xdataE(i)-x) < xtol & abs(ydataE(i)-y) < ytolE) | ...
                  (ax == 'Axis3' & abs(xdataU(i)-x) < xtol & abs(ydataU(i)-y) < ytolU)
%					We found point plotted, now find the time in                                                                                                                 
%					in the raw data, k is the point number in the
%					in the raw data set.
					k = i ;
					while abs(xdataN(i)-DataN(1,k)) > xtol/100
						k = k+1;
					end
					edits = [edits, k];
                   for j = 1:3
                      h(j) = findobj(gcf, 'Tag',strcat('Axis',num2str(j,'%1d')));
                      axes(h(j)); 
                      if j == 1
				  	     plot(DataN(1,k),ydataN(i),'rx','MarkerSize',12); 
                      elseif j == 2
				  	     plot(DataE(1,k),ydataE(i),'rx','MarkerSize',12); 
                      elseif j == 3
				         plot(DataU(1,k),ydataU(i),'rx','MarkerSize',12); 
                      end
                   end
				end
			end		
		end	
		set(gcbo,'UserData',edits);	
	case 'BlockEdit'
		zoom off;		% Turn off zoom in case it is on
		hedt = findobj(gcbf, 'Tag','Pushbutton4');
		edits = get(hedt,'UserData');
%		hp = findobj(gcbf, 'Tag','PlottedData');

       h = findobj(gcf, 'Tag','Axis1');
		DataN = get(h,'UserData');
       h = findobj(gcf, 'Tag','Axis2');
		DataE = get(h,'UserData');
       h = findobj(gcf, 'Tag','Axis3');
		DataU = get(h,'UserData');
%		Data = get(gcbf,'UserData'); 

%		Get the data actually plotted on the screen
		hn = findobj(gcbf, 'Tag','PlottedDataN');
		he = findobj(gcbf, 'Tag','PlottedDataE');
		hu = findobj(gcbf, 'Tag','PlottedDataU'); 
%		Get the data actually plotted on the screen		
		xdataN = get(hn,'Xdata') ;
		ydataN = get(hn,'Ydata') ;		
		xdataE = get(he,'Xdata') ;
		ydataE = get(he,'Ydata') ;		
		xdataU = get(hu,'Xdata') ;
		ydataU = get(hu,'Ydata') ;		
		
%		xdata = get(hp,'Xdata') ;
%		ydata = get(hp,'Ydata') ;		
%       Wait until user presses the mouse button
		k = waitforbuttonpress;
		point1 = get(gca,'CurrentPoint');    % button down detected
		finalRect = rbbox;                   % return Figure units
		point2 = get(gca,'CurrentPoint');    % button up detected
		point1 = point1(1,1:2);              % extract x and y
		point2 = point2(1,1:2);
		p1 = min(point1,point2);             % calculate locations
		offset = abs(point1-point2);         % and dimensions

		ax = get(gca,'Tag');
       if ax == 'Axis1'
		  for i = 1:length(xdataN)
			if xdataN(i) > p1(1) & xdataN(i) < p1(1)+offset(1) & ...
			   ydataN(i) > p1(2) & ydataN(i) < p1(2)+offset(2)
%				We found point plotted, now find the time in
%				in the raw data, k is the point number in the
%				in the raw data set.
				k = i ;
				while xdataN(i) ~= DataN(1,k) 
					k = k+1;
				end
				edits = [edits, k];
               for j = 1:3
                 h(j) = findobj(gcf, 'Tag',strcat('Axis',num2str(j,'%1d')));
                 axes(h(j));
                 if j == 1
				    plot(DataN(1,k),ydataN(i),'rx','MarkerSize',12); 
                 elseif j == 2
				    plot(DataE(1,k),ydataE(i),'rx','MarkerSize',12); 
                 elseif j == 3
				    plot(DataU(1,k),ydataU(i),'rx','MarkerSize',12); 
                 end
               end
%				plot(Data(1,k),ydata(i),'rx','MarkerSize',12);
			end
		  end	
       elseif ax == 'Axis2'
		  for i = 1:length(xdataE)
			if xdataE(i) > p1(1) & xdataE(i) < p1(1)+offset(1) & ...
			   ydataE(i) > p1(2) & ydataE(i) < p1(2)+offset(2)
%				We found point plotted, now find the time in
%				in the raw data, k is the point number in the
%				in the raw data set.
				k = i ;
				while xdataE(i) ~= DataE(1,k) 
					k = k+1;
				end
				edits = [edits, k];
               for j = 1:3
                 h(j) = findobj(gcf, 'Tag',strcat('Axis',num2str(j,'%1d')));
                 axes(h(j));
                 if j == 1
				    plot(DataN(1,k),ydataN(i),'rx','MarkerSize',12); 
                 elseif j == 2
				    plot(DataE(1,k),ydataE(i),'rx','MarkerSize',12); 
                 elseif j == 3
				    plot(DataU(1,k),ydataU(i),'rx','MarkerSize',12); 
                 end
               end
%				plot(Data(1,k),ydata(i),'rx','MarkerSize',12);
			end
		  end	
       elseif ax == 'Axis3'
		  for i = 1:length(xdataU)
			if xdataU(i) > p1(1) & xdataU(i) < p1(1)+offset(1) & ...
			   ydataU(i) > p1(2) & ydataU(i) < p1(2)+offset(2)
%				We found point plotted, now find the time in
%				in the raw data, k is the point number in the
%				in the raw data set.
				k = i ;
				while xdataU(i) ~= DataU(1,k) 
					k = k+1;
				end
				edits = [edits, k];
               for j = 1:3
                 h(j) = findobj(gcf, 'Tag',strcat('Axis',num2str(j,'%1d')));
                 axes(h(j));
                 if j == 1
				    plot(DataN(1,k),ydataN(i),'rx','MarkerSize',12); 
                 elseif j == 2
				    plot(DataE(1,k),ydataE(i),'rx','MarkerSize',12); 
                 elseif j == 3
				    plot(DataU(1,k),ydataU(i),'rx','MarkerSize',12); 
                 end
               end
%				plot(Data(1,k),ydata(i),'rx','MarkerSize',12);
			end
		  end
       end	
		set(hedt,'UserData',edits);		
		
	case 'Detrend'
		breakhndl = findobj(gcbf, 'Tag','Pushbutton5');	
		breaks = get(breakhndl,'UserData');
		breaks = checkbr(breaks);
		edithndl = findobj(gcbf, 'Tag','Pushbutton4');	
		edits = get(edithndl,'UserData');

       for k = 1:3  
       Comptext = CompName(k,:);
       PlotTitle = sprintf('Data %s %s',strrep(name,'_','\_'),Comptext);
       if k == 1 
         h1 = findobj(gcf, 'Tag','Axis1');
		  DataN = get(h1,'UserData');
         h2 = findobj(gcf, 'Tag','Axis2');
         DataE = get(h2,'UserData'); 
         h3 = findobj(gcf, 'Tag','Axis3');
		  DataU = get(h3,'UserData');
     	  for i = 1:length(DataN)
			 if (DataN(3,i) > sigmacut) | (DataE(3,i) > sigmacut) | (DataU(3,i) > sigmacut)
              edits = [edits, i];
            end
         end
         edits = unique(edits); 
       end

       iter = 1;
       while iter == 1
       if k == 1 
          axes(h1);
          Data = DataN;
       elseif k == 2
          axes(h2);
          Data = DataE;
       elseif k == 3
          axes(h3);
          Data = DataU;
       end

%		Data = get(gcbf,'UserData');
		eds = ones(1,length(Data));
		eds = logical(eds);
		for i = 1:length(edits)
			eds(edits(i)) = 0;
		end
		Data = Data(:,eds);

%		Do the least squares fit.  Set the time
%		reference to be the mid piont of the data
		meanT = mean(Data(1,:));
		nb = length(breaks);
		np = nb+2 ;
%		Form the partial derivatives for the estimation
%		Dimension first (for speed).  Parameters are:
%		1    -- Offset at MeanT
%		2    -- Linear rate (m/yr)
%		3-np -- Values of the offsets at the break times.
       clear a
		a = zeros(length(Data(1,:)),np);
		for i = 1:length(Data)
			p(1) = 1 ; p(2) = Data(1,i)-meanT;			
			for j = 1:nb
				if( Data(1,i) > breaks(j) )
					p(j+2) = 1;
				else
					p(j+2) = 0;
				end
			end  
			a(i,:) = p;
		end 

% add weight to a
       clear atw
       sumw = 0;
       for i = 1:np
          for j = 1:length(Data(1,:))  
             w = zeros(1,length(Data(1,:))) ; 
             w(j) = 1/(Data(3,j)*Data(3,j)) ;
             atw(i,j) = a(:,i)'*w' ;
             if i == 1
               sumw = sumw + w(j);
             end
          end
       end 

%		Do the Least squares solution.  From pre-fit
%		Normal equations and solve.  Compute statistics
%		from the residuals to the fit.
%		neq = a'*a ;
		neq = atw*a ;
%		In case we have breaks that can not be determined
%		add a small apriori sigma to the diagonal for the
%		for the break estimates
		for i = 3:np
			if neq(i,i) ~= 0
				neq(i,i) = neq(i,i)*(1+1.d-6);
			else
				neq(i,i) = 1;
			end 
		end
		
%		bvec = a'*Data(2,:)';
		bvec = atw*Data(2,:)';

		soln = inv(neq)*bvec ;
		res = (Data(2,:) - (a*soln)');
%		res = (Data(2,:) - (a*soln)')*1000 ;
%		Compute RMS scatter of residuals
		numdata = length(res);
       chisqr = 0;
       for i = 1:numdata
         chisqr = chisqr + ( res(i)^2 / Data(3,i)^2 );
       end
       nrms = sqrt(chisqr/(numdata-np));
       wrms = sqrt((numdata/(numdata-np))*chisqr/sumw);
%		resrms = sqrt(res*res'/numdata) ;
%		cov = inv(neq)*resrms^2 ;
		cov = inv(neq)*nrms^2 ;
%		Data(3,:) = Data(3,:);
%		Data(3,:) = Data(3,:)*1000;

% Look for outliers
       edtime = '';
    	for i = 1:length(Data)
		   if  ( abs(res(i))-(outliercut*Data(3,i)) > 0 )
             edtime = [edtime, Data(1,i)];
          end
       end
       if ( length(edtime) > 0 )
         edtime = unique(edtime);
         for i = 1:length(DataN)
            for j = 1:length(edtime)
              if edtime(j) == DataN(1,i)
                edits = [edits, i];
              end
            end
         end
         edits = unique(edits);
       else
         iter = 0;
       end
       end

		hold off		
		errorbar(Data(1,:),res,Data(3,:),'ro'); 
		hold on
		hp = plot(Data(1,:),res,'bo'); % Save the graphics handle
		set(hp,'MarkerFaceColor',[1 1 0]);
       if k == 1
  	      set(hp','Tag','PlottedDataN');
  	      set(gca,'UserData',DataN);
       elseif k == 2
  	      set(hp','Tag','PlottedDataE');
  	      set(gca,'UserData',DataE);
       elseif k == 3
   	  set(hp','Tag','PlottedDataU');
  	      set(gca,'UserData',DataU);
       end
%		set(hp','Tag','PlottedData');
		ylabel('Position (mm)'); 
		title(PlotTitle);
       set(gca,'Tag',strcat('Axis',num2str(k,'%1d')));
%		Show where the breaks are on the plot
		for i = 1:length(breaks)
			x = breaks(i);
			plot([x x],ylim,'g');
		end

%      Add statistics to plot
       xsize = xlim;
		ysize = ylim;

%      Add the 0 line and 3 sigma lines to the plot
       hl = line(xsize,[0 0]','Color','b','LineWidth',1);
       hl = line(xsize,[wrms*3 wrms*3],'Color','g','LineWidth',1);
       hl = line(xsize,[wrms*-3 wrms*-3]','Color','g','LineWidth',1);
       
%		Write out the solution 
		fprintf(1,'Detrend of %s %s WRMS scatter %8.2f mm NRMS %6.2f from %5d data\n', ...
			name, Comptext, wrms, nrms, length(res));
		stat_txt = sprintf('WRMS: %8.2f mm NRMS: %6.2f #: %5d', wrms, nrms, length(res));

		fprintf(1,'Solution:\n')

		fprintf(1,'Rate              %10.2f ± %6.2f mm/yr\n', ...
			soln(2), sqrt(cov(2,2)));
		rate_txt = sprintf('  Rate: %8.2f ± %6.2f mm/yr ',soln(2), sqrt(cov(2,2)));

%		fprintf(1,'Rate              %10.2f ± %6.2f mm/yr\n', ...
%			soln(2)*1000, sqrt(cov(2,2)));
%		Now write out breaks
       break_txt = '' ;
		for i = 1:length(breaks)
%			get the fractional part of the year and convert to month and
%			day
			yr = fix(breaks(i));
			fyr = mod(breaks(i),1); 
			if mod(yr,4) == 0 
				doy = fyr*366 + 1; 
			else
				doy = fyr*365 + 1;
			end
			ser = datenum(yr,1,doy); 
			bdate = datevec(ser); 
			fprintf(1,'Break %4d %2d %2d  %10.2f ± %6.2f mm\n',...
				bdate(1),bdate(2),bdate(3),soln(2+i), ...
				sqrt(cov(2+i,2+i)))

           break_txt = strcat(break_txt,sprintf(' Break %4d %2d %2d  %10.2f ± %6.2f mm ',...
				bdate(1),bdate(2),bdate(3),soln(2+i),sqrt(cov(2+i,2+i)))) ;

%			fprintf(1,'Break %4d %2d %2d  %10.2f ± %6.2f mm\n',...
%				bdate(1),bdate(2),bdate(3),soln(2+i)*1000, ...
%				sqrt(cov(2+i,2+i)))
		end
       return_txt = sprintf('\n') ;
       text(xsize(1),ysize(2),strcat(stat_txt,rate_txt,break_txt,return_txt)) ;
	
		end
       set(edithndl,'UserData',edits);		

	case 'Span'
		zoom off;		% Turn off zoom in case it is on
		breakhndl = findobj(gcbf, 'Tag','Pushbutton5');	
		breaks = get(breakhndl,'UserData');
		breaks = checkbr(breaks);
		edithndl = findobj(gcbf, 'Tag','Pushbutton4');	
		edits = get(edithndl,'UserData');

%       Wait until user presses the mouse button
		k = waitforbuttonpress;
		point1 = get(gca,'CurrentPoint');    % button down detected
		finalRect = rbbox;                   % return Figure units
		point2 = get(gca,'CurrentPoint');    % button up detected
		point1 = point1(1,1:2);              % extract x and y
		point2 = point2(1,1:2);
		p1 = min(point1,point2);             % calculate locations
		offset = abs(point1-point2);        % and dimensions

       for k = 1:3
         if k == 1
           h = findobj(gcf, 'Tag','Axis1');
		    DataN = get(h,'UserData');
		    Data  = get(h,'UserData');
         elseif k == 2
           h = findobj(gcf, 'Tag','Axis2');
           DataE = get(h,'UserData'); 
		    Data  = get(h,'UserData');
         elseif k == 3
           h = findobj(gcf, 'Tag','Axis3');
		    DataU = get(h,'UserData');
		    Data  = get(h,'UserData');
         end
         axes(h);

		  eds = ones(1,length(Data));
		  eds = logical(eds);
		  for i = 1:length(edits)
			eds(edits(i)) = 0;
		  end
		  Data = Data(:,eds);

         l = 0 ;
		  for i = 1:length(Data(1,:))
		    if Data(1,i) > point1(1) & Data(1,i) < point2(1)
%				We found point that needs to be plotted
             l = l+1 ;
             data_span(1:3,l) = Data(1:3,i) ;
           end
         end
           
         hold off		
         errorbar(data_span(1,:),data_span(2,:),data_span(3,:),'ro'); 
	      hold on
		  hp = plot(data_span(1,:),data_span(2,:),'bo'); % Save the graphics handle
         if k == 1
  	        set(hp','Tag','PlottedDataN');
  	        set(gca,'UserData',DataN);
         elseif k == 2
  	        set(hp','Tag','PlottedDataE');
  	        set(gca,'UserData',DataE);
         elseif k == 3
   	    set(hp','Tag','PlottedDataU');
  	        set(gca,'UserData',DataU);
         end
		  ylabel('Position (mm)'); 
         Comptext = CompName(k,:);
         PlotTitle = sprintf('Data %s %s',strrep(name,'_','\_'),Comptext);
		  title(PlotTitle);
         set(gca,'Tag',strcat('Axis',num2str(k,'%1d')));
       end

	case 'Save'
		breakhndl = findobj(gcbf, 'Tag','Pushbutton5');	
		breaks = get(breakhndl,'UserData');
		breaks = checkbr(breaks);
		edithndl = findobj(gcbf, 'Tag','Pushbutton4');	
		edits = get(edithndl,'UserData');
       h = findobj(gcf, 'Tag','Axis1');
	    Data  = get(h,'UserData');

		[fid, message] = fopen('output.file','a');
		if fid == -1
			disp(message)
			ok = 0;
			return
		end

       edits_sort = sort(edits);
       [head,tail] = strtok(name,'_');
       rename = strrep(name,tail,'_XCL');
		for i = 1:length(edits_sort)
%  	get the fractional part of the year and convert to month and day
			yr = fix(Data(1,edits_sort(i)));
			fyr = mod(Data(1,edits_sort(i)),1);
			if mod(yr,4) == 0 
				doy = fyr*366 + 1;
			else
				doy = fyr*365 + 1;
			end
			ser = datenum(yr,1,doy);
			bdate = datevec(ser);
			fprintf(fid,' rename %s %s %4d %2d %2d 00 00 %4d %2d %2d 24 00  \n', ...
           name,rename,bdate(1),bdate(2),bdate(3),bdate(1),bdate(2),bdate(3));
       end

       if length(breaks) ~= 0
         breaks = [breaks;max(Data(1,:))];
%         breaks = [breaks;Data(1,length(Data))]; Wrong - requires data are in time order SCM
         breaks_sort = sort(breaks);
         [head,tail] = strtok(name,'_');
	      for i = 1:length(breaks_sort)-1
%  	    get the fractional part of the year and convert to month and day
			  yrs = fix(breaks_sort(i));
			  yre = fix(breaks_sort(i+1));
			  fyrs = mod(breaks_sort(i),1);
			  fyre = mod(breaks_sort(i+1),1);
			  if mod(yrs,4) == 0 
		  		doys = fyrs*366 + 1;
			  else
				doys = fyrs*365 + 1;
			  end
			  if mod(yre,4) == 0 
		  		doye = fyre*366 + 1;
			  else
				doye = fyre*365 + 1;
			  end
			  sers = datenum(yrs,1,doys);
			  sere = datenum(yre,1,doye);
			  bdates = datevec(sers);
			  bdatee = datevec(sere);
             newtail = strcat('_',num2str(i,'%1d'),'PS');
             rename = strrep(name,tail,newtail);
             if ( mod(doys,1) > 0.5 )
               bhours = [24 00];
             else
               bhours = [00 00];
             end
             if ( mod(doye,1) > 0.5 )
               bhoure = [24 00];
             else
               bhoure = [00 00];
             end 
             if i < length(breaks_sort)-1
		        fprintf(fid,' rename %s %s %4d %2d %2d %2d %2d %4d %2d %2d %2d %2d \n', ...
               name,rename,bdates(1),bdates(2),bdates(3),bhours(1),bhours(2),bdatee(1), ...
               bdatee(2),bdatee(3),bhoure(1),bhoure(2));
             else
		        fprintf(fid,' rename %s %s %4d %2d %2d %2d %2d \n', ...
               name,rename,bdates(1),bdates(2),bdates(3),bhours(1),bhours(2));
             end
%             if i == 1
%			    fprintf(fid,' rename %s %s %4d %2d %2d 00 00 %4d %2d %2d 24 00 \n', ...
%               name,rename,bdates(1),bdates(2),bdates(3),bdatee(1),bdatee(2),bdatee(3));
%             else
%			    fprintf(fid,' rename %s %s %4d %2d %2d 24 00 %4d %2d %2d 24 00 \n', ...
%               name,rename,bdates(1),bdates(2),bdates(3),bdatee(1),bdatee(2),bdatee(3));
%             end
         end
       end

   fclose(fid); 
   end

function Initbe
% INITBE Initialized the breaks and edit arrays
%
% Set the breaks to a null array, and save call back
clear breaks
breaks = [];
breakhndl = findobj(gcbf, 'Tag','Pushbutton5');	
set(breakhndl,'UserData',breaks);
%
% Initialize and save the edits
clear edits
edits = [];
edithndl = findobj(gcbf, 'Tag','Pushbutton4');	
set(edithndl,'UserData',edits);

function unbrk = checkbr(breaks)
% CHECKBR Check for duplicate times with breaks
%
ed = ones(1,length(breaks));
for i = 1:length(breaks)-1
	for j = i+1:length(breaks)
		if breaks(j) == breaks(i)
			ed(j) = 0;
			fprintf(1,' Removing duplicate break %d\n',j)
		end
	end
end
ed = logical(ed);
unbrk = breaks(ed);


