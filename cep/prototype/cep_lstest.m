function x=cep_llstest
% function x=cep_llstest
%
% test the linear least squares function
%
% uses the following 2 sets to generate a function plot and approximates
% these with a linear least squares fit
% X=[ 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16];
% Y=[ 1  3  2  4  5  7  6  4  7  9  8  5  3  6  8  5];

X=[ 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16];
Y=[ 1  3  2  4  5  7  6  4  7  9  8  5  3  6  8  5];
[A B]=cep_lsline(X,Y);
R=A*X+B;

plot(X,Y,'ro',X,R,'b')
xlabel('X Value');
ylabel('Y Value');
title('Test of linear least squares fit to a data set');