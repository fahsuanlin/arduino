close all; clear all;

buffer=[];
tmp1=[];
tmp2=[];
tmp3=[];
tmp4=[];

flag_stop=0;

devices=serialportlist;
%device = serialport("/dev/tty.usbmodem143201",9600);
device = serialport(devices(end),57600); %Typically the last device is for the plugged-in trigger box. It may vary though.
%configureTerminator(device,"LF");


%clear USB port buffer
while(~flag_stop)
    tmp1=read(device,1,'uint8');
    if(isempty(tmp1))
        flag_stop=1;
    end;
end;


%clear USB port buffer
for i=1:12
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % send trigger here
    writeline(device,sprintf('%d',mod(i,2)));
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % read incoming data for time stamps
    tmp1=read(device,1,'uint8');
    if(~isempty(tmp1))
        tmp2=read(device,1,'uint8');
        tmp3=read(device,1,'uint8');
        tmp4=read(device,1,'uint8');
    
        buffer(i) = bitand(tmp1,255)+bitand(tmp2,255)*256+bitand(tmp3,255)*256*256+bitand(tmp4,255)*256*256*256;
    end; 
    
    fprintf('received by box for input [%d] at %d (ms)\n',i,buffer(i));     
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    pause(1);
end;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%end of experiment
writeline(device,sprintf('%d',9));

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% read incoming data for time stamps
tmp1=read(device,1,'uint8');
if(~isempty(tmp1))
    tmp2=read(device,1,'uint8');
    tmp3=read(device,1,'uint8');
    tmp4=read(device,1,'uint8');
    
    n_ttl = bitand(tmp1,255)+bitand(tmp2,255)*256+bitand(tmp3,255)*256*256+bitand(tmp4,255)*256*256*256;
else
    n_ttl=0;
end;

fprintf('[%d] trigger sent!\n',n_ttl);

for index=1:n_ttl
    tmp1=read(device,1,'uint8');
    if(~isempty(tmp1))
        tmp2=read(device,1,'uint8');
        tmp3=read(device,1,'uint8');
        tmp4=read(device,1,'uint8');
        
        ttl_time(index) = bitand(tmp1,255)+bitand(tmp2,255)*256+bitand(tmp3,255)*256*256+bitand(tmp4,255)*256*256*256;
    end;
    fprintf('TTL [%d]: %d (ms)\n',index,ttl_time(index));
end;


%close port
delete(device)

