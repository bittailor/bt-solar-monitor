CREATE TABLE RawMessages (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    ts TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    message text NOT NULL
) DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;