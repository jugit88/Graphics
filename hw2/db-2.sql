select PC.model from PC where speed>= 3.0;
select maker from Product natural join Laptop where hd >=100;
select PC.model, PC.price from Product,PC where Product.maker='B' and Product.model = PC.model union select Laptop.model, Laptop.price from Product , Laptop  where Product.maker='B' and Product.model = Laptop.model union select Printer.model, Printer.price from Product , Printer where Product.maker='B' and Product.model = Printer.model;
select model from Printer where color=true and Printer.type = 'laser';
select distinct maker from Product where Product.type='laptop' and  maker NOT IN  (select maker from Product where type='pc');
select distinct hd from PC P where (select count(*) from PC P1 where P.hd=P1.hd) > 1;
