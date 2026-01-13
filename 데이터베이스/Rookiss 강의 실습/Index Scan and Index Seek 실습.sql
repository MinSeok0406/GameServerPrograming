
-- 인덱스 접근 방식 (Access)
-- Index Scan vs Index Seek
-- Index Scan -> LEAF PAGE 순차적으로 검색, 보통 인덱스를 활용하지 않는 방법
-- Index Seek -> 인덱스를 활용해서 데이터를 검색하는 방법

-- 인덱스 정보 확인
-- EXEC sp_helpindex '테이블 이름'

-- 인덱스 번호 확인
-- SELECT index_id, name
-- FROM sys.indexes
-- WHERE object_id = object_id('테이블 이름');

-- 인덱스 조회
-- DBCC IND('데이터베이스 이름', '테이블 이름', 인덱스 번호); 

-- 논리적 읽기 -> 실제 데이터를 찾기 위해 읽은 페이지 수
-- SET STATISTICS TIME ON;
-- SET STATISTICS IO ON;

-- INDEX SCAN
-- SELECT *
-- FROM 테이블;

-- INDEX SEEK
-- SELECT *
-- FROM 테이블
-- WHERE 조건; -> 이 조건은 클러스터드 인덱스가 설정된 개체의 조건

-- INDEX SEEK + KEY LOOKUP
-- SELECT *
-- FROM 테이블
-- WHERE 조건; -> 이 조건은 논클러스터드 인덱스가 설정된 개체의 조건

-- INDEX SCAN + KEY LOOKUP
-- SELECT TOP 5 *
-- FROM 테이블
-- ORDER BY 개체; -> 논클러스터드 인덱스를 정렬할 때