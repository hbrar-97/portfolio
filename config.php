<?php

$dbhost = 'localhost'; //database server

$dbuser = 'root'; //database user
$dbpass = ''; //database password
$dbname = 'project_data'; //database name

$conn = mysqli_connect($dbhost, $dbuser, $dbpass) or die ('Error
connecting to mysql'); //mysql_connect used to connect to mysql
database
?>