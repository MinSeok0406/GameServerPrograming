USE HRDB2
GO

CREATE TABLE dbo.직원
(
	사원번호 char(5) COLLATE Korean_Wansung_CI_AS NOT NULL,
	이름 nchar(10) COLLATE Korean_Wansung_CI_AS NOT NULL,
	성별 char(1) COLLATE Korean_Wansung_CI_AS NOT NULL,
	입사일 date NOT NULL,
	전자우편 varchar(60) COLLATE Korean_Wansung_CI_AS NOT NULL,
	부서코드 char(3) COLLATE Korean_Wansung_CI_AS NOT NULL,
)
GO

INSERT INTO dbo.직원
	VALUES('S0001', N'홍길동', 'M', '2011-01-01', 'hong@dbnuri.com', 'SYS')
INSERT INTO dbo.직원
	VALUES('S0002', N'일지매', 'm', '2011-01-12', 'jimae@dbnuri.com', 'GEN')
INSERT INTO dbo.직원
	VALUES('S0004', N'김삼순', 'F', '2012-08-01', 'samsoon@dbnuri.com', 'MKT')
GO

SELECT 사원번호, 이름, 성별, 입사일, 전자우편, 부서코드
FROM dbo.직원
WHERE 성별 = 'M'
GO

SELECT 사원번호, 이름, 성별, 입사일, 전자우편, 부서코드
FROM dbo.직원
WHERE 성별 = 'M' COLLATE Korean_Wansung_CS_AS
GO