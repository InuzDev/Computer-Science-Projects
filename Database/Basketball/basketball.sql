USE master;
GO

IF DB_ID(N'Basketball') IS NOT NULL
BEGIN
    ALTER DATABASE Basketball SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
    DROP DATABASE Basketball;
END
GO

CREATE DATABASE Basketball;
GO

USE Basketball;
GO

-- Script para hacer las tablas de la asignacion
Create table Ciudad (
   CodCiudad INT IDENTITY(1,1) PRIMARY KEY NOT NULL,
   Nombre VARCHAR(256) NOT NULL
);
GO

Create table Equipo (
   CodEquipo INT IDENTITY(1,1) PRIMARY KEY NOT NULL,
   Nombre VARCHAR(64) NOT NULL,
   CodCiudad INT NOT NULL,

   CONSTRAINT FK_Equipo_Ciudad
      FOREIGN KEY (CodCiudad) REFERENCES Ciudad (CodCiudad)
);
GO

Create table Jugador (
   CodJugador INT IDENTITY(1,1) PRIMARY KEY NOT NULL,
   Nombre1 VARCHAR(64) NOT NULL,
   Nombre2 VARCHAR(64) NOT NULL,

   Apellido1 VARCHAR(64) NOT NULL,
   Apellido2 VARCHAR(64) NOT NULL,

   CiudadNacim INT NOT NULL,
   FechaNacim DATE NOT NULL,

   Numero INT NOT NULL,
   CodEquipo INT NOT NULL,

   CONSTRAINT FK_Jugador_Equipo
      FOREIGN KEY (CodEquipo) REFERENCES Equipo (CodEquipo),
   CONSTRAINT FK_Jugador_Ciudad
      FOREIGN KEY (CiudadNacim) REFERENCES Ciudad (CodCiudad),

   CONSTRAINT UQ_Jugador_Equipo_Numero
      UNIQUE (CodEquipo, Numero),
   CONSTRAINT CK_Jugador_Numero
      CHECK (Numero BETWEEN 0 AND 99)
);
GO

Create table Juego (
   CodJuego INT IDENTITY(1,1) PRIMARY KEY NOT NULL,
   Descripcion VARCHAR(50) NOT NULL,
   Equipo1 INT NOT NULL,
   Equipo2 INT NOT NULL,
   Fecha DATE NOT NULL,

   CONSTRAINT FK_Juego_Equipo1
      FOREIGN KEY (Equipo1) REFERENCES Equipo (CodEquipo),
   CONSTRAINT FK_Juego_Equipo2
      FOREIGN KEY (Equipo2) REFERENCES Equipo (CodEquipo),

   CONSTRAINT CK_Juego_DifEquipos
      CHECK (Equipo1 <> Equipo2)
);
GO

Create table Estadistica (
   CodEstadistica INT IDENTITY(1, 1) PRIMARY KEY NOT NULL,
   Descripcion VARCHAR(50) NOT NULL,
   Valor INT NOT NULL
);
GO

Create table EstadisticaJuego (
   CodJuego INT NOT NULL,
   CodEstadistica INT NOT NULL,
   CodJugador INT NOT NULL,
   Cantidad INT NOT NULL,

   CONSTRAINT PK_EstadisticaJuego
      PRIMARY KEY (CodJuego, CodEstadistica, CodJugador),

   CONSTRAINT FK_EstadisticaJuego_Juego
      FOREIGN KEY (CodJuego) REFERENCES Juego (CodJuego),
   CONSTRAINT FK_EstadisticaJuego_Estadistica
      FOREIGN KEY (CodEstadistica) REFERENCES Estadistica (CodEstadistica),
   CONSTRAINT FK_EstadisticaJuego_Jugador
      FOREIGN KEY (CodJugador) REFERENCES Jugador (CodJugador)
);
GO
-- Fin de ejercicio 1

-- Muestre los jugadores por ciudades donde juegan (código de la ciudad, nombre
-- de la ciudad y nombre del jugador) ordenados por nombre de ciudad
-- ascendentemente, nombre del jugador ascendentemente.
SELECT _ciudad.CodCiudad, _ciudad.Nombre AS Ciudad, _Jugador.Nombre1 + ' ' + _Jugador.Apellido1 AS Jugador
FROM Jugador _Jugador
INNER JOIN Equipo _equipo ON _equipo.CodEquipo = _Jugador.CodEquipo
INNER JOIN Ciudad _ciudad ON _ciudad.CodCiudad = _equipo.CodCiudad
ORDER BY _ciudad.Nombre ASC, _Jugador.Nombre1 ASC;

-- Resultado
-- CodCiudad   Ciudad          Jugador
-- ----------- --------------- ------------------
-- 4           La Romana            Eddy Mora
-- 4           La Romana            Hector Reyes
-- 4           La Romana            Gabriel Felix
-- 4           La Romana            Jonathan Acosta
-- 4           La Romana            Wilkin Duran
-- 3           Puerto Plata         Daniel Vargas
-- 3           Puerto Plata         Kelvin Taveras
-- 3           Puerto Plata         Oscar Disla
-- 3           Puerto Plata         Robert Nunez
-- 3           Puerto Plata         Starling Marte
-- 2           Santiago             Andres Rodriguez
-- 2           Santiago             Franklin Batista
-- 2           Santiago             Juan Martinez
-- 2           Santiago             Ramon Garcia
-- 2           Santiago             Victor Castillo
-- 1           Santo Domingo        Carlos Jimenez
-- 1           Santo Domingo        Jose Mejia
-- 1           Santo Domingo        Luis Santana
-- 1           Santo Domingo        Miguel Pena
-- 1           Santo Domingo        Pedro Ramirez


-- Muestre los miembros (nombre1, apellido1, numero) de los equipos de las
-- ciudades cuyo nombre comiencen con ‘P’, ordenado por apellido1
-- descendentemente.
SELECT _Jugador.Nombre1, _Jugador.Apellido1, _Jugador.Numero
FROM Jugador _Jugador
INNER JOIN Equipo _equipo ON _equipo.CodEquipo = _Jugador.CodEquipo
INNER JOIN Ciudad _ciudad ON _ciudad.CodCiudad = _equipo.CodCiudad
WHERE _ciudad.Nombre LIKE 'P%'
ORDER BY _Jugador.Apellido1 DESC;

-- Resultado
-- Nombre1                                                          Apellido1                                                        Numero
-- ---------------------------------------------------------------- ---------------------------------------------------------------- -----------
-- Daniel                                                           Vargas                                                           10
-- Kelvin                                                           Taveras                                                          4
-- Robert                                                           Nunez                                                            8
-- Starling                                                         Marte                                                            15
-- Oscar                                                            Disla                                                            32

-- Muestre los juegos donde la diferencia de puntos sea mayor a 10, indicando los
-- equipos participantes con la cantidad de puntos de cada equipo (código de juego,
-- descripción de juego, código equipo 1, cantidad de puntos anotados equipo 1y
-- código del equipo 2, cantidad de puntos equipo 2).
WITH PuntosEquipoJuego AS (
    SELECT _EstJuego.CodJuego, _Jugador.CodEquipo, SUM(_EstJuego.Cantidad * _Estadistica.Valor) AS Puntos
    FROM EstadisticaJuego _EstJuego
    INNER JOIN Estadistica _Estadistica ON _Estadistica.CodEstadistica = _EstJuego.CodEstadistica
    INNER JOIN Jugador _Jugador ON _Jugador.CodJugador = _EstJuego.CodJugador
    WHERE _EstJuego.CodEstadistica IN (1, 2, 9)
    GROUP BY _EstJuego.CodJuego, _Jugador.CodEquipo
)
SELECT _Juego.CodJuego, _Juego.Descripcion,
       _Juego.Equipo1 AS [Equipo 1], _Equipo1Puntos.Puntos AS [Puntos Equipo 1],
       _Juego.Equipo2 AS [Equipo 2], _Equipo2Puntos.Puntos AS [Puntos Equipo 2]
