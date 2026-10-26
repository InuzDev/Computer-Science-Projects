CREATE DATABASE "AcadDB(10165529+10160024)";
GO
USE "AcadDB(10165529+10160024)";
GO

CREATE TABLE Carrera       (IdCarrera      VARCHAR(5)  NOT NULL PRIMARY KEY, NombreCarrera VARCHAR(50) NOT NULL);
CREATE TABLE CategoriaPago (IdCategoriaPago      VARCHAR(3)  NOT NULL PRIMARY KEY, Descripcion VARCHAR(80) NOT NULL);
CREATE TABLE Nacionalidad  (IdNacionalidad VARCHAR(3)  NOT NULL PRIMARY KEY, Nombre      VARCHAR(30) NOT NULL);

CREATE TABLE Estudiante (
    IdEstudiante   VARCHAR(8)   NOT NULL PRIMARY KEY,
    Nombre         VARCHAR(25)  NOT NULL,
    Apellido       VARCHAR(20)  NOT NULL,
    IdCarrera      VARCHAR(5)   NOT NULL REFERENCES Carrera(IdCarrera),
    IdCategoriaPago      VARCHAR(3)   NOT NULL REFERENCES CategoriaPago(IdCategoriaPago),
    IdNacionalidad VARCHAR(3)   NOT NULL REFERENCES Nacionalidad(IdNacionalidad),
    Direccion      VARCHAR(200) NULL
);
CREATE TABLE Asignatura (
    IdAsignatura  VARCHAR(8)   NOT NULL PRIMARY KEY,
    Nombre         VARCHAR(30)  NOT NULL,
    Creditos       TINYINT      NOT NULL,
    HorasTeorica  TINYINT      NOT NULL,
    HorasPractica TINYINT      NOT NULL
);
CREATE TABLE PeriodoAcademico (
    IdPeriodo      VARCHAR(10) NOT NULL PRIMARY KEY,
    Descripcion     VARCHAR(25) NOT NULL,
    LimFechaPrematricula DATE        NULL,
    LimFechaRetiro  DATE        NULL,
    LimPublicacionCalificacion   DATE        NULL
);
CREATE TABLE Grupo (
    IdPeriodo     VARCHAR(10) NOT NULL REFERENCES PeriodoAcademico(IdPeriodo),
    IdAsignatura  VARCHAR(8)  NOT NULL REFERENCES Asignatura(IdAsignatura),
    IdGrupo       VARCHAR(4)  NOT NULL,
    CupoGrupo           INT         NOT NULL,
    Horario        VARCHAR(60) NULL,
    PRIMARY KEY (IdPeriodo, IdAsignatura, IdGrupo)
);
CREATE TABLE Diasemana (
    Dia         TINYINT    NOT NULL PRIMARY KEY,
    Descripcion VARCHAR(9) NOT NULL,
    NombreCorto CHAR(2)    NOT NULL
);
CREATE TABLE HorarioGrupo (
    IdPeriodo     VARCHAR(10) NOT NULL,
    IdAsignatura  VARCHAR(8)  NOT NULL,
    IdGrupo       VARCHAR(4)  NOT NULL,
    Dia            TINYINT     NOT NULL REFERENCES Diasemana(Dia),
    HoraInicio     TIME        NOT NULL,
    HoraFin        TIME        NOT NULL,
    PRIMARY KEY (IdPeriodo, IdAsignatura, IdGrupo, Dia, HoraInicio),
    FOREIGN KEY (IdPeriodo, IdAsignatura, IdGrupo) REFERENCES Grupo(IdPeriodo, IdAsignatura, IdGrupo)
);
CREATE TABLE Inscripcion (
    IdPeriodo       VARCHAR(10) NOT NULL REFERENCES PeriodoAcademico(IdPeriodo),
    IdEstudiante     VARCHAR(8)  NOT NULL REFERENCES Estudiante(IdEstudiante),
    FechaInscripcion DATE        NOT NULL,
    PRIMARY KEY (IdPeriodo, IdEstudiante)
);
CREATE TABLE GrupoInscrito (
    IdPeriodo     VARCHAR(10) NOT NULL,
    IdEstudiante   VARCHAR(8)  NOT NULL,
    IdAsignatura  VARCHAR(8)  NOT NULL,
    IdGrupo       VARCHAR(4)  NOT NULL,
    PRIMARY KEY (IdPeriodo, IdEstudiante, IdAsignatura, IdGrupo),
    FOREIGN KEY (IdPeriodo, IdEstudiante) REFERENCES Inscripcion(IdPeriodo, IdEstudiante),
    FOREIGN KEY (IdPeriodo, IdAsignatura, IdGrupo) REFERENCES Grupo(IdPeriodo, IdAsignatura, IdGrupo)
);
GO

