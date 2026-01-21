
-- 북마크 룩업 -> 많이 발생할수록 메모리에 부하가 와서 안좋음

-- Clustered의 경우 Index Seek가 느릴 수가 없다.
-- NonClustered의 경우, 데이터가 Leaf Page에 없다
-- 따라서 한 번 더 타고 가야함
	-- 1) RID -> Heap Table (Bookmark Lookup)
	-- 2) Key -> Clustered

-- 강제로 인덱스를 이용하는 방법
-- SELECT *
-- FROM '테이블이름' WITH(INDEX('만든 인덱스 이름'))

-- 룩업을 줄이는 방법

-- (옵션 1)
-- Covered Index 사용 -> 복합 인덱스 

-- (옵션 2)
-- CREATE NONCLUSTERED INDEX '만들 인덱스 이름'
-- ON '테이블 이름'('개체...') INCLUDE ('개체...');

-- 위와 같이 해도 힘들다면

-- (옵션 3)
-- Clustered Index 활용 
-- But Clustered Index는 테이블당 1개만 사용 가능