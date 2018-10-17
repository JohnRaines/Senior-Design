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