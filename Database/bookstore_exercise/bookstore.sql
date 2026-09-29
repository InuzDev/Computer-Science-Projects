USE bookstore10165529;
GO

-- 1. Ingrese 10 autores, 10 Editoriales, 10 libros
INSERT INTO Editorial (IdEditorial, Nombre) VALUES
('E0001', 'Orbit'), ('E0002', 'Domingo'), ('E0003', 'Detras del chip'),
('E0004', 'Into the Servers'), ('E0005', 'Foundations of software engineering'),
('E0006', 'General Development'), ('E0007', 'DELTA-F'), ('E0008', 'NewEngineering'),
('E0009', 'FoxReader'), ('E0010', 'Sharky Curious');

INSERT INTO Autor (IdAutor, Nombre, [Fecha de Nacimiento]) VALUES
('A0001', 'Charles David Jorge', '2006-12-28'),
('A0002', 'Xiao Ping Lu', '1999-11-23'),
('A0003', 'Yichao Wu', '2006-11-15'),
('A0004', 'Cesar Torres Frias', '2007-02-10'),
('A0005', 'Ping Wua Lu', '1983-03-23'),
('A0006', 'Yu Xiao Li', '2000-04-23'),
('A0007', 'Xu Ping Lua', '1999-03-21'),
('A0008', 'Ling Zupeng', '1998-02-12'),
('A0009', 'Hue Ping LuaXi', '1989-01-15'),
('A0010', 'Xing Hing Ping', '1968-06-23');

INSERT INTO Libro (IdLibro, ISBN, IdEditorial, Titulo, [Año de Edición]) VALUES
('L0001','9780000000011','E0001','Understanding the JVM (Zhou Zhiming)','2011'),
('L0002','9780000000028','E0002','Redis Design and Implementation','2014'),
('L0003','9780000000035','E0003','Orange''S: An Operating System Implementation','2009'),
('L0004','9780000000042','E0004','Programmer Self-Cultivation','2009'),
('L0005','9780000000059','E0005','Advanced Go Programming','2019'),
('L0006','9780000000066','E0006','Go Language Study Notes','2016'),
('L0007','9780000000073','E0007','Large-Scale Website Technical Architecture','2013'),
('L0008','9780000000080','E0008','Understanding Nginx: Module Development','2013'),
('L0009','9780000000097','E0009','Restoring the Truth of Operating Systems','2016'),
('L0010','9780000000103','E0010','An Introduction to Database Systems','2014');

/*
2. Ingrese los autores de las casas editoras y de los libros colocando múltiples autores a un
libro y asegurándose de colocar:
a. Solo varios autores de la misma casa editorial del libro.
b. Combinación de autores de la misma casa editorial del libro y casas editorial
diferente a las del libro.
c. Solo varios autores de una casa editorial diferente a la del libro
*/
INSERT INTO AutorEditorial (IdAutor, IdEditorial) VALUES
('A0001','E0001'), ('A0002','E0001'), ('A0003','E0001'),
('A0004','E0002'), ('A0005','E0002'),
('A0006','E0003'),
('A0007','E0004'),
('A0008','E0005'),
('A0009','E0006'),
('A0010','E0007'),
('A0005','E0008'),
('A0002','E0009'),
('A0006','E0010');

-- A) Solo varios autores de la misma casa editorial del libro.
INSERT INTO AutorLibro (IdAutor, IdLibro) VALUES
('A0001','L0001'), ('A0002','L0001'),
-- B) Combinación de autores de la misma casa editorial del libro y casas editorial diferente a las del libro.
('A0004','L0002'), ('A0005','L0002'), ('A0006','L0002'),
-- C) Solo varios autores de una casa editorial diferente a la del libro
('A0001','L0003'), ('A0002','L0003'), ('A0003','L0003'),
-- Otros Autores, con otros libros.
('A0007','L0004'), ('A0008','L0005'), ('A0009','L0006'),
('A0010','L0007'), ('A0005','L0008'), ('A0002','L0009'),
('A0006','L0010');
