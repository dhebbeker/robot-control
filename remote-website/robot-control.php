<?php 
$image = imagecreatetruecolor(1000, 1000);
$default_color = imagecolorallocate($image, 0xFF, 0xFF, 0xFF);

$positions = array();
for ($i = 0; $i < 10; $i ++) {
    $positions[] = rand(0, 1000);
}

$diameter = 10;
$start_x = 0;
$start_y = 0;

for ($i = 0; $i < count($positions); $i += 2) {
    $x = $positions[$i];
    $y = $positions[$i + 1];

    imagefilledellipse($image, $x, $y, $diameter, $diameter, $default_color);
    imagestring($image, 3, $x + $diameter / 2, $y + $diameter / 2, sprintf("%u;%u", $x, $y), $default_color);
    imageline($image, $start_x, $start_y, $x, $y, $default_color);

    $start_x = $x;
    $start_y = $y;
}

// Flush the image
header('Content-type: image/png');
imagepng($image);
imagedestroy($image);

?>