FROM Juego _Juego
INNER JOIN PuntosEquipoJuego _Equipo1Puntos ON _Equipo1Puntos.CodJuego = _Juego.CodJuego AND _Equipo1Puntos.CodEquipo = _Juego.Equipo1
INNER JOIN PuntosEquipoJuego _Equipo2Puntos ON _Equipo2Puntos.CodJuego = _Juego.CodJuego AND _Equipo2Puntos.CodEquipo = _Juego.Equipo2
WHERE ABS(_Equipo1Puntos.Puntos - _Equipo2Puntos.Puntos) > 10
ORDER BY _Juego.CodJuego;

-- Resultado
-- CodJuego    Descripcion                                        Equipo 1    Puntos Equipo 1 Equipo 2    Puntos Equipo 2
-- ----------- -------------------------------------------------- ----------- --------------- ----------- ---------------
-- 2           Temporada regular, juego 2                         3           61              4           72
-- 3           Temporada regular, juego 3                         1           85              3           61
-- 9           Temporada regular, juego 9                         3           62              1           75
-- 10          Temporada regular, juego 10                        4           78              2           67
-- 11          Temporada regular, juego 11                        4           73              1           85

-- Muestre el total de asistencias por jugador para el juego más reciente.
-- (código del jugador, nombre del jugador, apellido del jugador y el total de asistencias).
SELECT _Jugador.CodJugador, _Jugador.Nombre1, _Jugador.Apellido1, SUM(_EstJuego.Cantidad) AS [Total Asistencias]
FROM EstadisticaJuego _EstJuego
INNER JOIN Jugador _Jugador ON _Jugador.CodJugador = _EstJuego.CodJugador
WHERE _EstJuego.CodEstadistica = 3
  AND _EstJuego.CodJuego = (SELECT TOP 1 _Juego.CodJuego FROM Juego _Juego ORDER BY _Juego.Fecha DESC, _Juego.CodJuego DESC)
GROUP BY _Jugador.CodJugador, _Jugador.Nombre1, _Jugador.Apellido1
ORDER BY [Total Asistencias] DESC;

-- Resultado
-- CodJugador  Nombre1                                                          Apellido1                                                        Total Asistencias
-- ----------- ---------------------------------------------------------------- ---------------------------------------------------------------- -----------------
-- 7           Andres                                                           Rodriguez                                                        12
-- 9           Franklin                                                         Batista                                                          8
-- 6           Juan                                                             Martinez                                                         7
-- 14          Kelvin                                                           Taveras                                                          7
-- 8           Ramon                                                            Garcia                                                           6
-- 11          Daniel                                                           Vargas                                                           6
-- 12          Starling                                                         Marte                                                            4
-- 13          Robert                                                           Nunez                                                            2


-- Muestre el juego y jugador (código de juego, nombre1, apellido1, numero,
-- cantidad de rebotes) donde se ha tomado la mayor cantidad de rebotes.
SELECT _EstJuego.CodJuego, _Jugador.Nombre1, _Jugador.Apellido1, _Jugador.Numero, _EstJuego.Cantidad AS Rebotes
FROM EstadisticaJuego _EstJuego
INNER JOIN Jugador _Jugador ON _Jugador.CodJugador = _EstJuego.CodJugador
WHERE _EstJuego.CodEstadistica = 4
  AND _EstJuego.Cantidad = (
      SELECT MAX(_EstJuegoMax.Cantidad)
      FROM EstadisticaJuego _EstJuegoMax
      WHERE _EstJuegoMax.CodEstadistica = 4
  );

-- Resultado
-- CodJuego    Nombre1                                                          Apellido1                                                        Numero      Rebotes
-- ----------- ---------------------------------------------------------------- ---------------------------------------------------------------- ----------- -----------
-- 5           Pedro                                                            Ramirez                                                          34          19

-- Muestre el nombre1, apellido1, número y cantidad de asistencias de los jugadores
-- con más de 3 asistencias en al menos un juego.
SELECT _Jugador.Nombre1, _Jugador.Apellido1, _Jugador.Numero, SUM(_EstJuego.Cantidad) AS [Total Asistencias]
FROM EstadisticaJuego _EstJuego
INNER JOIN Jugador _Jugador ON _Jugador.CodJugador = _EstJuego.CodJugador
WHERE _EstJuego.CodEstadistica = 3
GROUP BY _Jugador.CodJugador, _Jugador.Nombre1, _Jugador.Apellido1, _Jugador.Numero
HAVING MAX(_EstJuego.Cantidad) > 3
ORDER BY [Total Asistencias] DESC;

-- Resultado
-- Nombre1                                                          Apellido1                                                        Numero      Total Asistencias
-- ---------------------------------------------------------------- ---------------------------------------------------------------- ----------- -----------------
-- Carlos                                                           Jimenez                                                          7           66
-- Daniel                                                           Vargas                                                           10          49
-- Andres                                                           Rodriguez                                                        12          48
-- Hector                                                           Reyes                                                            6           47
-- Franklin                                                         Batista                                                          3           40
-- Ramon                                                            Garcia                                                           21          32
-- Wilkin                                                           Duran                                                            2           32
-- Kelvin                                                           Taveras                                                          4           31
-- Juan                                                             Martinez                                                         5           31
-- Jose                                                             Mejia                                                            9           29
-- Miguel                                                           Pena                                                             23          28
-- Eddy                                                             Mora                                                             20          28
-- Jonathan                                                         Acosta                                                           14          24
-- Starling                                                         Marte                                                            15          22
-- Robert                                                           Nunez                                                            8           18
-- Luis                                                             Santana                                                          11          18


-- Muestre nombre del equipo, nombre1, apellido1 de los jugadores que nacieron
-- en la ciudad diferente a la del equipo para que juegan.
SELECT _equipo.Nombre AS Equipo, _Jugador.Nombre1, _Jugador.Apellido1
FROM Jugador _Jugador
INNER JOIN Equipo _equipo ON _equipo.CodEquipo = _Jugador.CodEquipo
WHERE _Jugador.CiudadNacim <> _equipo.CodCiudad;

-- Resultado
-- Equipo                                                           Nombre1                                                          Apellido1
-- ---------------------------------------------------------------- ---------------------------------------------------------------- ----------------------------------------------------------------
-- Relampagos del Ozama                                             Luis                                                             Santana
-- Relampagos del Ozama                                             Jose                                                             Mejia
-- Panteras del Cibao                                               Ramon                                                            Garcia
-- Panteras del Cibao                                               Franklin                                                         Batista
-- Delfines del Atlantico                                           Starling                                                         Marte
-- Bucaneros del Este                                               Jonathan                                                         Acosta
-- Bucaneros del Este                                               Wilkin                                                           Duran


-- Muestre nombre del equipo, nombre1, apellido1 de los jugadores que no dieron
-- asistencias en más de 4 juegos de lo que va de torneo.
SELECT _equipo.Nombre AS Equipo, _Jugador.Nombre1, _Jugador.Apellido1
FROM Jugador _Jugador
INNER JOIN Equipo _equipo ON _equipo.CodEquipo = _Jugador.CodEquipo
WHERE (
    SELECT COUNT(*)
    FROM Juego _Juego
    WHERE (_Juego.Equipo1 = _Jugador.CodEquipo OR _Juego.Equipo2 = _Jugador.CodEquipo)
      AND NOT EXISTS (
          SELECT 1
          FROM EstadisticaJuego _EstJuego
          WHERE _EstJuego.CodJuego = _Juego.CodJuego
            AND _EstJuego.CodJugador = _Jugador.CodJugador
            AND _EstJuego.CodEstadistica = 3
            AND _EstJuego.Cantidad > 0
      )
) > 4;

