% MATLAB script to receive serial input from Arduino

% Clear previous serial connections
clear;
clc;

% Define serial port parameters
port = '/dev/cu.usbmodem143201'; % Replace with your Arduino COM port
baudRate = 9600; % Must match Arduino's baud rate

% Create the serial port object
arduino = serialport(port, baudRate);

% Optional: Configure read settings
configureTerminator(arduino, "LF"); % Use line feed ('\n') as terminator
arduino.Timeout = 10; % Timeout in seconds

% Open a file to save data (optional)
fileID = fopen('./serial_data.txt', 'w');

% Start receiving data
disp('Receiving data from Arduino. Press Ctrl+C to stop.');

try
    while true
        % Read data from the serial port
        if arduino.NumBytesAvailable > 0
            data = readline(arduino); % Read one line of data
            disp(data); % Display the data in MATLAB's Command Window

            % Save the data to a file (optional)
            fprintf(fileID, '%s\n', data);
        end
    end
catch
    % Cleanup when interrupted
    disp('Stopped receiving data.');
    fclose(fileID);
    delete(arduino);
    clear arduino;
end