INSERT INTO Diasemana VALUES (1,'Lunes','Lu'), (2,'Martes','Ma'), (3,'Miercoles','Mi'),
                             (4,'Jueves','Ju'), (5,'Viernes','Vi'), (6,'Sabado','Sa'), (7,'Domingo','Do');
GO

ALTER TABLE Carrera          ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE CategoriaPago    ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE Nacionalidad     ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE Estudiante       ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE Asignatura       ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE PeriodoAcademico ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE Grupo            ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE Diasemana        ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE HorarioGrupo     ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE Inscripcion      ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
ALTER TABLE GrupoInscrito    ADD Actividad VARCHAR(30) NULL, Usuario VARCHAR(50) NULL, FechaHora DATETIME NULL;
GO

UPDATE Carrera          SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE CategoriaPago    SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE Nacionalidad     SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE Estudiante       SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE Asignatura       SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE PeriodoAcademico SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE Grupo            SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE Diasemana        SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE HorarioGrupo     SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE Inscripcion      SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
UPDATE GrupoInscrito    SET Actividad = 'Insertado', Usuario = SUSER_SNAME(), FechaHora = GETDATE() WHERE Actividad IS NULL;
GO

CREATE TABLE Auditoria (
    IdAuditoria INT IDENTITY(1,1) PRIMARY KEY,
    Tabla       VARCHAR(40) NOT NULL,
    Actividad   VARCHAR(30) NOT NULL,
    Columna     VARCHAR(15) NULL,
    FechaHora   DATETIME    NOT NULL,
    Usuario     VARCHAR(50) NOT NULL
);
GO

CREATE TRIGGER trg_Carrera_Control
ON Carrera
AFTER INSERT, UPDATE
AS
BEGIN
    SET NOCOUNT ON;

    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;

    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN

        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        UPDATE t
        SET    t.Actividad = 'Modificado',
               t.FechaHora = GETDATE()
        FROM   Carrera t
        JOIN   inserted i ON i.IdCarrera = t.IdCarrera;
    END
    ELSE
    BEGIN
        UPDATE t
        SET    t.Actividad = 'Insertado',
               t.Usuario   = SUSER_SNAME(),
               t.FechaHora = GETDATE()
        FROM   Carrera t
        JOIN   inserted i ON i.IdCarrera = t.IdCarrera;
    END
END
GO

CREATE TRIGGER trg_Carrera_Auditoria
ON Carrera
AFTER INSERT, UPDATE, DELETE
AS
BEGIN
    SET NOCOUNT ON;

    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Carrera', 'Insertando', NULL, GETDATE(), SUSER_SNAME()
        FROM   inserted;

    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Carrera', 'Eliminando', NULL, GETDATE(), SUSER_SNAME()
        FROM   deleted;

    ELSE
    BEGIN

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Carrera', 'Modificando', 'NombreCarrera', GETDATE(), SUSER_SNAME()
        FROM   inserted i
        JOIN   deleted  d ON d.IdCarrera = i.IdCarrera
        WHERE  EXISTS (SELECT i.NombreCarrera EXCEPT SELECT d.NombreCarrera);

    END
END
GO

CREATE FUNCTION dbo.fn_FormatoHora (@t TIME)
RETURNS VARCHAR(5)
AS
BEGIN
    RETURN CAST(DATEPART(HOUR, @t) AS VARCHAR(2))
         + CASE WHEN DATEPART(MINUTE, @t) = 0 THEN ''
                ELSE ':' + RIGHT('0' + CAST(DATEPART(MINUTE, @t) AS VARCHAR(2)), 2) END;
END
GO

CREATE FUNCTION dbo.fn_HorarioGrupo (@Periodo VARCHAR(10), @Asignatura VARCHAR(8), @Grupo VARCHAR(4))
RETURNS VARCHAR(60)
AS
BEGIN
    DECLARE @Resultado VARCHAR(60);

    WITH Bloques AS (
        SELECT h.HoraInicio, h.HoraFin,
               MIN(h.Dia) AS PrimerDia,
               STRING_AGG(d.NombreCorto, '') WITHIN GROUP (ORDER BY h.Dia) AS Dias
        FROM   HorarioGrupo h
        JOIN   Diasemana d ON d.Dia = h.Dia
        WHERE  h.IdPeriodo = @Periodo AND h.IdAsignatura = @Asignatura AND h.IdGrupo = @Grupo
        GROUP  BY h.HoraInicio, h.HoraFin
    )
    SELECT @Resultado = STRING_AGG(
               dbo.fn_FormatoHora(HoraInicio) + '-' + dbo.fn_FormatoHora(HoraFin) + ' ' + Dias, ' ')
           WITHIN GROUP (ORDER BY PrimerDia)
    FROM   Bloques;

    RETURN @Resultado;
