-- 1.a Que determine la cantidad de autores que tiene un libro
CREATE FUNCTION dbo.CantidadAutoresLibro (@IdLibro int)
RETURNS int AS BEGIN
   RETURN (SELECT COUNT(*)
      FROM AutorLibro
      WHERE IdLibro = @IdLibro);
END;

-- 1.b Que determine la cantidad de autores que tiene un libro que pertenecen a una
--     casa editora
CREATE FUNCTION dbo.CantidadAutoresLibroEditorial (@IdLibro int, @IdEditorial int)
RETURNS int AS BEGIN
    RETURN (SELECT COUNT(DISTINCT AutoresPorLibro.IdAutor)
            FROM AutorLibro AS AutoresPorLibro
            INNER JOIN AutorEditorial AS AutoresPorEditorial ON AutoresPorEditorial.IdAutor = AutoresPorLibro.IdAutor
            WHERE AutoresPorLibro.IdLibro = @IdLibro AND AutoresPorEditorial.IdEditorial = @IdEditorial);
END;

-- 1.c Que determine los libros de un autor para una casa editora.
CREATE FUNCTION dbo.LibrosAutorEditorial (@IdAutor int, @IdEditorial int)
RETURNS TABLE AS RETURN
(
    SELECT Libros.IdLibro, Libros.ISBN, RTRIM(Libros.Titulo) AS Titulo, RTRIM(Libros.[Año de Edición]) AS [Año de Edición]
    FROM Libro AS Libros
    INNER JOIN AutorLibro AS AutoresPorLibro ON AutoresPorLibro.IdLibro = Libros.IdLibro
    WHERE AutoresPorLibro.IdAutor = @IdAutor AND Libros.IdEditorial = @IdEditorial
);

-- 1.d NombreAutor. Que presente el nombre del autor en formato completo (Nombre
--     y Apellido), formato reducido 1 (Primer nombre, Primera letra del segundo
--     nombre con punto si tiene y Apellido), formato reducido 2 (Primer nombre,
--     Primera letra del segundo nombre con punto si tiene, Primer apellido, Segunda
--     letra del segundo apellido con punto, si tiene). Se asume que las últimas dos
--     palabras de ‘nombre’ son apellidos, lo demás es nombres.
CREATE FUNCTION dbo.NombreAutor (@IdAutor int, @Formato int)
RETURNS varchar(100) AS BEGIN
    DECLARE @nombreCompleto varchar(100);

    SELECT @nombreCompleto = LTRIM(RTRIM(Autores.Nombre))
    FROM Autor AS Autores
    WHERE Autores.IdAutor = @IdAutor;

    IF @nombreCompleto IS NULL OR @nombreCompleto = ''
    RETURN NULL;

    DECLARE @palabrasDelNombre TABLE (posicion int IDENTITY(1,1), palabra varchar(40));
    DECLARE @textoRestante varchar(100) = @nombreCompleto;
    DECLARE @posicionEspacio int;

    WHILE LEN(@textoRestante) > 0 BEGIN
        SET @posicionEspacio = CHARINDEX(' ', @textoRestante);

        IF @posicionEspacio = 0 BEGIN
            INSERT INTO @palabrasDelNombre (palabra) VALUES (@textoRestante);
            SET @textoRestante = '';
        END
        ELSE BEGIN
            INSERT INTO @palabrasDelNombre (palabra)
            VALUES (LEFT(@textoRestante, @posicionEspacio - 1));

            SET @textoRestante = LTRIM(SUBSTRING(@textoRestante, @posicionEspacio + 1, 100));
        END
    END;

    DECLARE @cantidadPalabras int = (SELECT COUNT(*) FROM @palabrasDelNombre);
    DECLARE @cantidadApellidos int = CASE WHEN @cantidadPalabras >= 3 THEN 2 WHEN @cantidadPalabras = 2 THEN 1 ELSE 0 END;
    DECLARE @cantidadNombres int = @cantidadPalabras - @cantidadApellidos;

    DECLARE @primerNombre varchar(40) = (SELECT palabra FROM @palabrasDelNombre WHERE posicion = 1);
    DECLARE @segundoNombre varchar(40) = CASE WHEN @cantidadNombres >= 2
             THEN (SELECT palabra FROM @palabrasDelNombre WHERE posicion = 2)
        END;

    DECLARE @primerApellido varchar(40) = CASE WHEN @cantidadApellidos >= 1
             THEN (SELECT palabra FROM @palabrasDelNombre WHERE posicion = @cantidadNombres + 1)
        END;

    DECLARE @segundoApellido varchar(40) = CASE WHEN @cantidadApellidos = 2
             THEN (SELECT palabra FROM @palabrasDelNombre WHERE posicion = @cantidadNombres + 2)
        END;

    DECLARE @inicialSegundoNombre varchar(3) = CASE WHEN @segundoNombre IS NOT NULL THEN LEFT(@segundoNombre, 1) + '.' END;
    DECLARE @inicialSegundoApellido varchar(3) = CASE WHEN @segundoApellido IS NOT NULL THEN LEFT(@segundoApellido, 1) + '.' END;
    DECLARE @nombreFormateado varchar(100);

    IF @Formato = 1
        SET @nombreFormateado = @nombreCompleto;
    ELSE IF @Formato = 2
        SET @nombreFormateado = @primerNombre + ISNULL(' ' + @inicialSegundoNombre, '') + ISNULL(' ' + @primerApellido, '');
    ELSE IF @Formato = 3
        SET @nombreFormateado = @primerNombre + ISNULL(' ' + @inicialSegundoNombre, '') + ISNULL(' ' + @primerApellido, '') + ISNULL(' ' + @inicialSegundoApellido, '');

    RETURN @nombreFormateado;
