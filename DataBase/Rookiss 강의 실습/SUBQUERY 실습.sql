USE BaseballData;

-- SubQuery (서브쿼리/하위쿼리)
-- SQL 명령문 안에 지정하는 하부 SELCET

SELECT TOP 1 playerID
FROM salaries
ORDER BY salary DESC;

SELECT *
FROM players
WHERE playerID = 'rodrial01'

-- 이것을 한 번에 하려면?
-- 단일행 서브쿼리
SELECT *
FROM players
WHERE playerID = (SELECT TOP 1 playerID FROM salaries ORDER BY salary DESC)

-- 다중행 -> IN 사용
SELECT *
FROM players
WHERE playerID IN (SELECT TOP 20 playerID FROM salaries ORDER BY salary DESC)

-- 서브쿼리는 WHERE에서 가장 많이 사용되지만, 나머지 구문에서도 사용 가능
SELECT (SELECT COUNT(*) FROM players) AS playerCount, (SELECT COUNT(*) FROM batting) AS battingCount;

-- INSERT에서도 사용 가능
SELECT *
FROM salaries
ORDER BY yearID DESC

-- INSERT INTO
INSERT INTO salaries
VALUES (2024, 'KOR', 'NL', 'MIN', (SELECT MAX(salary) FROM salaries))

-- INSERT SELECT
INSERT INTO salaries
SELECT 2024, 'KOR', 'NL', 'MINSE', (SELECT MAX(salary) FROM salaries);

INSERT INTO salaries_temp
SELECT yearID, playerID, salary FROM salaries;

SELECT *
FROM salaries_temp

----------------------------------------------------------

-- 상관 관계 서브쿼리
-- EXISTS, NOT EXISTS

SELECT *
FROM players
WHERE playerID IN (SELECT playerID FROM battingpost);

SELECT *
FROM players
WHERE EXISTS (SELECT playerID FROM battingpost WHERE battingpost.playerID = players.playerID)

