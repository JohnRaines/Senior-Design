<!DOCTYPE html>

<html>
	<head>
		<style>
			table{
				border-collapse: collapse;
				width: 50%;
			}
			td, th{
				border: 1px solid #dddddd;
				text-align: left;
				padding: 8px;
			}
		</style>
	</head>
	
	
	<form action="inputsave.php" method="POST">
	<!antenna one control>
	<h1>Antenna 1 controls</h1>
		Phase Shift<br>
		<input type="number" name="shift1" min="0" max="360" step="1" value="0">
	
		<br>Attenuation Shift<br>
		<input type="number" name="attn1" min="0" max="31.5" step="0.5" value="0">
	
	<!antenna two control>
	<h1>Antenna 2 controls</h1>
	
		Phase Shift<br>
		<input type="number" name="shift2" min="0" max="360" step="1" value="0">

		<br>Attenuation Shift<br>
		<input type="number" name="attn2" min="0" max="31.5" step="0.5" value="0">

	
	<!antenna three control>
	<h1>Antenna 3 controls</h1>
		Phase Shift<br>
		<input type="number" name="shift3" min="0" max="360" step="1" value="0">

		<br>Attenuation Shift<br>
		<input type="number" name="attn3" min="0" max="31.5" step="0.5" value="0">
		
	<br><br><input type="submit" value="Submit">
	</form>
	

	<! open the file, read the values, and generate the current status table>
	<?php
	$filename = 'values.txt';
	$file = fopen($filename, "r");
	$t1 = fgets($file);
	$t2 = fgets($file);
	$t3 = fgets($file);
	$t4 = fgets($file);
	$t5 = fgets($file);
	$t6 = fgets($file);
	fclose($file);
	
	echo "<h2>Current Status</h2>
	<table>
		<tr>
			<th>Antenna</th>
			<th>Shift(degrees)</th>
			<th>Attenuation(db)</th>
		</tr>
		
		<tr>
			<td>Antenna 1</td>";
			echo "<td>".($t1)."</td>";
			echo "<td>".($t2)."</td>";
		echo "</tr>
		<tr>
			<td>Antenna 2</td>
			<td>".($t3)."</td>
			<td>".($t4)."</td>
		</tr>
		<tr>
			<td>Antenna 3</td>
			<td>".($t5)."</td>
			<td>".($t6)."</td>
		</tr>
	</table>";
	
	?>
	
</html>