-- Resultado
-- Equipo                                                           Nombre1                                                          Apellido1
-- ---------------------------------------------------------------- ---------------------------------------------------------------- ----------------------------------------------------------------
-- Relampagos del Ozama                                             Pedro                                                            Ramirez
-- Panteras del Cibao                                               Victor                                                           Castillo


-- Muestre nombre del equipo, nombre1, apellido1 de los jugadores que hicieron un
-- triple doble (10 o más asistencias, 10 o más puntos, 10 o más rebotes en un juego)
-- en al menos 3 juegos.
WITH EstadisticasJugadorJuego AS (
    SELECT _EstJuego.CodJuego, _EstJuego.CodJugador,
           SUM(CASE WHEN _EstJuego.CodEstadistica IN (1, 2, 9) THEN _EstJuego.Cantidad * _Estadistica.Valor ELSE 0 END) AS Puntos,
           SUM(CASE WHEN _EstJuego.CodEstadistica = 3 THEN _EstJuego.Cantidad ELSE 0 END) AS Asistencias,
           SUM(CASE WHEN _EstJuego.CodEstadistica = 4 THEN _EstJuego.Cantidad ELSE 0 END) AS Rebotes
    FROM EstadisticaJuego _EstJuego
    INNER JOIN Estadistica _Estadistica ON _Estadistica.CodEstadistica = _EstJuego.CodEstadistica
    GROUP BY _EstJuego.CodJuego, _EstJuego.CodJugador
)
SELECT _equipo.Nombre AS Equipo, _Jugador.Nombre1, _Jugador.Apellido1
FROM EstadisticasJugadorJuego _EstJugJuego
INNER JOIN Jugador _Jugador ON _Jugador.CodJugador = _EstJugJuego.CodJugador
INNER JOIN Equipo _equipo ON _equipo.CodEquipo = _Jugador.CodEquipo
WHERE _EstJugJuego.Puntos >= 10 AND _EstJugJuego.Asistencias >= 10 AND _EstJugJuego.Rebotes >= 10
GROUP BY _equipo.Nombre, _Jugador.CodJugador, _Jugador.Nombre1, _Jugador.Apellido1
HAVING COUNT(*) >= 3;

-- Resultado
-- Equipo                                                           Nombre1                                                          Apellido1
-- ---------------------------------------------------------------- ---------------------------------------------------------------- ----------------------------------------------------------------
-- Relampagos del Ozama                                             Carlos                                                           Jimenez
-- Panteras del Cibao                                               Andres                                                           Rodriguez


-- Muestre nombre del equipo, nombre1, apellido1, promedio de puntos por
-- partido, promedio de asistencias, promedio de rebotes, de los primeros 5
-- jugadores con puntos, rebotes y asistencias por encima del promedio.
WITH EstadisticasJugadorJuego AS (
    SELECT _EstJuego.CodJuego, _EstJuego.CodJugador,
           SUM(CASE WHEN _EstJuego.CodEstadistica IN (1, 2, 9) THEN _EstJuego.Cantidad * _Estadistica.Valor ELSE 0 END) AS Puntos,
           SUM(CASE WHEN _EstJuego.CodEstadistica = 3 THEN _EstJuego.Cantidad ELSE 0 END) AS Asistencias,
           SUM(CASE WHEN _EstJuego.CodEstadistica = 4 THEN _EstJuego.Cantidad ELSE 0 END) AS Rebotes
    FROM EstadisticaJuego _EstJuego
    INNER JOIN Estadistica _Estadistica ON _Estadistica.CodEstadistica = _EstJuego.CodEstadistica
    GROUP BY _EstJuego.CodJuego, _EstJuego.CodJugador
),
PromediosJugador AS (
    SELECT _EstJugJuego.CodJugador,
           AVG(_EstJugJuego.Puntos * 1.0)      AS PromPuntos,
           AVG(_EstJugJuego.Asistencias * 1.0) AS PromAsistencias,
           AVG(_EstJugJuego.Rebotes * 1.0)     AS PromRebotes
    FROM EstadisticasJugadorJuego _EstJugJuego
    GROUP BY _EstJugJuego.CodJugador
)
SELECT TOP 5 _equipo.Nombre AS Equipo, _Jugador.Nombre1, _Jugador.Apellido1,
       ROUND(_Promedio.PromPuntos, 1)      AS [Promedio Puntos],
       ROUND(_Promedio.PromAsistencias, 1) AS [Promedio Asistencias],
       ROUND(_Promedio.PromRebotes, 1)     AS [Promedio Rebotes]
FROM PromediosJugador _Promedio
INNER JOIN Jugador _Jugador ON _Jugador.CodJugador = _Promedio.CodJugador
INNER JOIN Equipo _equipo ON _equipo.CodEquipo = _Jugador.CodEquipo
WHERE _Promedio.PromPuntos      > (SELECT AVG(PromPuntos)      FROM PromediosJugador)
  AND _Promedio.PromAsistencias > (SELECT AVG(PromAsistencias) FROM PromediosJugador)
  AND _Promedio.PromRebotes     > (SELECT AVG(PromRebotes)     FROM PromediosJugador)
ORDER BY _Promedio.PromPuntos DESC;

-- Resultado
-- Equipo                                                           Nombre1                                                          Apellido1                                                        Promedio Puntos                         Promedio Asistencias                    Promedio Rebotes
-- ---------------------------------------------------------------- ---------------------------------------------------------------- ---------------------------------------------------------------- --------------------------------------- --------------------------------------- ---------------------------------------
-- Relampagos del Ozama                                             Miguel                                                           Pena                                                             20.000000                               4.700000                                8.000000
-- Panteras del Cibao                                               Andres                                                           Rodriguez                                                        18.800000                               8.000000                                9.300000
-- Relampagos del Ozama                                             Carlos                                                           Jimenez                                                          18.200000                               11.000000                               9.200000
-- Delfines del Atlantico                                           Daniel                                                           Vargas                                                           17.800000                               8.200000                                7.200000
-- Bucaneros del Este                                               Eddy                                                             Mora                                                             17.700000                               4.700000                                7.000000

-- Punto 12
-- Script del modelo de datos con los datos.
USE master;
GO

IF DB_ID(N'Basketball') IS NOT NULL
BEGIN
    ALTER DATABASE Basketball SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
    DROP DATABASE Basketball;
END
GO

CREATE DATABASE Basketball;
GO

USE Basketball;
GO

-- Script para hacer las tablas de la asignacion
Create table Ciudad (
   CodCiudad INT IDENTITY(1,1) PRIMARY KEY NOT NULL,
   Nombre VARCHAR(256) NOT NULL
);
GO

Create table Equipo (
   CodEquipo INT IDENTITY(1,1) PRIMARY KEY NOT NULL,
   Nombre VARCHAR(64) NOT NULL,
   CodCiudad INT NOT NULL,

   CONSTRAINT FK_Equipo_Ciudad
      FOREIGN KEY (CodCiudad) REFERENCES Ciudad (CodCiudad)
);
GO

Create table Jugador (
   CodJugador INT IDENTITY(1,1) PRIMARY KEY NOT NULL,
   Nombre1 VARCHAR(64) NOT NULL,
   Nombre2 VARCHAR(64) NOT NULL,

   Apellido1 VARCHAR(64) NOT NULL,
   Apellido2 VARCHAR(64) NOT NULL,

   CiudadNacim INT NOT NULL,
   FechaNacim DATE NOT NULL,

   Numero INT NOT NULL,
   CodEquipo INT NOT NULL,

   CONSTRAINT FK_Jugador_Equipo
      FOREIGN KEY (CodEquipo) REFERENCES Equipo (CodEquipo),
   CONSTRAINT FK_Jugador_Ciudad
      FOREIGN KEY (CiudadNacim) REFERENCES Ciudad (CodCiudad),

   CONSTRAINT UQ_Jugador_Equipo_Numero
      UNIQUE (CodEquipo, Numero),
   CONSTRAINT CK_Jugador_Numero
      CHECK (Numero BETWEEN 0 AND 99)
);
GO

