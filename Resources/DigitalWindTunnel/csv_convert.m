%% Import data from csv file.
clear all


% Initialize variables.
filename = 'CSV_Input.csv';
delimiter = ',';
startRow = 2;


% Read columns of data as text:
formatSpec = '%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%[^\n\r]';

% Open the text file.
fileID = fopen(filename,'r','n','UTF-8');


% Read columns of data according to the format.
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'TextType', 'string', 'HeaderLines' ,startRow-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');

% Close the text file.
fclose(fileID);



%% Convert the contents of columns to numbers.
raw = repmat({''},length(dataArray{1}),length(dataArray)-1);

for col=1:length(dataArray)-1
    raw(1:length(dataArray{col}),col) = mat2cell(dataArray{col}, ones(length(dataArray{col}), 1));
end

for col=[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17]
    % Converts text in the input cell array to numbers.
    rawData = dataArray{col};
    for row=1:size(rawData, 1)
        % Create a regular expression to detect and remove non-numeric prefixes and
        % suffixes.
        regexstr = '(?<prefix>.*?)(?<numbers>([-]*(\d+[\,]*)+[\.]{0,1}\d*[eEdD]{0,1}[-+]*\d*[i]{0,1})|([-]*(\d+[\,]*)*[\.]{1,1}\d+[eEdD]{0,1}[-+]*\d*[i]{0,1}))(?<suffix>.*)';
        try
            result = regexp(rawData(row), regexstr, 'names');
            numbers = result.numbers;
            
            % Detected commas in non-thousand locations.
            invalidThousandsSeparator = false;
            if numbers.contains(',')
                thousandsRegExp = '^[-/+]*\d+?(\,\d{3})*\.{0,1}\d*$';
                if isempty(regexp(numbers, thousandsRegExp, 'once'))
                    numbers = NaN;
                    invalidThousandsSeparator = true;
                end
            end
            % Convert numeric text to numbers.
            if ~invalidThousandsSeparator
                numbers = textscan(char(strrep(numbers, ',', '')), '%f');
                numericData(row, col) = numbers{1};
                raw{row, col} = numbers{1};
            end
        catch
            raw{row, col} = rawData{row};
        end
    end
end

% Create output variable.
CSVInput = cell2mat(raw);

% Clear temporary variables
clearvars filename delimiter startRow formatSpec fileID dataArray ans raw col numericData rawData row regexstr result numbers invalidThousandsSeparator thousandsRegExp;



%% Read the data.

% Height information of measurements (Column 1)
% Scale the data based on the CFD model
height = CSVInput(:,1)/CSVInput(end,1)*5;     % 5 == TopLength
% Interpolation required in TInF and add x and y info as the wind tunnel
% only provides the height information
height_3D = [zeros(length(height),1) zeros(length(height),1) height; zeros(length(height),1) zeros(length(height),1) height];
% Transpose the matrix for matlab fprintf
height_3D = transpose(height_3D);


% Mean velocity at the streamwise direction (Column 2)
velocity = CSVInput(:,2);
% Interpolation required in TInF
vel = repmat(velocity,2,1);
% Transpose the matrix for matlab fprintf
vel = transpose(vel);


% Reynolds stress tensor (6 columns from Column 3 to Column 8)
Reynolds_stress = CSVInput(:,3:8);
% Interpolation required in TInF
Rs = repmat(Reynolds_stress,2,1);
% Transpose the matrix for matlab fprintf
Rs = transpose(Rs);

% Length scales (9 columns from Column 9 to Column 17)
Length_scale = CSVInput(:,9:17);
% Interpolation required in TInF
Ls = repmat(Length_scale,2,1);
% Transpose the matrix for matlab fprintf
Ls = transpose(Ls);


%% Output to the txt files

% Locate the case directory to constant/boundaryData/inlet

cd Openfoam_digital_twin/constant/boundaryData/inlet


% Speficy the file name of points
fileID = fopen('points','w');

% Follow the OpenFOAM reading format
formatSpec = '(%.5f %.5f %.5f)\n';
fprintf(fileID,'(\n')
fprintf(fileID,formatSpec,height_3D)
fprintf(fileID,')')
fclose(fileID);


% Speficy the file name of velocities
fileID = fopen('U','w');

% Follow the OpenFOAM reading format
formatSpec = '%.5f\n';
fprintf(fileID,'(\n')
fprintf(fileID,formatSpec,vel)
fprintf(fileID,')')
fclose(fileID);


% Speficy the file name of Reynolds stress
fileID = fopen('R','w');

% Follow the OpenFOAM reading format
formatSpec = '(%.5f %.5f %.5f %.5f %.5f %.5f)\n';
fprintf(fileID,'(\n')
fprintf(fileID,formatSpec,Rs)
fprintf(fileID,')')
fclose(fileID);


% Speficy the file name of length scale
fileID = fopen('L','w');

% Follow the OpenFOAM reading format
formatSpec = '(%.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f)\n';
fprintf(fileID,'(\n')
fprintf(fileID,formatSpec,Ls)
fprintf(fileID,')')
fclose(fileID);
