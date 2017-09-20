<?php
if (PHP_SAPI == 'cli-server') {
    // To help the built-in PHP dev server, check if the request was actually for
    // something which should probably be served as a static file
    $url  = parse_url($_SERVER['REQUEST_URI']);
    $file = __DIR__ . $url['path'];
    if (is_file($file)) {
        return false;
    }
}

require __DIR__ . '/../vendor/autoload.php';

// Create and configure Slim app
$config = ['settings' => [
    'addContentLengthHeader' => false,
]];

$app = new \Slim\App($config);

// Define app routes
$app->get('/hello/{name}', function ($request, $response, $args) {
    error_log("GET /hello/{$args['name']}");
    return $response->write("Hello " . $args['name']);
});

$app->post('/binary/test', function ($request, $response, $args) {
    error_log("POST /binary/test");
    $raw = $request->getBody()->getContents();
    error_log($raw);
    error_log(print_r(unpack('na/nb',$raw), true));
    error_log(print_r(unpack('n*',$raw), true));
    #error_log(print_r( $request->getBody(), true ));
    return $response->write("OK");
});

// Run app
$app->run();