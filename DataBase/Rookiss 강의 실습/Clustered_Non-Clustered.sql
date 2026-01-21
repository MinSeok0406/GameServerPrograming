USE BaseballData;

-- Clustered(영한 사전) vs Non-Clustered(색인)

-- Clustered
	-- Leaf Page = Data Page
	-- 데이터는 Clustered Index 키 순서로 정렬

-- Non-Clustered (Clustered Index 유무에 따라서 다르게 동작)
-- 1) Clustered Index가 없는 경우
	-- Clustered Index가 없으면 데이터는 Heap Table이라는 곳에 저장
	-- Heap RID -> Heap Table에 접근 데이터 추출

-- 2) Clustered Index가 있는 경우
	-- Heap Table이 없음 -> Heap RID도 없음. Leaf Table에 실제 데이터 있다.
	-- Clustered Index의 실제 키 값을 들고 있는다.