END;

-- 1.e AutoresLibro.  Que retorne un string con los nombres en formato reducido 2, de los autores de un libro separado por comas.
CREATE FUNCTION dbo.AutoresLibro (@IdLibro int)
RETURNS varchar(max) AS BEGIN
    DECLARE @listaAutores varchar(max);

    SELECT @listaAutores = STRING_AGG(dbo.NombreAutor(Autores.IdAutor, 3), ', ') WITHIN GROUP (ORDER BY Autores.Nombre)
    FROM AutorLibro AS AutoresPorLibro
    INNER JOIN Autor AS Autores
        ON Autores.IdAutor = AutoresPorLibro.IdAutor
    WHERE AutoresPorLibro.IdLibro = @IdLibro;

    RETURN @listaAutores;
END;

-- 2.a Autores de un libro ordenados por casa editora a la que pertenecen.
CREATE VIEW dbo.vw_AutoresLibroPorEditorial AS
SELECT TOP 100 PERCENT Libros.IdLibro, RTRIM(Libros.Titulo) AS Libro, Editoriales.IdEditorial, RTRIM(Editoriales.Nombre) AS CasaEditora, Autores.IdAutor, RTRIM(Autores.Nombre) AS Autor
FROM Libro AS Libros

INNER JOIN AutorLibro AS AutoresPorLibro ON AutoresPorLibro.IdLibro = Libros.IdLibro
INNER JOIN Autor AS Autores ON Autores.IdAutor = AutoresPorLibro.IdAutor

LEFT JOIN AutorEditorial AS AutoresPorEditorial ON AutoresPorEditorial.IdAutor = Autores.IdAutor
LEFT JOIN Editorial AS Editoriales ON Editoriales.IdEditorial = AutoresPorEditorial.IdEditorial
ORDER BY Editoriales.Nombre, Libros.Titulo, Autores.Nombre;

-- 2.b Libros con sus casas editora y sus autores.
CREATE VIEW dbo.vw_LibrosEditorasAutores AS SELECT Libros.IdLibro, Libros.ISBN, RTRIM(Libros.Titulo) AS Libro, RTRIM(Libros.[Año de Edición]) AS [Año de Edición], RTRIM(Editoriales.Nombre) AS CasaEditora, dbo.AutoresLibro(Libros.IdLibro) AS Autores
FROM Libro AS Libros
LEFT JOIN Editorial AS Editoriales ON Editoriales.IdEditorial = Libros.IdEditorial;