END
GO

CREATE TRIGGER trg_HorarioGrupo_Horario
ON HorarioGrupo
AFTER INSERT, UPDATE, DELETE
AS
BEGIN
    SET NOCOUNT ON;

    ;WITH Afectados AS (
        SELECT IdPeriodo, IdAsignatura, IdGrupo FROM inserted
        UNION
        SELECT IdPeriodo, IdAsignatura, IdGrupo FROM deleted
    )
    UPDATE g
    SET    g.Horario = dbo.fn_HorarioGrupo(g.IdPeriodo, g.IdAsignatura, g.IdGrupo)
    FROM   Grupo g
    JOIN   Afectados a ON a.IdPeriodo    = g.IdPeriodo
                      AND a.IdAsignatura = g.IdAsignatura
                      AND a.IdGrupo      = g.IdGrupo;
END
GO

CREATE TRIGGER trg_CategoriaPago_Control ON CategoriaPago AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM CategoriaPago t JOIN inserted i ON i.IdCategoriaPago = t.IdCategoriaPago;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM CategoriaPago t JOIN inserted i ON i.IdCategoriaPago = t.IdCategoriaPago;
END
GO
CREATE TRIGGER trg_CategoriaPago_Auditoria ON CategoriaPago AFTER INSERT, UPDATE, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'CategoriaPago', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'CategoriaPago', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
    ELSE
    BEGIN
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'CategoriaPago', 'Modificando', 'Descripcion', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdCategoriaPago = i.IdCategoriaPago
        WHERE EXISTS (SELECT i.Descripcion EXCEPT SELECT d.Descripcion);
    END
END
GO

CREATE TRIGGER trg_Nacionalidad_Control ON Nacionalidad AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM Nacionalidad t JOIN inserted i ON i.IdNacionalidad = t.IdNacionalidad;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM Nacionalidad t JOIN inserted i ON i.IdNacionalidad = t.IdNacionalidad;
END
GO
CREATE TRIGGER trg_Nacionalidad_Auditoria ON Nacionalidad AFTER INSERT, UPDATE, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Nacionalidad', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Nacionalidad', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
    ELSE
    BEGIN
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Nacionalidad', 'Modificando', 'Nombre', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdNacionalidad = i.IdNacionalidad
        WHERE EXISTS (SELECT i.Nombre EXCEPT SELECT d.Nombre);
    END
END
GO

CREATE TRIGGER trg_Asignatura_Control ON Asignatura AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM Asignatura t JOIN inserted i ON i.IdAsignatura = t.IdAsignatura;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM Asignatura t JOIN inserted i ON i.IdAsignatura = t.IdAsignatura;
END
GO
CREATE TRIGGER trg_Asignatura_Auditoria ON Asignatura AFTER INSERT, UPDATE, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Asignatura', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Asignatura', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
    ELSE
    BEGIN
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Asignatura', 'Modificando', 'Nombre', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdAsignatura = i.IdAsignatura
        WHERE EXISTS (SELECT i.Nombre EXCEPT SELECT d.Nombre);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Asignatura', 'Modificando', 'Creditos', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdAsignatura = i.IdAsignatura
        WHERE EXISTS (SELECT i.Creditos EXCEPT SELECT d.Creditos);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Asignatura', 'Modificando', 'HorasTeorica', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdAsignatura = i.IdAsignatura
        WHERE EXISTS (SELECT i.HorasTeorica EXCEPT SELECT d.HorasTeorica);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Asignatura', 'Modificando', 'HorasPractica', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdAsignatura = i.IdAsignatura
        WHERE EXISTS (SELECT i.HorasPractica EXCEPT SELECT d.HorasPractica);
    END
END
GO

CREATE TRIGGER trg_Estudiante_Control ON Estudiante AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);

            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM Estudiante t JOIN inserted i ON i.IdEstudiante = t.IdEstudiante;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM Estudiante t JOIN inserted i ON i.IdEstudiante = t.IdEstudiante;
