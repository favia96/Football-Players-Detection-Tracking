%% Tirocinio-Tesi: graphs tracking players (Federico Favia 2018)
clear all
close all
clc

%% lettura file txt col numero di blob contati in ogni frame
g1 = importdata('sassuolo_30sec_percentage_g1.csv');
g2 = importdata('sassuolo_30sec_percentage_g2.csv');
%g3 = importdata('sassuolo_30sec_percentage_g3.csv');
g4 = importdata('sassuolo_30sec_percentage_g4.csv');
%g5 = importdata('sassuolo_30sec_percentage_g5.csv');
g6 = importdata('sassuolo_30sec_percentage_g6.csv');
%g7 = importdata('sassuolo_30sec_percentage_g7.csv');
%g8 = importdata('sassuolo_30sec_percentage_g8.csv');
%g9 = importdata('sassuolo_30sec_percentage_g9.csv');
g10 = importdata('sassuolo_30sec_percentage_g10.csv');
%g11 = importdata('sassuolo_30sec_percentage_g11.csv');
%g12 = importdata('sassuolo_30sec_percentage_g12.csv');
g13 = importdata('sassuolo_30sec_percentage_g13.csv');
fps = 30; %framerate video
time = (0:1/fps:(g1.data(end,1)/fps) );
thresh = 65; %percentuale sotto alla quale il giocatore è identificato con poca accuratezza

%% graph
figure('units','normalized','outerposition',[0 0 1 1])
hold on, grid on
plot(time,g1.data(:,2),'-k','LineWidth',1) %black
plot(time,g2.data(:,2),'-r','LineWidth',1) %red
%plot(time,g3.data(:,2),'LineWidth',1)
plot(time,g4.data(:,2),'LineWidth',1)
%plot(time,g5.data(:,2),'LineWidth',1)
plot(time,g6.data(:,2),'LineWidth',1)
%plot(time,g7.data(:,2),'LineWidth',1)
%plot(time,g8.data(:,2),'LineWidth',1)
%plot(time,g9.data(:,2),'LineWidth',1)
plot(time,g10.data(:,2),'LineWidth',1)
%plot(time,g11.data(:,2),'LineWidth',1)
%plot(time,g12.data(:,2),'LineWidth',1)
plot(time,g13.data(:,2),'LineWidth',1)
plot(time,thresh*ones(size(time)),'LineWidth',1)
xlabel('time [s]')
ylabel('%accuracy')
title('#accuracy tracking')
legend('g1','g2','g4','g6','g10','g13','thresh','location','southeast') %nuovo3
saveas(gcf,'sassuolo_30sec.png')