Create table Juego (
   CodJuego INT IDENTITY(1,1) PRIMARY KEY NOT NULL,
   Descripcion VARCHAR(50) NOT NULL,
   Equipo1 INT NOT NULL,
   Equipo2 INT NOT NULL,
   Fecha DATE NOT NULL,

   CONSTRAINT FK_Juego_Equipo1
      FOREIGN KEY (Equipo1) REFERENCES Equipo (CodEquipo),
   CONSTRAINT FK_Juego_Equipo2
      FOREIGN KEY (Equipo2) REFERENCES Equipo (CodEquipo),

   CONSTRAINT CK_Juego_DifEquipos
      CHECK (Equipo1 <> Equipo2)
);
GO

Create table Estadistica (
   CodEstadistica INT IDENTITY(1, 1) PRIMARY KEY NOT NULL,
   Descripcion VARCHAR(50) NOT NULL,
   Valor INT NOT NULL
);
GO

Create table EstadisticaJuego (
   CodJuego INT NOT NULL,
   CodEstadistica INT NOT NULL,
   CodJugador INT NOT NULL,
   Cantidad INT NOT NULL,

   CONSTRAINT PK_EstadisticaJuego
      PRIMARY KEY (CodJuego, CodEstadistica, CodJugador),

   CONSTRAINT FK_EstadisticaJuego_Juego
      FOREIGN KEY (CodJuego) REFERENCES Juego (CodJuego),
   CONSTRAINT FK_EstadisticaJuego_Estadistica
      FOREIGN KEY (CodEstadistica) REFERENCES Estadistica (CodEstadistica),
   CONSTRAINT FK_EstadisticaJuego_Jugador
      FOREIGN KEY (CodJugador) REFERENCES Jugador (CodJugador)
);
GO

USE Basketball;
GO

DELETE FROM EstadisticaJuego;
DELETE FROM Juego;
DELETE FROM Jugador;
DELETE FROM Estadistica;
DELETE FROM Equipo;
DELETE FROM Ciudad;
GO

-- Ciudad
SET IDENTITY_INSERT Ciudad ON;
INSERT INTO Ciudad (CodCiudad, Nombre) VALUES
   (1, 'Santo Domingo'),
   (2, 'Santiago'),
   (3, 'Puerto Plata'),
   (4, 'La Romana'),
   (5, 'San Pedro de Macoris'),
   (6, 'Punta Cana'),
   (7, 'San Francisco de Macoris');
SET IDENTITY_INSERT Ciudad OFF;
GO

-- Equipo
SET IDENTITY_INSERT Equipo ON;
INSERT INTO Equipo (CodEquipo, Nombre, CodCiudad) VALUES
   (1, 'Relampagos del Ozama', 1),
   (2, 'Panteras del Cibao', 2),
   (3, 'Delfines del Atlantico', 3),
   (4, 'Bucaneros del Este', 4);
SET IDENTITY_INSERT Equipo OFF;
GO

-- Jugador (5 por equipo)
SET IDENTITY_INSERT Jugador ON;
INSERT INTO Jugador (CodJugador, Nombre1, Nombre2, Apellido1, Apellido2, CiudadNacim, FechaNacim, Numero, CodEquipo) VALUES
   (1, 'Carlos', 'Manuel', 'Jimenez', 'Peralta', 1, '1998-03-14', 7, 1),
   (2, 'Luis', 'Alberto', 'Santana', 'Reyes', 2, '1996-07-22', 11, 1),
   (3, 'Miguel', 'Angel', 'Pena', 'Gomez', 1, '1999-01-30', 23, 1),
   (4, 'Jose', 'Rafael', 'Mejia', 'Diaz', 5, '1995-11-05', 9, 1),
   (5, 'Pedro', 'Antonio', 'Ramirez', 'Cruz', 1, '1993-05-18', 34, 1),
   (6, 'Juan', 'Carlos', 'Martinez', 'Lopez', 2, '1997-02-09', 5, 2),
   (7, 'Andres', 'Jose', 'Rodriguez', 'Ferreira', 2, '1994-09-27', 12, 2),
   (8, 'Ramon', 'Emilio', 'Garcia', 'Nunez', 7, '1998-12-03', 21, 2),
   (9, 'Franklin', 'Daniel', 'Batista', 'Rosario', 1, '2000-04-16', 3, 2),
   (10, 'Victor', 'Hugo', 'Castillo', 'Medina', 2, '1992-08-11', 44, 2),
   (11, 'Daniel', 'Enrique', 'Vargas', 'Almonte', 3, '1996-06-25', 10, 3),
   (12, 'Starling', 'Jose', 'Marte', 'Polanco', 6, '1999-10-13', 15, 3),
   (13, 'Robert', 'Luis', 'Nunez', 'Beltre', 3, '1997-03-07', 8, 3),
   (14, 'Kelvin', 'Manuel', 'Taveras', 'Guzman', 3, '1995-01-19', 4, 3),
   (15, 'Oscar', 'Rene', 'Disla', 'Frias', 3, '1991-12-29', 32, 3),
   (16, 'Hector', 'Luis', 'Reyes', 'Sosa', 4, '1998-05-21', 6, 4),
   (17, 'Jonathan', 'Miguel', 'Acosta', 'Perez', 5, '1996-04-02', 14, 4),
   (18, 'Eddy', 'Rafael', 'Mora', 'Santos', 4, '1997-09-08', 20, 4),
   (19, 'Wilkin', 'Jose', 'Duran', 'Abreu', 6, '2001-02-14', 2, 4),
   (20, 'Gabriel', 'Antonio', 'Felix', 'Montero', 4, '1993-10-31', 55, 4);
SET IDENTITY_INSERT Jugador OFF;
GO

-- Estadistica (mismos datos del enunciado)
SET IDENTITY_INSERT Estadistica ON;
INSERT INTO Estadistica (CodEstadistica, Descripcion, Valor) VALUES
   (1, 'Puntos de dos', 2),
   (2, 'Puntos de tres', 3),
   (3, 'Asistencias', 1),
   (4, 'Rebotes', 1),
   (5, 'Bolas robadas', 1),
   (6, 'Faltas personales', 1),
   (7, 'Faltas tecnicas', 1),
   (8, 'Bolas perdidas', 1),
   (9, 'Tiros libres', 1);
SET IDENTITY_INSERT Estadistica OFF;
GO

-- Juego (12 juegos: cada pareja de equipos se enfrenta dos veces)
SET IDENTITY_INSERT Juego ON;
INSERT INTO Juego (CodJuego, Descripcion, Equipo1, Equipo2, Fecha) VALUES
   (1, 'Temporada regular, juego 1', 1, 2, '2026-08-16'),
   (2, 'Temporada regular, juego 2', 3, 4, '2026-08-20'),
   (3, 'Temporada regular, juego 3', 1, 3, '2026-08-24'),
   (4, 'Temporada regular, juego 4', 2, 4, '2026-08-28'),
   (5, 'Temporada regular, juego 5', 1, 4, '2026-09-01'),
   (6, 'Temporada regular, juego 6', 2, 3, '2026-09-05'),
   (7, 'Temporada regular, juego 7', 2, 1, '2026-09-09'),
   (8, 'Temporada regular, juego 8', 4, 3, '2026-09-13'),
   (9, 'Temporada regular, juego 9', 3, 1, '2026-09-17'),
   (10, 'Temporada regular, juego 10', 4, 2, '2026-09-21'),
   (11, 'Temporada regular, juego 11', 4, 1, '2026-09-25'),
   (12, 'Temporada regular, juego 12', 3, 2, '2026-09-29');
SET IDENTITY_INSERT Juego OFF;
GO