END
GO
CREATE TRIGGER trg_Estudiante_Auditoria ON Estudiante AFTER INSERT, UPDATE, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Estudiante', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Estudiante', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
    ELSE
    BEGIN
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Estudiante', 'Modificando', 'Nombre', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdEstudiante = i.IdEstudiante
        WHERE EXISTS (SELECT i.Nombre EXCEPT SELECT d.Nombre);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Estudiante', 'Modificando', 'Apellido', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdEstudiante = i.IdEstudiante
        WHERE EXISTS (SELECT i.Apellido EXCEPT SELECT d.Apellido);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Estudiante', 'Modificando', 'IdCarrera', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdEstudiante = i.IdEstudiante
        WHERE EXISTS (SELECT i.IdCarrera EXCEPT SELECT d.IdCarrera);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Estudiante', 'Modificando', 'IdCategoriaPago', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdEstudiante = i.IdEstudiante
        WHERE EXISTS (SELECT i.IdCategoriaPago EXCEPT SELECT d.IdCategoriaPago);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Estudiante', 'Modificando', 'IdNacionalidad', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdEstudiante = i.IdEstudiante
        WHERE EXISTS (SELECT i.IdNacionalidad EXCEPT SELECT d.IdNacionalidad);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Estudiante', 'Modificando', 'Direccion', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdEstudiante = i.IdEstudiante
        WHERE EXISTS (SELECT i.Direccion EXCEPT SELECT d.Direccion);
    END
END
GO

CREATE TRIGGER trg_PeriodoAcademico_Control ON PeriodoAcademico AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM PeriodoAcademico t JOIN inserted i ON i.IdPeriodo = t.IdPeriodo;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM PeriodoAcademico t JOIN inserted i ON i.IdPeriodo = t.IdPeriodo;
END
GO
CREATE TRIGGER trg_PeriodoAcademico_Auditoria ON PeriodoAcademico AFTER INSERT, UPDATE, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'PeriodoAcademico', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'PeriodoAcademico', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
    ELSE
    BEGIN
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'PeriodoAcademico', 'Modificando', 'Descripcion', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdPeriodo = i.IdPeriodo
        WHERE EXISTS (SELECT i.Descripcion EXCEPT SELECT d.Descripcion);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'PeriodoAcademico', 'Modificando', 'LimFechaPrematr', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdPeriodo = i.IdPeriodo
        WHERE EXISTS (SELECT i.LimFechaPrematricula EXCEPT SELECT d.LimFechaPrematricula);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'PeriodoAcademico', 'Modificando', 'LimFechaRetiro', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdPeriodo = i.IdPeriodo
        WHERE EXISTS (SELECT i.LimFechaRetiro EXCEPT SELECT d.LimFechaRetiro);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'PeriodoAcademico', 'Modificando', 'LimPublicCalif', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.IdPeriodo = i.IdPeriodo
        WHERE EXISTS (SELECT i.LimPublicacionCalificacion EXCEPT SELECT d.LimPublicacionCalificacion);
    END
END
GO

CREATE TRIGGER trg_Diasemana_Control ON Diasemana AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM Diasemana t JOIN inserted i ON i.Dia = t.Dia;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM Diasemana t JOIN inserted i ON i.Dia = t.Dia;
END
GO
CREATE TRIGGER trg_Diasemana_Auditoria ON Diasemana AFTER INSERT, UPDATE, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Diasemana', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Diasemana', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
    ELSE
    BEGIN
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Diasemana', 'Modificando', 'Descripcion', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.Dia = i.Dia
        WHERE EXISTS (SELECT i.Descripcion EXCEPT SELECT d.Descripcion);

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Diasemana', 'Modificando', 'NombreCorto', GETDATE(), SUSER_SNAME()
        FROM inserted i JOIN deleted d ON d.Dia = i.Dia
        WHERE EXISTS (SELECT i.NombreCorto EXCEPT SELECT d.NombreCorto);
    END
END
GO

CREATE TRIGGER trg_Inscripcion_Control ON Inscripcion AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM Inscripcion t
        JOIN inserted i ON i.IdPeriodo = t.IdPeriodo AND i.IdEstudiante = t.IdEstudiante;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM Inscripcion t
        JOIN inserted i ON i.IdPeriodo = t.IdPeriodo AND i.IdEstudiante = t.IdEstudiante;
