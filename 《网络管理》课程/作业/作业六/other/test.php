<?php
$hrSWRunPerf = array(
    "HOST-RESOURCES-MIB::hrSWRunPerfCPU.1" => "INTEGER: 1",
    "HOST-RESOURCES-MIB::hrSWRunPerfCPU.31015" => "INTEGER: 152",
    "HOST-RESOURCES-MIB::hrSWRunPerfMem.1" => "INTEGER: 3375104 KBytes",
    "HOST-RESOURCES-MIB::hrSWRunPerfMem.861" => "INTEGER: 161229488 KBytes"
);

// print_r($a);

// $aa = [];
$aa = array();


foreach ($hrSWRunPerf as $key => $value) {
    echo $key . "\n";
    echo $value . "\n";

    $temp = explode(".", $key);
    $process_id = $temp[1];

    $judge = substr($key, 31, 3);

    $temp1 = explode(" ", $value);


    if ($judge == "CPU") {
        $cpu = $temp1[1];
        $a = array($process_id, $cpu);
        array_push($aa, $a);
    }
    if ($judge == "Mem") {
        $mem = $temp1[1];

        foreach ($aa as &$t) {
            if ($t[0] == $process_id) {
                print("1\n");
                $t[2] = $mem;
            }
        }
    }
}
print_r($aa);
