USE BaseballData;

SELECT *
FROM dbo.players
WHERE birthYear IS NOT NULL
ORDER BY birthYear DESC, birthMonth DESC, birthDay DESC
OFFSET 100 ROWS FETCH NEXT 100 ROWS ONLY;	-- 100 ~ 200을 호출할 때 사용하는 문법