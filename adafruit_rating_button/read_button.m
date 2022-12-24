c=characteristic(b,"180D","2A37")
c.DataAvailableFcn = @displayCharacteristicData


% cont=1;
% 
% tmp=zeros(2,20);
% while(cont)
%     tmp(:,1:end-1)=tmp(:,2:end);
%     tmp(:,end)=read(c)';
% 
% 
%     plot(tmp(2,:)); drawnow;
%     
%     pause(0.1); 
% 
% end;