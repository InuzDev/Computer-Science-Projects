-- Find a customer which order is higher than 100$
SELECT cus.Name, ord.order_id, ord.total
FROM Customers cus JOIN Orders ord
ON cus.customer_id = ord.customer_id
WHERE ord.total > 100

-- Return customers names, cities and the product names they ordered, and the order totals
-- For orders that are greater than 100$ AND product belongs to the supplements cattegory

SELECT cus.Name, cus.City, ord.total, prod.product_name
FROM Customers cus JOIN Orders ord
	ON cus.customer_id = ord.customer_id
JOIN Products prod
	ON ord.product_id = prod.product_id
WHERE ord.total > 100 AND prod.category = 'Supplements'

-- Combine the names of two tables without duplicate names
SELECT onlcus.name FROM OnlineCustomers onlcus UNION SELECT strcus.name FROM StoreCustomers strcus

--- Databases should grow in rows, not columns

--- Functions and triggers

-- Making a function, should be easy.
CREATE FUNCTION dbo.CalculateAnnualSalary
(
	@MonthlySalary DECIMAL(10, 2)
)
RETURNS DECIMAL(12, 2)
AS
BEGIN
	RETURN @MonthlySalary * 12;
END;
GO

--- Function exercise

-- Make a scalar function
-- Imagine the following table

CREATE TABLE dbo.Products
(
	ProductID INT PRIMARY KEY,
	PRODUCTName VARCHAR(100),
	Price DECIMAL(10, 2)
);

-- Now create a function that accepts a product's price and a discount percentage, then returns the final price
-- after applying the discount

CREATE FUNCTION dbo.CalculateDiscountedPrice
(
	@OriginalPrice DECIMAL(10, 2),
	@DiscountPercentage TINYINT
)
RETURNS DECIMAL(10, 2)
AS BEGIN
	RETURN @OriginalPrice - (@OriginalPrice * (@DiscountPercentage / 100.0));
END;
GO

-- Trigger
CREATE TRIGGER dbo.AfterUpdate_EmployeeSalary  ON dbo.Employees
AFTER UPDATE
AS BEGIN
	SET NOCOUNT ON;

	PRINT 'Employee salary got updated'
END;
GO

--- Trigger exercise

-- Imagine we have this table
CREATE TABLE dbo.Inventory
(
   ProductID INT PRIMARY KEY,
   ProductName VARCHAR(100),
   Stock INT
)

-- We need to create a trigger AFTER UPDATE
CREATE TRIGGER dbo.trg_PreventNegativeStock ON dbo.Inventory
AFTER UPDATE
AS BEGIN
   SET NOCOUNT ON;

   IF EXIST (
      SELECT inv.Stock FROM dbo.Inventory inv
      WHERE inv.Stock < 0
   )
END
