function [A,B] = lsline(X,Y)
% function [A,B] = lsline(X,Y)
%
%Calculates a linear least squares approximation to a set of data
%
% Input  - X is the 1 x n absyssa vector
%        - Y is the 1 x n ordinate vector
% Output - A is the Coefficient of x in Y=Ax+B
%        - B is the Constant Coefficient in Y=Ax+B
%
% This code is sourced from ...
%         "Numerical Methods using Matlab, 3rd edition"
%          by Matthews & Fink, Prentice Hall, 1992

% calcualte the means of the axes
xmean=mean(X);
ymean=mean(Y);

% calc x^2 and xy for each
sumx2=(X-xmean)*(X-xmean)';
sumxy=(X-xmean)*(Y-ymean)';

% calculate A and B
A=sumxy/sumx2;
B=ymean-A*xmean;