END
GO
CREATE TRIGGER trg_Inscripcion_Auditoria ON Inscripcion AFTER INSERT, UPDATE, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Inscripcion', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Inscripcion', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
    ELSE
    BEGIN

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Inscripcion', 'Modificando', 'FechaInscrip', GETDATE(), SUSER_SNAME()
        FROM inserted i
        JOIN deleted d ON d.IdPeriodo = i.IdPeriodo AND d.IdEstudiante = i.IdEstudiante
        WHERE EXISTS (SELECT i.FechaInscripcion EXCEPT SELECT d.FechaInscripcion);
    END
END
GO

CREATE TRIGGER trg_Grupo_Control ON Grupo AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF TRIGGER_NESTLEVEL(OBJECT_ID('dbo.trg_HorarioGrupo_Horario'), 'AFTER', 'DML') > 0 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM Grupo t
        JOIN inserted i ON i.IdPeriodo = t.IdPeriodo AND i.IdAsignatura = t.IdAsignatura
                       AND i.IdGrupo = t.IdGrupo;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM Grupo t
        JOIN inserted i ON i.IdPeriodo = t.IdPeriodo AND i.IdAsignatura = t.IdAsignatura
                       AND i.IdGrupo = t.IdGrupo;
END
GO
CREATE TRIGGER trg_Grupo_Auditoria ON Grupo AFTER INSERT, UPDATE, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Grupo', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Grupo', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
    ELSE
    BEGIN
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'Grupo', 'Modificando', 'CupoGrupo', GETDATE(), SUSER_SNAME()
        FROM inserted i
        JOIN deleted d ON d.IdPeriodo = i.IdPeriodo AND d.IdAsignatura = i.IdAsignatura
                      AND d.IdGrupo = i.IdGrupo
        WHERE EXISTS (SELECT i.CupoGrupo EXCEPT SELECT d.CupoGrupo);

        IF TRIGGER_NESTLEVEL(OBJECT_ID('dbo.trg_HorarioGrupo_Horario'), 'AFTER', 'DML') = 0
            INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
            SELECT 'Grupo', 'Modificando', 'Horario', GETDATE(), SUSER_SNAME()
            FROM inserted i
            JOIN deleted d ON d.IdPeriodo = i.IdPeriodo AND d.IdAsignatura = i.IdAsignatura
                          AND d.IdGrupo = i.IdGrupo
            WHERE EXISTS (SELECT i.Horario EXCEPT SELECT d.Horario);
    END
END
GO

CREATE TRIGGER trg_HorarioGrupo_Control ON HorarioGrupo AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM HorarioGrupo t
        JOIN inserted i ON i.IdPeriodo = t.IdPeriodo AND i.IdAsignatura = t.IdAsignatura
                       AND i.IdGrupo = t.IdGrupo AND i.Dia = t.Dia AND i.HoraInicio = t.HoraInicio;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM HorarioGrupo t
        JOIN inserted i ON i.IdPeriodo = t.IdPeriodo AND i.IdAsignatura = t.IdAsignatura
                       AND i.IdGrupo = t.IdGrupo AND i.Dia = t.Dia AND i.HoraInicio = t.HoraInicio;
