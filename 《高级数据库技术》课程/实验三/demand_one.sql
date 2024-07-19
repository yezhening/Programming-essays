-- 数据库
use yzndb;

-- 人
CREATE TABLE yzn_person 
(
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL
);

-- 插入数据
DELIMITER //

CREATE PROCEDURE insert_yzn_person(starter int, total int)
BEGIN
    DECLARE i INT;
    SET i = starter;
    
    WHILE i <= total DO
        INSERT INTO yzn_person (name) 
        VALUES (CONCAT('虚构名称', i));
        
        SET i = i + 1;
    END WHILE;
END //

DELIMITER ;

CALL insert_yzn_person(1, 1000000);
-- 从1开始，插入100 0000一百万行数据
CALL insert_yzn_person(1000001, 10000000);
-- 从1000001开始，再插入1000 0000 - 100 0000 = 900 0000九千万行数据，共插入1000 0000千万行数据

-- 查询数量
SELECT count(*) 
FROM yzn_person;