<?php
require __DIR__ . '/../vendor/autoload.php';
require __DIR__ . '/../configuration/configuration.generated.php';

date_default_timezone_set('Europe/Zurich');

$app = new \Slim\App([
    'settings' => [
        'displayErrorDetails' => true
    ]
]);

$configuration = SolarApiConfiguration::get();

// -- Dependency Container
$container = $app->getContainer();
$container['pdo'] = function($c) use($configuration) {
    $cfg = $configuration->db;
    $pdo = new PDO(
        $cfg->dns, 
        $cfg->user, 
        $cfg->pw,
        array(PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8'));
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    return $pdo;
};

$container['SolarApi\AdafruitIoPublisher'] = function($c) use($configuration) {
    $cfg = $configuration->adafruit;
    $httpClient = new GuzzleHttp\Client([
        'base_uri' => 'https://io.adafruit.com/api/v2/',
        'headers' => ['X-AIO-Key' => $cfg->aioKey]
    ]);
    return new SolarApi\AdafruitIoPublisher($cfg, $httpClient) ;
};

$container['publishers'] = function($c) use($configuration) {
    return [
        $c['SolarApi\AdafruitIoPublisher']    
    ];
};

$container['SolarApi\WebHookController'] = function($c) {
    return new SolarApi\WebHookController(
        $c['pdo'] ,
        new SolarApi\MessageFactory(), 
        $c['publishers']);
};

// -- Middleware
$app->add(new SolarApi\EnsureApiKey($configuration->solarApi->token));

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
