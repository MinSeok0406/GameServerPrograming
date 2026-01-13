USE BaseballData;

-- COUNT -> 매개변수로 * 붙일 수 있는 유일한 함수
-- SUM
-- AVG
-- MIN
-- MAX

-- 집계함수는 집합이 null이면 무시

SELECT COUNT(*)
FROM players

-----------------

SELECT DISTINCT birthYear
FROM players
ORDER BY birthYear

-- 집계함수(DISTINCT 집합)
SELECT COUNT(DISTINCT birthCity)
FROM players;

-----------------

-- 선수들의 평균 weight 구해보자
SELECT AVG(weight)
FROM players;

SELECT SUM(weight) / COUNT(weight)
FROM players;


-- 선수들의 평균 weight 구해보자
-- 단, weight = NULL인 경우라면 weight = 0로 친다
SELECT AVG(CASE WHEN weight IS NULL THEN 0 ELSE weight END)
FROM players;


-- MIN MAX
SELECT MIN(weight), MAX(weight)
FROM players;