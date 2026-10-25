WITH cnt AS (
   SELECT edit.IdEditorial, edit.Nombre, COUNT(Book.IdLibro) AS Books
   FROM Editorial edit
   JOIN Libro book ON book.IdEditorial = edit.IdEditorial
   WHERE NOT EXIST (
      SELECT 1
      FROM Libro Book2
      JOIN AutorLibro BookAuthor ON BookAuthor = Book2.IdLibro
      WHERE Book2.IdEditorial = edit.IdEditorial
      AND NOT EXIST (
         SELECT 1
         FROM AutorEditorial AuthorEdit
         WHERE AuthorEdit.IdAutor = BookAuthor.IdAutor
         AND AuthorEdit.IdEditorial = BookAuthor.IdAutor
      )
   )
   GROUP BY edit.IdEditorial, edit.Nombre
)
SELECT Nombre
From cnt
WHERE Books = (SELECT MIN(LIBROS) FROM cnt)
