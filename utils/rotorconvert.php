<?php

function wheel_wiring_form($input = "ABCDEFGHIJKLMNOPQRSTUVWXYZ") {
    $form = "<form action=\"rotorconvert.php\" method=\"post\">\n";
    $form .= "<input type=\"text\" name=\"wheel_wiring\" placeholder=\"{$input}\" size=\"30\">\n";
    $form .= "<input type=\"submit\" value=\"Send\">\n";
    $form .= "</form>\n";
    return $form;
}

function wheel_char_to_num($wheel_chars) {
    $len = strlen($wheel_chars);
    $out = '';
    for($i = 0; $i < $len; $i++) {
        $out .= ord(strtolower($wheel_chars[$i])) - 97;
        if ($i < ($len - 1)) $out .= ',';
    }
    return $out;
}

function page_header() {
    $out = "<h1>Convert Enigma wheel wiring characters to number array</h1>\n"; 
    $out .= "<p>This tool converts Enigma wheel wiring string arrays to integer arrays, where <code>A = 0</code> and <code>Z = 25</code>.</p>\n";
    $out .= "<p>See <a href=\"https://www.cryptomuseum.com/crypto/enigma/wiring.htm\">this page on Crypto Museum</a> for wiring string arrays.</p>\n";
    return $out;
}

$output = page_header();

if (isset($_POST['wheel_wiring'])) {
    $output .= wheel_wiring_form($_POST['wheel_wiring']);
    $output .= "<textarea rows=\"1\" cols=\"80\">\n";
    $output .= wheel_char_to_num($_POST['wheel_wiring']);
    $output .= "\n</textarea>\n";
} else {
    $output .= wheel_wiring_form();
}

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Convert Enigma wheel wiring characters to number array</title>
</head>
<body>
<?php
echo $output;
?>
</body>
</html>