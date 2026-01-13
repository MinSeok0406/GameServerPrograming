USE BaseballData;

-- 조인 원리
	-- 1) Nested Loop (NL) 조인
	-- 2) Merge (병합) 조인
	-- 3) Hash (해시) 조인

-- Merge
SELECT *
FROM players AS p
	INNER JOIN salaries AS s
	ON p.playerID = s.playerID;

-- NL
SELECT TOP 5 *
FROM players AS p
	INNER JOIN salaries AS s
	ON p.playerID = s.playerID;

-- Hash
SELECT *
FROM salaries AS s
	INNER JOIN teams AS t
	ON s.teamID = t.teamID;

-- NL
SELECT *
FROM players AS p
	INNER JOIN salaries AS s
	ON p.playerID = s.playerID
	OPTION(LOOP JOIN);

SELECT *
FROM players AS p
	INNER JOIN salaries AS s
	ON p.playerID = s.playerID
	OPTION(FORCE ORDER, LOOP JOIN);

-- 오늘의 결론 --
-- NL 특징
-- 먼저 액세스한 (OUTER)테이블의 로우를 차례 차례 -> (INNER)테이블에 랜덤 엑세스
-- (INNER) 테이블에 인덱스가 없으면 노답
-- 부분범위 처리에 좋다 (EX. TOP 5)