USE GameDB

-- ¿Œµ¶Ω∫ CREATE INDEX / DROP INDEX

CREATE UNIQUE INDEX i1
ON accounts(accountName);

DROP INDEX accounts.i1;