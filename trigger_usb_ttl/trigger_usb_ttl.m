close all; clear all;

devices=serialportlist;

%device = serialport("/dev/tty.usbmodem143201",9600);
device = serialport(devices(end),9600);

for i=1:11
    writeline(device,sprintf('%d',mod(i,2)))
    pause(1);
end;
delete(device)