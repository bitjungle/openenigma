<?php

function wheel_wiring_form($input = '', $index_start = '0') {
    $selected_zero = '';
    $selected_one = '';
    if ($index_start == '0') {
        $selected_zero = 'checked="checked"';
    } else {
        $selected_one = 'checked="checked"';
    }
    $form = "<form action=\"rotorconvert.php\" method=\"post\">\n";
    $form .= "Enter wiring string: <input type=\"text\" name=\"wheel_wiring\" placeholder=\"ABCDEFGHIJKLMNOPQRSTUVWXYZ\" value=\"{$input}\" size=\"40\"><br/>\n";
    $form .= "Start index at: <input type=\"radio\" name=\"index_start\" value=\"0\" {$selected_zero}> 0 ";
    $form .= "<input type=\"radio\" name=\"index_start\" value=\"1\" {$selected_one}> 1 <br>\n";
    $form .= "<input type=\"submit\" value=\"Send\">\n";
    $form .= "</form>\n";
    return $form;
}

function wheel_char_to_num($wheel_chars, $index_start = '0') {
    $corr = 97;
    if ($index_start != '0') $corr = 96;
    $len = strlen($wheel_chars);
    if ($len != 26) return 'Wiring string must be 26 exactly characters long';
    $out = '';
    for($i = 0; $i < $len; $i++) {
        $pos = ord(strtolower($wheel_chars[$i]));
        if ($pos < 97 || $pos > 123) {
            return 'Invalid character in wiring string';
        } else {
            $out .= $pos - $corr;
        }
        if ($i < ($len - 1)) $out .= ',';
    }
    return $out;
}

function page_header() {
    $out = "<h1>Convert Enigma wheel wiring characters to number array</h1>\n"; 
    $out .= "<p>This tool converts Enigma wheel wiring string arrays to comma 
            separated integer arrays, from <code>A = 0/1</code> to <code>Z = 25/26</code>.</p>\n";
    $out .= "<p>See <a href=\"https://www.cryptomuseum.com/crypto/enigma/wiring.htm\">
            this page on Crypto Museum</a> for wiring string arrays.</p>\n";
    return $out;
}

$output = page_header();

if (isset($_POST['wheel_wiring'])) {
    $output .= wheel_wiring_form($_POST['wheel_wiring'], $_POST['index_start']);
    $output .= "<textarea rows=\"1\" cols=\"80\">\n";
    $output .= wheel_char_to_num($_POST['wheel_wiring'], $_POST['index_start']);
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