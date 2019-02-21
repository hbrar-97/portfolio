<?php
$dbconnect = mysql_connect('localhost','root','');
$dbselect = mysql_select_db('project');

mysql_query("INSERT INTO sp02 (sp02, Pulse, SNOMED_CT_CODE)
VALUES ('".mysql_real_escape_string($_GET['sp02'])."',
'".mysql_real_escape_string($_GET['Pulse'])."',
'".mysql_real_escape_string($_GET['SNOMED_CT_CODE'])."')");

if (mysql_query) {
	echo "success";
	}
?>