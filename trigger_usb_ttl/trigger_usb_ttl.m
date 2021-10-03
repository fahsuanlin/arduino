device = serialport("/dev/tty.usbmodem143201",9600);
for i=1:10
    writeline(device,sprintf('%d',mod(i,2)))
    pause(1);
end;
delete(device)