-- Juego 1: Relampagos del Ozama vs Panteras del Cibao
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (1, 1, 1, 4),
   (1, 2, 1, 1),
   (1, 3, 1, 11),
   (1, 4, 1, 12),
   (1, 6, 1, 3),
   (1, 8, 1, 3),
   (1, 9, 1, 5),
   (1, 1, 2, 2),
   (1, 2, 2, 2),
   (1, 3, 2, 3),
   (1, 4, 2, 5),
   (1, 5, 2, 1),
   (1, 6, 2, 2),
   (1, 8, 2, 2),
   (1, 9, 2, 2),
   (1, 1, 3, 5),
   (1, 2, 3, 1),
   (1, 3, 3, 5),
   (1, 4, 3, 12),
   (1, 6, 3, 2),
   (1, 8, 3, 1),
   (1, 9, 3, 3),
   (1, 1, 4, 3),
   (1, 2, 4, 2),
   (1, 3, 4, 4),
   (1, 4, 4, 3),
   (1, 5, 4, 2),
   (1, 6, 4, 3),
   (1, 8, 4, 2),
   (1, 9, 4, 3),
   (1, 1, 5, 3),
   (1, 3, 5, 2),
   (1, 4, 5, 14),
   (1, 5, 5, 2),
   (1, 6, 5, 4),
   (1, 8, 5, 3),
   (1, 9, 5, 2),
   (1, 1, 6, 4),
   (1, 2, 6, 1),
   (1, 3, 6, 5),
   (1, 4, 6, 3),
   (1, 5, 6, 2),
   (1, 6, 6, 2),
   (1, 8, 6, 1),
   (1, 9, 6, 3),
   (1, 1, 7, 5),
   (1, 2, 7, 1),
   (1, 3, 7, 6),
   (1, 4, 7, 10),
   (1, 5, 7, 1),
   (1, 6, 7, 5),
   (1, 8, 7, 2),
   (1, 9, 7, 2),
   (1, 1, 8, 4),
   (1, 3, 8, 4),
   (1, 4, 8, 8),
   (1, 5, 8, 1),
   (1, 6, 8, 2),
   (1, 8, 8, 2),
   (1, 9, 8, 3),
   (1, 1, 9, 3),
   (1, 2, 9, 2),
   (1, 3, 9, 6),
   (1, 4, 9, 2),
   (1, 5, 9, 3),
   (1, 6, 9, 2),
   (1, 7, 9, 1),
   (1, 8, 9, 2),
   (1, 9, 9, 4),
   (1, 1, 10, 5),
   (1, 4, 10, 8),
   (1, 5, 10, 1),
   (1, 6, 10, 3),
   (1, 8, 10, 2),
   (1, 9, 10, 3);

-- Juego 2: Delfines del Atlantico vs Bucaneros del Este
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (2, 1, 11, 2),
   (2, 2, 11, 2),
   (2, 3, 11, 5),
   (2, 4, 11, 5),
   (2, 6, 11, 1),
   (2, 7, 11, 1),
   (2, 8, 11, 4),
   (2, 9, 11, 3),
   (2, 1, 12, 3),
   (2, 2, 12, 1),
   (2, 3, 12, 6),
   (2, 4, 12, 4),
   (2, 5, 12, 1),
   (2, 6, 12, 4),
   (2, 8, 12, 1),
   (2, 9, 12, 2),
   (2, 1, 13, 2),
   (2, 2, 13, 1),
   (2, 3, 13, 3),
   (2, 4, 13, 11),
   (2, 5, 13, 2),
   (2, 6, 13, 2),
   (2, 8, 13, 3),
   (2, 9, 13, 6),
   (2, 1, 14, 2),
   (2, 2, 14, 1),
   (2, 3, 14, 7),
   (2, 4, 14, 4),
   (2, 5, 14, 2),
   (2, 6, 14, 2),
   (2, 8, 14, 1),
   (2, 9, 14, 3),
   (2, 1, 15, 5),
   (2, 3, 15, 1),
   (2, 4, 15, 11),
   (2, 6, 15, 3),
   (2, 8, 15, 3),
   (2, 9, 15, 4),
   (2, 1, 16, 6),
   (2, 2, 16, 1),
   (2, 3, 16, 11),
   (2, 4, 16, 3),
   (2, 5, 16, 1),
   (2, 6, 16, 2),
   (2, 7, 16, 1),
   (2, 8, 16, 2),
   (2, 9, 16, 3),
   (2, 1, 17, 4),
   (2, 2, 17, 1),
   (2, 3, 17, 5),
   (2, 4, 17, 10),
   (2, 5, 17, 1),
   (2, 6, 17, 4),
   (2, 8, 17, 2),
   (2, 9, 17, 3),
   (2, 1, 18, 6),
   (2, 3, 18, 6),
   (2, 4, 18, 7),
   (2, 6, 18, 3),
   (2, 8, 18, 3),
   (2, 9, 18, 5),
   (2, 1, 19, 2),
   (2, 2, 19, 1),
   (2, 3, 19, 6),
   (2, 4, 19, 4),
   (2, 5, 19, 2),
   (2, 6, 19, 2),
   (2, 8, 19, 4),
   (2, 9, 19, 3),
   (2, 1, 20, 5),
   (2, 3, 20, 3),
   (2, 4, 20, 7),
   (2, 6, 20, 3),
   (2, 8, 20, 2),
   (2, 9, 20, 3);

-- Juego 3: Relampagos del Ozama vs Delfines del Atlantico
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (3, 1, 1, 1),
   (3, 2, 1, 4),
   (3, 3, 1, 12),
   (3, 4, 1, 2),
   (3, 5, 1, 2),
   (3, 6, 1, 1),
   (3, 8, 1, 2),
   (3, 9, 1, 2),
   (3, 1, 2, 4),
   (3, 2, 2, 2),
   (3, 3, 2, 4),
   (3, 4, 2, 7),
   (3, 5, 2, 1),
   (3, 6, 2, 4),
   (3, 7, 2, 1),
   (3, 8, 2, 1),
   (3, 9, 2, 3),
   (3, 1, 3, 5),
   (3, 2, 3, 4),
   (3, 3, 3, 5),
   (3, 4, 3, 5),
   (3, 5, 3, 2),
   (3, 6, 3, 4),
   (3, 8, 3, 2),
   (3, 9, 3, 3),
   (3, 1, 4, 2),
   (3, 2, 4, 3),
   (3, 3, 4, 5),
   (3, 4, 4, 3),
   (3, 5, 4, 2),
   (3, 6, 4, 4),
   (3, 8, 4, 2),
   (3, 9, 4, 2),
   (3, 1, 5, 5),
   (3, 4, 5, 8),
   (3, 5, 5, 1),
   (3, 6, 5, 3),
   (3, 8, 5, 2),
   (3, 9, 5, 2),
   (3, 1, 11, 3),
   (3, 2, 11, 3),
   (3, 3, 11, 8),
   (3, 4, 11, 3),
   (3, 5, 11, 2),
   (3, 6, 11, 2),
   (3, 8, 11, 2),
   (3, 9, 11, 2),
   (3, 1, 12, 2),
   (3, 2, 12, 1),
   (3, 3, 12, 2),
   (3, 4, 12, 7),
   (3, 5, 12, 1),
   (3, 6, 12, 1),
   (3, 8, 12, 2),
   (3, 9, 12, 2),
   (3, 1, 13, 5),
   (3, 2, 13, 1),
   (3, 3, 13, 1),
   (3, 4, 13, 8),
   (3, 6, 13, 2),
   (3, 8, 13, 2),
   (3, 9, 13, 3),
   (3, 1, 14, 2),
   (3, 2, 14, 1),
   (3, 3, 14, 4),
   (3, 4, 14, 3),
   (3, 5, 14, 3),
   (3, 6, 14, 2),
   (3, 8, 14, 1),
   (3, 9, 14, 2),
   (3, 1, 15, 4),
   (3, 4, 15, 9),
   (3, 5, 15, 1),
   (3, 6, 15, 5),
   (3, 8, 15, 1),
   (3, 9, 15, 2);

