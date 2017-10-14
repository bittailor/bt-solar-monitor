<?php
require __DIR__ . '/../vendor/autoload.php';
require __DIR__ . '/../filestorage/configuration/configuration.php';

date_default_timezone_set('Europe/Zurich');

$app = new \Slim\App([
    'settings' => [
        'displayErrorDetails' => true
    ]
]);


// -- Dependency Container
$container = $app->getContainer();
$container['pdo'] = function($c) {
    $pdo = new PDO(
        SOLAR_API_PDO_DNS, 
        SOLAR_API_PDO_USER, 
        SOLAR_API_PDO_PW,
        array(PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8'));
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    return $pdo;
};

$container['SolarApi\WebHookController'] = function($c) {
    return new SolarApi\WebHookController($c['pdo'] ,new SolarApi\MessageFactory(), new SolarApi\MessageToXivelyConverter());
};

// -- Middleware
$app->add(new SolarApi\EnsureApiKey(SOLAR_API_API_KEY));

// -- Routes
$app->post('/hook/execute', SolarApi\WebHookController::class . ':execute');

$app->get('/hello/{name}', function ($request, $response, $args) {
    error_log('just a test');
    return $response->write("Hello " . $args['name'] . "  -- " . ini_get('error_log'));
});

$app->get('/env', function ($request, $response, $args) {
    return $response->write(print_r($_SERVER, true));
});

$app->get('/info', function ($request, $response, $args) {
    phpinfo();
});


// -- Run app
$app->run();
