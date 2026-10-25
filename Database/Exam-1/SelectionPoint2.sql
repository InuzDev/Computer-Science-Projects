WITH cnt AS (
   SELECT edit.IdEditorial, edit.Nombre, COUNT(AuthorEdit.IdAutor) AS Authors
   FROM Editorial edit
   JOIN AutorEditorial AuthorEdit ON AuthorEdit.IdEDitorial = edit.IdEditorial
   WHERE EXISTS (
      SELECT 1
      FROM AutorLibro author1
      JOIN AutorLibro author2 ON author2.IdLibro = author1.IdLibro AND author2.IdAutor <> author1.IdAutor

      WHERE authro1.IdAutor = AuthorEdit.IdAutor
      AND NOT EXISTS (
         SELECT 1
         FROM AutorEditorial authoredit2
         WHERE authoredit2.IdAutor = author2.IdAutor
         AND authoredit2.IdEditorial = edit.IdEditorial
      )
   )
   GROUP BY edit.IdEditorial, edit.Nombre
)

SELECT Nombre
FROM cnt
WHERE Authors = (SELECT MIN(Authors) FROM cnt);