-- Juego 4: Panteras del Cibao vs Bucaneros del Este
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (4, 1, 6, 3),
   (4, 2, 6, 1),
   (4, 3, 6, 2),
   (4, 4, 6, 2),
   (4, 5, 6, 3),
   (4, 6, 6, 5),
   (4, 8, 6, 2),
   (4, 9, 6, 5),
   (4, 1, 7, 4),
   (4, 2, 7, 3),
   (4, 3, 7, 10),
   (4, 4, 7, 13),
   (4, 5, 7, 2),
   (4, 6, 7, 3),
   (4, 8, 7, 2),
   (4, 9, 7, 5),
   (4, 1, 8, 6),
   (4, 2, 8, 2),
   (4, 3, 8, 6),
   (4, 4, 8, 6),
   (4, 5, 8, 1),
   (4, 6, 8, 3),
   (4, 8, 8, 3),
   (4, 9, 8, 4),
   (4, 1, 9, 3),
   (4, 2, 9, 1),
   (4, 3, 9, 4),
   (4, 4, 9, 3),
   (4, 5, 9, 2),
   (4, 6, 9, 1),
   (4, 8, 9, 3),
   (4, 9, 9, 3),
   (4, 1, 10, 2),
   (4, 4, 10, 5),
   (4, 5, 10, 1),
   (4, 6, 10, 3),
   (4, 9, 10, 2),
   (4, 1, 16, 2),
   (4, 2, 16, 2),
   (4, 3, 16, 6),
   (4, 4, 16, 3),
   (4, 5, 16, 2),
   (4, 6, 16, 4),
   (4, 8, 16, 3),
   (4, 9, 16, 4),
   (4, 1, 17, 3),
   (4, 2, 17, 1),
   (4, 3, 17, 5),
   (4, 4, 17, 7),
   (4, 6, 17, 3),
   (4, 8, 17, 1),
   (4, 9, 17, 5),
   (4, 1, 18, 8),
   (4, 2, 18, 2),
   (4, 3, 18, 3),
   (4, 4, 18, 6),
   (4, 6, 18, 4),
   (4, 8, 18, 3),
   (4, 9, 18, 2),
   (4, 1, 19, 2),
   (4, 2, 19, 2),
   (4, 3, 19, 9),
   (4, 4, 19, 2),
   (4, 5, 19, 2),
   (4, 6, 19, 2),
   (4, 8, 19, 4),
   (4, 9, 19, 3),
   (4, 1, 20, 4),
   (4, 3, 20, 3),
   (4, 4, 20, 4),
   (4, 5, 20, 1),
   (4, 6, 20, 4),
   (4, 8, 20, 1),
   (4, 9, 20, 4);

-- Juego 5: Relampagos del Ozama vs Bucaneros del Este
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (5, 1, 1, 6),
   (5, 2, 1, 1),
   (5, 3, 1, 12),
   (5, 4, 1, 12),
   (5, 5, 1, 1),
   (5, 6, 1, 2),
   (5, 8, 1, 3),
   (5, 9, 1, 2),
   (5, 1, 2, 2),
   (5, 2, 2, 1),
   (5, 3, 2, 4),
   (5, 4, 2, 7),
   (5, 5, 2, 1),
   (5, 6, 2, 3),
   (5, 8, 2, 1),
   (5, 9, 2, 2),
   (5, 1, 3, 8),
   (5, 2, 3, 2),
   (5, 3, 3, 2),
   (5, 4, 3, 7),
   (5, 5, 3, 1),
   (5, 6, 3, 2),
   (5, 7, 3, 1),
   (5, 8, 3, 2),
   (5, 9, 3, 3),
   (5, 1, 4, 3),
   (5, 2, 4, 2),
   (5, 3, 4, 6),
   (5, 4, 4, 2),
   (5, 5, 4, 2),
   (5, 6, 4, 1),
   (5, 8, 4, 4),
   (5, 9, 4, 2),
   (5, 1, 5, 5),
   (5, 4, 5, 19),
   (5, 6, 5, 3),
   (5, 8, 5, 3),
   (5, 9, 5, 3),
   (5, 1, 16, 4),
   (5, 2, 16, 3),
   (5, 3, 16, 6),
   (5, 4, 16, 2),
   (5, 5, 16, 2),
   (5, 6, 16, 2),
   (5, 8, 16, 4),
   (5, 9, 16, 2),
   (5, 1, 17, 4),
   (5, 2, 17, 3),
   (5, 3, 17, 4),
   (5, 4, 17, 10),
   (5, 6, 17, 4),
   (5, 8, 17, 3),
   (5, 9, 17, 5),
   (5, 1, 18, 2),
   (5, 2, 18, 1),
   (5, 3, 18, 5),
   (5, 4, 18, 6),
   (5, 5, 18, 1),
   (5, 6, 18, 1),
   (5, 8, 18, 2),
   (5, 9, 18, 4),
   (5, 1, 19, 4),
   (5, 2, 19, 2),
   (5, 3, 19, 3),
   (5, 4, 19, 2),
   (5, 5, 19, 3),
   (5, 6, 19, 2),
   (5, 8, 19, 3),
   (5, 9, 19, 5),
   (5, 1, 20, 3),
   (5, 4, 20, 13),
   (5, 5, 20, 1),
   (5, 6, 20, 2),
   (5, 8, 20, 3),
   (5, 9, 20, 4);

-- Juego 6: Panteras del Cibao vs Delfines del Atlantico
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (6, 1, 6, 1),
   (6, 2, 6, 3),
   (6, 3, 6, 5),
   (6, 4, 6, 4),
   (6, 5, 6, 2),
   (6, 6, 6, 1),
   (6, 8, 6, 3),
   (6, 9, 6, 2),
   (6, 1, 7, 6),
   (6, 2, 7, 4),
   (6, 3, 7, 5),
   (6, 4, 7, 2),
   (6, 6, 7, 2),
   (6, 8, 7, 2),
   (6, 9, 7, 2),
   (6, 1, 8, 5),
   (6, 2, 8, 1),
   (6, 3, 8, 6),
   (6, 4, 8, 14),
   (6, 5, 8, 2),
   (6, 6, 8, 3),
   (6, 8, 8, 3),
   (6, 9, 8, 4),
   (6, 1, 9, 5),
   (6, 2, 9, 2),
   (6, 3, 9, 7),
   (6, 4, 9, 2),
   (6, 5, 9, 1),
   (6, 6, 9, 2),
   (6, 8, 9, 3),
   (6, 9, 9, 3),
   (6, 1, 10, 2),
   (6, 4, 10, 4),
   (6, 5, 10, 1),
   (6, 6, 10, 3),
   (6, 8, 10, 1),
   (6, 9, 10, 3),
   (6, 1, 11, 6),
   (6, 2, 11, 2),
   (6, 3, 11, 11),
   (6, 4, 11, 11),
   (6, 6, 11, 2),
   (6, 8, 11, 2),
   (6, 9, 11, 4),
   (6, 1, 12, 6),
   (6, 2, 12, 2),
   (6, 3, 12, 1),
   (6, 4, 12, 8),
   (6, 5, 12, 1),
   (6, 6, 12, 4),
   (6, 8, 12, 1),
   (6, 9, 12, 5),
   (6, 1, 13, 2),
   (6, 3, 13, 2),
   (6, 4, 13, 5),
   (6, 5, 13, 1),
   (6, 6, 13, 4),
   (6, 8, 13, 3),
   (6, 9, 13, 4),
   (6, 1, 14, 2),
   (6, 2, 14, 3),
   (6, 3, 14, 6),
   (6, 4, 14, 3),
   (6, 5, 14, 3),
   (6, 6, 14, 2),
   (6, 8, 14, 3),
   (6, 9, 14, 3),
   (6, 1, 15, 3),
   (6, 4, 15, 13),
   (6, 5, 15, 1),
   (6, 6, 15, 4),
   (6, 9, 15, 3);

