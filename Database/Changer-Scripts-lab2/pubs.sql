-- En la base de datos "pubs", el cual trae
-- SqlServer como ejemplo, realice las siguientes consultas

-- Obtener el nombre y apellido de los empleados con su titulo de puesto
-- de trabajo y la casa editora para la que trabaja

SELECT emp.fname AS [Primer nombre], emp.lname AS [Segundo Nombre], _job.job_desc AS [Puesto de trabajo], pub.pub_name AS [Casa editora]
FROM employee emp
JOIN jobs _job ON emp.job_id = _job.job_id
JOIN publishers pub ON emp.pub_id = pub.pub_id;

-- Obtener nombre, el tipo, casa editorial de los titulos, sin repeticion, que han sido vendidos de contado ('On invoice').
SELECT DISTINCT t.title AS [Titulo], t.type AS [Tipo], pub.pub_name AS [Nombre de la casa editora]
FROM titles t
JOIN publishers pub ON t.pub_id = pub.pub_id
JOIN sales sale ON t.title_id = sale.title_id
WHERE sale.payterms = 'On invoice';

-- Obtener el nombre y casa editora de los titulos que se han vendido al menos 2 veces

SELECT t.title AS [Titulo], pub.pub_name AS [Nombre de la casa editora]
FROM titles t
JOIN publishers pub ON t.pub_id = pub.pub_id
JOIN sales sale ON t.title_id = sale.title_id
GROUP BY t.title, pub.pub_name
HAVING COUNT(*) >= 2;

-- Obtener las ventas de la tienda cuyo Id es '7067'. Nombre de la tienda, titulo vendido,
-- casa editora, y precio
SELECT stor.stor_name AS [Tienda], t.title AS [Titulo], pub.pub_name AS [Casa editora], t.price AS [Costo]
FROM sales sale
JOIN stores stor ON sale.stor_id = stor.stor_id
JOIN titles t ON sale.title_id = t.title_id
JOIN publishers pub ON t.pub_id = pub.pub_id
WHERE sale.stor_id = '7067';
