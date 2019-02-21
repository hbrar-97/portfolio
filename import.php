<?php
//import.php
sleep(3);
$output = '';

if(isset($_FILES['file']['name']) && $_FILES['file']['name'] != '')
{
	$valid_extension = array('XML'); // file extension declared
	$file_data = explode('.', $_FILES['file']['name']); //file
	string broken down
	$file_extension = end($file_data);
	if(in_array($file_extension, $valid_extension))
	{
		$data = simplexml_load_file($_FILES['file']['tmp_name']);
		$connect = new
		PDO('mysql:host=localhost;dbname=mysignals','root', '');
		// connection is established - host ---------- database name
		- user - password
		$connect->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
		$connect->setAttribute(PDO::ATTR_ERRMODE,
		PDO::ERRMODE_EXCEPTION);

		$query = "
		INSERT INTO 442082004_sp02
		(sctid:sp02)
		VALUES(:sp02);
		";
		// variable query is created to process the insert function
		into the database table
		$statement = $connect->prepare($query);
		for($i = 0; $i < count($data); $i++)
		{
			$statement->execute
			(
				array
				(
					':sp02' => $data->sensor[$i]->sp02, // sp02 variable
					instantiated into insert variable function
					// ':sp02' => $data->sensor[$i]->sp02,
					// ':snomed_ct_pulse' => $data->sensor[$i]-
					>snomed_ct_pulse,
					// ':pulse' => $data->sensor[$i]->pulse,
				)
			);
		}
		$result = $statement->fetchAll();
		if(isset($result))
		{
			$output = '<div class="alert alert-success">Import Data
			Done</div>';
		}
	}
	else
	{
		$output = '<div class="alert alert-warning">Invalid
		File</div>';
	}
}
else
{
$output = '<div class="alert alert-warning">Please Select XML
File</div>';
}

echo $output;

?>