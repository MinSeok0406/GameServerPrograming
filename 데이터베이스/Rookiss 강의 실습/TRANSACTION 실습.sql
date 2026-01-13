USE GameDB;

SELECT *
FROM accounts

-- TRAN 명시하지 않으면, 자동으로 COMMIT
INSERT INTO accounts VALUES(1, 'MINSEOK', 100, GETUTCDATE(), GETUTCDATE());

-- BEGIN TRAN
-- COMMIT
-- ROLLBACK

-- 성공/실패 여부에 따라 COMMIT (=COMMIT을 수동으로 하겠다)
BEGIN TRAN;
	INSERT INTO accounts VALUES(1, 'MINSEOK', 100, GETUTCDATE(), GETUTCDATE());
ROLLBACK;

BEGIN TRAN;
	INSERT INTO accounts VALUES(2, 'MINSEOK2', 100, GETUTCDATE(), GETUTCDATE());
COMMIT;

-- 응용
BEGIN TRY
	BEGIN TRAN;
		INSERT INTO accounts VALUES(1, 'MINSEOK', 100, GETUTCDATE(), GETUTCDATE());
		INSERT INTO accounts VALUES(2, 'MINSEOK2', 100, GETUTCDATE(), GETUTCDATE());
	COMMIT;
END TRY
BEGIN CATCH
	IF @@TRANCOUNT > 0	-- 현재 활성화된 트랜잭션의 수를 반환
		ROLLBACK
END CATCH

-- TRAN 사용할 때 주의할 점
-- TRAN 안에는 꼭!!! 원자적으로 실행될 애들만 넣자
-- C# List<Player> List<Salary> 원자적으로 수정 -> lock을 잡고 실행 -> writelock (상호배타적인 락) readlock (공유 락)

