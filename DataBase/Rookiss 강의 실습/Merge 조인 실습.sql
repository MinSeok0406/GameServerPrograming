USE BaseballData;

SET STATISTICS TIME ON;
SET STATISTICS IO ON;
SET STATISTICS PROFILE ON;

-- Merge(병합) 조인 = Sort Merge(정렬 병합) 조인

SELECT *
FROM players AS p
	INNER JOIN salaries AS s
	ON p.playerID = s.playerID;

-- One-To-Many (outer가 unique해야 함 => PK, Unique)
-- Merge 조인도 조건이 붙는다 (outer가 unique해야 함)
-- 일일히 Random Access -> Clustered Scan 후 정렬

SELECT *
FROM schools AS s
	INNER JOIN schoolsplayers AS p
	ON s.schoolID = p.schoolID;

-- 오늘의 결론 --
-- Merge -> Sort Merge 조인
-- 1) 양쪽 집합을 Sort(정렬)하고 Merge(병합)한다
	-- 이미 정렬된 상태라면 Sort는 생략 (특히, Clustered로 물리적 정렬된 상태라면 Best)
	-- 정렬할 데이터가 너무 많으면 GG -> Hash
-- 2) Random Access 위주로 수행되진 않는다
-- 3) Many-to-Many(다대다)보다는 One-to-Many(일대다) 조인에 효과적
