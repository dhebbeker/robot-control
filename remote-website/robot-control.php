<?php 
$image = imagecreatetruecolor(1000, 1000);
$default_color = imagecolorallocate($image, 0xFF, 0xFF, 0xFF);

$data_server = "http://kps32.ddns.net/?positions";
$positionString = file_get_contents($data_server);


$positions = array();
if($positionString === FALSE) {
    echo "Could not read the data from ".$data_server.".";
} else {
    $positions = explode(";", $positionString);
}

$diameter = 10;
$offset_x = 500;
$offset_y = 500;
$start_x = $offset_x;
$start_y = $offset_y;

for ($i = 0; $i+1 < count($positions); $i += 2) {
    $x = ((int) $positions[$i])/10+$offset_x;
    $y = ((int) $positions[$i + 1])/10+$offset_y;

    imagefilledellipse($image, $x, $y, $diameter, $diameter, $default_color);
    imagestring($image, 3, $x + $diameter / 2, $y + $diameter / 2, sprintf("%u", $i/2), $default_color);
    imageline($image, $start_x, $start_y, $x, $y, $default_color);

    $start_x = $x;
    $start_y = $y;
}

// Flush the image
header('Content-type: image/png');
imagepng($image);
imagedestroy($image);

?>