-- Juego 7: Panteras del Cibao vs Relampagos del Ozama
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (7, 1, 1, 3),
   (7, 2, 1, 4),
   (7, 3, 1, 8),
   (7, 4, 1, 5),
   (7, 5, 1, 1),
   (7, 6, 1, 2),
   (7, 8, 1, 2),
   (7, 9, 1, 4),
   (7, 1, 2, 4),
   (7, 3, 2, 4),
   (7, 4, 2, 7),
   (7, 5, 2, 1),
   (7, 6, 2, 3),
   (7, 8, 2, 3),
   (7, 9, 2, 3),
   (7, 1, 3, 6),
   (7, 2, 3, 2),
   (7, 3, 3, 3),
   (7, 4, 3, 4),
   (7, 5, 3, 3),
   (7, 6, 3, 1),
   (7, 8, 3, 2),
   (7, 9, 3, 4),
   (7, 1, 4, 4),
   (7, 2, 4, 2),
   (7, 3, 4, 5),
   (7, 4, 4, 4),
   (7, 5, 4, 1),
   (7, 6, 4, 1),
   (7, 8, 4, 4),
   (7, 9, 4, 3),
   (7, 1, 5, 3),
   (7, 2, 5, 1),
   (7, 4, 5, 14),
   (7, 5, 5, 1),
   (7, 6, 5, 4),
   (7, 8, 5, 3),
   (7, 9, 5, 3),
   (7, 1, 6, 3),
   (7, 2, 6, 1),
   (7, 3, 6, 5),
   (7, 4, 6, 2),
   (7, 5, 6, 2),
   (7, 6, 6, 2),
   (7, 8, 6, 4),
   (7, 9, 6, 4),
   (7, 1, 7, 5),
   (7, 2, 7, 2),
   (7, 3, 7, 10),
   (7, 4, 7, 11),
   (7, 5, 7, 1),
   (7, 6, 7, 2),
   (7, 8, 7, 3),
   (7, 9, 7, 3),
   (7, 1, 8, 5),
   (7, 2, 8, 2),
   (7, 3, 8, 3),
   (7, 4, 8, 11),
   (7, 5, 8, 1),
   (7, 6, 8, 4),
   (7, 8, 8, 1),
   (7, 9, 8, 4),
   (7, 1, 9, 3),
   (7, 2, 9, 1),
   (7, 3, 9, 7),
   (7, 4, 9, 3),
   (7, 5, 9, 1),
   (7, 6, 9, 2),
   (7, 8, 9, 4),
   (7, 9, 9, 5),
   (7, 1, 10, 4),
   (7, 2, 10, 1),
   (7, 4, 10, 9),
   (7, 5, 10, 1),
   (7, 6, 10, 2),
   (7, 8, 10, 2),
   (7, 9, 10, 3);

-- Juego 8: Bucaneros del Este vs Delfines del Atlantico
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (8, 1, 11, 4),
   (8, 2, 11, 3),
   (8, 3, 11, 9),
   (8, 4, 11, 6),
   (8, 8, 11, 2),
   (8, 9, 11, 5),
   (8, 1, 12, 5),
   (8, 3, 12, 5),
   (8, 4, 12, 9),
   (8, 5, 12, 1),
   (8, 6, 12, 3),
   (8, 8, 12, 2),
   (8, 9, 12, 5),
   (8, 1, 13, 3),
   (8, 2, 13, 1),
   (8, 3, 13, 6),
   (8, 4, 13, 5),
   (8, 6, 13, 2),
   (8, 8, 13, 1),
   (8, 9, 13, 3),
   (8, 1, 14, 4),
   (8, 2, 14, 1),
   (8, 3, 14, 4),
   (8, 4, 14, 1),
   (8, 5, 14, 1),
   (8, 6, 14, 3),
   (8, 8, 14, 2),
   (8, 9, 14, 3),
   (8, 1, 15, 2),
   (8, 3, 15, 3),
   (8, 4, 15, 7),
   (8, 5, 15, 2),
   (8, 6, 15, 2),
   (8, 8, 15, 2),
   (8, 9, 15, 1),
   (8, 1, 16, 4),
   (8, 2, 16, 3),
   (8, 3, 16, 6),
   (8, 4, 16, 3),
   (8, 5, 16, 5),
   (8, 8, 16, 2),
   (8, 9, 16, 4),
   (8, 1, 17, 4),
   (8, 2, 17, 1),
   (8, 3, 17, 3),
   (8, 4, 17, 4),
   (8, 6, 17, 1),
   (8, 8, 17, 3),
   (8, 9, 17, 3),
   (8, 1, 18, 9),
   (8, 2, 18, 1),
   (8, 3, 18, 7),
   (8, 4, 18, 9),
   (8, 5, 18, 1),
   (8, 6, 18, 1),
   (8, 8, 18, 5),
   (8, 9, 18, 4),
   (8, 1, 19, 2),
   (8, 2, 19, 1),
   (8, 3, 19, 5),
   (8, 4, 19, 2),
   (8, 5, 19, 4),
   (8, 6, 19, 1),
   (8, 8, 19, 5),
   (8, 9, 19, 2),
   (8, 1, 20, 3),
   (8, 4, 20, 7),
   (8, 5, 20, 2),
   (8, 6, 20, 1),
   (8, 8, 20, 1),
   (8, 9, 20, 1);

-- Juego 9: Delfines del Atlantico vs Relampagos del Ozama
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (9, 1, 1, 5),
   (9, 2, 1, 1),
   (9, 3, 1, 12),
   (9, 4, 1, 11),
   (9, 5, 1, 1),
   (9, 6, 1, 3),
   (9, 8, 1, 5),
   (9, 9, 1, 2),
   (9, 1, 2, 3),
   (9, 2, 2, 3),
   (9, 3, 2, 2),
   (9, 4, 2, 8),
   (9, 6, 2, 3),
   (9, 9, 2, 4),
   (9, 1, 3, 6),
   (9, 2, 3, 1),
   (9, 3, 3, 7),
   (9, 4, 3, 12),
   (9, 6, 3, 4),
   (9, 8, 3, 2),
   (9, 9, 3, 3),
   (9, 1, 4, 2),
   (9, 2, 4, 2),
   (9, 3, 4, 5),
   (9, 4, 4, 1),
   (9, 5, 4, 2),
   (9, 8, 4, 5),
   (9, 9, 4, 4),
   (9, 1, 5, 3),
   (9, 4, 5, 10),
   (9, 5, 5, 3),
   (9, 6, 5, 4),
   (9, 8, 5, 1),
   (9, 9, 5, 3),
   (9, 1, 11, 5),
   (9, 2, 11, 1),
   (9, 3, 11, 10),
   (9, 4, 11, 13),
   (9, 5, 11, 2),
   (9, 6, 11, 2),
   (9, 8, 11, 3),
   (9, 9, 11, 2),
   (9, 1, 12, 4),
   (9, 2, 12, 2),
   (9, 3, 12, 4),
   (9, 4, 12, 6),
   (9, 5, 12, 2),
   (9, 6, 12, 4),
   (9, 8, 12, 2),
   (9, 9, 12, 1),
   (9, 1, 13, 1),
   (9, 2, 13, 2),
   (9, 3, 13, 4),
   (9, 4, 13, 5),
   (9, 6, 13, 4),
   (9, 8, 13, 2),
   (9, 9, 13, 3),
   (9, 1, 14, 2),
   (9, 2, 14, 2),
   (9, 3, 14, 3),
   (9, 4, 14, 3),
   (9, 5, 14, 1),
   (9, 6, 14, 2),
   (9, 8, 14, 3),
   (9, 9, 14, 4),
   (9, 1, 15, 2),
   (9, 4, 15, 12),
   (9, 6, 15, 1),
   (9, 8, 15, 1),
   (9, 9, 15, 3);

