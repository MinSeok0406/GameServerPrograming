SELECT TOP (1000) [time]
  FROM [BaseballData].[dbo].[DateTimeTest]

  SELECT CAST('20200425 05:03:22' AS DATETIME)

  -- 현재시간 표시
  SELECT GETDATE();
  SELECT CURRENT_TIMESTAMP

  -- YYYYMMDD
  -- YYYYMMDD hh:mm:ss.nnn
  -- YYYY-MM-DDThh:mm