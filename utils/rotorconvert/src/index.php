<?php
/** 
 * MIT License
*
* Copyright (c) 2019 bitjungle Rune Mathisen
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/**
 * 
 */
function wheel_wiring_form($input = '', $turnover = '') {
    $selected_zero = '';
    $selected_one = '';
    $form = "<form action=\"index.php\" method=\"post\">\n";
    $form .= "Enter wiring string: <input type=\"text\" name=\"wheel_wiring\" placeholder=\"ABCDEFGHIJKLMNOPQRSTUVWXYZ\" value=\"{$input}\" size=\"40\"><br/>\n";
    $form .= "Turnover: <input type=\"text\" name=\"wheel_turnover\" value=\"{$turnover}\" size=\"4\">\n";
    $form .= "<input type=\"submit\" value=\"Convert\">\n";
    $form .= "</form>\n";
    return $form;
}

/**
 * Take a string of characters [A-Z] as input, and convert to two
 * numeric arrays (forward and return wheel values).
 */
function wheel_char_to_num($wheel_chars, $turnover = '') {
    $num_array = []; // numeric output array
    $num_array_inv = []; // numeric output array, return values
    $turnover_pos = []; // Array for storing the turnover positions
    if ($turnover != '') {
        $turnover_len = strlen($turnover);
        for ($i = 0; $i < $turnover_len; $i++) {
            $turnover_pos[$i] = ord(strtolower($turnover[$i])) - 97;
        }
    }
    $len = strlen($wheel_chars); 
    if ($len != 26) return 'Wiring string must be 26 exactly characters long';
    for ($i = 0; $i < $len; $i++) { // Loop through the input string
        $pos = ord(strtolower($wheel_chars[$i])); // Convert from character to integer
        if ($pos < 97 || $pos > 123) { // The numeric character must be within the range 97-123
            return 'Invalid character in wiring string';
        } elseif (in_array($i, $turnover_pos)) {
            $num_array[$i] = $pos - 97 + 100; // Mark the turnover position
            $num_array_inv[$pos - 97] = $i; // Return value is left unaltered
        } else { // Shift the values down to the 0-25 range
            $num_array[$i] = $pos - 97; 
            $num_array_inv[$pos - 97] = $i;
        }
    }
    ksort($num_array_inv);
    return array('ROTORVALS' => $num_array, 'ROTORVALSI' => $num_array_inv);
}

/**
 * Background and usage info
 */
function page_header() {
    $out = "<h1>Convert Enigma wheel wiring characters to number array</h1>\n"; 
    $out .= "<p style=\"width: 75%;\">This tool converts Enigma wheel wiring string arrays 
            to comma separated integer arrays, from <code>A = 0</code> to <code>Z = 25</code>.
            These integer arrays are e.g. used in the software implementation of the 
            Open Enigma. See <a href=\"https://www.cryptomuseum.com/crypto/enigma/wiring.htm\">
            this page on Crypto Museum</a> for wiring string arrays. If you enter a turnover 
            character in the form below, a value of 100 will be added to the corresponding 
            wheel position.</p>\n";
    $out .= "<hr/>\n";
    return $out;
}


// ============= Main program starts here =============

$output = page_header();

if (isset($_POST['wheel_wiring'])) {
    $vals = wheel_char_to_num($_POST['wheel_wiring'], $_POST['wheel_turnover']);
    $output .= wheel_wiring_form($_POST['wheel_wiring'], $_POST['wheel_turnover']);
    $output .= "<hr/>\n<p>Result:</p>\n<textarea rows=\"2\" cols=\"100\">\n";
    if (is_array($vals)) { // Looks like we have valid output
        $output .= "ROTORVALS:  ";
        $output .=  implode(',', $vals['ROTORVALS']);
        $output .= "\nROTORVALSI: ";
        $output .=  implode(',', $vals['ROTORVALSI']);
    } else { // We have an error message
        $output .= $vals;
    }
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
<hr />
<footer>
    <div style="font-size: x-small; margin-top: 2em;">
    <a href="https://opensource.org/licenses/MIT">MIT License</a> <br/>
    &copy; 2019 bitjungle <br/>
    <a href="https://github.com/bitjungle/openenigma/blob/master/utils/rotorconvert/">
    Source code on Github</a>
    </div>
</footer>
</body>
</html>