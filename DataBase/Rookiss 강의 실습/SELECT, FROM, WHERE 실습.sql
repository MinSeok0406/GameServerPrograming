SELECT *
FROM BaseballData.dbo.players
WHERE birthYear != 1866 AND birthCountry != 'USA';

SELECT *
FROM BaseballData.dbo.players
WHERE birthYear LIKE '19%';

-- AND가 OR보다 우선순위가 높다
-- LIKE 문자열
-- % 임의의 문자열
-- _ 임의의 문자 1개