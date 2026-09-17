-- Obtener el nombre y apellido de los empleados, junto con su puesto de trabajo
-- y, nombre y apellido del jefe.

SELECT
emp.FirstName as [Primer Nombre], emp.LastName as [Segundo Nombre], emp.Title as [Titulo del empleo],
boss.Firstname AS [Nombre del Jefe], boss.LastName AS [Segundo nombre del Jefe]
FROM Employees emp
LEFT JOIN Employees boss ON emp.ReportsTo = boss.EmployeeID
ORDER BY boss.FirstName, boss.LastName;

-- Obtener los productos (Id, nombre) que en un mes cualquiera haya sido incluido en 4 ordenes o mas
SELECT DISTINCT prod.ProductID AS [ID Producto], prod.ProductName AS [Nombre Producto]
FROM Products prod
WHERE prod.ProductID IN (
   SELECT ord_det.ProductID
   FROM [Order Details] ord_det
   JOIN Orders ord ON ord_det.OrderId = ord.OrderID
   GROUP BY ord_det.ProductID, YEAR(ord.OrderDate), MONTH(ord.OrderDate)
   HAVING COUNT(DISTINCT ord_det.OrderID) >= 4
);

-- 2) Para tener datos de todos los productos que realmente se hayan ordenados.
SELECT prod.ProductID AS [ID Producto], prod.ProductName AS [Nombre Producto]
FROM Products prod
JOIN [Order Details] ord_det ON prod.ProductId = ord_det.ProductID
JOIN Orders ord ON ord_det.OrderId = ord.OrderID
GROUP BY prod.ProductID, prod.ProductName, YEAR(ord.OrderDate), MONTH(ord.OrderDate)
HAVING COUNT(DISTINCT ord_det.OrderID) >= 4;

-- 3) Similar al primero, pero esta vez contamos la cantidad de productos que se han vendido, para no tener
--    que repetirlos.
SELECT prod.ProductID AS [ID Producto], prod.ProductName AS [Nombre Product], COUNT(DISTINCT ord_det.OrderID) AS [Cantidad de Ordenes]
FROM Products prod
JOIN [Order Details] ord_det ON prod.ProductId = ord_det.ProductID
JOIN Orders ord ON ord_det.OrderId = ord.OrderID
GROUP BY prod.ProductID, prod.ProductName, YEAR(ord.OrderDate), MONTH(ord.OrderDate)
HAVING COUNT(DISTINCT ord_det.OrderID) >= 4;

-- 4) Similar al primero nuevamente, esta vez evitando totalmente su repeticion y mostrando el maximo de ordenes en un mes.
SELECT prod.ProductID AS [ID Producto], prod.ProductName AS [Nombre Product], MAX(sub.CantidadOrdenes) AS [Maximo de ordenes en un mes]
FROM Products prod
JOIN (
	SELECT ord_det.ProductID, COUNT(DISTINCT ord_det.OrderID) AS CantidadOrdenes
	FROM [Order Details] ord_det
	JOIN Orders ord ON ord_det.OrderId = ord.OrderID
	GROUP BY ord_det.ProductID, YEAR(ord.OrderDate), MONTH(ord.OrderDate)
	HAVING COUNT(DISTINCT ord_det.OrderID) >= 4
) sub ON prod.ProductID = sub.ProductID
GROUP BY prod.ProductID, prod.ProductName;

-- Obtener la cantidad de ordenes realizadas por dia para el mes de mayo del 1997
SELECT DAY(OrderDate) AS Dia, COUNT(*) AS CantidadOrdenes
FROM Orders
WHERE YEAR(OrderDate) = 1997 AND MONTH(OrderDate) = 5
GROUP BY DAY(OrderDate)
ORDER BY Dia;
