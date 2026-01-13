USE BaseballData;


SELECT *,
	CASE birthMonth
		WHEN 1 THEN N'겨울'
		WHEN 2 THEN N'겨울'
		WHEN 3 THEN N'봄'
		WHEN 4 THEN N'봄'
		WHEN 5 THEN N'봄'
		WHEN 6 THEN N'여름'
		WHEN 7 THEN N'여름'
		WHEN 8 THEN N'여름'
		WHEN 9 THEN N'가을'
		WHEN 10 THEN N'가을'
		WHEN 11 THEN N'가을'
		WHEN 12 THEN N'겨울'
		ELSE N'몰라용'
	END AS birthSeason
FROM players;

SELECT *,
	CASE 
		WHEN birthMonth <= 2 THEN N'겨울'
		WHEN birthMonth <= 5 THEN N'봄'
		WHEN birthMonth <= 8 THEN N'여름'
		WHEN birthMonth <= 11 THEN N'가을'
		ELSE N'겨울'
	END AS birthSeason
FROM players;