-- Juego 10: Bucaneros del Este vs Panteras del Cibao
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (10, 1, 6, 4),
   (10, 3, 6, 7),
   (10, 4, 6, 3),
   (10, 5, 6, 1),
   (10, 6, 6, 1),
   (10, 8, 6, 4),
   (10, 9, 6, 1),
   (10, 1, 7, 6),
   (10, 3, 7, 5),
   (10, 4, 7, 8),
   (10, 5, 7, 1),
   (10, 6, 7, 4),
   (10, 8, 7, 2),
   (10, 9, 7, 6),
   (10, 1, 8, 2),
   (10, 2, 8, 2),
   (10, 3, 8, 7),
   (10, 4, 8, 10),
   (10, 5, 8, 1),
   (10, 6, 8, 3),
   (10, 8, 8, 3),
   (10, 9, 8, 4),
   (10, 1, 9, 1),
   (10, 2, 9, 3),
   (10, 3, 9, 8),
   (10, 4, 9, 2),
   (10, 5, 9, 4),
   (10, 6, 9, 3),
   (10, 8, 9, 3),
   (10, 9, 9, 3),
   (10, 1, 10, 3),
   (10, 2, 10, 1),
   (10, 4, 10, 8),
   (10, 5, 10, 1),
   (10, 6, 10, 4),
   (10, 8, 10, 1),
   (10, 9, 10, 3),
   (10, 1, 16, 4),
   (10, 2, 16, 4),
   (10, 3, 16, 11),
   (10, 4, 16, 3),
   (10, 5, 16, 3),
   (10, 6, 16, 3),
   (10, 8, 16, 3),
   (10, 9, 16, 3),
   (10, 1, 17, 3),
   (10, 2, 17, 2),
   (10, 3, 17, 3),
   (10, 4, 17, 8),
   (10, 5, 17, 3),
   (10, 6, 17, 3),
   (10, 8, 17, 3),
   (10, 9, 17, 3),
   (10, 1, 18, 5),
   (10, 3, 18, 4),
   (10, 4, 18, 7),
   (10, 5, 18, 3),
   (10, 6, 18, 4),
   (10, 8, 18, 1),
   (10, 9, 18, 3),
   (10, 1, 19, 2),
   (10, 2, 19, 3),
   (10, 3, 19, 5),
   (10, 4, 19, 3),
   (10, 6, 19, 1),
   (10, 8, 19, 2),
   (10, 9, 19, 4),
   (10, 1, 20, 4),
   (10, 4, 20, 10),
   (10, 5, 20, 2),
   (10, 6, 20, 2),
   (10, 8, 20, 2),
   (10, 9, 20, 2);

-- Juego 11: Bucaneros del Este vs Relampagos del Ozama
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (11, 1, 1, 5),
   (11, 2, 1, 3),
   (11, 3, 1, 11),
   (11, 4, 1, 13),
   (11, 5, 1, 3),
   (11, 6, 1, 1),
   (11, 8, 1, 2),
   (11, 9, 1, 4),
   (11, 1, 2, 4),
   (11, 2, 2, 1),
   (11, 3, 2, 1),
   (11, 4, 2, 7),
   (11, 5, 2, 2),
   (11, 6, 2, 2),
   (11, 8, 2, 2),
   (11, 9, 2, 3),
   (11, 1, 3, 5),
   (11, 3, 3, 6),
   (11, 4, 3, 8),
   (11, 5, 3, 1),
   (11, 6, 3, 3),
   (11, 9, 3, 4),
   (11, 1, 4, 4),
   (11, 2, 4, 4),
   (11, 3, 4, 4),
   (11, 4, 4, 3),
   (11, 5, 4, 2),
   (11, 6, 4, 3),
   (11, 7, 4, 1),
   (11, 8, 4, 3),
   (11, 9, 4, 4),
   (11, 1, 5, 2),
   (11, 2, 5, 1),
   (11, 4, 5, 8),
   (11, 5, 5, 1),
   (11, 6, 5, 4),
   (11, 8, 5, 1),
   (11, 9, 5, 3),
   (11, 1, 16, 5),
   (11, 2, 16, 2),
   (11, 3, 16, 7),
   (11, 4, 16, 5),
   (11, 5, 16, 1),
   (11, 6, 16, 3),
   (11, 8, 16, 3),
   (11, 9, 16, 3),
   (11, 1, 17, 4),
   (11, 2, 17, 3),
   (11, 3, 17, 4),
   (11, 4, 17, 6),
   (11, 5, 17, 1),
   (11, 6, 17, 3),
   (11, 8, 17, 1),
   (11, 9, 17, 1),
   (11, 1, 18, 3),
   (11, 2, 18, 3),
   (11, 3, 18, 3),
   (11, 4, 18, 7),
   (11, 6, 18, 3),
   (11, 8, 18, 1),
   (11, 9, 18, 1),
   (11, 1, 19, 1),
   (11, 3, 19, 4),
   (11, 4, 19, 2),
   (11, 5, 19, 2),
   (11, 6, 19, 2),
   (11, 8, 19, 2),
   (11, 9, 19, 3),
   (11, 1, 20, 6),
   (11, 3, 20, 3),
   (11, 4, 20, 9),
   (11, 6, 20, 3),
   (11, 8, 20, 4),
   (11, 9, 20, 3);

-- Juego 12: Delfines del Atlantico vs Panteras del Cibao
INSERT INTO EstadisticaJuego (CodJuego, CodEstadistica, CodJugador, Cantidad) VALUES
   (12, 1, 6, 1),
   (12, 2, 6, 4),
   (12, 3, 6, 7),
   (12, 4, 6, 2),
   (12, 5, 6, 2),
   (12, 6, 6, 2),
   (12, 8, 6, 2),
   (12, 9, 6, 3),
   (12, 1, 7, 4),
   (12, 2, 7, 1),
   (12, 3, 7, 12),
   (12, 4, 7, 12),
   (12, 5, 7, 2),
   (12, 6, 7, 4),
   (12, 8, 7, 3),
   (12, 9, 7, 2),
   (12, 1, 8, 4),
   (12, 2, 8, 2),
   (12, 3, 8, 6),
   (12, 4, 8, 7),
   (12, 5, 8, 2),
   (12, 6, 8, 3),
   (12, 8, 8, 2),
   (12, 9, 8, 2),
   (12, 1, 9, 3),
   (12, 2, 9, 3),
   (12, 3, 9, 8),
   (12, 4, 9, 3),
   (12, 5, 9, 3),
   (12, 6, 9, 2),
   (12, 8, 9, 3),
   (12, 9, 9, 2),
   (12, 1, 10, 3),
   (12, 4, 10, 10),
   (12, 6, 10, 5),
   (12, 8, 10, 4),
   (12, 9, 10, 4),
   (12, 1, 11, 3),
   (12, 2, 11, 3),
   (12, 3, 11, 6),
   (12, 4, 11, 5),
   (12, 5, 11, 1),
   (12, 6, 11, 4),
   (12, 8, 11, 3),
   (12, 9, 11, 3),
   (12, 1, 12, 5),
   (12, 2, 12, 2),
   (12, 3, 12, 4),
   (12, 4, 12, 9),
   (12, 5, 12, 2),
   (12, 6, 12, 3),
   (12, 8, 12, 2),
   (12, 9, 12, 3),
   (12, 1, 13, 3),
   (12, 2, 13, 2),
   (12, 3, 13, 2),
   (12, 4, 13, 4),
   (12, 6, 13, 4),
   (12, 8, 13, 3),
   (12, 9, 13, 3),
   (12, 1, 14, 2),
   (12, 2, 14, 3),
   (12, 3, 14, 7),
   (12, 4, 14, 4),
   (12, 5, 14, 1),
   (12, 6, 14, 2),
   (12, 8, 14, 2),
   (12, 9, 14, 3),
   (12, 1, 15, 2),
   (12, 4, 15, 5),
   (12, 6, 15, 3),
   (12, 8, 15, 2),
   (12, 9, 15, 3);
GO
