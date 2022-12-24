function displayCharacteristicData(src,evt)
    [data,timestamp] = read(src,'latest');
    disp(data);
    disp(timestamp);
end
