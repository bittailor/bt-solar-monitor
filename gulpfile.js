var spawn = require('child_process').spawn;

const { series, parallel } = require('gulp');
var path = require('path');
var fs = require('fs-extra');
var Q = require('q');

const log = require('fancy-log');
const colors = require('ansi-colors');

var secrets = require('./secrets');

const pwd = process.cwd();

var conf = {
    deviceOsFolder: '/Users/bittailor/.particle/toolchains/deviceOS/3.1.0/main',
    
    device: "bt-electron-solar",
    platform: "electron",
    platforms: ["electron","photon"],
    
    //device: "bt-photon-2",
    //platform: "photon",
    fw: {
        out: 'fw/target',
        ext: 'fw/external'
    },
    web: {
        api: 'web-api/solar-api'
    }
};

function sh(cmd, args, cwd, stdio = 'inherit' ) {
    var deferred = Q.defer();
    log(colors.gray(`|${cwd}> ${cmd} ${args.join(" ")}`));
    var options = {
        stdio: stdio,
        cwd: cwd,
    }
    spawn(cmd, args, options).on('exit', (code) => {
        if(code !== 0) {
            deferred.reject(code);
        } else {
            deferred.resolve(code);
        }     
    });
    return deferred.promise;
}

function fw_host_test() {
    var out = path.join(conf.fw.out, 'host-test');
    return fs.mkdirs(out)
    .then(() => {       
        return sh('cmake', ['-DCMAKE_BUILD_TYPE=Debug','../../'], out);
    })
    .then(()=>{
        // notifier.notify('Test run OK');
        return sh('make', ['check'], out);
    });
}


function compile_local(done) {
    const tasks = conf.platforms.map(platform => {
        var compile = () => {
            return sh('make',[ /*'-s',*/ 'all', 'PLATFORM=' + platform, 'APPDIR=' + path.join(pwd, 'fw')],conf.deviceOsFolder)
        }   
        compile.displayName = `compile_local_${platform}` ;
        return compile;
    });
    return series(...tasks, tasksDone => {
        tasksDone()
        done()
    })();
}

function compile_online (cb) {
    fs.mkdirs(conf.fw.out,function(){
        spawn('echo', ['compile', conf.platform, '.' ,  '--saveTo' ,  path.join('target' , 'fw.bin')],{
            stdio: 'inherit',
            cwd:'./fw'
        }).on('exit', (code) => {
            cb(code)
        }); 
    })
}

//-------

function install_composer(){    
    return sh('composer', ['install'], conf.web.api) 
}

function web_api_test () {
    var cwd = conf.web.api;
    return sh('php',[
        '-d', 'xdebug.mode=debug',
        '-d', 'xdebug.start_with_request=yes',
        'vendor/bin/phpunit',
        'tests'
    ],cwd);
    //return sh('vendor/bin/phpunit', ['tests'], cwd);
}

function web_api_serve () {
    var cwd = path.join(conf.web.api,'public');
    return sh('php', [
            '-d', 'xdebug.mode=debug',
            '-d', 'xdebug.start_with_request=yes',
            '-S', 'localhost:8080'
        ], cwd);
}

function web_api_deploy() {
    return sh('lftp', [ '-u', `${secrets.solar.hosting.user},${secrets.solar.hosting.pw}`, '-e', `set ftp:ssl-allow no; mirror --no-symlinks --reverse ${path.join(pwd,'web-api/solar-api')} /solar; exit`, 'ftp.bockmattli.ch' ]);
};

function web_ui_deploy() {
    return sh('lftp', [ '-u', `${secrets.solar.hosting.user},${secrets.solar.hosting.pw}`, '-e', `set ftp:ssl-allow no; mirror --no-symlinks --reverse ${path.join(pwd,'web-ui')} /solardaten; exit`, 'ftp.bockmattli.ch' ]);
};

exports.install_composer = install_composer;
exports.web_api_serve = web_api_serve;
exports.web_api_test = web_api_test;
exports.web_api_deploy = web_api_deploy

//-----


// The `clean` function is not exported so it can be considered a private task.
// It can still be used within the `series()` composition.
function clean(cb) {
  // body omitted
  cb();
}

// The `build` function is exported so it is public and can be run with the `gulp` command.
// It can also be used within the `series()` composition.
function build(cb) {
  // body omitted
  cb();
}

exports.build = build;
exports.default = series(clean, build, fw_host_test, compile_local);