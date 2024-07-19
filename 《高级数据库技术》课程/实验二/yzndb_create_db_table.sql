-- 数据库
CREATE DATABASE IF NOT EXISTS yzndb character set utf8mb4;
USE yzndb;

-- 城市（city）
CREATE TABLE city 
(
    city_id INT PRIMARY KEY,
    city_name VARCHAR(100) NOT NULL
);

-- 支行（branch）
CREATE TABLE branch 
(
    branch_id INT PRIMARY KEY,
    branch_name VARCHAR(100) NOT NULL,
    assets DECIMAL(15, 2) NOT NULL,
    city_id INT,
    FOREIGN KEY (city_id) REFERENCES city(city_id)
);

-- 部门（department）
CREATE TABLE department 
(
    department_id INT PRIMARY KEY,
    department_name VARCHAR(100) NOT NULL
);

-- 员工（employee）
CREATE TABLE employee 
(
    id_num VARCHAR(100) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    phone_num VARCHAR(100) NOT NULL,
    home_address VARCHAR(100) NOT NULL,
    start_work_date DATE NOT NULL,
    manager_id_num VARCHAR(100),
    department_id INT,
    branch_id INT,
    FOREIGN KEY (manager_id_num) REFERENCES employee(id_num),
    FOREIGN KEY (department_id) REFERENCES department(department_id),
    FOREIGN KEY (branch_id) REFERENCES branch(branch_id)
);

-- 客户（customer）
CREATE TABLE customer 
(
    id_num VARCHAR(100) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    street VARCHAR(100) NOT NULL,
    city_id INT,
    employee_id VARCHAR(100),
    FOREIGN KEY (city_id) REFERENCES city(city_id),
    FOREIGN KEY (employee_id) REFERENCES employee(id_num)
);

-- 账户（account）
CREATE TABLE account 
(
    account_num INT PRIMARY KEY,
    balance DECIMAL(15, 2) NOT NULL,
    branch_id INT,
    last_access_date DATE NOT NULL,
    account_type BOOLEAN NOT NULL,
    customer_id VARCHAR(100),
    FOREIGN KEY (branch_id) REFERENCES branch(branch_id),
    FOREIGN KEY (customer_id) REFERENCES customer(id_num)
);

-- 储蓄账户（saving_account）
CREATE TABLE saving_account 
(
    account_num INT PRIMARY KEY,
    interest_rate DECIMAL(5, 4) NOT NULL,
    FOREIGN KEY (account_num) REFERENCES account(account_num)
);

-- 贷款（loan）
CREATE TABLE loan 
(
    loan_num INT PRIMARY KEY,
    loan_amount DECIMAL(15, 2) NOT NULL
);

-- 贷款账户（loan_account）
CREATE TABLE loan_account 
(
    account_num INT PRIMARY KEY,
    overdraft_amount DECIMAL(15, 2) NOT NULL,
    branch_id2 INT,
    loan_num INT,
    FOREIGN KEY (account_num) REFERENCES account(account_num),
    FOREIGN KEY (branch_id2) REFERENCES branch(branch_id),
    FOREIGN KEY (loan_num) REFERENCES loan(loan_num)
);

-- 付款（payment）
CREATE TABLE payment 
(
    payment_id INT PRIMARY KEY,
    loan_num INT,
    payment_date DATE NOT NULL,
    payment_amount DECIMAL(15, 2) NOT NULL,
    FOREIGN KEY (loan_num) REFERENCES loan(loan_num)
);