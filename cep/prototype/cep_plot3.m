function [ output_args ] = cep_plot3( data1, data2, data3, name )
% function [ output_args ] = cep_plot3( data1, data2, data3 )
%
% takes 3 sets of independant data and plots a linear least squares approximation to each
% data
%
% Inputs  - data1 is the North component 
%         - data2 is the East component
%         - data3 is the Vertical component
%
% Outputs - a plot of 3 LLS lines and the corresponding data sets
% 


% calc the LLS lines
[A1 B1]=cep_lsline(data1(1,:),data1(2,:));
[A2 B2]=cep_lsline(data2(1,:),data2(2,:));
[A3 B3]=cep_lsline(data3(1,:),data3(2,:));

% calc corresponding Y vals
out1=A1*data1(1,:)+B1;
out2=A2*data2(1,:)+B2;
out3=A3*data3(1,:)+B3;

% render 3 plots
    subplot(3,1,1);
hold on;  % pause till all plots rendered
        errorbar(data1(1,:), data1(2,:), data1(3,:), 'b');
        plot(data1(1,:),data1(2,:),'.r', data1(1,:),out1,'m');
        title( strcat(name, ' : North') );
        ylabel('Position (um)');
    
    subplot(3,1,2);
hold off;
hold on;
        errorbar(data2(1,:), data2(2,:), data2(3,:), 'b');
        plot(data2(1,:),data2(2,:),'.r', data2(1,:),out2,'m');
        title( strcat(name, ' : East') );
        ylabel('Position (um)');
    
    subplot(3,1,3);
hold off;
hold on;
        errorbar(data3(1,:), data3(2,:), data3(3,:), 'b');
        plot(data3(1,:),data3(2,:),'.r', data3(1,:),out3,'m');
        title( strcat(name, ' : Vertical') );
        ylabel('Position (um)');
    
hold off; % ok, NOW draw the plots