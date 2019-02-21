<?php
require("config.php"); //implements config php file to ensure
//connection to database is established

$query = "INSERT INTO 442082004_sp02(sctid:sp02)
	VALUES('$_POST[sp02]')"; //query used to insert
		//POST request used
		//data into table inside given database

	if(!@mysqli_query($query))
	{
		echo "&Answer; SQL Error - ".mysqli_error(); //if
		mysql_query() is not executed

		//it should be returned SQL Error.
		return;
	}
?>