END
GO
CREATE TRIGGER trg_HorarioGrupo_Auditoria ON HorarioGrupo AFTER INSERT, UPDATE, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted) AND NOT EXISTS (SELECT 1 FROM deleted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'HorarioGrupo', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE IF EXISTS (SELECT 1 FROM deleted) AND NOT EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'HorarioGrupo', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
    ELSE
    BEGIN

        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'HorarioGrupo', 'Modificando', 'HoraFin', GETDATE(), SUSER_SNAME()
        FROM inserted i
        JOIN deleted d ON d.IdPeriodo = i.IdPeriodo AND d.IdAsignatura = i.IdAsignatura
                      AND d.IdGrupo = i.IdGrupo AND d.Dia = i.Dia AND d.HoraInicio = i.HoraInicio
        WHERE EXISTS (SELECT i.HoraFin EXCEPT SELECT d.HoraFin);
    END
END
GO

CREATE TRIGGER trg_GrupoInscrito_Control ON GrupoInscrito AFTER INSERT, UPDATE AS
BEGIN
    SET NOCOUNT ON;
    IF TRIGGER_NESTLEVEL(@@PROCID, 'AFTER', 'DML') > 1 RETURN;
    IF EXISTS (SELECT 1 FROM deleted)
    BEGIN
        IF EXISTS (SELECT 1 FROM deleted WHERE Usuario <> SUSER_SNAME())
        BEGIN
            RAISERROR('Solo el usuario que inserto puede modificar el record.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END
        UPDATE t SET t.Actividad = 'Modificado', t.FechaHora = GETDATE()
        FROM GrupoInscrito t
        JOIN inserted i ON i.IdPeriodo = t.IdPeriodo AND i.IdEstudiante = t.IdEstudiante
                       AND i.IdAsignatura = t.IdAsignatura AND i.IdGrupo = t.IdGrupo;
    END
    ELSE
        UPDATE t SET t.Actividad = 'Insertado', t.Usuario = SUSER_SNAME(), t.FechaHora = GETDATE()
        FROM GrupoInscrito t
        JOIN inserted i ON i.IdPeriodo = t.IdPeriodo AND i.IdEstudiante = t.IdEstudiante
                       AND i.IdAsignatura = t.IdAsignatura AND i.IdGrupo = t.IdGrupo;
END
GO
CREATE TRIGGER trg_GrupoInscrito_Auditoria ON GrupoInscrito AFTER INSERT, DELETE AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (SELECT 1 FROM inserted)
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'GrupoInscrito', 'Insertando', NULL, GETDATE(), SUSER_SNAME() FROM inserted;
    ELSE
        INSERT INTO Auditoria (Tabla, Actividad, Columna, FechaHora, Usuario)
        SELECT 'GrupoInscrito', 'Eliminando', NULL, GETDATE(), SUSER_SNAME() FROM deleted;
END
GO

INSERT INTO PeriodoAcademico (IdPeriodo, Descripcion) VALUES ('202420053', 'Periodo 2024-2');
INSERT INTO Asignatura (IdAsignatura, Nombre, Creditos, HorasTeorica, HorasPractica)
VALUES ('ICC233T', 'Asignatura A', 4, 3, 1), ('ICC103T', 'Asignatura B', 4, 3, 1);
INSERT INTO Grupo (IdPeriodo, IdAsignatura, IdGrupo, CupoGrupo)
VALUES ('202420053','ICC233T','4847',30), ('202420053','ICC103T','5912',30), ('202420053','ICC233T','6345',30);

INSERT INTO HorarioGrupo (IdPeriodo, IdAsignatura, IdGrupo, Dia, HoraInicio, HoraFin) VALUES
 ('202420053','ICC233T','4847',1,'10:00','13:00'), ('202420053','ICC233T','4847',3,'10:00','13:00'),
 ('202420053','ICC103T','5912',1,'18:00','21:00'), ('202420053','ICC103T','5912',3,'18:00','21:00'),
 ('202420053','ICC233T','6345',1,'18:00','20:00'), ('202420053','ICC233T','6345',3,'18:00','19:00'),
 ('202420053','ICC233T','6345',5,'18:00','20:00');

SELECT IdGrupo, Horario FROM Grupo;

UPDATE HorarioGrupo SET HoraFin = '19:00' WHERE IdGrupo = '6345' AND Dia = 1;
DELETE FROM HorarioGrupo WHERE IdGrupo = '4847' AND Dia = 3;
SELECT IdGrupo, Horario FROM Grupo;

INSERT INTO Carrera (IdCarrera, NombreCarrera) VALUES ('ICC', 'Ingenieria en Computacion');
UPDATE Carrera SET NombreCarrera = 'Ing. en Sistemas y Computacion' WHERE IdCarrera = 'ICC';
DELETE FROM Carrera WHERE IdCarrera = 'ICC';
SELECT * FROM Auditoria ORDER BY IdAuditoria;

INSERT INTO CategoriaPago (IdCategoriaPago, Descripcion) VALUES ('A', 'Regular');
UPDATE CategoriaPago SET Descripcion = 'Regular plus' WHERE IdCategoriaPago = 'A';
DELETE FROM CategoriaPago WHERE IdCategoriaPago = 'A';
SELECT Tabla, Actividad, Columna, Usuario FROM Auditoria ORDER BY IdAuditoria;

INSERT INTO Grupo (IdPeriodo, IdAsignatura, IdGrupo, CupoGrupo) VALUES ('202420053', 'ICC103T', '7000', 25);
INSERT INTO HorarioGrupo (IdPeriodo, IdAsignatura, IdGrupo, Dia, HoraInicio, HoraFin)
VALUES ('202420053', 'ICC103T', '7000', 2, '08:00', '10:30');
SELECT IdGrupo, Horario, Actividad FROM Grupo WHERE IdGrupo = '7000';
