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
    
    a_panel_p FLOAT,
    a_panel_v FLOAT,
    a_charger_i FLOAT,
    a_charger_v FLOAT,
    a_load_i FLOAT,

    b_panel_p FLOAT,
    b_panel_v FLOAT,
    b_charger_i FLOAT,
    b_charger_v FLOAT,
    b_load_i FLOAT
    
) DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;

DROP TABLE IF EXISTS PublishJobs;
CREATE TABLE PublishJobs (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    rawMessage_id INT UNSIGNED NOT NULL
) DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;


DROP TRIGGER IF EXISTS addPublishJobForRawMessagesInserts;

delimiter #

CREATE TRIGGER addPublishJobForRawMessagesInserts AFTER INSERT ON RawMessages
FOR EACH ROW
BEGIN
  INSERT INTO PublishJobs (rawMessage_id) values (new.id);
END#

delimiter ;
