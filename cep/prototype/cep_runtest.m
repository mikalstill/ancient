function foo = runtest( name )
% runtest( name )
%
% runs a test reading data from 3 files called name.dat1, name.dat2 and name.dat3
% these 3 data sets are then plotted with residuals and a linear regression
%
% INPUTS  - name is the name of the file omitting the extension
 
[data1, size1, h11, h12, h13] = loadfile( strcat(name, '.dat1') );
[data2, size2, h21, h22, h23] = loadfile( strcat(name, '.dat2') );
[data3, size3, h31, h32, h33] = loadfile( strcat(name, '.dat3') );

cep_plot3( data1, data2, data3, name );