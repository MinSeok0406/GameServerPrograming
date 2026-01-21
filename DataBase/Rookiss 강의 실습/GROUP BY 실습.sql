USE BaseballData;


-- Q) 2004년도에 가장 많은 홈런을 날린 팀은?
SELECT TOP 1 teamID, SUM(HR) AS homeRuns
FROM batting
WHERE yearID = 2004
GROUP BY teamID
ORDER BY homeRuns DESC;

-- Q) 2004년도에 200홈런 이상을 날린 팀의 목록?
SELECT teamID, SUM(HR) AS homeRuns
FROM batting
WHERE yearID = 2004
GROUP BY teamID
HAVING SUM(HR) >= 200
ORDER BY homeRuns DESC;

-- Q) 단일 년도에 가장 많은 홈런을 날린 팀은?
SELECT teamID, yearID, SUM(HR) AS homeRuns
FROM batting
GROUP BY teamID, yearID
ORDER BY homeRuns DESC;

-- FROM			책상에서
-- WHERE		공을
-- GROUP BY		색상별로 분류해서
-- HAVING		분류한 다음에 빨간색은 제외하고
-- SELECT		갖고 와서
-- ORDER BY		크기 별로 나열해주세요