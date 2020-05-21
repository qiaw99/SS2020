SELECT P.SVN
FROM PERSON P, wohnt_in W, ADRESSE A
WHERE P.SVN = W.SVN
AND W.PLZ = A.PLZ
AND W.Straße = A.Straße
AND W.Hausnummer = A.Hausnummer
AND P.Beruf = "Koch"
AND A.Ort = "Hamburg"

------

SELECT P.Vorname, P.Nachname, P.Beruf
FROM PERSON P, ADRESSE A, wohnt_in W
WHERE P.SVN = W.SVN
AND W.PLZ = A.PLZ
AND A.Art = "Dienstadresse"
AND A.Ort = "Hannover"
GROUP BY P.Beruf

-----

SELECT P.Vorname, P.Nachname
FROM PERSON P, wohnt_in W, ADRESSE A
WHERE P.SVN = W.SVN
AND W.PLZ = A.PLZ
AND W.Straße = A.Straße
AND W.Hausnummer = A.Hausnummer
AND A.Ort = "Dresden"
AND W.von >= 01.01.2019
AND W.von <= 31.12.2019

-----
SELECT P.Vorname, P.Nachname
FROM PERSON P
WHERE SVN NOT IN(
SELECT DISTINCT SVN 
FROM ÜBT_AUS
)
