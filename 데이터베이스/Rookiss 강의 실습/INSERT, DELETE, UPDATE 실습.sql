USE BaseballData;


-- INSERT DELETE UPDATE
SELECT *
FROM salaries
ORDER BY yearID DESC;

-- INSERT INTO [테이블명] VALUES (값, ...) -> 값 하나라도 빼먹으면 Error
INSERT INTO salaries
VALUES (2024, 'KOR', 'NL', 'MINSEOK', 1525124758)

-- INSERT INTO [테이블명](열, ...) VALUES (값, ...)
INSERT INTO salaries(yearID, teamID, playerID, lgID, salary)
VALUES (2024, 'KOR', 'MINSEOK2', 'NL', 2154245)

-- DELETE FROM [테이블명] WHERE [조건]
DELETE FROM salaries
WHERE yearID >= 2024

-- UPDATE [테이블명] SET [열 = 값, ...] WHERE [조건]
UPDATE salaries
SET salary *= 0.5
WHERE yearID = 2020


