<?php
	$log_file_name = 'values.txt';
	$message = $_POST['shift1']." ".$_POST['attn1']." ".$_POST['shift2']." ".$_POST['attn2']." ".$_POST['shift3']." ".$_POST['attn3']." ";
	file_put_contents($log_file_name, $message);
	header('Location: /');
?>