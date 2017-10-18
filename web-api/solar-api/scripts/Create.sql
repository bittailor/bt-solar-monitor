DROP TABLE IF EXISTS RawMessages;
CREATE TABLE RawMessages (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    ts TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    message TEXT NOT NULL
) DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;

DROP TABLE IF EXISTS Measurements;
CREATE TABLE Measurements (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    ts TIMESTAMP DEFAULT 0,
    panel_a_i FLOAT,  
    panel_a_v FLOAT,  
    panel_b_i FLOAT,  
    panel_b_v FLOAT,
    battery_a_i FLOAT,  
    battery_a_v FLOAT,  
    battery_b_i FLOAT,  
    battery_b_v FLOAT, 
    load_i FLOAT,  
    load_v FLOAT,
    controller_i FLOAT,  
    controller_v FLOAT
) DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;

CREATE TABLE PublishJobs (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    rawMessage_id INT UNSIGNED NOT NULL
) DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;

delimiter #

CREATE TRIGGER addPublishJobForRawMessagesInserts AFTER INSERT ON RawMessages
FOR EACH ROW
BEGIN
  INSERT INTO PublishJobs (rawMessage_id) values (new.id);
END#

delimiter ;
