-- 城市
INSERT INTO city (city_id, city_name) 
VALUES (1, '虚构城市');

-- 支行
INSERT INTO branch (branch_id, branch_name, assets, city_id) 
VALUES (1, '虚构支行', 100000.00, 1);

-- 部门
INSERT INTO department (department_id, department_name) 
VALUES (1, '虚构部门');

-- 员工
INSERT INTO employee (id_num, name, phone_num, home_address, start_work_date, manager_id_num, department_id, branch_id) 
VALUES ('e1234567890', 'yzn', '1234567890', '虚构地址', '2024-01-01', 'e1234567890', 1, 1);

-- 客户
INSERT INTO customer (id_num, name, street, city_id, employee_id) 
VALUES ('c1234567890', '虚构客户姓名', '虚构街道', 1, 'e1234567890');

-- 账户
INSERT INTO account (account_num, balance, branch_id, last_access_date, account_type, customer_id) 
VALUES (1, 1000.00, 1, '2024-01-01', TRUE, 'c1234567890');

-- 储蓄账户
INSERT INTO saving_account (account_num, interest_rate) 
VALUES (1, 0.02);

-- 贷款
INSERT INTO loan (loan_num, loan_amount) 
VALUES (1, 5000.00);

-- 贷款账户
INSERT INTO loan_account (account_num, overdraft_amount, branch_id2, loan_num) 
VALUES (1, 500.00, 1, 1);

-- 付款
INSERT INTO payment (payment_id, loan_num, payment_date, payment_amount) 
VALUES (1, 1, '2024-01-01', 100.00);

-- 百名客户
DELIMITER //

CREATE PROCEDURE insert_customer()
BEGIN
    DECLARE i INT DEFAULT 1;
    
    WHILE i <= 100 DO
        INSERT INTO customer (id_num, name, street, city_id, employee_id) 
        VALUES (CONCAT('c', i), CONCAT('虚构客户姓名', i), CONCAT('虚构街道', i), 1, 'e1234567890');
        SET i = i + 1;
    END WHILE;
END //

-- 一万条以上的存贷款流水数据
CREATE PROCEDURE insert_loan()
BEGIN
    DECLARE i INT DEFAULT 1;
    
    WHILE i <= 10000 DO
        INSERT INTO payment (payment_id, loan_num, payment_date, payment_amount) 
        VALUES (i+1, 1, DATE_ADD(NOW(), INTERVAL -i DAY), FLOOR(RAND() * 500));
        SET i = i + 1;
    END WHILE;
END //

DELIMITER ;

CALL insert_customer();
CALL insert_loan();