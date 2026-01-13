USE BaseballData;

-- 1) 생략

-- 2) ORDER BY
-- 원인) ORDER BY 순서로 정렬을 해야 하니깐 Sort
SELECT *
FROM players
ORDER BY college;

-- 3) GROUP BY
-- 원인) 집계를 하기 위해
SELECT college, COUNT(college)
FROM players
WHERE college LIKE 'C%'
GROUP BY college;

-- 4) DISTINCT
-- 원인) 중복 제거
SELECT DISTINCT college
FROM players
WHERE college LIKE 'C%'

-- 5) UNION
-- 원인) 중복 제거
SELECT college
FROM players
WHERE college LIKE 'B%'
UNION
SELECT college
FROM players
WHERE college LIKE 'C%'

-- 6) RANKING WINDOWS FUNCTION
-- 원인) 집계를 하기 위해
SELECT ROW_NUMBER() OVER (ORDER BY college)
FROM players;

-- 오늘의 결론 --

-- Sorting (정렬) 을 줄이자

-- O(NLogN) -> DB는 데이터가 어마어마하게 많다
-- 너무 용량이 커서 가용 메모리로 커버가 안되면 -> 디스크까지 찾아간다
-- Sorting이 언제 일어나는지 파악하고 있어야 함

-- Sorting이 일어날 때
-- 1) SORT MERGE JOIN
	-- 원인) 알고리즘 특성상 Merge하기 전에 Sort를 해야 함
-- 2) ORDER BY
-- 3) GROUP BY
-- 4) DISTINCT
-- 5) UNION
-- 6) RANKING WINDOWS FUNCTION
-- 7) MIN MAX

-- INDEX를 잘 활용하면, Sorting을 하지 않아도 됨.