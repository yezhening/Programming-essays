-- 创建客户
CREATE TABLE client 
(
    id INT PRIMARY KEY AUTO_INCREMENT,
    balance DECIMAL(10, 2) NOT NULL
);

-- 创建客户星级
CREATE TABLE client_star 
(
    id INT PRIMARY KEY,
    star INT NOT NULL,
    FOREIGN KEY (id) REFERENCES client(id)
);

-- 插入客户
INSERT INTO client (balance) 
VALUES (10000.00), (5000.00), (1500.00), (500.00), (-100.00);

-- 插入客户星级
DELIMITER //

CREATE PROCEDURE assign_client_star()
BEGIN
    DECLARE client_num INT;
    DECLARE cur_id INT DEFAULT 1;
    DECLARE cur_balance DECIMAL(10, 2);
    DECLARE cur_star INT;
    
    SELECT COUNT(*) 
    INTO client_num 
    FROM client;
  
    WHILE cur_id <= client_num DO
        SELECT balance 
        INTO 
        cur_balance 
        FROM client 
        WHERE id = cur_id;
        
        IF cur_balance > 10000 THEN
            SET cur_star = 5;
        ELSEIF cur_balance > 5000 THEN
            SET cur_star = 4;
        ELSEIF cur_balance > 1000 THEN
            SET cur_star = 3;
        ELSEIF cur_balance > 0 THEN
            SET cur_star = 2;
        ELSE
            SET cur_star = 1;
        END IF;
        
        INSERT INTO client_star (id, star) 
        VALUES (cur_id, cur_star);
        
        SET cur_id = cur_id + 1;
    END WHILE;
END //

DELIMITER ;

CALL assign_client_star();

-- 查询客户星级
SELECT * FROM client_star;