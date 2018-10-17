<?php
	$log_file_name = 'values.txt';
	$message = $_POST['shift1']."\r\n".$_POST['attn1']."\r\n".$_POST['shift2']."\r\n".$_POST['attn2']."\r\n".$_POST['shift3']."\r\n".$_POST['attn3']."\r\n";
	file_put_contents($log_file_name, $message);
	header('Location: /');
?>