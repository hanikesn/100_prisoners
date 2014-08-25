<?php

function fisherYatesShuffle(&$items)
{
    $items = array_values($items);
    for ($i = count($items) - 1; $i > 0; $i--)
    {
        $j = @mt_rand(0, $i);
        $tmp = $items[$i];
        $items[$i] = $items[$j];
        $items[$j] = $tmp;
    }
}

function create_drawer($num) {
    $drawer = range(0, $num-1);
    shuffle($drawer);
    return $drawer;
}

function prisoner_rnd($drawer, $id) {
    $tries = range(0, sizeof($drawer)-1);
    shuffle($tries);
    for ($i = 0; $i < sizeof($drawer)/2; $i++) {
        if($drawer[$tries[$i]] == $id)
            return true;
    }
    return false;
}

function prisoner_best($drawer, $id) {
    $cur = $id;
    $tries = 0;
    while($id != $drawer[$cur]) {
        $cur = $drawer[$cur];
        $tries++;
        if($tries >= 50) {
            return false;
        }
    }
    return true;
}

function test_strategy($drawer, $strategy)
{
    $counter = 0;
    for ($i = 0; $i < sizeof($drawer); $i++) {
        if ($strategy($drawer, $i))
            $counter++;
    }
    return $counter;
}

const NUM = 100;
const COUNT = 10000;

$count_best = 0;
$count_rnd = 0;
$count_100_best = 0;
$count_100_rnd = 0;
$hist_best = [];

for($i = 0;$i < COUNT; $i++) {
    $drawer = create_drawer(NUM);
    $best = test_strategy($drawer, 'prisoner_best');
    $count_best += $best;
    $hist_best[$best]++;
    if($best == 100) {
        $count_100_best++;
    }
    $rnd = test_strategy($drawer, 'prisoner_rnd');
    $count_rnd += $rnd;
    if($rnd == 100)
        $count_100_rnd++;
}

echo "Best 100: " . $count_100_best/(COUNT) . "\n";
echo "Rnd 100: " . $count_100_rnd/(COUNT) . "\n";

echo "Best: " . $count_best/(NUM*COUNT) . "\n";
echo "Rnd: " . $count_rnd/(NUM*COUNT) . "\n";

ksort($hist_best);

print_r($hist_best);