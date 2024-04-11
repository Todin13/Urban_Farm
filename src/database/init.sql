-- Create the Experiment table
CREATE TABLE Experiment (
    ID INT PRIMARY KEY,
    Name VARCHAR(255)
);

-- Create the Sensors table
CREATE TABLE Sensors (
    ID INT PRIMARY KEY,
    Version VARCHAR(255)
);

-- Create the Experiment Data table
CREATE TABLE ExperimentData (
    ID SERIAL PRIMARY KEY,
    Sensor_ID INT,
    Experiment_ID INT,
    FOREIGN KEY (Sensor_ID) REFERENCES Sensors(ID),
    FOREIGN KEY (Experiment_ID) REFERENCES Experiment(ID)
);

-- Create the Warnings table
CREATE TABLE Warnings (
    ID SERIAL PRIMARY KEY,
    Sensor_ID INT,
    Experiment_ID INT,
    Type VARCHAR(255),
    FOREIGN KEY (Sensor_ID) REFERENCES Sensors(ID),
    FOREIGN KEY (Experiment_ID) REFERENCES Experiment(ID)
);

-- Create the Sensors Data table
CREATE TABLE SensorsData (
    ID SERIAL PRIMARY KEY,
    Sensor_ID INT,
    Sensor_Version VARCHAR(255),
    Plant_ID INT,
    Time TIMESTAMP,
    Temperature FLOAT,
    Humidity FLOAT,
--    FOREIGN KEY (Sensor_ID) REFERENCES Sensors(ID)
);
