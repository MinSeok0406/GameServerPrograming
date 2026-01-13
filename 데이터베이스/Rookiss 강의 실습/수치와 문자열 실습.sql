USE BaseballData;

SELECT 2021 - birthYear AS koreanAge
FROM players
WHERE deathYear IS NULL AND birthYear IS NOT NULL AND 2021 - birthYear <= 80
ORDER BY koreanAge;

-- FROM
-- WHERE
-- SELECT
-- ORDER BY
-- 이 순으로 시스템 적용

SELECT SUBSTRING('20200425', 1, 4);

SELECT TRIM('            HelloWorld');