<?php

require __DIR__ . '/../vendor/autoload.php';
require __DIR__ . '/../configuration/configuration.generated.php';

date_default_timezone_set('Europe/Zurich');
$cfg = SolarApiConfiguration::get()->beebotte;  
$client = new GuzzleHttp\Client([]);
$beebotte = new \SolarApi\BeebottePublisher($cfg,$client);
$beebotte->setup();
